#ifndef C_LAB_PROJECT_OPENU_2023_DATA_HANDLER_H
#define C_LAB_PROJECT_OPENU_2023_DATA_HANDLER_H

#include "globals.h"

/*Checks the kind of directive and advanced the pointer after the directive.
 * @param char **line - A pointer to the line
 * @return - directives directive - The directive type
 * */
directives directive_identifier(char **line);

/*insert data one by one to dataArray, check amount of valid commas in directive line, if error occurred delete the
 * line we processed and return to the first pass.
 * @param - *DC , the data counter
 * @param - char *line - the buffer we held of the original line
 * @param - machine_word ***array - dataArray allocated array of machine words where we held all the data from the file
 * @param - int type - the type of directive this line gave (data or string)
 * @param - *arrSize - the size of the array so we can so know when and if we can allocate more memory
 * @param - errors *errorNum - Keep an Enum with number of error if it happened, set by default to NO_ERROR
 * */
void dataInsertion(int *DC, char *line, machine_word **array, int type, errors *errorNum, int memoryNum);
#endif
