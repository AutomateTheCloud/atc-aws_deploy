#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.ssl.download_certificate
# script:  download_certificate.sh
# purpose: Downloads SSL Certificate
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="download_certificate"
declare -r SELF_IDENTITY_H="SSL (Download Certificate)"
declare -a ARGUMENTS_DESCRIPTION=(
    '-s <certificate_id> : SSL certificate ID'
    '-n <common_name> : Common name of SSL certificate'
    '-r <region> : Region (optional)'
)

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=(
    "${LIB_FUNCTIONS_AWS_METADATA}"
    "${LIB_FUNCTIONS_AWS_SSM}"
    "${LIB_FUNCTIONS_AWS_SSL}"
)

###------------------------------------------------------------------------------------------------
## Variables
VARIABLES_REQUIRED=(
    'ssl_certificate_id'
    'ssl_common_name'
)

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hVs:n:r:" OPTION; do
    case $OPTION in
        s) SSL_CERTIFICATE_ID=$OPTARG;;
        n) SSL_COMMON_NAME=$OPTARG;;
        r) AWS_REGION=$OPTARG;;
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

ssl_download_certificate "${SSL_CERTIFICATE_ID}" "${SSL_COMMON_NAME}" "${AWS_REGION}"
RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $RETURNVAL; fi

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
