#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.bootstrap.swap
# script:  swap.sh
# purpose: Configures Swap File
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="bootstrap_swap"
declare -r SELF_IDENTITY_H="Bootstrap (Swap)"
declare -a ARGUMENTS_DESCRIPTION=(
    '-s <size_in_mb> : Swap Size in MB'
)

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=("${LIB_FUNCTIONS_AWS_METADATA}")
REQUIRED_EXECUTABLES+=('dd' 'swapon' 'swapoff' 'mkswap' 'free')

###------------------------------------------------------------------------------------------------
## Variables
SWAP_FILE="/var/swapfile"
SWAP_SIZE=""
LOG_EXECUTION=""
TMP_ERROR_MSG=""

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hVs:" OPTION; do
    case $OPTION in
        s) SWAP_SIZE=$OPTARG;;
        h) usage; exit 0;;
        V) echo "$(return_script_version "${0}")"; exit 0;;
        *) echo "ERROR: There is an error with one or more of the arguments"; usage; exit $E_BAD_ARGS;;
        ?) echo "ERROR: There is an error with one or more of the arguments"; usage; exit $E_BAD_ARGS;;
    esac
done
start_logic
log "${SELF_IDENTITY_H}: Started"

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Initialize
line_break
log_highlight "Initialize"

if(is_empty "${SWAP_SIZE}"); then
    TMP_ERROR_MSG="Swap Size not specified"
    log_error "- ${TMP_ERROR_MSG}"
    exit_logic $E_BAD_ARGS "${TMP_ERROR_MSG}"
fi

if (! is_int "${SWAP_SIZE}"); then
    TMP_ERROR_MSG="Swap Size is not defined as an integer"
    log_error "- ${TMP_ERROR_MSG}"
    exit_logic $E_BAD_ARGS "${TMP_ERROR_MSG}"
fi

line_break
log "- File: [${SWAP_FILE}]"
log "- Size: [${SWAP_SIZE} mb]"

## END Initialize
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Preflight
line_break
log_highlight "Preflight"

generate_temp_file LOG_EXECUTION "swap execution log"

log_notice "${SELF_IDENTITY_H}: Deactivating Swap"
sync_disks
call_sleep 1
$(which swapoff) -a
sync_disks
call_sleep 1

## END Preflight
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Execution
line_break
log_highlight "Execution"

log_notice "${SELF_IDENTITY_H}: Creating swap file"
>${LOG_EXECUTION}
$(which touch) ${SWAP_FILE}
> ${SWAP_FILE}
sync_disks
call_sleep 1
$(which dd) if=/dev/zero of=${SWAP_FILE} bs=1M count=${SWAP_SIZE} >${LOG_EXECUTION} 2>&1
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    TMP_ERROR_MSG="Failed to create swap file (dd_returned::${RETURNVAL})"
    log_error "- ${TMP_ERROR_MSG}"
    log_add_from_file "${LOG_EXECUTION}" "output of dd operation"
    exit_logic $E_OBJECT_FAILED_TO_CREATE "${TMP_ERROR_MSG}"
fi
$(which chmod) 600 ${SWAP_FILE} >/dev/null 2>&1

log_notice "${SELF_IDENTITY_H}: Initializing swap file"
> ${LOG_EXECUTION}
sync_disks
call_sleep 1
$(which mkswap) ${SWAP_FILE} > ${LOG_EXECUTION} 2>&1
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    TMP_ERROR_MSG="Failed to initialize swap file (mkswap_Returned::${RETURNVAL})"
    log_error "- ${TMP_ERROR_MSG}"
    log_add_from_file "${LOG_EXECUTION}" "output of mkswap operation"
    exit_logic $E_OBJECT_FAILED_TO_CREATE "${TMP_ERROR_MSG}"
fi

log_notice "${SELF_IDENTITY_H}: Enable swap file"
> ${LOG_EXECUTION}
sync_disks
call_sleep 1
$(which swapon) ${SWAP_FILE} > ${LOG_EXECUTION} 2>&1
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    TMP_ERROR_MSG="Failed to enable swap file (swapon_Returned::${RETURNVAL})"
    log_error "- ${TMP_ERROR_MSG}"
    log_add_from_file "${LOG_EXECUTION}" "output of swapon operation"
    exit_logic $E_OBJECT_FAILED_TO_CREATE "${TMP_ERROR_MSG}"
else
    log "${SELF_IDENTITY_H}: Successfully enabled swap file"
fi

log_notice "${SELF_IDENTITY_H}: Available memory"
> ${LOG_EXECUTION}
$(which free) > ${LOG_EXECUTION} 2>&1
log_add_from_file "${LOG_EXECUTION}" "output of free operation"

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
