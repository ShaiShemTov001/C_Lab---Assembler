
#ifndef C_LAB_PROJECT_OPENU_2023_SECOND_PASS_H
#define C_LAB_PROJECT_OPENU_2023_SECOND_PASS_H
#include "globals.h"
#include "linked_lists.h"

/*The second pass function is the final path of the process we search for symbols that used as operand and encoding them
 * into the memory image, after that our memory image is complete and if theres no errors we print the .ob .ent.ext files
 * @param - machine_word *memoryImage - The array that contains the memory
 * @param - const char *sourceName - The file name we process
 * @param - int IC - The instruction counter
 * @param - int DC - The data counter
 * @param - missingMemoryInfo *missingInfoHead - A pointer to the head of missingMemoryInfo list
 * @param - symbols *head - A pointer to the symbols list
 * @param - bool isError - Indicate us if there was an error in the first pass
 * */
status
secondPass(machine_word *memoryImage, char *source, missingMemoryInfo *missingInfoHead, symbols *symbolHead);


#endif

