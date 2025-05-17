#include "first_pass.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "globals.h"
#include "linked_lists.h"
#include "data_handler.h"
#include "code_handler.h"
#include "utils.h"

status firstPass(FILE **source_file, char **sourceName, symbols **symbolHead, missingMemoryInfo **infoHead,
                 machine_word **dataArray, machine_word **memoryImage, int *DC, int *IC) {
    status firstPassStatus = SUCCESSFULL;
    missingMemoryInfo *infoCurrent = NULL;
    symbols *newLabel = NULL, *symbolCurrent = NULL;
    int sentenceType = 0;
    int memoryNum = SAVED_MEMORY; /*Initial the first "free" place in the memory image after we save space for computer memory*/
    int lineNumber = 0;
    directives directiveType = 0;
    bool labelFlag = FALSE, entryFlag = FALSE;
    char line[MAX_LINE_LENGTH + 1] = {'\0'};  /* Buffer to hold each line of the file */
    char lineCpy[MAX_LINE_LENGTH * 2] = {'\0'};
    char *linePtr;
    char c;
    commandVariables *instructionContent = NULL;
    errors errorNum = NO_ERROR;
    char fixedLine[MAX_LINE_LENGTH * 2];

    /* Read and convert each line the binary representation and find the data\instruction type, also insert to the code image */
    while (fgets(line, sizeof(line), *source_file) != NULL) {
        labelFlag = FALSE;
        errorNum = NO_ERROR;
        lineNumber++;
        memoryNum = *IC + *DC;
        if (memoryNum >= MAX_MEMORY_SIZE) { /* If 'memoryNum' exceeds maximum size */
            printError(MEMORY_FULL, lineNumber, *sourceName);
            return FATAL_ERROR;
        }
        /* Check if line is longer then 81 char (including '\0') and clean garbage chars*/
        if (strchr(line, '\n') == NULL && !feof(*source_file)) {
            if (*skip_spaces(line) != ';') {
                printError(LINE_DEVIATION, lineNumber, *sourceName);
                firstPassStatus = SYNTAX_ERROR;
            }
            while ((c != '\n' && c != EOF)) {
                c = fgetc(*source_file);
            }
            continue;
        }
        /*Fixing the line that we can easily process */
        fixLine(line, fixedLine);
        linePtr = fixedLine;
        /*Copy the original line from source file to buffer and process the buffer */
        strcpy(lineCpy, linePtr);
        linePtr = lineCpy;
        /*Identify the line we process */
        sentenceType = sentence_identifier(lineCpy, &errorNum);
        /*If sentence type is invalid, continue to the net line */
        if (errorNum != NO_ERROR) {
            printError(errorNum, lineNumber, *sourceName);
            firstPassStatus = SYNTAX_ERROR;
            continue;
        }
        if (sentenceType == EMPTY || sentenceType == COMMENT) continue;
        /* Analyze the Label, crate the Label list or add to the list if already exist */
        if (sentenceType == LABEL) {
            /*Calling the labelDefinition function to add the label to the symbol list*/
            labelDefinition(&errorNum, &labelFlag, &linePtr, symbolHead, memoryNum, &directiveType,
                            &symbolCurrent, &newLabel, &entryFlag);
            if (errorNum == FAILED_ALLOCATED_MEMORY) {
                return FATAL_ERROR;
            }
            /*This error is actually a warning so we do not change the error flag, just print it and move on */
            if (errorNum == LABEL_BEFORE_EX_EN) {
                printError(errorNum, lineNumber, *sourceName);
                errorNum = NO_ERROR;
            }
            if (errorNum != NO_ERROR) {
                printError(errorNum, lineNumber, *sourceName);
                firstPassStatus = SYNTAX_ERROR;
                continue;
            }
        } /*finish process label */
        if (labelFlag == FALSE) {
            /*Identify the directive type of the sentence and move the pointer after the type */
            directiveType = directive_identifier(&linePtr);
            linePtr = skip_spaces(linePtr);
        }
        if (directiveType == DATA || directiveType == STRING) {
            /*If we in label type of sentence we change the memory location of the symbol to DC */
            if (labelFlag) {
                symbolCurrent->memory = *DC;
            }
            /*Insert the data to data array as machine word */
            dataInsertion(DC, linePtr, dataArray, directiveType, &errorNum, memoryNum);
            if (errorNum != NO_ERROR) {
                printError(errorNum, lineNumber, *sourceName);
                firstPassStatus = SYNTAX_ERROR;
                continue;
            }
            if (entryFlag && labelFlag) {
                newLabel->type = ENTRY;
                entryFlag = FALSE;
            }
            continue;

        } else if (directiveType == ENTRY || directiveType == EXTERN) {
            /*Calling the process entry extern method to create the label to the symbol list as entry\extern */
            processEntryExtern(linePtr, symbolHead, directiveType, &errorNum, &newLabel, &symbolCurrent);
            if (errorNum == FAILED_ALLOCATED_MEMORY) {
                return FATAL_ERROR;
            }
            if (errorNum != NO_ERROR) {
                printError(errorNum, lineNumber, *sourceName);
                firstPassStatus = SYNTAX_ERROR;
                continue;
            }
            /*If we reach here it means we process code line */
        } else if (directiveType == NONE_DIRECTIVE) {
            /*If this line contains label we set the symbol struct */
            if (labelFlag) { /* If 'labelFlag' is true */
                symbolCurrent->isCode = TRUE; /* Set 'isCode' field in 'symbolCurrent' to TRUE */
                symbolCurrent->memory = *IC; /* Set 'memory' field in 'symbolCurrent' to the value of 'IC' */
                if (entryFlag) { /* If 'entryFlag' is true */
                    symbolCurrent->type = ENTRY; /* Set 'type' field in 'symbolCurrent' to ENTRY */
                    entryFlag = FALSE; /* Reset 'entryFlag' to false */
                } else {
                    symbolCurrent->type = NONE_DIRECTIVE; /* Set 'type' field in 'symbolCurrent' to NONE_DIRECTIVE */
                }
            }
            /* Create commandVariable struct to process the code line */
            instructionContent = (commandVariables *) mallocWithPrint(sizeof(commandVariables));
            if (instructionContent == NULL) {
                return FATAL_ERROR;
            }
            /*Calling the command validation to process the code line */
            command_validation(linePtr, &instructionContent, &errorNum);
            if (errorNum != NO_ERROR) {
                printError(errorNum, lineNumber, *sourceName);
                firstPassStatus = SYNTAX_ERROR;
                free(instructionContent);
                continue;
            }
            /*return the struct full of the operand type and data and the kind of command*/
            /*insert the commandVariables to the memoryImage as machine word */
            if (code_insertion(IC, instructionContent, *memoryImage, infoHead, &infoCurrent, lineNumber, memoryNum) ==
                FATAL_ERROR) {
                free(instructionContent);
                return FATAL_ERROR;
            }
            free(instructionContent);
        }
    }/*end of while loop*/
    /*Connects between memoryImage and data array, remove Unnecessary symbols or an extern\entry declaration symbols from symbols table*/
    if (finalizeLabelsMemory(symbolHead, *IC, *DC, *memoryImage, *dataArray) == FATAL_ERROR) {
        printError(MEMORY_FULL, lineNumber, *sourceName);
        return FATAL_ERROR;
    }
    return firstPassStatus;
}

void labelDefinition(errors *errorNum, bool *labelFlag, char **linePtr, symbols **head, int memoryNum,
                     directives *directiveType, symbols **current, symbols **newLabel, bool *entryFlag) {
    char *tempPtr = NULL;
    char labelName[MAX_LABEL_LENGTH] = {'\0'};
    char *pLabelName = labelName;
    *labelFlag = TRUE;
    label_checker(*linePtr, pLabelName, 0, errorNum);
    *linePtr += strlen(pLabelName)+1;
    if (*errorNum == NO_ERROR) { /* If 'errorNum' is NO_ERROR, indicating a valid label name */
        *newLabel = searchSymbolTable(*head, labelName, 0, 0); /* Search for label in the symbol table */
        if (*newLabel == NULL || (*newLabel)->type == ENTRY) { /* If label does not exist in the symbol table or it's an ENTRY label */
            if (*newLabel != NULL) {
                *entryFlag = TRUE;
            }
            *newLabel = createSymbolTable(labelName, memoryNum, 0); /* Create a new label */
            if (*newLabel == NULL) { /* If memory allocation fails */
                *errorNum = FAILED_ALLOCATED_MEMORY;
                return;
            }
            if (*head == NULL) { /* If the symbol table is empty */
                *head = *newLabel; /* Set 'head' and 'current' to point to 'newLabel' */
                *current = *newLabel;
            } else {
                (*current)->next = *newLabel; /* Set the 'next' pointer of 'current' to point to 'newLabel' */
                *current = *newLabel; /* Update 'current' to point to 'newLabel' */
            }
        } else { /* If the label already exists and is not an ENTRY label */
            *errorNum = ALREADY_DEFINED_LABEL;
            return;
        }
    } else { /* If 'errorNum' is not NO_ERROR */
        return;
    }
    tempPtr = *linePtr; /* Store the value of 'linePtr' in 'tempPtr' */
    *directiveType = directive_identifier(linePtr); /* Determine the type of the directive */
    if (*directiveType == DATA || *directiveType == STRING) { /* If directive is DATA or STRING */
        (*current)->type = DATA; /* Set 'type' field in 'current' to DATA */
    }
    else if (*directiveType == EXTERN || *directiveType == ENTRY) {
        *errorNum = LABEL_BEFORE_EX_EN;
        if (*head == *newLabel) { /* If 'head' points to 'newLabel' */
            *head = NULL; /* Set 'head' to NULL */
        } else {
            remove_label(*head, *newLabel); /* Remove 'newLabel' from the symbol table */
        }
        *linePtr = tempPtr; /* Restore the value of 'linePtr' */
        *labelFlag = FALSE;
    }
    *linePtr = skip_spaces(*linePtr);
}/*end of if label*/

void processEntryExtern(char *linePtr, symbols **head, directives directiveType, errors *errorNum, symbols **newLabel,
                        symbols **current) {
    char labelName[MAX_LABEL_LENGTH] = {'\0'};
    char *pLabelName = labelName;
    int commaCounter = 0;
    if (*linePtr == '\0' || *linePtr == '\n') {
        *errorNum = MISSING_LABEL;
        return;
    }
    while (*linePtr != '\0' && *linePtr != '\n') {
        label_checker(linePtr, pLabelName, directiveType, errorNum);
        linePtr += strlen(pLabelName);
        if (*errorNum == NO_ERROR) { /*error in label name*/
            *newLabel = searchSymbolTable(*head, labelName, 0, 0);
            if (*newLabel == NULL) { /*does not exist in the label table */
                if (directiveType == ENTRY) { /* If directiveType is ENTRY */
                    *newLabel = createSymbolTable(labelName, DEFAULT_ENTRY_MEMORY, directiveType); /* Create symbol table entry with default memory and directiveType */
                } else if (directiveType == EXTERN) { /* If directiveType is EXTERN */
                    *newLabel = createSymbolTable(labelName, DEFAULT_EXTERN_MEMORY, directiveType); /* Create symbol table entry with default memory and directiveType */
                } else if (*newLabel == NULL) { /* If 'newLabel' is NULL (memory allocation failed) */
                    *errorNum = FAILED_ALLOCATED_MEMORY; /* Set 'errorNum' to indicate memory allocation failure */
                    return;
                }
                if (*head == NULL) { /* If the symbol table is empty */
                    *head = *newLabel; /* Set 'head' and 'current' to point to 'newLabel' */
                    *current = *newLabel;
                } else {
                    (*current)->next = *newLabel; /* Set the 'next' pointer of 'current' to point to 'newLabel' */
                    *current = *newLabel; /* Update 'current' to point to 'newLabel' */
                }
            } else {
                /*If we found a label and its an entry or extern it means we already declare about this label */
                if ((*newLabel)->type == ENTRY || (*newLabel)->type == EXTERN) {
                    *errorNum = ALREADY_DEFINED_LABEL;
                    return;
                } else {
                    /*If not it mean we used it as operand so we update the directive type to entry or extern */
                    (*newLabel)->type = directiveType;
                }
                /*Checks that there is no more declarations after the label */
            }
            if ((*newLabel)->type == ENTRY) { /* If 'newLabel' type is ENTRY */
                linePtr = skip_spaces(linePtr);
                if (*linePtr != '\0' && *linePtr != '\n') { /* If 'linePtr' is not at the end or newline character */
                    *errorNum = EXTRANEOUS_TEXT;
                    return;
                }
            }
            /*Extern declaration can be with more then one label, if so we check the amount of commas */
            if ((*newLabel)->type == EXTERN) {
                while (*linePtr != '\0' && (*linePtr == '\t' || *linePtr == '\n' || *linePtr == ' ' || *linePtr == ',')) {
                    if (*linePtr == ',') commaCounter++;
                    linePtr++;
                }
                if (commaCounter >= 2) {
                    *errorNum = MULTIPLE_COMMAS;
                    return;
                }
                if (commaCounter == 1) {
                    linePtr = skip_spaces(linePtr);
                    if (*linePtr == '\0' || *linePtr == '\n') {
                        *errorNum = MISSING_LABEL;
                        return;
                    }
                }
                if (commaCounter <= 0) {
                    if (*linePtr == '\0' || *linePtr == '\n') {
                        return;
                    }
                    *errorNum = MISSING_COMMA;
                    return;
                }
                commaCounter = 0;
            }
        }
    }
}

status finalizeLabelsMemory(symbols** head, int IC, int DC, machine_word *memoryImage, machine_word *dataArray) {
    int i;
    symbols *current = NULL;
    if(IC + DC >= MAX_MEMORY_SIZE){
        return FATAL_ERROR;
    }
    /*remove the Entry declaration from the label list */
    *head = removeEntryDeclarationFromSymbolTable(*head);
    current = *head; /* Set 'current' to point to the beginning of the symbol table */
    while (current != NULL) { /* Loop through the symbol table */
        if (current->type != NONE_DIRECTIVE && current->type != EXTERN && current->isCode == FALSE) {
            current->memory = current->memory + IC; /* Adjust 'memory' field based on 'IC' value */
        }
        current = current->next; /* Move to the next node in the symbol table */
    }
    /*connects between the memory image to the data array */
    for (i = 0; i < DC; i++) {
        memoryImage[IC + i] = dataArray[i];
    }
    return SUCCESSFULL;
}
