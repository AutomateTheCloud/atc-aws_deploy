###--------------------------------------------------------------------------------
# script:  info_files.inc
# purpose: AWS Deploy Bootstrap Information File Info
# version: 1.0.0
#
# function list:
#          - retrieve_application_info_file
###------------------------------------------------------------------------------------------------
REQUIRED_EXECUTABLES+=()

###--------------------------------------------------------------------------------
# Variables

###--------------------------------------------------------------------------------
# Application
FILE_DEPLOY_INFO_APPLICATION="${DIRECTORY_DEPLOY}/application.info"

###--------------------------------------------------------------------------------
# AWS
FILE_DEPLOY_INFO_AWS="${DIRECTORY_DEPLOY}/aws.info"
ARRAY_DEPLOY_INFO_AWS=(
    'stack_id'
    'stack_name'
    'aws_account_id'
    'org_name'
    'org_abbr'
    'region'
    'environment'
    'project_name'
    'project_abbr'
    'function_name'
    'function_abbr'
    'owner'
    'contact'
    'vpc_stack_name'
)

###------------------------------------------------------------------------------------------------
## FUNCTION: retrieve_application_info_file()
## - Retrieves Application Info and dumps to specified file
## - Arguments
##   - $1: Output File
function retrieve_application_info_file() {
    local FUNCTION_DESCRIPTION="Retrieve Application Info File"
    local TMP_FILE_OUTPUT="${1}"

    local AWS_INFO_VARIABLES_REQUIRED=(
        'region'
        'environment'
        'project_abbr'
        'function_abbr'
    )
    local RETURNVAL=""
    local TMP_PARAMETER_PATH_GLOBAL=""
    local TMP_PARAMETER_PATH_FUNCTION=""
    local TMP_FILE_APPLICATION_VARS_GLOBAL=""
    local TMP_FILE_APPLICATION_VARS_FUNCTION=""

    if(is_empty "${TMP_FILE_OUTPUT}"); then
        log_error "${FUNCTION_DESCRIPTION}: Output File not specified"
        return $E_BAD_ARGS
    fi
    if ! $(which touch) "${TMP_FILE_OUTPUT}"  > /dev/null 2>&1; then
        log_error "- Cannot write to specified Output file [${TMP_FILE_OUTPUT}]"
        return $E_OBJECT_FAILED_TO_CREATE
    fi

    load_array_properties_from_file "AWS_INFO_VARIABLES_REQUIRED[@]" "${FILE_DEPLOY_INFO_AWS}" "TMP_AWS_INFO"
    verify_array_key_values "AWS_INFO_VARIABLES_REQUIRED[@]" "TMP_AWS_INFO"
    RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $E_BAD_ARGS; fi

    TMP_PARAMETER_PATH_GLOBAL="/secrets/${TMP_AWS_INFO_PROJECT_ABBR}/global/${TMP_AWS_INFO_ENVIRONMENT}/"
    TMP_PARAMETER_PATH_FUNCTION="/secrets/${TMP_AWS_INFO_PROJECT_ABBR}/${TMP_AWS_INFO_FUNCTION_ABBR}/${TMP_AWS_INFO_ENVIRONMENT}/"

    generate_temp_file TMP_FILE_APPLICATION_VARS_GLOBAL "application vars file (global)"
    generate_temp_file TMP_FILE_APPLICATION_VARS_FUNCTION "application vars file (function)"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    log "- Output File:               [${TMP_FILE_OUTPUT}]"
    log "- Parameter Path (Global):   [${TMP_PARAMETER_PATH_GLOBAL}]"
    log "- Parameter Path (Function): [${TMP_PARAMETER_PATH_FUNCTION}]"
    log "- AWS Region:                [${TMP_AWS_INFO_REGION}]"

    log "${FUNCTION_DESCRIPTION}: retrieving parameters (Global)"
    parameters_to_properties_file "${TMP_PARAMETER_PATH_GLOBAL}" "${TMP_FILE_APPLICATION_VARS_GLOBAL}" yes "${TMP_AWS_INFO_REGION}"
    RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi

    log "${FUNCTION_DESCRIPTION}: retrieving parameters (Function)"
    parameters_to_properties_file "${TMP_PARAMETER_PATH_FUNCTION}" "${TMP_FILE_APPLICATION_VARS_FUNCTION}" yes "${TMP_AWS_INFO_REGION}"
    RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi

    log "${FUNCTION_DESCRIPTION}: compiling Application Info file"
    cat ${TMP_FILE_APPLICATION_VARS_GLOBAL} ${TMP_FILE_APPLICATION_VARS_FUNCTION} | grep -v "^#" | sort | uniq > ${TMP_FILE_OUTPUT}

    return 0
}
