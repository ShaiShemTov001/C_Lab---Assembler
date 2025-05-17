#include <stdlib.h>
#include <string.h>
#include "linked_lists.h"
#include "utils.h"

mcro *createMacro(const char *name) {
    mcro *macro = (mcro *) mallocWithPrint(sizeof(mcro));
    if(macro == NULL){
        return NULL;
    }
    strcpy(macro->name, name);  /* Copy the name to the macro's name field */
    macro->content = NULL;  /* Set the macro's content to NULL initially */
    macro->next = NULL;  /* Set the macro's next pointer to NULL initially */
    return macro;  /* Return the newly created macro */
}
status appendContent(mcro *macro, const char *line) {
    char* temp = NULL;
    size_t length = strlen(line);  /* Get the length of the line */
    size_t contentLength = 0;
    if (macro->content == NULL) {
        /* If the macro's content is empty, allocate memory for the content and copy the line */
        macro->content = (char *) mallocWithPrint(length + 1);
        if(macro -> content == NULL) return FATAL_ERROR;
        strcpy(macro->content, line);  /* Copy the line to the macro's content */
    } else {
        /* If the macro's content is not empty, reallocate memory to accommodate the new line */
        contentLength = strlen(macro->content);  /* Get the length of the existing content */
        temp = (char *)realloc(macro->content, contentLength + length + 1);
        if(temp == NULL){
            printError(FAILED_ALLOCATED_MEMORY, 0, "");
            return FATAL_ERROR;/* Exit the program with an error code */
        }
        macro->content = temp;
        strcat(macro->content, line);  /* Concatenate the new line to the existing content */
    }
    return SUCCESSFULL;
}

mcro* searchMacroList(mcro *head, char* name) {
    mcro *current = head;  /* Start with the head of the macro linked list */
    while (current != NULL) {  /* Iterate through each macro */
        if (strcmp(current->name, name) == 0) {  /* Check if the current macro's name matches the search name */
            return current;
        }
        current = current->next;
    }
    return NULL;  /* Return NULL when the macro is not found */
}

void freeMacros(mcro *head) {
    mcro *current = head;  /* Start with the head of the macro linked list */
    mcro *temp = NULL;
    while (current != NULL) {  /* Iterate through each macro */
        temp = current;
        current = current->next;
        if (temp->content != NULL) {
            free(temp->content);
        }
        free(temp);
    }
}


symbols *createSymbolTable(const char *name, int memoryNum, int type) {
    symbols *label = (symbols *) mallocWithPrint(sizeof(symbols));
    if(label == NULL){
        return NULL;
    }
    strcpy(label -> name, name); /*Copy the name of the label to the name field */
    label -> memory = memoryNum; /*Insert the memory number to the memory location field */
    label -> type = type; /*Insert the directive type to the type field */
    label -> next = NULL;
    label -> isCode = FALSE; /*Flag that indicate if we are in instruction line */
    return label;
}

void remove_label(symbols* current, symbols* toRemove) {
    symbols *prev = NULL;
    if (current == NULL || toRemove == NULL) { /*check there is something to remove */
        return;
    }
    if(current == toRemove){
        free(toRemove);
        return;
    }
    /* Iterate throw all the list */
    while(current != toRemove && current != NULL){
        /*Remove the symbol from the list */
        prev = current;
        current = current->next;
    }
    prev -> next = current -> next;
    /*free the struct we remove */
    free(toRemove);
}

symbols* searchSymbolTable(symbols *head, char* name, int memory_num, int type) {
    symbols *current = head;
    /*searching by Label name*/
    if (memory_num == 0 && type == 0) {
        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                return current;
            }
            current = current->next;
        }
        /*searching memory location*/
    } else if(memory_num != 0) {
        while (current != NULL) {
            if (memory_num == current->memory) {
                return current;
            }
            current = current->next;
        }
    }
        /*searching by type*/
    else{
        while (current != NULL) {
            if (type == current->type) {
                return current;
            }
            current = current->next;
        }
    }
    return NULL; /*return NULL if we didn't found*/
}

symbols *removeEntryDeclarationFromSymbolTable(symbols *head){
    symbols *prev = NULL;
    symbols *current = NULL;
    symbols *temp = NULL;
    /*If the head is entry declaration */
    while (head != NULL && head->memory == DEFAULT_ENTRY_MEMORY) { /* While 'head' is not NULL and memory of 'head' is DEFAULT_ENTRY_MEMORY */
        temp = head; /* Assign 'head' to 'temp' */
        head = head->next; /* Move 'head' to the next node */
        free(temp); /* Free memory allocated for the node being deleted */
    }
    if(head != NULL){
        prev = head;
        current = head -> next;
        /*Search throw all the symbol list and remove the label declarations  */
        while (current != NULL) { /* Loop through the symbol table */
            if (current->memory == DEFAULT_ENTRY_MEMORY) { /* If the memory field is equal to DEFAULT_ENTRY_MEMORY */
                temp = current; /* Assign 'current' to 'temp' */
                current = current->next; /* Move 'current' to the next node */
                prev->next = current; /* Update 'next' pointer of 'prev' to skip the node being deleted */
                free(temp); /* Free memory allocated for the node being deleted */
            } else {
                prev = current; /* Move 'prev' to 'current' */
                current = current->next; /* Move 'current' to the next node */
            }
        }
    }
    return head;
}
void freeSymbolTable(symbols *head) {
    symbols *current = head;  /* Start with the head of the macro linked list */
    symbols *temp = NULL;
    while (current != NULL) {  /* Iterate through each macro */
        temp = current;
        current = current ->next;
        free(temp);
    }
}
/*--------------end symbols list ---------------- */

missingMemoryInfo *createMissingMemoryInfo(int memoryToFill, int lineNumber, char *operand){
    missingMemoryInfo *missingInfo = (missingMemoryInfo*) mallocWithPrint(sizeof(missingMemoryInfo));
    if(missingInfo == NULL){
        return NULL;
    }
    strcpy(missingInfo -> operand, operand); /*Copy the label name to the operand field */
    missingInfo -> memoryToFill = memoryToFill; /*insert the place in memory we need to fill  */
    missingInfo -> lineNumber = lineNumber; /*insert the line number of the label */
    missingInfo -> next = NULL;
    return missingInfo;
}
void freeMissingMemoryInfoList(missingMemoryInfo *head) {
    missingMemoryInfo *current = head;  /* Start with the head of the macro linked list */
    missingMemoryInfo *temp = NULL;
    while (current != NULL) {  /* Iterate through each macro */
        temp = current;
        current = current -> next;
        free(temp);
    }
}
