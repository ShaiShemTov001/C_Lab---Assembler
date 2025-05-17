
#ifndef C_LAB_PROJECT_OPENU_2023_PRE_ASSEMBLER_H
#define C_LAB_PROJECT_OPENU_2023_PRE_ASSEMBLER_H

#include "globals.h"
#include "linked_lists.h"
#include <stdio.h>

/*Pre-assembler path is scanning the file and search for mcro declaration, if exist we create macro list and spreading
 * the mcro content in every call to this macro in the file
 * @param - const char* source - the name of the source file we process
 * */
status pre_assembler(char *source, FILE **source_file, mcro **newMcro, FILE **dest_file, mcro **head);

/*Process macro declaration finds a macro declaration search it in the macro list and create new one if needed,
 * also the function handling errors if occurred
 * @param - char *linePtr - The line we process
 * @param - int lineNumber - the line number we process , used for the printError function
 * @param - const char *source - the file name we process - used for the printError function
 * @param - mcro **head - The head of the mcro list
 * @param - mcro **current - A pointer to insert the new macro to the list
 * @param - bool *mcroFlag - Indicate that we are in macro declaration
 * @param - bool *errorFlag - Indicate that error occurred
 * @param - mcro **newMcro - The mcro we created
 * */
status process_macro_declaration(char *linePtr, int lineNumber, const char *source , mcro **head, mcro **current,
                                 bool *mcroFlag, bool *errorFlag, mcro **newMcro);

/*Process endmcro search for get an end mcro declaration and check that the line is valid
 * @param - char *linePtr - The line we process
 * @param - bool *errorFlag - Indicate that error occurred
 * @param - int lineNumber - the line number we process , used for the printError function
 * @param - const char *source - the file name we process - used for the printError function
 * */
void process_endmcro(char *linePtr, bool *errorFlag, bool *mcroFlag, int lineNumber, const char *source);

/*process regular line is to print line to the ".am" file when we are not in mcro declaration if there is calling to
 * mcro from the list we need to spread the mcro content in the line
 * @param - mcro **head - A starting point to search the mcro if called
 * @param - char *linePtr - The line we process
 * @param - int lineNumber - the line number we process , used for the printError function
 * @param - const char *source - the file name we process - used for the printError function
 * @param - char *line - the line as we scanned to print in the ".am" file
 * @param - FILE *dest_file - The ".am" file we make
 * @param - bool @errorFlag - Indicate the error occurred
 * @param - mcro **new mcro - A buffer to hold the mcro we found if so
 * */
void process_regular_line(mcro **head, char *linePtr, int lineNumber, const char *source, char *line,
                          FILE *dest_file, bool *errorFlag, mcro** newMcro);



#endif
