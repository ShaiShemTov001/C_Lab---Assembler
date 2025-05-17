#ifndef C_LAB_PROJECT_OPENU_2023_WRITE_FILES_H
#define C_LAB_PROJECT_OPENU_2023_WRITE_FILES_H

#include "linked_lists.h"
#include "globals.h"

/*Print entry to file run on the symbols list and print all the symbol that held the type ENTRY, we can print them because
 * we already remove the .entry declaration
 * @param symbols *symbol head - The head of the symbols list where we start search
 * @param - const char *source - The source file name we want to make
 * */
void printEntryToFile(symbols *symbolHead, char *destName, FILE **destFile);

/*Print memory image converts the memory image to base64 representation and print it
 * @param - machine_word *memoryImage - The memory image we print
 * @param - int IC - Counter of instructions
 * @param - int DC - Counter of data
 * @param - const char *source - The file name we want to make
 * */
void printMemoryImage(machine_word *memoryImage, int IC, int DC, char *destName, FILE **destFile);

/*Print extern to file use the missing memory info list to search for the extern symbols that used as operand in the file
 * When we found one we search it in the symbols list to check that it have been declared if so we print it with the information
 * we save in the missing info struct
 * @param - symbols *symbolHead - The head of the symbols list where we start search
 * @param missingMemoryInfo *missingInfoHead - The head of the missingMemoryInfo
 * @param const char* source - The name of the file we want to make
 * */
void printExternToFile(symbols *symbolHead, missingMemoryInfo *missingInfoHead, char *destName, FILE **destFile);
#endif


