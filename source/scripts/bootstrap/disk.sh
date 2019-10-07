#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.bootstrap.disk
# script:  disk.sh
# purpose: Builds Dynamic Disk (/apps) for use within AWS EC2 Environments
# version: 1.0.0
#
# notes:
#          - Will use all additional disks (anything not /dev/sda) to generate a RAID0 Array
#          - Disk will be mounted to /apps
#          - Created disk will be LUKS encrypted, after which the luks.password will be destroyed
#            - This means data recovery on /apps will be impossible after a reboot
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Config
declare -r SELF_IDENTITY="bootstrap_disk"
declare -r SELF_IDENTITY_H="Bootstrap (Disk)"
declare -a ARGUMENTS_DESCRIPTION=(
    '-f : Force mount creation'
)

###------------------------------------------------------------------------------------------------
## Load Defaults
declare -r GLOBAL_CONFIG_FILE="/opt/aws_deploy/config/global.config"
source "${GLOBAL_CONFIG_FILE}" || exit 3

###------------------------------------------------------------------------------------------------
## Declare dependencies
REQUIRED_SOURCE_FILES+=("${LIB_FUNCTIONS_AWS_METADATA}")
REQUIRED_EXECUTABLES+=('mount' 'umount' 'cryptsetup')

###------------------------------------------------------------------------------------------------
## Variables
FORCE_OVERWRITE=no
DISK_AWS_EPHEMERAL="/dev/md0"
DEVICE_AWS_EPHEMERAL="ephemeral"
MOUNTPOINT_AWS_TARGET="/apps"
FILE_LUKS_PASSWORD="/root/.luks.pass"
RAID_ARRAYS=""
ROOT_DRIVE=""
DRIVE_SCHEME=""
DRIVES=""
DEVICE_NAME=""
DEVICE_PATH=""
EPHEMERAL_COUNT=0
EPHEMERALS=""

###------------------------------------------------------------------------------------------------
## Main
# Process Arguments
while getopts "hVf" OPTION; do
    case $OPTION in
        f) FORCE_OVERWRITE=yes;;
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

if $(which mount) | grep "on ${MOUNTPOINT_AWS_TARGET} type" >/dev/null; then
    if option_enabled FORCE_OVERWRITE; then
        log_notice "${SELF_IDENTITY_H}: Mount Point target already mounted [${MOUNTPOINT_AWS_TARGET}], (overwrite flag [-f] specified)"
        log "- Attempting to unmount [${MOUNTPOINT_AWS_TARGET}]"
        $(which umount) -l ${MOUNTPOINT_AWS_TARGET} >/dev/null 2>&1
        call_sleep 1
        sync_disks
        call_sleep 1
        if $(which mount) | grep "on ${MOUNTPOINT_AWS_TARGET} type" >/dev/null; then
            log_error "${SELF_IDENTITY_H}: Failed to unmount Mount Point target, aborting operation"
            exit_logic $E_OBJECT_ALREADY_EXISTS
        fi
    else
        log_error "${SELF_IDENTITY_H}: Mount Point target already mounted [${MOUNTPOINT_AWS_TARGET}], aborting operation"
        exit_logic $E_OBJECT_ALREADY_EXISTS
    fi
fi

## END Initialize
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Preflight
line_break
log_highlight "Preflight"

log_notice "${SELF_IDENTITY_H}: Detecting Disk Information"
if [ -h /dev/mapper/${DEVICE_AWS_EPHEMERAL} ]; then
    log "- Encrypted disk detected, removing disk"
    $(which cryptsetup) luksClose ${DEVICE_AWS_EPHEMERAL} >/dev/null 2>&1
    call_sleep 1
    sync_disks
    call_sleep 1
fi

RAID_ARRAYS="$(cat /proc/mdstat |grep "^md" | awk '{print $1}')"
for a in ${RAID_ARRAYS}; do
    log "- RAID disk detected, removing RAID array [/dev/${a}]"
    $(which mdadm) --stop /dev/${a} >/dev/null 2>&1
    call_sleep 1
    sync_disks
    call_sleep 1
done

# Configure Raid - take into account xvdb or sdb
ROOT_DRIVE="$(df -h | grep -v grep | awk 'NR==2{print $1}')"
 if [ "${ROOT_DRIVE}" == "/dev/xvda1" ]; then
    log "- Detected 'xvd' drive naming scheme [root: ${ROOT_DRIVE}]"
    DRIVE_SCHEME='xvd'
else
    log "- Detected 'sd' drive naming scheme [root: ${ROOT_DRIVE}]"
    DRIVE_SCHEME='sd'
fi

log_notice "${SELF_IDENTITY_H}: Detecting Ephemeral disks"
EPHEMERALS=$(curl --silent ${URL_AWS_BASE}/meta-data/block-device-mapping/ | grep "^ephemeral\|^ebs")
for e in ${EPHEMERALS}; do
    log "- Probing ${e} .."
    DEVICE_NAME=$(curl --silent ${URL_AWS_BASE}/meta-data/block-device-mapping/$e)
    # might have to convert 'sdb' -> 'xvdb'
    DEVICE_NAME=$(echo ${DEVICE_NAME} | sed "s/sd/${DRIVE_SCHEME}/")
    DEVICE_PATH="/dev/${DEVICE_NAME}"

    # test that the device actually exists since you can request more ephemeral drives than are available
    # for an instance type and the meta-data API will happily tell you it exists when it really does not.
    if [ -b ${DEVICE_PATH} ]; then
        log "- Detected ephemeral disk: ${DEVICE_PATH}"
        DRIVES="${DRIVES} ${DEVICE_PATH}"
        EPHEMERAL_COUNT=$((EPHEMERAL_COUNT + 1 ))
    else
        log "- Ephemeral disk ${e}, ${DEVICE_PATH} is not present. skipping"
    fi
done

## END Preflight
##-----------------------------------------------------------------------------------------------------------------------------------------

##-----------------------------------------------------------------------------------------------------------------------------------------
## START Execution
line_break
log_highlight "Execution"

if [ "${EPHEMERAL_COUNT}" = 0 ]; then
    log_warning "${SELF_IDENTITY_H}: No ephemeral disk detected, nothing to do"
    exit_logic 0 "No ephemeral disk detected, nothing to do"
fi

# overwrite first few blocks in case there is a filesystem, otherwise mdadm will prompt for input
log_notice "${SELF_IDENTITY_H}: Preparing disks for use"
for DRIVE in ${DRIVES}; do
    log "- Preparing Disk [${DRIVE}]"
    $(which mdadm) --zero-superblock ${DRIVE} >/dev/null 2>&1
    dd if=/dev/zero of=${DRIVE} bs=4096 count=1024 >/dev/null 2>&1
done

log_notice "${SELF_IDENTITY_H}: Executing Partprobe"
$(which partprobe) >/dev/null 2>&1
call_sleep 1
sync_disks
call_sleep 1

log_notice "${SELF_IDENTITY_H}: Creating RAID array [--raid-devices=${EPHEMERAL_COUNT} ${DRIVES}] [${DISK_AWS_EPHEMERAL}]"
$(which mdadm) --create ${DISK_AWS_EPHEMERAL} --level=0 -c256 --force --name=0 --raid-devices=${EPHEMERAL_COUNT} ${DRIVES} >/dev/null 2>&1
echo DEVICE ${DRIVES} > /etc/mdadm.conf
$(which mdadm) --detail --scan >> /etc/mdadm.conf
$(which blockdev) --setra 65536 ${DISK_AWS_EPHEMERAL} >/dev/null 2>&1

log_notice "${SELF_IDENTITY_H}: Encrypting Disk"
log "- Generating LUKS password"
$(which date) +%s | $(which sha256sum) | $(which base64) | head -c 32 > ${FILE_LUKS_PASSWORD}
log "- Encrypting disk - ${DISK_AWS_EPHEMERAL}"
$(which cryptsetup) --batch-mode --key-file ${FILE_LUKS_PASSWORD} --hash sha256 luksFormat ${DISK_AWS_EPHEMERAL} >/dev/null 2>&1
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    log_error "${SELF_IDENTITY_H}: failed to encrypt disk (cryptsetup returned: $RETURNVAL)"
    exit_logic $E_OBJECT_FAILED_TO_CREATE
fi

log_notice "${SELF_IDENTITY_H}: Creating encrypted filesystem [/dev/mapper/${DEVICE_AWS_EPHEMERAL}]"
$(which cryptsetup) --batch-mode --key-file ${FILE_LUKS_PASSWORD} luksOpen ${DISK_AWS_EPHEMERAL} ${DEVICE_AWS_EPHEMERAL} >/dev/null 2>&1
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    log_error "${SELF_IDENTITY_H}: failed to open encrypted disk (cryptsetup returned: $RETURNVAL)"
    exit_logic $E_OBJECT_FAILED_TO_CREATE
fi

log_notice "${SELF_IDENTITY_H}: Formatting filesystem [/dev/mapper/${DEVICE_AWS_EPHEMERAL}]"
$(which mkfs.xfs) -f /dev/mapper/${DEVICE_AWS_EPHEMERAL} >/dev/null 2>&1
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    log_error "${SELF_IDENTITY_H}: failed to format filesystem (mkfs.xfs returned: $RETURNVAL)"
    exit_logic $E_OBJECT_FAILED_TO_CREATE
fi
call_sleep 1
sync_disks
call_sleep 1

log_notice "${SELF_IDENTITY_H}: Generating mount point [${MOUNTPOINT_AWS_TARGET}]"
mkdir -p ${MOUNTPOINT_AWS_TARGET} >/dev/null 2>&1
touch ${MOUNTPOINT_AWS_TARGET}

log_notice "${SELF_IDENTITY_H}: Mounting filesystem [/dev/mapper/${DEVICE_AWS_EPHEMERAL} => ${MOUNTPOINT_AWS_TARGET}]"
$(which mount) /dev/mapper/${DEVICE_AWS_EPHEMERAL} ${MOUNTPOINT_AWS_TARGET}
RETURNVAL="$?"
if [ ${RETURNVAL} -ne 0 ]; then
    log_error "${SELF_IDENTITY_H}: failed to mount filesystem (mount returned: $RETURNVAL)"
    exit_logic $E_OBJECT_FAILED_TO_CREATE
fi
call_sleep 1
sync_disks
call_sleep 1

log_notice "${SELF_IDENTITY_H}: Destroying LUKS password file"
echo "00000000000000000000000" > ${FILE_LUKS_PASSWORD}
> ${FILE_LUKS_PASSWORD}
rm -f ${FILE_LUKS_PASSWORD} >/dev/null 2>&1

## END Execution
##-----------------------------------------------------------------------------------------------------------------------------------------

log_success "${SELF_IDENTITY_H}: Finished"
exit_logic 0
