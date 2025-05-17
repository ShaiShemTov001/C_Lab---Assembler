
#ifndef C_LAB_PROJECT_OPENU_2023_UTILS_H
#define C_LAB_PROJECT_OPENU_2023_UTILS_H

#include <ctype.h>
#include <stdlib.h>
#include "globals.h"
#include <stdio.h>


/*Skip spaces gets pointer and skip all the white spaces till she reach the first char
 * @param - char* str - The pointer to start skipping from
 * */
char* skip_spaces(char* str);

/*Malloc with print make the check we actually get the memory we ask from the computer and print error if not
 * @param - size_t size - The size of memory we want to get
 * @return - void *prt - A pointer to the given memory
 * */
void *mallocWithPrint(size_t size);

/* Checks the validation of a given label refers to extern label if needed
 * @param - char **line - a pointer to the line
 * @param - char **labelName - a pointer to labelName
 * @param - int type the type of directive
 * */
void label_checker(char *line, char *labelName, directives type, errors *errorNum);

/*Invalid name check get label and checks if the name is not name of instruction or mcro as declaration
 * @param - char* ptr - the label we check
 * @return - bool T\F - TRUE if the name is invalid FALSE if valid
 * */
bool invalidNameCheck (char* ptr);

/* This function returns which kind of sentence we analyze
 * @param - char *line the line we analyze from the source file
 * */
sentenceTypes sentence_identifier(char* line, errors *errorNum);

/*Fix line gets a line we scanned from the file and fix him so we can process it more easily
 * @param - char *line - the line we fixing
 * @param - char *fixedLine - The new line after we fix it
 * */
void fixLine(char *line, char *fixedLine);

/*Print error gets an Enum error and type error message to the user according to the value of the Enum
 * @param - errors errorNum - The Enum type error
 * @param - int lineNumber - The line detect the error used to print to the user
 * @param - const char *source - The source where we detect the error
 * @return - bool T\F - T if we are print error F if not
 * */
void printError(errors errorNum, int lineNumber, const char *source);

/*Free functions units all the free method we use to one place so we can control what needs to be free each time
 * @param - FILE **source_file - Pointer to the file we process
 * @param - FILE **dest_file - Pointer to the file we create
 * @param - char ** sourceName - Pointer to the name of the file (.am\.as\)
 * @param - char **destName - Pointer to the name to the file we create (.ob\.ent\.ext)
 * @param - mcro **macroHead - Pointer to the head of the macro list
 * @param - symbol **symbolsHead - Pointer to the head of the symbols list
 * @param - missingMemoryInfo **infoHead - Pointer to the head of the missing memory info list
 * @param machine_word *dataArray - Pointer to the data array
 * */
void freeFunction(FILE **source_file, FILE **dest_file, char **sourceName, char **destName, mcro **macroHead,
                  symbols **symbolHead, missingMemoryInfo **infoHead);

#endif

