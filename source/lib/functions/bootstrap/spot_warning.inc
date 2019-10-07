###--------------------------------------------------------------------------------
# script:  spot_warning.inc
# purpose: AWS Deploy Spot Warning Handler
# version: 1.0.0
###------------------------------------------------------------------------------------------------
REQUIRED_EXECUTABLES+=()

###--------------------------------------------------------------------------------
# Variables
declare DIRECTORY_SPOT_WARNING_EXECUTION="/apps/spot_warning_execution"
FILE_BIN_SPOT_WARNING_MONITOR="/usr/bin/spot_warning_monitor"
FILE_SERVICE_SPOT_WARNING="/etc/systemd/system/spot_warning.service"
SERVICE_SPOT_WARNING="spot_warning.service"
SPOT_WARNING_POLLING_INTERVAL="15"

# Ensure Spot Warning Execution Directory exists
if [ ! -d ${DIRECTORY_SPOT_WARNING_EXECUTION} ]; then
    rm -f ${DIRECTORY_SPOT_WARNING_EXECUTION} 2>/dev/null
    $(which mkdir) -p ${DIRECTORY_SPOT_WARNING_EXECUTION} 2>/dev/null
fi
