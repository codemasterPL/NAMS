/***************************************************************************
 * NAMS - Not Another MUD Server                                           *
 * Copyright (C) 2012 Matthew Goff (matt@goff.cc) <http://www.ackmud.net/> *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ***************************************************************************/
/**
 * @file config.h
 * @brief NAMS configuration options.
 *
 *  All NAMS configuration options are kept within this file. This includes
 *  defines for specific folders, memory tuning, frequently used string
 *  messages, etc.
 */
#ifndef DEC_CONFIG_H
#define DEC_CONFIG_H

/***************************************************************************
 *                             ACCOUNT OPTIONS                             *
 ***************************************************************************/
/** @name Account Options */ /**@{*/
/**
 * @def CFG_ACT_CHARACTER_MAX
 * @brief The maximum number of characters an account may have.
 * @par Default: 10
 */
#define CFG_ACT_CHARACTER_MAX 10

/**
 * @def CFG_ACT_LOGIN_MAX
 * @brief The number of previous hosts to track for login history.
 * @par Default: 3
 */
#define CFG_ACT_LOGIN_MAX 3

/**
 * @def CFG_ACT_NAME_MAX_LEN
 * @brief The maximum allowable length for an account name.
 * @par Default: 32
 */
#define CFG_ACT_NAME_MAX_LEN 32

/**
 * @def CFG_ACT_NAME_MIN_LEN
 * @brief The minimum allowable length for an account name.
 * @par Default: 4
 */
#define CFG_ACT_NAME_MIN_LEN 4

/**
 * @def CFG_ACT_PASSWORD_MAX_LEN
 * @brief The maximum allowable length for an account password.
 * @par Default: 32
 */
#define CFG_ACT_PASSWORD_MAX_LEN 32

/**
 * @def CFG_ACT_PASSWORD_MIN_LEN
 * @brief The minimum allowable length for an account password.
 * @par Default: 4
 */
#define CFG_ACT_PASSWORD_MIN_LEN 4
/**@}*/

/***************************************************************************
 *                              DATA OPTIONS                               *
 ***************************************************************************/
/** @name Data Options */ /**@{*/
/**
 * @def CFG_DAT_DIR_ACCOUNT
 * @brief Directory for accounts to be loaded from.
 * @par Default: "account"
 */
#define CFG_DAT_DIR_ACCOUNT "account"

/**
 * @def CFG_DAT_DIR_COMMAND
 * @brief Directory for commands to be loaded from.
 * @par Default: "command"
 */
#define CFG_DAT_DIR_COMMAND "command"

/**
 * @def CFG_DAT_DIR_ETC
 * @brief Directory for misc files to be written to and loaded from.
 * @par Default: "etc"
 */
#define CFG_DAT_DIR_ETC "etc"

/**
 * @def CFG_DAT_DIR_LOG
 * @brief Directory for logfiles to be written to.
 * @par Default: "log"
 */
#define CFG_DAT_DIR_LOG "log"

/**
 * @def CFG_DAT_DIR_OBJ
 * @brief Directory for plugins to be loaded from.
 * @par Default: "obj"
 */
#define CFG_DAT_DIR_OBJ "obj"

/**
 * @def CFG_DAT_DIR_VAR
 * @brief Directory for temporary files.
 * @par Default: "var"
 */
#define CFG_DAT_DIR_VAR "var"

/**
 * @def CFG_DAT_FILE_ACT_EXT
 * @brief File extension to use for account setting files.
 * @par Default: "act"
 */
#define CFG_DAT_FILE_ACT_EXT "act"

/**
 * @def CFG_DAT_FILE_REBOOT
 * @brief File for reboot data to be temporarily stored in.
 * @par Default: "reboot.dat"
 */
#define CFG_DAT_FILE_REBOOT "reboot.dat"

/**
 * @def CFG_DAT_FILE_SETTINGS
 * @brief File for runtime and account settings.
 * @par Default: "settings.dat"
 */
#define CFG_DAT_FILE_SETTINGS "settings.dat"
/**@}*/

/***************************************************************************
 *                              GAME OPTIONS                               *
 ***************************************************************************/
/** @name Game Options */ /**@{*/
/**
 * @def CFG_GAM_CMD_IGNORE_CASE
 * @brief Ignore case sensitivity on input from sockets.
 * @par Default: true
 */
#define CFG_GAM_CMD_IGNORE_CASE true

/**
 * @def CFG_GAM_PULSE_RATE
 * @brief How many cycles per second should be processed.
 * @par Default: 100
 */
#define CFG_GAM_PULSE_RATE 100
/**@}*/

/***************************************************************************
 *                              LOG OPTIONS                                *
 ***************************************************************************/
/** @name Log Options */ /**@{*/
/**
 * @def CFG_LOG_SERVER
 * @brief Default log for boot / shutdown and general information.
 * @par Default: "server.log"
 */
#define CFG_LOG_SERVER "server.log"
/**@}*/

/***************************************************************************
 *                              PLUGIN OPTIONS                             *
 ***************************************************************************/
/** @name Plugin Options */ /**@{*/
/**
 * @def CFG_PLG_BUILD_CMD
 * @brief The compiler command to use when building plugins.
 * @par Default: "g++"
 */
#define CFG_PLG_BUILD_CMD "g++"

/**
 * @def CFG_PLG_BUILD_OPT
 * @brief All build options passed during compiling a plugin.
 * @par Default: "-std=c++0x -Isrc/h -fpic -ldl -rdynamic -shared 2>&1"
 */
#define CFG_PLG_BUILD_OPT "-std=c++0x -Isrc/h -fpic -ldl -rdynamic -shared 2>&1"

/**
 * @def CFG_PLG_BUILD_EXT_IN
 * @brief File extension for files to be compiled.
 * @par Default: "cpp"
 */
#define CFG_PLG_BUILD_EXT_IN "cpp"

/**
 * @def CFG_PLG_BUILD_EXT_OUT
 * @brief File extension for files after they are compiled.
 * @par Default: "so"
 */
#define CFG_PLG_BUILD_EXT_OUT "so"

/**
 * @def CFG_PLG_MAX_ARR
 * @brief The maximum number of elements for each data type array in a plugin.
 * @par Default: 5
 */
#define CFG_PLG_MAX_ARR 5
/**@}*/

/***************************************************************************
 *                              MEMORY OPTIONS                             *
 ***************************************************************************/
/** @name Memory Options */ /**@{*/
/**
 * @def CFG_MEM_MAX_BITSET
 * @brief Maximum size of all bitset elements within the server.
 * @par Default: 128
 */
#define CFG_MEM_MAX_BITSET 128
/**@}*/

/***************************************************************************
 *                            SECURITY OPTIONS                             *
 ***************************************************************************/
/** @name Security Options */ /**@{*/
/**
 * @def CFG_SEC_CRYPT_METHOD
 * @brief The algorithm to use with crypt(). See man 3 crypt for details.
 * @par Default: 6
 */
#define CFG_SEC_CRYPT_METHOD 6

/**
 * @def CFG_SEC_CRYPT_SALT
 * @brief The initial salt string to use with crypt().
 * @par Default: "$" CFG_SEC_CRYPT_METHOD "$"
 */
#define CFG_SEC_CRYPT_SALT "$" SX( CFG_SEC_CRYPT_METHOD ) "$"

/**
 * @def CFG_SEC_DIR_MODE
 * @brief The chmod mode to set on directories (accounts) created by the server.
 * @par Default: 0755
 */
#define CFG_SEC_DIR_MODE 0755
/**@}*/

/***************************************************************************
 *                              SOCKET OPTIONS                             *
 ***************************************************************************/
/** @name Socket Options */ /**@{*/
/**
 * @def CFG_SOC_BIND_ADDR
 * @brief IP address to bind the listening server socket to.
 * @par Default: "::"
 */
#define CFG_SOC_BIND_ADDR "::"

/**
 * @def CFG_SOC_MIN_PORTNUM
 * @brief The listening server socket must be above this port.
 * @par Default: 1024
 */
#define CFG_SOC_MIN_PORTNUM 1024

/**
 * @def CFG_SOC_MAX_IDLE
 * @brief (Minutes / tick rate) before disconneting an idle socket. Approx 30 minutes.
 * @par Default: ( ( 30 * 60 * USLEEP_MAX ) / ( USLEEP_MAX / CFG_GAM_PULSE_RATE ) )
 */
#define CFG_SOC_MAX_IDLE ( ( 30 * 60 * USLEEP_MAX ) / ( USLEEP_MAX / CFG_GAM_PULSE_RATE ) )

/**
 * @def CFG_SOC_MAX_PENDING
 * @brief The maximum number of pending connections to allow in the listen backlog.
 * @par Default: 5
 */
#define CFG_SOC_MAX_PENDING 5

/**
 * @def CFG_SOC_MAX_PORTNUM
 * @brief The listening server socket must be below this port.
 * @par Default: 65536
 */
#define CFG_SOC_MAX_PORTNUM 65536

/**
 * @def CFG_SOC_PORTNUM
 * @brief Port number to listen on if not specified on the command line.
 * @par Default: 4321
 */
#define CFG_SOC_PORTNUM 4321
/**@}*/

/***************************************************************************
 *                              STRING OPTIONS                             *
 ***************************************************************************/
/** @name String Options */ /**@{*/
/**
 * @def CFG_STR_ACT_NAME_ALNUM
 * @brief String additionally sent if an account name is invalid due to non-alphanumeric characters.
 * @par Default: "Account name must be alphanumeric characters only." CRLF
 */
#define CFG_STR_ACT_NAME_ALNUM "Account name must be alphanumeric characters only." CRLF

/**
 * @def CFG_STR_ACT_NAME_CONFIRM
 * @brief String sent to request the user confirms their new account name.
 * @par Default: "Do you wish to create a new account named %s (Y/N)? "
 */
#define CFG_STR_ACT_NAME_CONFIRM "Do you wish to create a new account named %s (Y/N)? "

/**
 * @def CFG_STR_ACT_NAME_GET
 * @brief String sent to request the user enters their account name.
 * @par Default: "Please enter your account name: "
 */
#define CFG_STR_ACT_NAME_GET "Please enter your account name: "

/**
 * @def CFG_STR_ACT_NAME_INVALID
 * @brief String sent when an invalid account name is received.
 * @par Default: "Invalid account name." CRLF
 */
#define CFG_STR_ACT_NAME_INVALID "Invalid account name." CRLF

/**
 * @def CFG_STR_ACT_NAME_LENGTH
 * @brief String additionally sent when an account name is invalid due to length.
 * @par Default: "Account name must be between %d and %d characters." CRLF
 */
#define CFG_STR_ACT_NAME_LENGTH "Account name must be between %d and %d characters." CRLF

/**
 * @def CFG_STR_ACT_NAME_PROHIBITED
 * @brief String additionally sent when an account name matches the prohibited names list.
 * @par Default: "That account name has been prohibited from use." CRLF
 */
#define CFG_STR_ACT_NAME_PROHIBITED "That account name has been prohibited from use." CRLF

/**
 * @def CFG_STR_ACT_NEW
 * @brief String sent when a new account will be created.
 * @par Default: "New account." CRLF
 */
#define CFG_STR_ACT_NEW "New account." CRLF

/**
 * @def CFG_STR_ACT_NEW_ERROR
 * @brief String sent if an error occurs when first creating a new account.
 * @par Default: "An error occured while attempting to create that account." CRLF
 */
#define CFG_STR_ACT_NEW_ERROR "An error occured while attempting to create that account." CRLF

/**
 * @def CFG_STR_ACT_PASSWORD_CONFIRM
 * @brief String sent to request the user confirms their new account password.
 * @par Default: "Please confirm password: "
 */
#define CFG_STR_ACT_PASSWORD_CONFIRM "Please confirm password: "

/**
 * @def CFG_STR_ACT_PASSWORD_GET
 * @brief String sent to request the user inputs their password.
 * @par Default: "Password: "
 */
#define CFG_STR_ACT_PASSWORD_GET "Password: "

/**
 * @def CFG_STR_ACT_PASSWORD_INVALID
 * @brief String sent when an invalid account password is received.
 * @par Default: "Invalid account password." CRLF
 */
#define CFG_STR_ACT_PASSWORD_INVALID "Invalid account password." CRLF

/**
 * @def CFG_STR_ACT_PASSWORD_LENGTH
 * @brief String additionally sent when an account password is invalid due to length.
 * @par Default: "Account password must be between %d and %d characters." CRLF
 */
#define CFG_STR_ACT_PASSWORD_LENGTH "Account password must be between %d and %d characters." CRLF

/**
 * @def CFG_STR_ACT_PASSWORD_MISMATCH
 * @brief String sent when a new password confirmation fails.
 * @par Default: "Passwords don't match." CRLF
 */
#define CFG_STR_ACT_PASSWORD_MISMATCH "Passwords don't match." CRLF

/**
 * @def CFG_STR_CMD_INVALID
 * @brief String sent when an invalid command is received.
 * @par Default: "Invalid command." CRLF
 */
#define CFG_STR_CMD_INVALID "Invalid command." CRLF

/**
 * @def CFG_STR_EXIT_FAILURE
 * @brief String to write to log on EXIT_FAILURE.
 * @par Default: "Server terminated."
 */
#define CFG_STR_EXIT_FAILURE "Server terminated."

/**
 * @def CFG_STR_EXIT_SUCCESS
 * @brief String to write to log on EXIT_SUCCESS.
 * @par Default: "Normal termination of server."
 */
#define CFG_STR_EXIT_SUCCESS "Normal termination of server."

/**
 * @def CFG_STR_FILE_COMMAND_READ
 * @brief String to output prior to loading command plugins.
 * @par Default: "Loading commands..."
 */
#define CFG_STR_FILE_COMMAND_READ "Loading commands..."

/**
 * @def CFG_STR_FILE_DONE
 * @brief String to output after file read/write operations are complete.
 * @par Default: "Done."
 */
#define CFG_STR_FILE_DONE "Done."

/**
 * @def CFG_STR_FILE_SETTINGS_READ
 * @brief String to output prior to loading settings files.
 * @par Default: "Loading settings..."
 */
#define CFG_STR_FILE_SETTINGS_READ "Loading settings..."

/**
 * @def CFG_STR_FILE_SETTINGS_WRITE
 * @brief String to output prior to writing settings files.
 * @par Default: "Saving settings..."
 */
#define CFG_STR_FILE_SETTINGS_WRITE "Saving settings..."

/**
 * @def CFG_STR_IDLE
 * @brief String sent on disconnecting an idle socket.
 * @par Default: CRLF "Idle limit reached. Goodbye." CRLF
 */
#define CFG_STR_IDLE CRLF "Idle limit reached. Goodbye." CRLF

/**
 * @def CFG_STR_LOGIN
 * @brief String sent on initial socket connection.
 * @par Default: "Welcome to " CFG_STR_VERSION CRLF "For a list of commands type 'help'." CRLF
 */
#define CFG_STR_LOGIN "Welcome to " CFG_STR_VERSION CRLF "For a list of commands type 'help'." CRLF

/**
 * @def CFG_STR_MAX_BUFLEN
 * @brief Maximum length of all char type buffers.
 * @par Default: 16384
 */
#define CFG_STR_MAX_BUFLEN 16384

/**
 * @def CFG_STR_QUIT
 * @brief String sent to the client when they quit.
 * @par Default: "Goodbye." CRLF
 */
#define CFG_STR_QUIT "Goodbye." CRLF

/**
 * @def CFG_STR_SEL_INVALID
 * @brief String send to the client after an invalid selection is made during an input request.
 * @par Default: "Invalid selection." CRLF
 */
#define CFG_STR_SEL_INVALID "Invalid selection." CRLF

/**
 * @def CFG_STR_SHUTDOWN
 * @brief String broadcast to all clients when the server is shutting down.
 * @par Default: "Server shutting down." CRLF
 */
#define CFG_STR_SHUTDOWN "Server shutting down." CRLF

/**
 * @def CFG_STR_UTILS_ERROR
 * @brief String to prepend to logs flagged UTILS_TYPE_ERROR.
 * @par Default: "[ERROR ] "
 */
#define CFG_STR_UTILS_ERROR "[ERROR] "

/**
 * @def CFG_STR_UTILS_INFO
 * @brief String to prepend to logs flagged UTILS_TYPE_INFO.
 * @par Default: "[INFO  ] "
 */
#define CFG_STR_UTILS_INFO "[INFO  ] "

/**
 * @def CFG_STR_UTILS_SOCKET
 * @brief String to prepend to logs flagged UTILS_TYPE_SOCKET.
 * @par Default: "[SOCKET] "
 */
#define CFG_STR_UTILS_SOCKET "[SOCKET] "

/**
 * @def CFG_STR_VERSION
 * @brief The current server version.
 * @par Default: "NAMS Development 0.0.0"
 */
#define CFG_STR_VERSION "NAMS Development 0.0.0"
/**@}*/

#endif
