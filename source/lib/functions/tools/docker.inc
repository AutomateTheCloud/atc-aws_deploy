###------------------------------------------------------------------------------------------------
# script:  docker.inc
# purpose: Collection of functions related to Docker
# version: 1.0.0
#
# function list:
#          - docker_compose_down
#          - docker_compose_up
#          - docker_create_user
#          - docker_install
#          - docker_reset
#          - docker_reset_images
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
# Variables
DOCKER_DIRECTORY="/apps/docker"

DOCKER_YUM_PACKAGES=(
    'docker'
)

DOCKER_GID="2001"
DOCKER_UID="2001"
DOCKER_GROUPNAME="docker"
DOCKER_USERNAME="docker"

###------------------------------------------------------------------------------------------------
## FUNCTION: docker_compose_down()
## - Stops Docker containers according to Docker-Compose file
## - Arguments
##   - $1: Docker Compose file
function docker_compose_down() {
    local FUNCTION_DESCRIPTION="Docker Compose (Down)"
    local TMP_FILE_DOCKER_COMPOSE="${1}"
    local DOCKER_OUTPUT=""

    if(is_empty "${TMP_FILE_DOCKER_COMPOSE}"); then
        log_error "${FUNCTION_DESCRIPTION}: Docker Compose file not specified"
        return $E_BAD_ARGS
    fi
    if [ ! -f "${TMP_FILE_DOCKER_COMPOSE}" ]; then
        log_error "${FUNCTION_DESCRIPTION}: Docker Compose file not found [${TMP_FILE_DOCKER_COMPOSE}]"
        return $E_OBJECT_NOT_FOUND
    fi

    generate_temp_file DOCKER_OUTPUT "docker output"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    log ">> Docker Compose: [${TMP_FILE_DOCKER_COMPOSE}]"
    log_add_from_file "${TMP_FILE_DOCKER_COMPOSE}" "${FUNCTION_DESCRIPTION}: Docker Compose file"

    $(which docker-compose) --no-ansi -f "${TMP_FILE_DOCKER_COMPOSE}" down >${DOCKER_OUTPUT} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${DOCKER_OUTPUT}" "${FUNCTION_DESCRIPTION}: Docker Compose (output)"
    if [ ${RETURNVAL} -ne 0 ]; then
        TMP_ERROR_MSG="failed to execute Docker Compose (down) (returned::${RETURNVAL})"
        log_error ">> ${TMP_ERROR_MSG}"
        return $E_DOCKER_IMAGE_FAILURE
    fi
    log "${FUNCTION_DESCRIPTION}: successfully executed Docker Compose (down)"
    return 0
}


###------------------------------------------------------------------------------------------------
## FUNCTION: docker_compose_up()
## - Starts Docker containers according to Docker-Compose file
## - Arguments
##   - $1: Docker Compose file
function docker_compose_up() {
    local FUNCTION_DESCRIPTION="Docker Compose (Up)"
    local TMP_FILE_DOCKER_COMPOSE="${1}"
    local DOCKER_OUTPUT=""

    if(is_empty "${TMP_FILE_DOCKER_COMPOSE}"); then
        log_error "${FUNCTION_DESCRIPTION}: Docker Compose file not specified"
        return $E_BAD_ARGS
    fi
    if [ ! -f "${TMP_FILE_DOCKER_COMPOSE}" ]; then
        log_error "${FUNCTION_DESCRIPTION}: Docker Compose file not found [${TMP_FILE_DOCKER_COMPOSE}]"
        return $E_OBJECT_NOT_FOUND
    fi

    generate_temp_file DOCKER_OUTPUT "docker output"

    log_notice "${FUNCTION_DESCRIPTION}: started"
    log ">> Docker Compose: [${TMP_FILE_DOCKER_COMPOSE}]"
    log_add_from_file "${TMP_FILE_DOCKER_COMPOSE}" "${FUNCTION_DESCRIPTION}: Docker Compose file"

    $(which docker-compose) --no-ansi -f "${TMP_FILE_DOCKER_COMPOSE}" up -d >${DOCKER_OUTPUT} 2>&1
    RETURNVAL="$?"
    log_add_from_file "${DOCKER_OUTPUT}" "${FUNCTION_DESCRIPTION}: Docker Compose (output)"
    if [ ${RETURNVAL} -ne 0 ]; then
        TMP_ERROR_MSG="failed to execute Docker Compose (up) (returned::${RETURNVAL})"
        log_error ">> ${TMP_ERROR_MSG}"
        return $E_DOCKER_IMAGE_FAILURE
    fi
    log "${FUNCTION_DESCRIPTION}: successfully executed Docker Compose (up)"
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: docker_create_user()
## - Creates Docker User and Group
function docker_create_user() {
    local FUNCTION_DESCRIPTION="Docker (Create User)"

    log_notice "${SELF_IDENTITY_H}: Creating Docker User"
    $(which groupadd) -g ${DOCKER_GID} ${DOCKER_GROUPNAME} >/dev/null 2>&1
    $(which useradd) -u ${DOCKER_UID} -g ${DOCKER_GID} -m -s /bin/bash ${DOCKER_USERNAME} >/dev/null 2>&1

    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: docker_install()
## - Installs Docker and Support processes
function docker_install() {
    local FUNCTION_DESCRIPTION="Docker (Install)"

    log_notice "${FUNCTION_DESCRIPTION}: installing Docker"
    yum_install_packages "DOCKER_YUM_PACKAGES[@]"
    RETURNVAL="$?"
    if [ ${RETURNVAL} -ne 0 ]; then return $RETURNVAL; fi

    log_notice "${FUNCTION_DESCRIPTION}: installing Docker Compose"
    local DOCKER_COMPOSE_URL="$(curl -s https://api.github.com/repos/docker/compose/releases/latest | grep "browser_download_url.*docker-compose-$(uname -s)-$(uname -m)\"" | cut -d : -f 2,3 | tr -d \" | awk '{$1=$1};1')"
    curl -L ${DOCKER_COMPOSE_URL} -o /usr/local/bin/docker-compose >/dev/null 2>&1
    chmod +x /usr/local/bin/docker-compose

    log_notice "${FUNCTION_DESCRIPTION}: configuring Docker"
    $(which mkdir) -p ${DOCKER_DIRECTORY} >/dev/null 2>&1
    $(which mkdir) -p /etc/docker/ >/dev/null 2>&1
cat << EOF > /etc/docker/daemon.json
{
    "graph": "${DOCKER_DIRECTORY}"
}
EOF

    log_notice "${FUNCTION_DESCRIPTION}: starting Docker"
    sleep 1; sync; wait; sleep 1
    /bin/systemctl restart docker.service >/dev/null 2>&1
    RETURNVAL="$?"
    if [ ${RETURNVAL} -ne 0 ]; then
        TMP_ERROR_MSG="failed to start Docker (returned::${RETURNVAL})"
        log_error ">> ${TMP_ERROR_MSG}"
        exit_logic $E_SERVICE_FAILURE "${TMP_ERROR_MSG}"
    fi
    log "${FUNCTION_DESCRIPTION}: successfully installed Docker"
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: docker_reset()
## - Resets Docker setup, stopping and removing present containers
function docker_reset() {
    local FUNCTION_DESCRIPTION="Docker (Reset)"
    if which docker > /dev/null 2>&1; then
        log_notice "${FUNCTION_DESCRIPTION}: removing all containers"
        # Stop all running containers
        $(which docker) stop $($(which docker) ps -aq) >/dev/null 2>&1
        # Remove all containers
        $(which docker) rm $($(which docker) ps -aq) >/dev/null 2>&1
        call_sleep 1
    fi
    return 0
}

###------------------------------------------------------------------------------------------------
## FUNCTION: docker_reset_images()
## - Removes all Docker Images
function docker_reset_images() {
    local FUNCTION_DESCRIPTION="Docker (Reset Images)"
    if which docker > /dev/null 2>&1; then
        log_notice "${FUNCTION_DESCRIPTION}: removing all images"
        # Remove all images
        $(which docker) rmi $($(which docker) images -q) >/dev/null 2>&1
        call_sleep 1
    fi
    return 0
}
