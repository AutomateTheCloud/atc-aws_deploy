#!/bin/bash
###------------------------------------------------------------------------------------------------
# alias:   aws_deploy.spot_warning_monitor
# script:  spot_warning_monitor.sh
# purpose: Monitors for Spot Two-Minute Warning and executes scripts found in [/apps/spotwarning_execution] if warning is detected
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Load Defaults
source "/opt/aws_deploy/lib/functions/bootstrap/spot_warning.inc"

while true; do
    if [ -z $(curl -Is http://169.254.169.254/latest/meta-data/spot/termination-time | head -1 | grep 404 | cut -d ' ' -f 2) ]; then
        echo "Spot Two-Minute Warning detected, executing scripts"
        for TMP_FILE in $(eval "find ${DIRECTORY_SPOT_WARNING_EXECUTION} -type f -name '*.sh' 2>/dev/null | sort"); do
            echo "Executing: [${TMP_FILE}]"
            chmod u+x "${TMP_FILE}"
            ${TMP_FILE}
            sleep 1
        done
        sleep 180 # sleep past the two-minute mark so the Spot Warning service doesnt execute this script again when restarting the service
        break
    else
        sleep ${SPOT_WARNING_POLLING_INTERVAL}
    fi
done
