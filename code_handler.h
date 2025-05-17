#ifndef C_LAB_PROJECT_OPENU_2023_CODE_HANDLER_H
#define C_LAB_PROJECT_OPENU_2023_CODE_HANDLER_H

#include "globals.h"
#include "linked_lists.h"

/*Command validation gets a command line and checks the validation of the given line,
 * insert the address type to commandVariables
 * @param - char *line - The line we process
 * @param - commandVariables **data - A struct where we keep the line characteristics
 * @param - errors *errorNum - Keep an Enum with number of error if it happened, set by default to NO_ERROR
 * */
void command_validation(char *line, commandVariables **data, errors *errorNum);

/*Command num gets a pointer that held a command from the line and check if this command is existing according to Enum opcode
 * @param - char *ptr - A pointer to the command
 * @return - Enum opcode or Error if isn't valid
 * */
opcode command_num(char *ptr);

/*Check operand check a given operand from the line we process and check if his name is valid
 * @param - char * sourceOperand - The operand we process
 * @param - errors *errorNum - Keep an Enum with number of error if it happened, set by default to NO_ERROR
 * @return - Enum address according to the operand address type, returns Error if the name or kind isn't valid
 * */
address check_operand(char *sourceOperand, errors *errorNum);

/*Code extractor fill the struct commandVariables, checks that the amount of operand is the same as the amount
 * command should receive also checks the amount of commas is valid.
 * @param - char *line - The line we process
 * @param - commandVariables **data - A struct where we keep the line characteristics
 * @param - errors *errorNum - Keep an Enum with number of error if it happened, set by default to NO_ERROR
 * */
void code_extractor(char *line, commandVariables **data, errors *errorNum);


/*Code insertion inserts the code lines into the memory image in binary representation, also create a missingMemoryInfo
 * node to a label we found as operand.
 * @param - int *IC - Counter that held how many code line we convert
 * @param - commandVariables *instructionContent - A struct where we keep the line characteristics
 * @param - machine_word *array - The memoryImage array
 * @param - missingMemoryInfo **head - linked list of struct that held an entry or extern the user use as operand and need to be
 * encoding in the second pass
 * @param - missingMemoryInfo **current - A missingMemoryInfo pointer use to insert new missingMemoryInfo node to the linked list
 * @param - int lineNumber - keep the file line number to insert into missingMemoryInfo as the memory location of a label used as
 * operand
 * */
status code_insertion(int *IC, commandVariables *instructionContent, machine_word *array, missingMemoryInfo **head,
                      missingMemoryInfo **current, int lineNumber, int memoryNum);


#endif
