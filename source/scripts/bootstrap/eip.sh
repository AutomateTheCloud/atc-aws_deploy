#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.bootstrap.eip
# script:  eip.sh
# purpose: Configures and Attaches Elastic IP if present
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="bootstrap_eip"
declare -r SELF_IDENTITY_H="Bootstrap (EIP)"
declare -a ARGUMENTS_DESCRIPTION=(
    '-e <eip_id> : EIP ID'
)

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=(
    "${LIB_FUNCTIONS_AWS_METADATA}"
)

###------------------------------------------------------------------------------------------------
## Variables
VARIABLES_REQUIRED=(
    'eip_id'
    'instance_id'
    'aws_region'
)

EIP_ASSOCIATION_ID=""
ASSOCIATION_ID=""

LOG_FILE_AWS_CLI_RESPONSE=""
LOG_FILE_AWS_CLI_ERROR=""

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hVe:" OPTION; do
    case $OPTION in
        e) EIP_ID=$OPTARG;;
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

log_notice "${SELF_IDENTITY_H}: Loading Variables"
aws_metadata_instance_id INSTANCE_ID
aws_metadata_region AWS_REGION

verify_array_key_values "VARIABLES_REQUIRED[@]"
RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $E_BAD_ARGS; fi

generate_temp_file LOG_FILE_AWS_CLI_RESPONSE "aws response file"
generate_temp_file LOG_FILE_AWS_CLI_ERROR "aws error file"

line_break
log "- EIP ID:      [${EIP_ID}]"
log "- Instance ID: [${INSTANCE_ID}]"
log "- AWS Region:  [${AWS_REGION}]"

## END Initialize
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Execution
line_break
log_highlight "Execution"

log_notice "${SELF_IDENTITY_H}: Detecting current Elastic IP Association"

EIP_ASSOCIATION_ID="$($(which aws) --region ${AWS_REGION} ec2 describe-addresses --allocation-ids ${EIP_ID} 2>/dev/null | grep "AssociationId" | head -1 | sed -e 's/{//g' -e 's/}//g' | cut -d',' -f1 | awk -F ": " '{print $2}' | cut -d '"' -f2)"

if(! is_empty "${EIP_ASSOCIATION_ID}"); then
    log_warning "${SELF_IDENTITY_H}: Elastic IP Association detected, attempting to Disassociate"
    log "- EIP Association ID: [${EIP_ASSOCIATION_ID}]"
    $(which aws) --region ${AWS_REGION} ec2 disassociate-address --association-id "${EIP_ASSOCIATION_ID}" >/dev/null 2>&1
    call_sleep 30 "Waiting for Disassociation"
else
    log "- No Elastic IP Association detected"
fi

log_notice "${SELF_IDENTITY_H}: Attaching Elastic IP"
$(which aws) --region ${AWS_REGION} ec2 associate-address --instance-id "${INSTANCE_ID}" --allocation-id "${EIP_ID}" > ${LOG_FILE_AWS_CLI_RESPONSE} 2>${LOG_FILE_AWS_CLI_ERROR}
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    log_warning "- Failed to Attach Elastic IP, attempting to retry in 60 seconds"
    log_add_from_file "${LOG_FILE_AWS_CLI_RESPONSE}" "aws_cli response file"
    log_add_from_file "${LOG_FILE_AWS_CLI_ERROR}" "aws_cli error file"
    call_sleep 60
    log_notice "${SELF_IDENTITY_H}: Attaching Elastic IP [attempt: 2]"
    > ${LOG_FILE_AWS_CLI_RESPONSE}
    > ${LOG_FILE_AWS_CLI_ERROR}
    $(which aws) --region ${AWS_REGION} ec2 associate-address --instance-id "${INSTANCE_ID}" --allocation-id "${EIP_ID}" > ${LOG_FILE_AWS_CLI_RESPONSE} 2>${LOG_FILE_AWS_CLI_ERROR}
    RETURNVAL="$?"
    if [ ${RETURNVAL} -ne 0 ]; then
        log_error "- Failed to Attach Elastic IP"
        log_add_from_file "${LOG_FILE_AWS_CLI_RESPONSE}" "aws_cli response file"
        log_add_from_file "${LOG_FILE_AWS_CLI_ERROR}" "aws_cli error file"
        exit_logic $E_OBJECT_FAILED_TO_CREATE "Failed to Attach Elastic IP"
    fi
fi
log_success "${SELF_IDENTITY_H}: Elastic IP successfully attached"

call_sleep 30 "giving amazon time to process request for meta-data"

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
