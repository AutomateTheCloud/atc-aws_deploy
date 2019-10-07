###------------------------------------------------------------------------------------------------
# script:  details.inc
# purpose: Application specific details to assist in the generation of README.md
# version: 1.0.0
###------------------------------------------------------------------------------------------------
DETAILS_NAME="AWS Deploy"
DETAILS_NAME_ABBR="aws_deploy"
DETAILS_DESCRIPTION=(
    'Assists in the deployment of AWS EC2 instances'
    '- Automates process of encrypting and mounting ephemeral disks (mounts to /apps)'
    '- EFS management'
    '- SSL Certificate acquisition'
    '- DNS management'
    '- Docker support'
)
DETAILS_PREREQUISITES=(
    'Linux (tested on Amazon Linux 2)'
    'BASH shell'
)
DETAILS_DEPENDENCIES=(
    'AWS CLI'
    'curl'
)
