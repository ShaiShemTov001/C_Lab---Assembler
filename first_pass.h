#ifndef C_LAB_PROJECT_OPENU_2023_FIRST_PASS_H
#define C_LAB_PROJECT_OPENU_2023_FIRST_PASS_H

#include "linked_lists.h"

/* first process of the source file , puts all the data and the instructions with their operands in the memory image
 * accept the symbols, create the symbol list for the second pass.
 * @param - char * source the source file we open to analyze
 * */
status
firstPass(FILE **source_file, char **sourceName, symbols **symbolHead, missingMemoryInfo **infoHead,
          machine_word **dataArray, machine_word **memoryImage, int *DC, int *IC);

/*Label Definition gets a label from the first pass, first search it in the symbol list and create new symbol if needed
 * @param - errors *errorNum - Keep an Enum with number of error if it happened, set by default to NO_ERROR
 * #param - bool *labelFlag - Indicate that we are in label definition for the rest of the first pass
 * @param - char **linePtr - A pointer to the line we process
 * #param - symbol **head - Head of the symbols list
 * @param - int memoryNum - Use to insert the memory location of the current symbol we define
 * @param - directive *directiveType - Use to insert the directive type (if we in directive line) to the current label we define
 * @param - symbols **current - Used as pointer the insert the new label to the symbols list
 * @param - symbol ** newLabel - Sent as NULL, override with the new symbol we create
 * @param - bool *isError - A flag indicate that error happened used to determine if we print files at the end of the second pass
 * @param - bool *entryFlag - Indicate that this symbol is an entry type
 * */
void labelDefinition(errors *errorNum, bool *labelFlag, char **linePtr, symbols **head, int memoryNum,
                     directives *directiveType, symbols **current, symbols **newLabel, bool *entryFlag);

/*Process Entry Extern process declaration of entry or extern symbols in such case we initiate their memory location
 * invalid memory number so we can delete them later
 * @param - char *linePtr the line we process
 * @param - symbols **head - Head of the symbols list
 * @param - directive directiveType- insert the label the directive type (entry or extern
 * @param - errors *errorNum - Keep an Enum with number of error if it happened, set by default to NO_ERROR
 * @param - isError - A flag indicate that error happened used to determine if we print files at the end of the second pass
 * @param - symbols **newLabel - Sent as NULL, override with the new symbol we create
 * @param - symbols **current - Used as pointer the insert the new label to the symbols list
 * */
void processEntryExtern(char *linePtr, symbols **head, directives directiveType, errors *errorNum, symbols **newLabel,
                        symbols **current);

/*Finalize Labels Memory removes label declaration from the symbols list and connect between memoryImage and dataArray
 * @param - symbols **head - Head of the symbols list
 * @param - int IC - Counter for instruction lines
 * @param - int DC - Counter for directive lines
 * @param - machine_word *memoryImage - The memoryImage of the file
 * @param - machine_word *dataArray - Array of all tha Data from the file
 * */
status finalizeLabelsMemory(symbols** head, int IC, int DC, machine_word *memoryImage, machine_word *dataArray);


#endif
