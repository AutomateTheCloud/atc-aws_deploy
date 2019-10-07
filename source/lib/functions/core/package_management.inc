###------------------------------------------------------------------------------------------------
# script:  package_management.inc
# purpose: Collection of functions related to Package Management (yum, pip, ruby gems)
# version: 1.0.0
#
# function list:
#          - gem_install_packages
#          - pip_is_package_installed
#          - pip_install_packages
#          - yum_install_packages
#          - yum_install_repos
#          - yum_install_rpms
#          - yum_is_package_installed
#          - yum_update_all
#          - yum_update_security
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## FUNCTION: gem_install_packages()
## - performs Ruby Gem Install against specified packages
## - Arguments
##   - $1: Packages Array
function gem_install_packages() {
    local FUNCTION_DESCRIPTION="Gem (Install Packages)"
    if [[ ${1} = *"@"* ]]; then
        declare -a TMP_ARRAY_PACKAGES=("${!1}")
    else
        declare -a TMP_ARRAY_PACKAGES=("${1}")
    fi
    local TMP_PACKAGE=""
    local TMP_PACKAGES_LIST=""

    generate_temp_file DISCARDABLE_VARIABLE "package output log"
    local PACKAGE_OUTPUT_LOG="${DISCARDABLE_VARIABLE}"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    for TMP_PACKAGE in "${TMP_ARRAY_PACKAGES[@]}"; do
        TMP_PACKAGES_LIST="${TMP_PACKAGES_LIST} ${TMP_PACKAGE}"
        log "- ${TMP_PACKAGE}"
    done
    TMP_PACKAGES_LIST="$(trim "${TMP_PACKAGES_LIST}")"

    if(is_empty "${TMP_PACKAGES_LIST}"); then
        log "${FUNCTION_DESCRIPTION}: No Packages specified, skipping installation"
        return 0
    fi

    $(which gem) install ${TMP_PACKAGES_LIST} > ${PACKAGE_OUTPUT_LOG} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${PACKAGE_OUTPUT_LOG}" "output of operation"
    if [ ${RETURNVAL} -ne 0 ]; then
        log_error "${FUNCTION_DESCRIPTION}: Failed to install packages (returned::${RETURNVAL})"
        return $E_GEM_FAILURE
    fi
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: pip_is_package_installed()
## - returns true if specified Pip package is installed, false otherwise
## - Arguments
##   - $1: Package name
function pip_is_package_installed() {
    local PACKAGE_NAME="${1}"
    if (is_empty "${PACKAGE_NAME}"); then return 1; fi
    if $(which pip) list | awk '{print $1}' |grep -q -x ${PACKAGE_NAME} >/dev/null 2>&1; then
        return 0
    else
        return 1
    fi
}

###------------------------------------------------------------------------------------------------
## FUNCTION: pip_install_packages()
## - performs Pip Install
## - Arguments
##   - $1: Packages Array
function pip_install_packages() {
    local FUNCTION_DESCRIPTION="Pip (Install Packages)"
    if [[ ${1} = *"@"* ]]; then
        declare -a TMP_ARRAY_PACKAGES=("${!1}")
    else
        declare -a TMP_ARRAY_PACKAGES=("${1}")
    fi
    local TMP_PACKAGE=""
    local TMP_PACKAGES_LIST=""

    generate_temp_file DISCARDABLE_VARIABLE "package output log"
    local PACKAGE_OUTPUT_LOG="${DISCARDABLE_VARIABLE}"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    for TMP_PACKAGE in "${TMP_ARRAY_PACKAGES[@]}"; do
        TMP_PACKAGES_LIST="${TMP_PACKAGES_LIST} ${TMP_PACKAGE}"
        log "- ${TMP_PACKAGE}"
    done
    TMP_PACKAGES_LIST="$(trim "${TMP_PACKAGES_LIST}")"

    if(is_empty "${TMP_PACKAGES_LIST}"); then
        log "${FUNCTION_DESCRIPTION}: No Packages specified, skipping installation"
        return 0
    fi

    $(which pip) install ${TMP_PACKAGES_LIST} > ${PACKAGE_OUTPUT_LOG} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${PACKAGE_OUTPUT_LOG}" "output of operation"
    if [ ${RETURNVAL} -ne 0 ]; then
        log_error "${FUNCTION_DESCRIPTION}: Failed to install packages (returned::${RETURNVAL})"
        return $E_PIP_FAILURE
    fi
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: yum_install_packages()
## - performs Yum Install against specified packages
## - Arguments
##   - $1: Packages Array
function yum_install_packages() {
    local FUNCTION_DESCRIPTION="Yum (Install Packages)"
    if [[ ${1} = *"@"* ]]; then
        declare -a TMP_ARRAY_PACKAGES=("${!1}")
    else
        declare -a TMP_ARRAY_PACKAGES=("${1}")
    fi
    local TMP_PACKAGE=""
    local TMP_PACKAGES_LIST=""

    generate_temp_file DISCARDABLE_VARIABLE "package output log"
    local PACKAGE_OUTPUT_LOG="${DISCARDABLE_VARIABLE}"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    for TMP_PACKAGE in "${TMP_ARRAY_PACKAGES[@]}"; do
        TMP_PACKAGES_LIST="${TMP_PACKAGES_LIST} ${TMP_PACKAGE}"
        log "- ${TMP_PACKAGE}"
    done
    TMP_PACKAGES_LIST="$(trim "${TMP_PACKAGES_LIST}")"

    if(is_empty "${TMP_PACKAGES_LIST}"); then
        log "${FUNCTION_DESCRIPTION}: No Packages specified, skipping installation"
        return 0
    fi

    $(which yum) -y install ${TMP_PACKAGES_LIST} > ${PACKAGE_OUTPUT_LOG} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${PACKAGE_OUTPUT_LOG}" "output of operation"
    if [ ${RETURNVAL} -ne 0 ]; then
        log_error "${FUNCTION_DESCRIPTION}: Failed to install packages (returned::${RETURNVAL})"
        return $E_YUM_FAILURE
    fi
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: yum_install_repos()
## - Copies repo files to /etc/yum.repos/
## - Arguments
##   - $1: Repos Array
##   - $2: Base directory where files live
function yum_install_repos() {
    local FUNCTION_DESCRIPTION="Yum (Install Repos)"
    if [[ ${1} = *"@"* ]]; then
        declare -a TMP_ARRAY_REPOS=("${!1}")
    else
        declare -a TMP_ARRAY_REPOS=("${1}")
    fi
    local TMP_BASE_REPO_DIRECTORY="${2}"
    local TMP_REPO=""

    TMP_BASE_REPO_DIRECTORY="$(echo "${TMP_BASE_REPO_DIRECTORY}" | sed 's:/*$::')"
    if(is_empty "${TMP_BASE_REPO_DIRECTORY}"); then
        log_error "${FUNCTION_DESCRIPTION}: Repo Base directory not defined"
        return $E_BAD_ARGS
    fi

    log_notice "${FUNCTION_DESCRIPTION}: started"
    for TMP_REPO in "${TMP_ARRAY_REPOS[@]}"; do
        log "- ${TMP_REPO}"
        if [ ! -f "${TMP_BASE_REPO_DIRECTORY}/${TMP_REPO}" ]; then
            log_error "${FUNCTION_DESCRIPTION}: Repo file does not exist [${TMP_BASE_REPO_DIRECTORY}/${TMP_REPO}]"
            return $E_OBJECT_NOT_FOUND
        fi
        /bin/cp -f ${TMP_BASE_REPO_DIRECTORY}/${TMP_REPO} /etc/yum.repos.d/
        RETURNVAL="$?"
        if [ ${RETURNVAL} -ne 0 ]; then
            log_error "${FUNCTION_DESCRIPTION}: Failed to copy Repo [${TMP_BASE_REPO_DIRECTORY}/${TMP_REPO}] (cp_returned::${RETURNVAL})"
            return $E_YUM_FAILURE
        fi
    done
    $(which yum) clean all > /dev/null 2>&1
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: yum_install_rpms()
## - performs Yum Install against specified RPMs
## - Arguments
##   - $1: RPMs Array
##   - $2: RPM Directory
function yum_install_rpms() {
    local FUNCTION_DESCRIPTION="Yum (Install RPMs)"
    if [[ ${1} = *"@"* ]]; then
        declare -a TMP_ARRAY_RPMS=("${!1}")
    else
        declare -a TMP_ARRAY_RPMS=("${1}")
    fi
    local TMP_RPM_DIRECTORY="${2}"
    TMP_RPM_DIRECTORY="$(echo "${TMP_RPM_DIRECTORY}" | sed 's:/*$::')"
    local TMP_RPM=""
    local TMP_PACKAGES_LIST=""

    if (is_empty "${TMP_RPM_DIRECTORY}"); then
        log_error "${FUNCTION_DESCRIPTION}: RPM Directory not specified"
        return $E_BAD_ARGS
    fi

    generate_temp_file DISCARDABLE_VARIABLE "RPMs output log"
    local RPM_OUTPUT_LOG="${DISCARDABLE_VARIABLE}"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    for TMP_RPM in "${TMP_ARRAY_RPMS[@]}"; do
        TMP_PACKAGES_LIST="${TMP_PACKAGES_LIST} ${TMP_RPM_DIRECTORY}/${TMP_RPM}"
        log "- ${TMP_RPM}"
    done
    TMP_PACKAGES_LIST="$(trim "${TMP_PACKAGES_LIST}")"

    if(is_empty "${TMP_PACKAGES_LIST}"); then
        log "${FUNCTION_DESCRIPTION}: No Packages specified, skipping installation"
        return 0
    fi

    $(which yum) -y localinstall ${TMP_PACKAGES_LIST} > ${RPM_OUTPUT_LOG} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${RPM_OUTPUT_LOG}" "output of operation"
    if [ ${RETURNVAL} -ne 0 ]; then
        log_error "${FUNCTION_DESCRIPTION}: Failed to install RPMs (returned::${RETURNVAL})"
        return $E_YUM_FAILURE
    fi
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: yum_is_package_installed()
## - returns true if specified Yum package is installed, false otherwise
## - Arguments
##   - $1: Package name
function yum_is_package_installed() {
    local PACKAGE_NAME="${1}"
    if (is_empty "${PACKAGE_NAME}"); then return 1; fi
    if yum list installed "${PACKAGE_NAME}" >/dev/null 2>&1; then
        return 0
    else
        return 1
    fi
}

###------------------------------------------------------------------------------------------------
## FUNCTION: yum_update_all()
## - performs Yum Update
function yum_update_all() {
    local FUNCTION_DESCRIPTION="Yum (Update All)"
    generate_temp_file DISCARDABLE_VARIABLE "package output log"
    local PACKAGE_OUTPUT_LOG="${DISCARDABLE_VARIABLE}"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    $(which yum) -y update > ${PACKAGE_OUTPUT_LOG} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${PACKAGE_OUTPUT_LOG}" "output of operation"
    if [ ${RETURNVAL} -ne 0 ]; then
        log_error "${FUNCTION_DESCRIPTION}: Failed to perform update (returned::${RETURNVAL})"
        return $E_YUM_FAILURE
    fi
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: yum_update_security()
## - performs Yum Update for Security related packages only
function yum_update_security() {
    local FUNCTION_DESCRIPTION="Yum (Update Security)"
    generate_temp_file DISCARDABLE_VARIABLE "package output log"
    local PACKAGE_OUTPUT_LOG="${DISCARDABLE_VARIABLE}"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    $(which yum) -y --security update > ${PACKAGE_OUTPUT_LOG} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${PACKAGE_OUTPUT_LOG}" "output of operation"
    if [ ${RETURNVAL} -ne 0 ]; then
        log_error "${FUNCTION_DESCRIPTION}: Failed to perform update (returned::${RETURNVAL})"
        return $E_YUM_FAILURE
    fi
    return 0
}
