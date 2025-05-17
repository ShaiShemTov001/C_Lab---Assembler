#ifndef C_LAB_PROJECT_OPENU_2023_LINKED_LISTS_H
#define C_LAB_PROJECT_OPENU_2023_LINKED_LISTS_H

#include <string.h>
#include "utils.h"

/*Create macro creates a new macro node
 * @param - const chat *name - name of the macro
 * @return - mcro - the macro we created
 * */
mcro *createMacro(const char *name);

/*Append Content add to the macro Struct all the macro content until we reach to endmacro declaration
 * @param - mcro *macro - The current macro we process
 * @param - const char *line - The line we need to append
 * */
status appendContent(mcro *macro, const char *line);

/*Search Macro List searches if we already define this macro
 * @param - mcro *head - Starting point of the search is the head of the list
 * @param - char *name - The name of the marco we search
 * @return - mcro - Returning the macro we found if so, if not returning NULL
 * */
mcro* searchMacroList(mcro *head, char* name);

/*Free Macros free all the macros we create in the file after we finish the pre-assembler step
 * @param - mcro *head - Starting point of the search is the head of the list
 * */
void freeMacros(mcro *head);

/*Create symbols list create a symbol table first it makes a node and then insert it to the list
 * @param - const char *name - the name of the symbol
 * @param - int memoryNum - the memory location were the symbol declared
 * @param - int type - The directive type of the symbol
 * @return - symbols - the symbol we created
 * */
symbols *createSymbolTable(const char *name, int memoryNum, int type);

/*Remove labels from the symbols list
 * @param - symbols *current - A pointer to start the search for remove
 * @param - symbols *toRemove - The label we want to remove from the list
 * */
void remove_label(symbols* current, symbols* toRemove);

/*Search Symbol in the symbol table we can search by the name the memory location or the type of the symbol
 * @param - symbols *head - Starting point of the search is the head of the list
 * @param - char *name - The name we search
 * @param - int memory_num - The memory location we search
 * @param - int type - The type we search
 * @return - symbols - Returning the symbol we found if so , else returning NULL
 * */
symbols* searchSymbolTable(symbols *head, char* name, int memory_num, int type);

/*Remove Entry declaration removes all the symbols that was only a declaration of symbols
 * @param - symbols *head - *head - Starting point to search and remove
 * @return - symbols *head - The list after we remove the entry's declaration
 * */
symbols *removeEntryDeclarationFromSymbolTable (symbols *head);

/*Free symbols table free the symbol list after we finish the second line
 * @param - symbols *head - Starting point of the free function is the head of the list
 * */
void freeSymbolTable(symbols *head);

/*Missing memory info is a node struct that held the information about labels used as operand we build this list to
 * encoding all the symbols at the second pass
 * @param - int memoryToFill - The location memory we need to fill in the memoryImage
 * @param - int LineNumber - The line number of the symbol as operand
 * @param - char *operand - The name of the symbol
 * @return - missingInfo - the struct we just build
 * */
missingMemoryInfo *createMissingMemoryInfo(int memoryToFill, int lineNumber, char *operand);

/*Free the missing memory info list
 * @param - missingMemoryInfo *head - A starting point to free all the list
 * */
void freeMissingMemoryInfoList(missingMemoryInfo *head);

#endif
