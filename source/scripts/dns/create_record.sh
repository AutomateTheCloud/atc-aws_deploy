#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.dns.create_record
# script:  create_record.sh
# purpose: Creates Route53 DNS Record
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="dns_create_record"
declare -r SELF_IDENTITY_H="DNS (Create Record)"
declare -a ARGUMENTS_DESCRIPTION=(
    '-n <record_name> : Record Name'
    '-t <record_type> : Record Type (A/CNAME/etc)'
    '-r <resource> : Record Resource'
    '-T <ttl_in_seconds> : Record TTL'
    '-z <route53_zone_id>: Zone ID'
)

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=(
    "${LIB_FUNCTIONS_AWS_METADATA}"
    "${LIB_FUNCTIONS_AWS_ROUTE53}"
)

###------------------------------------------------------------------------------------------------
## Variables
VARIABLES_REQUIRED=(
    'record_name'
    'record_type'
    'record_resource'
    'record_ttl'
    'record_zone_id'
)

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hVn:t:r:T:z:" OPTION; do
    case $OPTION in
        n)  RECORD_NAME=$OPTARG;;
        t)  RECORD_TYPE=$OPTARG;;
        r)  RECORD_RESOURCE=$OPTARG;;
        T)  RECORD_TTL=$OPTARG;;
        z)  RECORD_ZONE_ID=$OPTARG;;
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

if(is_empty "${RECORD_TTL}"); then
    RECORD_TTL="${AWS_ROUTE53_DEFAULT_TTL}"
fi

verify_array_key_values "VARIABLES_REQUIRED[@]"
RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $RETURNVAL; fi

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

route53_upsert_record "${RECORD_ZONE_ID}" "${RECORD_NAME}" "${RECORD_TYPE}" "${RECORD_TTL}" "${RECORD_RESOURCE}"
RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $RETURNVAL; fi

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
