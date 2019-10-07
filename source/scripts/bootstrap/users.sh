#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.bootstrap.users
# script:  users.sh
# purpose: Configures Additional Users
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="bootstrap_users"
declare -r SELF_IDENTITY_H="Bootstrap (Users)"
declare -a ARGUMENTS_DESCRIPTION=(
    '-U <user_list> : User List (username:admin_flag_yes_no), semicolon delimitted'
)

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=()

###------------------------------------------------------------------------------------------------
## Variables
VARIABLES_REQUIRED=(
    'user_list'
)

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hVU:" OPTION; do
    case $OPTION in
        U) USER_LIST=$OPTARG;;
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

verify_array_key_values "VARIABLES_REQUIRED[@]"
RETURNVAL="$?"; if [ ${RETURNVAL} -ne 0 ]; then exit_logic $E_BAD_ARGS; fi

line_break
log "- Users:"
for TMP_OBJECT in $(echo "${USER_LIST}" | sed "s/;/ /g"); do
    TMP_USER="$(trim "$(echo "${TMP_OBJECT}" | awk -F: '{print $1}')")"
    TMP_ADMIN_FLAG="$(trim "$(echo "${TMP_OBJECT}" | awk -F: '{print $2}')")"
    if(! is_empty "${TMP_USER}"); then
        if option_enabled TMP_ADMIN_FLAG; then
            log "  - ${TMP_USER} (admin)"
        else
            log "  - ${TMP_USER}"
        fi
    fi
done

## END Initialize
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Execution
line_break
log_highlight "Execution"

log_notice "${SELF_IDENTITY_H}: Creating Users"
for TMP_OBJECT in $(echo "${USER_LIST}" | sed "s/;/ /g"); do
    TMP_USER="$(trim "$(echo "${TMP_OBJECT}" | awk -F: '{print $1}')")"
    TMP_ADMIN_FLAG="$(trim "$(echo "${TMP_OBJECT}" | awk -F: '{print $2}')")"
    if(! is_empty "${TMP_USER}"); then
        log "- [${TMP_USER}]"
        if id -u "${TMP_USER}" > /dev/null 2>&1; then
            log_warning "User already exists, skipping"
        else
            $(which useradd) ${TMP_USER} > /dev/null 2>&1
            $(which usermod) -a -G ec2-user ${TMP_USER} > /dev/null 2>&1
        fi
        if option_enabled TMP_ADMIN_FLAG; then
            log "  - Admin, creating sudoers file"
            echo "${TMP_USER} ALL=(ALL) NOPASSWD:ALL" > "/etc/sudoers.d/${TMP_USER/./_}"
        fi
    fi
done

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
