#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.bootstrap.timezone
# script:  timezone.sh
# purpose: Sets Timezone
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="bootstrap_timezone"
declare -r SELF_IDENTITY_H="Bootstrap (Timezone)"
declare -a ARGUMENTS_DESCRIPTION=(
    '-t <timezone> : Timezone to use'
    '-l : List Available Timezones'
)

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=("${LIB_FUNCTIONS_AWS_METADATA}")

###------------------------------------------------------------------------------------------------
## Variables
FILE_CLOCK="/etc/sysconfig/clock"
TIMEZONE_STRING=""
LIST_AVAILABLE_TIMEZONES=no
TIMEZONE_LOOKUP_FILE="/usr/share/zoneinfo/zone.tab"

###------------------------------------------------------------------------------------------------
## Functions
function list_timezones() {
    local FUNCTION_DESCRIPTION="Timezone - List"
	local TMP_TIMEZONE_LIST=""
    generate_temp_file TMP_TIMEZONE_LIST "timezone list"
    cat ${TIMEZONE_LOOKUP_FILE} | grep "^[^#;]"  | awk '{print $3}' | sort | uniq > ${TMP_TIMEZONE_LIST}
    log_add_from_file "${TMP_TIMEZONE_LIST}" "${FUNCTION_DESCRIPTION}: Timezone List"
}

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hVt:l" OPTION; do
    case $OPTION in
        t) TIMEZONE_STRING=$OPTARG;;
        l) LIST_AVAILABLE_TIMEZONES=yes;;
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

if option_enabled LIST_AVAILABLE_TIMEZONES; then
    list_timezones
    exit_logic 0
fi

if(is_empty "${TIMEZONE_STRING}"); then
    TMP_ERROR_MSG="Timezone not specified (-t <timezone>)"
    log_error "${SELF_IDENTITY_H}: ${TMP_ERROR_MSG}"
    exit_logic $E_BAD_ARGS "${TMP_ERROR_MSG}"
fi

line_break
log "- Timezone: [${TIMEZONE_STRING}]"

## END Initialize
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Preflight
line_break
log_highlight "Preflight"

## END Preflight
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Execution
line_break
log_highlight "Execution"

log_notice "${SELF_IDENTITY_H}: Generating Clock file [${FILE_CLOCK}]"
cat > ${FILE_CLOCK} << ZZEOF
ZONE="${TIMEZONE_STRING}"
UTC=false
ZZEOF

log_notice "${SELF_IDENTITY_H}: Setting Local Time symlink"
ln -sf /usr/share/zoneinfo/${TIMEZONE_STRING} /etc/localtime

log_notice "${SELF_IDENTITY_H}: Restarting rsyslog to reflect the timezone changes"
sleep 1; sync; wait; sleep 1
/bin/systemctl restart rsyslog.service >/dev/null 2>&1

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
