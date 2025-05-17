#include "write_files.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "conversions.h"

void printEntryToFile(symbols *symbolHead, char *destName, FILE **destFile) {
    symbols *current = symbolHead;
    bool isEmpty = TRUE;
    /*Search entry's to print from the symbol list */
    while (current != NULL) { /* Loop through the symbol table */
        if (current->type != ENTRY) { /* If the type of 'current' is not ENTRY */
            current = current->next; /* Move to the next node */
        } else {
            fprintf(*destFile, "%s\t%d\n", current->name, current->memory); /* Print name and memory to the destination file */
            current = current->next; /* Move to the next node */
            isEmpty = FALSE; /* Set 'isEmpty' flag to FALSE */
        }
    }
    /*If we dont find any we do not make an entry file */
    if(isEmpty){
        remove(destName);
    }
}

void printExternToFile(symbols *symbolHead, missingMemoryInfo *missingInfoHead, char *destName, FILE **destFile) {
    symbols *symbolCurrent = symbolHead;
    missingMemoryInfo *missingInfoCurrent = missingInfoHead;
    /* Allocate memory for the source file name */
    bool isEmpty = TRUE;

    /*Running on the missing memory list and search for extern to print  */
    while (missingInfoCurrent != NULL) {
        while (symbolCurrent != NULL) {
            /*IF we find an extern we check if he had the same name as the missing info list and print it if he does */
            if (strcmp(symbolCurrent->name, missingInfoCurrent->operand) == 0 && symbolCurrent -> type == EXTERN) {
                fprintf(*destFile, "%s\t%d\n", missingInfoCurrent->operand, missingInfoCurrent->memoryToFill);
                isEmpty = FALSE;
                break;
            }
            symbolCurrent = symbolCurrent->next;
        }
        symbolCurrent = symbolHead;
        missingInfoCurrent = missingInfoCurrent->next;
    }
    /*If we dont find extern to print we do not make extern file */
    if(isEmpty){
        remove(destName);
    }
}

void printMemoryImage(machine_word *memoryImage, int IC, int DC, char *destName, FILE **destFile) {
    int i = SAVED_MEMORY;
    bool isEmpty = TRUE;
    base64 base64output;
    /*First we print the DC and IC counters */
    fprintf(*destFile, "%d %d\n", IC-SAVED_MEMORY, DC);
    /*Running throw all the memoryImage convert it to base64 representation and print it */
    for(; i < IC+DC; i++){
        base64output = convertMachineWordToBase64(memoryImage[i]);
        fprintf(*destFile,"%c%c\n", base64output.first, base64output.last);
        isEmpty = FALSE;
    }
    if(isEmpty){
        remove(destName);
    }
}
