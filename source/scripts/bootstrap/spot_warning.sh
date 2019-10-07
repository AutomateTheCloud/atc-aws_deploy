#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.bootstrap.spot_warning
# script:  spot_warning.sh
# purpose: Configures Instance to detect Spot Shutdown Two-Minute Warning
# version: 1.0.0
#
# notes:
#          - Generates Service to monitor for Spot Two-Minute Warning
#          - Service will execute the contents of /apps/spot_warning_execution once the Two-Minute
#              Warning is detected
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="bootstrap_spot_warning"
declare -r SELF_IDENTITY_H="Bootstrap (Spot Warning)"
declare -a ARGUMENTS_DESCRIPTION=()

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=("${LIB_FUNCTIONS_BOOTSTRAP_SPOT_WARNING}")

###------------------------------------------------------------------------------------------------
## Variables

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hV" OPTION; do
    case $OPTION in
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

log_notice "${SELF_IDENTITY_H}: Installing Spot Warning Monitor Tool"
cp -f ${SCRIPTS_COMMANDS_SPOT_WARNING_MONITOR} ${FILE_BIN_SPOT_WARNING_MONITOR} >/dev/null 2>&1
chmod 0755 ${FILE_BIN_SPOT_WARNING_MONITOR} >/dev/null 2>&1

log_notice "${SELF_IDENTITY_H}: Installing Spot Warning Service"
cp -f ${LIB_DEPENDENCIES_SPOT_WARNING_SERVICE} ${FILE_SERVICE_SPOT_WARNING}
/bin/systemctl daemon-reload >/dev/null 2>&1

log_notice "${SELF_IDENTITY_H}: Starting Spot Warning Service"
/bin/systemctl enable ${SERVICE_SPOT_WARNING} >/dev/null 2>&1
/bin/systemctl start ${SERVICE_SPOT_WARNING} >/dev/null 2>&1

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
