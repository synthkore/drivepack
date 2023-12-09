// *****************************************************************
// ****                       drivePACK                         ****
// ****                  www.tolaemon.com/dpack                 ****
// ****                       Source code                       ****
// ****                       8/05/2022                        ****
// ****                     Jordi Bartolome                     ****
// ****                                                         ****
// **** IMPORTANT:                                              ****
// **** Using this code or any part of it means accepting all   ****
// **** conditions exposed in: http://www.tolaemon.com/dpack    ****
// *****************************************************************

#ifndef COMMANDS_DEFS_H_
#define COMMANDS_DEFS_H_

#define MAX_COMMAND_LEN 64 //maximum length of keys buffer
#define MAX_TOKEN_STR_SIZE 16 // maximum length in bytes of a command or parameter
#define MAX_TOKEN_STR_LEN MAX_TOKEN_STR_SIZE -1 // maximum allowed alphanumeric characters in the string ( it does not include the '\0' )
#define MAX_TOKENS_X_COMMAND 10 //maximum number of tokens in one command
#define TOKEN_END_SYMBOL 32 // the symbol which separates tokens
#define STRING_TOKEN_LIMIT_SYMBOL 34//symbol which limits string tokens

#define TOKEN_IS_EMPTY 0   // no token type has been detected
#define TOKEN_IS_ALIAS 1   // refers to name that refers to an element, generally corresponds to a user defined variable or object ( the user creates the variable or object with the alias name ).
#define TOKEN_IS_CONST 2   // refers to constant string, generaly corresponds to a command
#define TOKEN_IS_VALUE 3   // refers to a numeric value

#define COMMAND_END_SYMBOL 0 // the symbol which ends a command
#define COMMAND_END_KEY 13 // the symbol which ends a command

// commands list
#define COMMAND_HELP		"help\0"
#define COMMAND_MOUNT		"mnt\0"
#define COMMAND_DIR			"dir\0"
#define COMMAND_CHANGE_DIR	"cd\0"
#define COMMAND_MAKEDIR		"mkdir\0"
#define COMMAND_GPIO		"gpio\0"
#define   PARAM_GPIO_DATA	"data\0"
#define   PARAM_GPIO_CTRL	"ctrl\0"
#define   PARAM_GPIO_IN		"in\0"
#define   PARAM_GPIO_OUT	"out\0"
#define COMMAND_RM			"rm\0"
#define COMMAND_TYPE		"type\0"
#define COMMAND_WRITE		"write\0"
#define   PARAM_TYPE_TXT	"txt\0"
#define   PARAM_TYPE_HEX    "hex\0"
#define COMMAND_ROM      "rom\0"
#define   PARAM_ROM_DUMP    "dump\0"
#define   PARAM_ROM_WRITE   "write\0"
#define   PARAM_ROM_LOAD    "load\0"
#define   PARAM_ROM_TYPE    "type\0"
#define   PARAM_ROM_INFO    "info\0"
#define   PARAM_ROM_RUN     "run\0"
#define   PARAM_ROM_CHECK   "check\0"
#define   PARAM_ROM_RX1K    "rx1k\0"
#define   PARAM_ROM_TX1K    "tx1k\0"
#define COMMAND_INFO     "info\0"

#define COMMAND_SCRIPT   "scr\0"

// the command is a token array
typedef struct {
	uint8_t token[MAX_TOKENS_X_COMMAND][MAX_TOKEN_STR_SIZE]; // array of strings with command tokens
	uint8_t token_type[MAX_TOKENS_X_COMMAND];// contains the type of each token: value, alias, or string,or null
	uint8_t ready_to_execute; // 1 if the command can be executed, 0 if there is nothing to execute
	uint16_t total; // number of tokens into the command
}t_command;


#endif /* COMMANDS_DEFS_H_ */