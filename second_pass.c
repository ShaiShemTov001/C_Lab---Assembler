#include "second_pass.h"
#include <stdlib.h>
#include "utils.h"
#include "globals.h"
#include "conversions.h"
#include "linked_lists.h"

status
secondPass(machine_word *memoryImage, char *source, missingMemoryInfo *missingInfoHead, symbols *symbolHead) {
    status secondPassStatus = SUCCESSFULL;
    symbols *currentSymbol = NULL;
    missingMemoryInfo *missingInfoCurrent = missingInfoHead;
    imdt_drct_word drctWord;
    machine_word mWord;
    /*run on the missingInfo to search symbols as operand */
    while(missingInfoCurrent != NULL) {
        /*if we found we search them on the symbols list */
        currentSymbol = searchSymbolTable(symbolHead, missingInfoCurrent->operand, 0, 0);
        /*If we don't found in means that the labels has never been declared and we use is as operand - error */
        if (currentSymbol == NULL) {
            printError(UNDECLARED_LABEL_OPERAND, missingInfoCurrent->lineNumber, source);
            secondPassStatus = SYNTAX_ERROR;
            missingInfoCurrent = missingInfoCurrent->next;
            continue;
        }
            /*If we found that symbol we encoding him into the empty space we save for him in the memoryImage */
        else {
            drctWord.data = currentSymbol->memory; /* Set 'data' field in 'drctWord' to the memory of 'currentSymbol' */
            if (currentSymbol->type == EXTERN) { /* If 'currentSymbol' type is EXTERN */
                drctWord.data = 0; /* Set 'data' field to 0 for EXTERN type */
                drctWord.ARE = E; /* Set 'ARE' field to E for EXTERN type */
            } else {
                drctWord.ARE = R; /* Set 'ARE' field to R for other types */
            }
            mWord = create_machine_word_with_imdt_drct_word(&drctWord); /* Create machine word using 'drctWord' */
            memoryImage[missingInfoCurrent->memoryToFill] = mWord; /* Update memoryImage with the created machine word */
            missingInfoCurrent = missingInfoCurrent->next; /* Move to the next missing info node */
        }
    }
    return secondPassStatus;
}

