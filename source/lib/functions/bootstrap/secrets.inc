###--------------------------------------------------------------------------------
# script:  secrets.inc
# purpose: AWS Deploy Secrets Related
# version: 1.0.0
#
# function list:
#          - retrieve_application_info_file
#          - retrieve_application_secrets
#          - retrieve_secrets_files
#          - retrieve_secrets_files_global
#          - retrieve_secrets_source
#          - retrieve_secrets_source_files
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

###------------------------------------------------------------------------------------------------
## FUNCTION: retrieve_application_secrets()
## - Retrieves Application Secrets and dumps to specified file
## - Arguments
##   - $1: Output File
function retrieve_application_secrets() {
    local FUNCTION_DESCRIPTION="Retrieve Application Secrets"
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

###------------------------------------------------------------------------------------------------
## FUNCTION: retrieve_secrets_files()
## - Retrieves Secrets Files from Array specification
## - Arguments
##   - $1: Secrets Files Array
function retrieve_secrets_files() {
    local FUNCTION_DESCRIPTION="Retrieve Secrets Files"
    declare -a SECRETS_FILES_ARRAY=("${!1}")
    local AWS_INFO_VARIABLES_REQUIRED=(
        'region'
        'environment'
        'project_abbr'
        'function_abbr'
    )
    local RETURNVAL=""
    local TMP_PARAMETER_PATH=""
    local TMP_PARAMETER=""
    local TMP_PARAMETER_FILE=""
    local TMP_OUTPUT_FILE=""

    load_array_properties_from_file "AWS_INFO_VARIABLES_REQUIRED[@]" "${FILE_DEPLOY_INFO_AWS}" "TMP_AWS_INFO"
    verify_array_key_values "AWS_INFO_VARIABLES_REQUIRED[@]" "TMP_AWS_INFO"
    RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $E_BAD_ARGS; fi
    TMP_PARAMETER_PATH="/secrets/${TMP_AWS_INFO_PROJECT_ABBR}/${TMP_AWS_INFO_FUNCTION_ABBR}/${TMP_AWS_INFO_ENVIRONMENT}/files"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    log "- Parameter Path: [${TMP_PARAMETER_PATH}]"
    log "- AWS Region:     [${TMP_AWS_INFO_REGION}]"
    log "- File Count      [${#SECRETS_FILES_ARRAY[@]}]"
    line_break

    for TMP_PARAMETER in "${SECRETS_FILES_ARRAY[@]}"; do
        TMP_PARAMETER_FILE="$(echo "${TMP_PARAMETER}" | awk -F: '{print $1}' | sed -e 's/^ *//g' -e 's/ *$//g')"
        TMP_OUTPUT_FILE="$(echo "${TMP_PARAMETER}" | awk -F: '{print $2}' | sed -e 's/^ *//g' -e 's/ *$//g')"
        log "${FUNCTION_DESCRIPTION}: Retrieving File: [${TMP_PARAMETER_FILE} => ${TMP_OUTPUT_FILE}]"
        parameter_get_file_multi_part "${TMP_OUTPUT_FILE}" "${TMP_PARAMETER_PATH}/${TMP_PARAMETER_FILE}" yes "${TMP_AWS_INFO_REGION}"
        RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi
    done

    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: retrieve_secrets_files_global()
## - Retrieves Secrets Files from Array specification (from Global path)
## - Arguments
##   - $1: Secrets Files Array
function retrieve_secrets_files_global() {
    local FUNCTION_DESCRIPTION="Retrieve Secrets Files (Global)"
    declare -a SECRETS_FILES_ARRAY=("${!1}")
    local AWS_INFO_VARIABLES_REQUIRED=(
        'region'
        'environment'
        'project_abbr'
        'function_abbr'
    )
    local RETURNVAL=""
    local TMP_PARAMETER_PATH=""
    local TMP_PARAMETER=""
    local TMP_PARAMETER_FILE=""
    local TMP_OUTPUT_FILE=""

    load_array_properties_from_file "AWS_INFO_VARIABLES_REQUIRED[@]" "${FILE_DEPLOY_INFO_AWS}" "TMP_AWS_INFO"
    verify_array_key_values "AWS_INFO_VARIABLES_REQUIRED[@]" "TMP_AWS_INFO"
    RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $E_BAD_ARGS; fi
    TMP_PARAMETER_PATH="/secrets/${TMP_AWS_INFO_PROJECT_ABBR}/global/${TMP_AWS_INFO_ENVIRONMENT}/files"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    log "- Parameter Path: [${TMP_PARAMETER_PATH}]"
    log "- AWS Region:     [${TMP_AWS_INFO_REGION}]"
    log "- File Count      [${#SECRETS_FILES_ARRAY[@]}]"
    line_break

    for TMP_PARAMETER in "${SECRETS_FILES_ARRAY[@]}"; do
        TMP_PARAMETER_FILE="$(echo "${TMP_PARAMETER}" | awk -F: '{print $1}' | sed -e 's/^ *//g' -e 's/ *$//g')"
        TMP_OUTPUT_FILE="$(echo "${TMP_PARAMETER}" | awk -F: '{print $2}' | sed -e 's/^ *//g' -e 's/ *$//g')"
        log "${FUNCTION_DESCRIPTION}: Retrieving File: [${TMP_PARAMETER_FILE} => ${TMP_OUTPUT_FILE}]"
        parameter_get_file_multi_part "${TMP_OUTPUT_FILE}" "${TMP_PARAMETER_PATH}/${TMP_PARAMETER_FILE}" yes "${TMP_AWS_INFO_REGION}"
        RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi
    done

    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: retrieve_secrets_source()
## - Retrieves Secrets from Source and dumps to specified file
## - Arguments
##   - $1: Output File
##   - $2: Project Abbreviation
##   - $3: Function Abbreviation
##   - $4: Environment
##   - $5: AWS Region
function retrieve_secrets_source() {
    local FUNCTION_DESCRIPTION="Retrieve Secrets (Source)"
    local TMP_FILE_OUTPUT="${1}"
    local TMP_SOURCE_PROJECT_ABBR="${2}"
    local TMP_SOURCE_FUNCTION_ABBR="${3}"
    local TMP_SOURCE_ENVIRONMENT="${4}"
    local TMP_SOURCE_REGION="${5}"

    local SOURCE_VARIABLES_REQUIRED=(
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

    TMP_PARAMETER_PATH_GLOBAL="/secrets/${TMP_SOURCE_PROJECT_ABBR}/global/${TMP_SOURCE_ENVIRONMENT}/"
    TMP_PARAMETER_PATH_FUNCTION="/secrets/${TMP_SOURCE_PROJECT_ABBR}/${TMP_SOURCE_FUNCTION_ABBR}/${TMP_SOURCE_ENVIRONMENT}/"

    generate_temp_file TMP_FILE_APPLICATION_VARS_GLOBAL "application vars file (global)"
    generate_temp_file TMP_FILE_APPLICATION_VARS_FUNCTION "application vars file (function)"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    log "- Output File:               [${TMP_FILE_OUTPUT}]"
    log "- Parameter Path (Global):   [${TMP_PARAMETER_PATH_GLOBAL}]"
    log "- Parameter Path (Function): [${TMP_PARAMETER_PATH_FUNCTION}]"
    log "- AWS Region:                [${TMP_SOURCE_REGION}]"

    log "${FUNCTION_DESCRIPTION}: retrieving parameters (Global)"
    parameters_to_properties_file "${TMP_PARAMETER_PATH_GLOBAL}" "${TMP_FILE_APPLICATION_VARS_GLOBAL}" yes "${TMP_SOURCE_REGION}"
    RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi

    log "${FUNCTION_DESCRIPTION}: retrieving parameters (Function)"
    parameters_to_properties_file "${TMP_PARAMETER_PATH_FUNCTION}" "${TMP_FILE_APPLICATION_VARS_FUNCTION}" yes "${TMP_SOURCE_REGION}"
    RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi

    log "${FUNCTION_DESCRIPTION}: compiling Application Info file"
    cat ${TMP_FILE_APPLICATION_VARS_GLOBAL} ${TMP_FILE_APPLICATION_VARS_FUNCTION} | grep -v "^#" | sort | uniq > ${TMP_FILE_OUTPUT}

    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: retrieve_secrets_source_files()
## - Retrieves Secrets Files from Array specification
## - Arguments
##   - $1: Secrets Files Array
##   - $2: Project Abbreviation
##   - $3: Function Abbreviation
##   - $4: Environment
##   - $5: AWS Region
function retrieve_secrets_source_files() {
    local FUNCTION_DESCRIPTION="Retrieve Secrets Files"
    declare -a SECRETS_FILES_ARRAY=("${!1}")
    local TMP_SOURCE_PROJECT_ABBR="${2}"
    local TMP_SOURCE_FUNCTION_ABBR="${3}"
    local TMP_SOURCE_ENVIRONMENT="${4}"
    local TMP_SOURCE_REGION="${5}"
    local SOURCE_VARIABLES_REQUIRED=(
        'region'
        'environment'
        'project_abbr'
        'function_abbr'
    )
    local RETURNVAL=""
    local TMP_PARAMETER_PATH=""
    local TMP_PARAMETER=""
    local TMP_PARAMETER_FILE=""
    local TMP_OUTPUT_FILE=""
    
    TMP_PARAMETER_PATH="/secrets/${TMP_SOURCE_PROJECT_ABBR}/${TMP_SOURCE_FUNCTION_ABBR}/${TMP_SOURCE_ENVIRONMENT}/files"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    log "- Parameter Path: [${TMP_PARAMETER_PATH}]"
    log "- AWS Region:     [${TMP_SOURCE_REGION}]"
    log "- File Count      [${#SECRETS_FILES_ARRAY[@]}]"
    line_break

    for TMP_PARAMETER in "${SECRETS_FILES_ARRAY[@]}"; do
        TMP_PARAMETER_FILE="$(echo "${TMP_PARAMETER}" | awk -F: '{print $1}' | sed -e 's/^ *//g' -e 's/ *$//g')"
        TMP_OUTPUT_FILE="$(echo "${TMP_PARAMETER}" | awk -F: '{print $2}' | sed -e 's/^ *//g' -e 's/ *$//g')"
        log "${FUNCTION_DESCRIPTION}: Retrieving File: [${TMP_PARAMETER_FILE} => ${TMP_OUTPUT_FILE}]"
        parameter_get_file_multi_part "${TMP_OUTPUT_FILE}" "${TMP_PARAMETER_PATH}/${TMP_PARAMETER_FILE}" yes "${TMP_SOURCE_REGION}"
        RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi
    done

    return 0
}