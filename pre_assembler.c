#include "pre_assembler.h"
#include "linked_lists.h"
#include "utils.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>

status pre_assembler(char *source, FILE **source_file, mcro **newMcro, FILE **dest_file, mcro **head) {
    mcro *current = NULL;  /* Current macro being processed */
    bool mcroFlag = FALSE;  /* Flag to indicate if a macro is being processed */
    char line[MAX_LINE_LENGTH] = {'\0'};  /* Buffer to hold each line of the file */
    char *linePtr = NULL;  /* Pointer to the current line */
    int lineNumber = 0;
    bool errorFlag = FALSE;
    char c;
    /*Scanning each line from the file process it, and write it in ".am" file */
    while (fgets(line, sizeof(line), *source_file) != NULL) {
        if (strchr(line, '\n') == NULL && !feof(*source_file)) {
            /* Write the incomplete line to the destination file */
            fprintf(*dest_file, "%s", line);
            /* Continue reading characters until a newline character or the end of the file is encountered */
            while ((c = fgetc(*source_file)) != '\n' && c != EOF) {
                /* Write the character to the destination file */
                fputc(c, *dest_file);
            }
        }
        lineNumber++;
        linePtr = skip_spaces(line);
        if (strncmp(linePtr, "mcro", strlen("mcro")) == 0 && mcroFlag == FALSE) {
            /*If we are in a macro declaration we call the process macro declaration function */
            if(process_macro_declaration(linePtr, lineNumber, source, head, &current, &mcroFlag, &errorFlag, newMcro) == FATAL_ERROR){
                return FATAL_ERROR;
            }
        } else if (mcroFlag == TRUE) {
            linePtr = skip_spaces(linePtr);
            if (strncmp(linePtr, "endmcro", strlen("endmcro")) == 0) {
                /*If we are at endmcro line we need to finish process the mcro */
                process_endmcro(linePtr, &errorFlag, &mcroFlag, lineNumber, source);
            } else {
                /*If we are not at endmcro line Append the line to the content of the current macro */
                if(appendContent(current, line) == FATAL_ERROR)
                    return FATAL_ERROR;
            }
        } else {
            /*processing line that not in macro declaration */
            process_regular_line(head, linePtr, lineNumber, source, line, *dest_file, &errorFlag, newMcro);
        }
    }
    if(errorFlag == TRUE)
        return SYNTAX_ERROR;
    return SUCCESSFULL;
}

status process_macro_declaration(char *linePtr, int lineNumber, const char *source , mcro **head, mcro **current,
                                 bool *mcroFlag, bool *errorFlag, mcro **newMcro) {
    int scanned = 0;
    char rest[MAX_LINE_LENGTH] = {'\0'};
    char mcro[MAX_MACRO_LENGTH] = {'\0'};
    char mcroName[MAX_MACRO_LENGTH] = {'\0'};
    *mcroFlag = TRUE;  /* Set the macro flag to TRUE indicating that a macro is being processed */
    scanned = sscanf(linePtr, "%s %s %s", mcro, mcroName, rest);  /* Extract the macro name */
    linePtr += strlen("mcro ");
    linePtr = skip_spaces(linePtr);
    if(mcroName[strlen(mcroName)] != '\0'){/*checks if name is too long*/
        printError(MACRO_NAME_DEVIATION, lineNumber, source);
        *errorFlag = TRUE;
        *mcroFlag = FALSE;
        return SYNTAX_ERROR;
    }
    if (strcmp(mcro, "mcro") != 0) {/*check if there is no extra characters*/
        printError(UNDEFINED_MACRO_DECLARATION, lineNumber, source);
        *errorFlag = TRUE;
        *mcroFlag = FALSE;
        return SYNTAX_ERROR;
    } else if (scanned == 3) {/*checks extra text*/
        printError(EXTRANEOUS_TEXT, lineNumber, source);
        *errorFlag = TRUE;
        *mcroFlag = FALSE;
        return SYNTAX_ERROR;
    } else if (invalidNameCheck(linePtr)) {/*checks if name is invalid*/
        printError(INVALID_MACRO_NAME, lineNumber, source);
        *errorFlag = TRUE;
        *mcroFlag = FALSE;
        return SYNTAX_ERROR;
    } else if (strlen(mcroName) >= MAX_MACRO_LENGTH) {/*checks if the name itself is more then the max*/
        printError(MACRO_NAME_DEVIATION, lineNumber, source);
        *errorFlag = TRUE;
        *mcroFlag = FALSE;
        return SYNTAX_ERROR;
    }
    *newMcro = createMacro(mcroName);  /* Create a new macro with the extracted name */
    if(*newMcro == NULL){
        printError(FAILED_ALLOCATED_MEMORY, lineNumber, source);
        return FATAL_ERROR;
    }
    if (*head == NULL) {
        *head = *newMcro;  /* Set the new macro as the head of the linked list */
        *current = *newMcro;  /* Set the new macro as the current macro */
    } else {
        (*current)->next = *newMcro;  /* Link the new macro to the current macro */
        *current = *newMcro;  /* Update the current macro to the new macro */
    }
    return SUCCESSFULL;
}

void process_endmcro(char *linePtr, bool *errorFlag, bool *mcroFlag, int lineNumber, const char *source) {
    linePtr += strlen("endmcro");  /* Move the pointer after "endmcro" */
    linePtr = skip_spaces(linePtr);

    if (*linePtr != '\0' && *linePtr != '\n') {/*checks for extra text*/
        printError(EXTRANEOUS_TEXT, lineNumber, source);
        *errorFlag = TRUE;
        *mcroFlag = FALSE;
        return;
    }
    *mcroFlag = FALSE;  /* Reset the macro flag to indicate that macro processing has ended */
}

void process_regular_line(mcro **head, char *linePtr, int lineNumber, const char *source, char *line,
                          FILE *dest_file, bool *errorFlag, mcro **newMcro){
    int scanned = 0;
    char rest[MAX_LINE_LENGTH] = {'\0'};
    char mcroName[MAX_MACRO_LENGTH] = {'\0'};
    scanned = sscanf(linePtr, "%s %s", mcroName, rest);  /* Extract a word from the line */
    *newMcro = searchMacroList(*head, mcroName);  /* Search for a macro with the extracted word as the name */
    if(scanned == 2 && *newMcro != NULL){/*checks for extra text*/
        printError(EXTRANEOUS_TEXT,lineNumber,source);
        *errorFlag = TRUE;
        return;
    }
    if (*newMcro != NULL) {/*if we dont found macro call we just write the line as is*/
        fprintf(dest_file, "%s", (*newMcro)->content);  /* Print the content of the macro to the destination file */
        *newMcro = NULL;
        mcroName[0] = '\0';
    } else {
        fprintf(dest_file, "%s", line);  /* Print the line as is to the destination file */
    }
}