###------------------------------------------------------------------------------------------------
# script:  definitions.inc
# purpose: Declaration of library source files to be used to ensure consistency
# version: 1.0.0
###------------------------------------------------------------------------------------------------

###------------------------------------------------------------------------------------------------
## Configuration files
declare CONFIG_GLOBAL="${DIRECTORY_BASE}/config/global.config"

###------------------------------------------------------------------------------------------------
## Directories
declare DIRECTORY_CONFIG="${DIRECTORY_BASE}/config"
declare DIRECTORY_LIB="${DIRECTORY_BASE}/lib"
declare DIRECTORY_LIB_DEPENDENCIES="${DIRECTORY_LIB}/dependencies"
declare DIRECTORY_LIB_FUNCTIONS="${DIRECTORY_LIB}/functions"
declare DIRECTORY_LIB_REFERENCE="${DIRECTORY_LIB}/reference"
declare DIRECTORY_SCRIPTS="${DIRECTORY_BASE}/scripts"

###------------------------------------------------------------------------------------------------
## Distribution Files
declare DISTRIBUTION_README="${DIRECTORY_BASE}/README.md"
declare DISTRIBUTION_LICENSE="${DIRECTORY_BASE}/LICENSE.md"

###------------------------------------------------------------------------------------------------
## Reference Files
declare LIB_REFERENCE_DEFINITIONS="${DIRECTORY_BASE}/lib/reference/definitions.inc"
declare LIB_REFERENCE_DETAILS="${DIRECTORY_BASE}/lib/reference/details.inc"
declare LIB_REFERENCE_EXIT_CODES="${DIRECTORY_BASE}/lib/reference/exit_codes.inc"

###------------------------------------------------------------------------------------------------
## Dependencies
declare LIB_DEPENDENCIES_SPOT_WARNING_SERVICE="${DIRECTORY_BASE}/lib/dependencies/spot_warning.service"

###------------------------------------------------------------------------------------------------
## Function: AWS
declare LIB_FUNCTIONS_AWS_AMI="${DIRECTORY_BASE}/lib/functions/aws/ami.inc"
declare LIB_FUNCTIONS_AWS_CLOUDFORMATION="${DIRECTORY_BASE}/lib/functions/aws/cloudformation.inc"
declare LIB_FUNCTIONS_AWS_CLOUDFRONT="${DIRECTORY_BASE}/lib/functions/aws/cloudfront.inc"
declare LIB_FUNCTIONS_AWS_CODEDEPLOY="${DIRECTORY_BASE}/lib/functions/aws/codedeploy.inc"
declare LIB_FUNCTIONS_AWS_EC2="${DIRECTORY_BASE}/lib/functions/aws/ec2.inc"
declare LIB_FUNCTIONS_AWS_ELASTICACHE_MEMCACHED="${DIRECTORY_BASE}/lib/functions/aws/elasticache_memcached.inc"
declare LIB_FUNCTIONS_AWS_ELASTICACHE_REDIS="${DIRECTORY_BASE}/lib/functions/aws/elasticache_redis.inc"
declare LIB_FUNCTIONS_AWS_ENI="${DIRECTORY_BASE}/lib/functions/aws/eni.inc"
declare LIB_FUNCTIONS_AWS_IAM="${DIRECTORY_BASE}/lib/functions/aws/iam.inc"
declare LIB_FUNCTIONS_AWS_INITIALIZE="${DIRECTORY_BASE}/lib/functions/aws/initialize.inc"
declare LIB_FUNCTIONS_AWS_KMS="${DIRECTORY_BASE}/lib/functions/aws/kms.inc"
declare LIB_FUNCTIONS_AWS_LAMBDA="${DIRECTORY_BASE}/lib/functions/aws/lambda.inc"
declare LIB_FUNCTIONS_AWS_LOADBALANCER="${DIRECTORY_BASE}/lib/functions/aws/loadbalancer.inc"
declare LIB_FUNCTIONS_AWS_METADATA="${DIRECTORY_BASE}/lib/functions/aws/metadata.inc"
declare LIB_FUNCTIONS_AWS_RDS="${DIRECTORY_BASE}/lib/functions/aws/rds.inc"
declare LIB_FUNCTIONS_AWS_RDS_AURORA_POSTGRESQL="${DIRECTORY_BASE}/lib/functions/aws/rds_aurora_postgresql.inc"
declare LIB_FUNCTIONS_AWS_RDS_MYSQL="${DIRECTORY_BASE}/lib/functions/aws/rds_mysql.inc"
declare LIB_FUNCTIONS_AWS_RDS_POSTGRESQL="${DIRECTORY_BASE}/lib/functions/aws/rds_postgresql.inc"
declare LIB_FUNCTIONS_AWS_ROUTE53="${DIRECTORY_BASE}/lib/functions/aws/route53.inc"
declare LIB_FUNCTIONS_AWS_S3="${DIRECTORY_BASE}/lib/functions/aws/s3.inc"
declare LIB_FUNCTIONS_AWS_S3_WEBSITE="${DIRECTORY_BASE}/lib/functions/aws/s3_website.inc"
declare LIB_FUNCTIONS_AWS_SSM="${DIRECTORY_BASE}/lib/functions/aws/ssm.inc"
declare LIB_FUNCTIONS_AWS_SSL="${DIRECTORY_BASE}/lib/functions/aws/ssl.inc"
declare LIB_FUNCTIONS_AWS_VPC="${DIRECTORY_BASE}/lib/functions/aws/vpc.inc"

###------------------------------------------------------------------------------------------------
## Function: Bootstrap
declare LIB_FUNCTIONS_BOOTSTRAP_INFO_FILES="${DIRECTORY_BASE}/lib/functions/bootstrap/info_files.inc"
declare LIB_FUNCTIONS_BOOTSTRAP_SECRETS="${DIRECTORY_BASE}/lib/functions/bootstrap/secrets.inc"
declare LIB_FUNCTIONS_BOOTSTRAP_SPOT_WARNING="${DIRECTORY_BASE}/lib/functions/bootstrap/spot_warning.inc"

###------------------------------------------------------------------------------------------------
## Function: Core
declare LIB_FUNCTIONS_CORE_COLOR="${DIRECTORY_BASE}/lib/functions/core/color.inc"
declare LIB_FUNCTIONS_CORE_COMMON="${DIRECTORY_BASE}/lib/functions/core/common.inc"
declare LIB_FUNCTIONS_CORE_LOGGING="${DIRECTORY_BASE}/lib/functions/core/logging.inc"
declare LIB_FUNCTIONS_CORE_PACKAGE_MANAGEMENT="${DIRECTORY_BASE}/lib/functions/core/package_management.inc"
declare LIB_FUNCTIONS_CORE_SCRIPT_LOGIC="${DIRECTORY_BASE}/lib/functions/core/script_logic.inc"

###------------------------------------------------------------------------------------------------
## Function: Tools
declare LIB_FUNCTIONS_TOOLS_CURL="${DIRECTORY_BASE}/lib/functions/tools/curl.inc"
declare LIB_FUNCTIONS_TOOLS_DOCKER="${DIRECTORY_BASE}/lib/functions/tools/docker.inc"
declare LIB_FUNCTIONS_TOOLS_MAIL="${DIRECTORY_BASE}/lib/functions/tools/mail.inc"
declare LIB_FUNCTIONS_TOOLS_SQLITE="${DIRECTORY_BASE}/lib/functions/tools/sqlite.inc"
declare LIB_FUNCTIONS_TOOLS_SSH="${DIRECTORY_BASE}/lib/functions/tools/ssh.inc"
declare LIB_FUNCTIONS_TOOLS_SSL="${DIRECTORY_BASE}/lib/functions/tools/ssl.inc"
declare LIB_FUNCTIONS_TOOLS_TAR="${DIRECTORY_BASE}/lib/functions/tools/tar.inc"

###------------------------------------------------------------------------------------------------
## Scripts: Bootstrap
declare SCRIPTS_BOOTSTRAP_DISK="${DIRECTORY_BASE}/scripts/bootstrap/disk.sh"
declare SCRIPTS_BOOTSTRAP_EIP="${DIRECTORY_BASE}/scripts/bootstrap/eip.sh"
declare SCRIPTS_BOOTSTRAP_SPOT_WARNING="${DIRECTORY_BASE}/scripts/bootstrap/spot_warning.sh"
declare SCRIPTS_BOOTSTRAP_SWAP="${DIRECTORY_BASE}/scripts/bootstrap/swap.sh"
declare SCRIPTS_BOOTSTRAP_TIMEZONE="${DIRECTORY_BASE}/scripts/bootstrap/timezone.sh"
declare SCRIPTS_BOOTSTRAP_USERS="${DIRECTORY_BASE}/scripts/bootstrap/users.sh"

###------------------------------------------------------------------------------------------------
## Scripts: Commands
declare SCRIPTS_COMMANDS_SPOT_WARNING_MONITOR="${DIRECTORY_BASE}/scripts/commands/spot_warning_monitor.sh"

###------------------------------------------------------------------------------------------------
## Scripts: DNS
declare SCRIPTS_DNS_CREATE_RECORD="${DIRECTORY_BASE}/scripts/dns/create_record.sh"

###------------------------------------------------------------------------------------------------
## Scripts: SSL
declare SCRIPTS_SSL_DOWNLOAD_CERTIFICATE="${DIRECTORY_BASE}/scripts/ssl/download_certificate.sh"

###------------------------------------------------------------------------------------------------
## Scripts: Utility
declare SCRIPTS_UTILITY_README_GENERATOR="${DIRECTORY_BASE}/scripts/utility/readme_generator.sh"
declare SCRIPTS_UTILITY_INSTALL="${DIRECTORY_BASE}/scripts/utility/install.sh"
