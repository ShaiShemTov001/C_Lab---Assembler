#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "code_handler.h"
#include "globals.h"


/* Skip leading spaces in a string */
char* skip_spaces(char* str) {
    while (*str != '\0' && (*str == '\t' || *str == '\n' || *str == ' ' || *str == '\r')) {
        str++; /* Increment pointer to skip spaces, tabs, and newlines */
    }
    return str; /* Return the updated pointer */
}
/*malloc function, if couldn't allocate memory, print error*/
void *mallocWithPrint(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        printError(FAILED_ALLOCATED_MEMORY, 0, "");
    }
    return ptr;
}

void label_checker(char *line, char *labelName, directives type , errors *errorNum) {
    char *label;
    strncpy(labelName, line, MAX_LABEL_LENGTH);
    labelName = skip_spaces(labelName);
    if (*labelName == ',') { /* If the first character of 'labelName' is a comma */
        *errorNum = ILLEGAL_COMMA;
        return;
    }
    /*In extern there isn't ':' after the label*/
    if(type != EXTERN && type != ENTRY){
        label = strtok(labelName, ":");
    }
    else{
        label = strtok(labelName, " \t\n");
    }
    if(label[MAX_LABEL_LENGTH - 1] != '\0'){
        *errorNum = LABEL_NAME_DEVIATION;
        return;
    }
    /*Call the function to check if the label name is valid */
    if (invalidNameCheck(label)) {
        *errorNum = INVALID_LABEL_NAME;
        return;
    }
    if (isalpha(*label)) { /* If the first character of 'label' is alphabetic */
        while (*label != '\0' && isalnum(*label)) { /* Loop while 'label' characters are alphanumeric */
            label++; /* Move to the next character */
        }
        if (*label != '\0') { /* If 'label' is not at the end */
            *errorNum = INVALID_LABEL_NAME; /* Set 'errorNum' to indicate invalid label name */
            return; /* Exit the function */
        }
    }
}

bool invalidNameCheck (char* ptr) {
    char *invalidLabel[] = {"mov", "cmp", "add","sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
                            "red", "jsr", "rts", "stop", "mcro"};
    int i;
    if(!isalpha(*ptr)){
        return TRUE;/*invalid name*/
    }
    for (i = 0; i < sizeof(invalidLabel) / sizeof(invalidLabel[0]); i++) {
        if (strcmp(ptr, invalidLabel[i]) == 0) {
            return TRUE;/*invalid name*/
        }
    }
    return FALSE;
}

sentenceTypes sentence_identifier(char* line, errors *errorNum){
    char lineCpy[MAX_LINE_LENGTH+1] = ""; /*buffer so we wont change the original line */
    char* ptr = NULL;
    int result = ERROR; /* default initiate if dont change we have an error */
    strcpy(lineCpy, line);
    ptr = skip_spaces(lineCpy);
    strtok(ptr, " \t\n");
    if(*ptr == '\0') result = EMPTY;
    else if(*ptr == ';') result = COMMENT;
    else if (command_num(ptr) != ERROR) result = INSTRUCTION;
    else if (*ptr == '.') result = DIRECTIVE;
    else if(ptr[strlen(ptr)-1] == ':') result = LABEL;
    else *errorNum = COMMAND_UNDEFINED;
    return result;
}

void fixLine(char *line, char *fixedLine) {
    int i = 0, j = 0;
    size_t size = strlen(line);
    bool prevWhiteSpace = FALSE, addComma = FALSE, prevComma = FALSE;
    char *str = fixedLine;
    /*Change the '\n' and '\r' char to \'0' so we can check the last char in given line */
    line[strcspn(line, "\r\n")] = 0;
    line = skip_spaces(line); /* Remove leading spaces in 'line' */
    for (i = 0; i < size; i++) {
        if (addComma) { /* If 'addComma' flag is true */
            str[j++] = ','; /* Add a comma to 'str' */
            i--; /* Decrement 'i' to repeat the current character */
            addComma = FALSE; /* Reset 'addComma' flag */
            continue; /* Skip to the next iteration */
        }

        if (line[i] != '\0' && (line[i] == '\t' || line[i] == '\n' || line[i] == ' ')) {
            if (prevComma) {
                continue; /* Skip if previous character was a comma */
            }
            if (prevWhiteSpace) {
                continue; /* Skip if previous character was a white space */
            } else {
                prevWhiteSpace = TRUE; /* Set 'prevWhiteSpace' flag to true */
                str[j++] = ' '; /* Add a space to 'str' */
                continue; /* Skip to the next iteration */
            }
        }
        if (line[i] == ',') { /* If the character is a comma */
            if (prevComma) {
                str[j++] = ','; /* Add a comma to 'str' if previous character was also a comma */
                continue; /* Skip to the next iteration */
            }
            if (prevWhiteSpace) {
                prevComma = TRUE; /* Set 'prevComma' flag to true */
                str[j++] = ','; /* Add a comma to 'str' */
                continue; /* Skip to the next iteration */
            } else {
                str[j++] = ' '; /* Add a space to 'str' */
                addComma = TRUE; /* Set 'addComma' flag to true */
                prevComma = TRUE; /* Set 'prevComma' flag to true */
            }
        } else {
            prevComma = FALSE; /* Reset 'prevComma' flag */
            prevWhiteSpace = FALSE; /* Reset 'prevWhiteSpace' flag */
            str[j++] = line[i]; /* Add the character to 'str' */
        }
    }
    if (prevComma) {
        str[j++] = ','; /* Add a comma to 'str' if 'prevComma' flag is true */
    }
    str[j] = '\0'; /* Add null terminator to 'str' to terminate the string */
}

void printError(errors errorNum, int lineNumber, const char *source){
    switch (errorNum) {
        case MEMORY_FULL:
            fprintf(stderr, "Error! File: %s, Line: %d: Cannot receive another line: Memory full\n", source, lineNumber);
            break;
        case FAILED_ALLOCATED_MEMORY:
            fprintf(stderr, "Error! Failed to allocate memory\n");
            break;
        case OPENING_FILE:
            fprintf(stderr, "Error! File: %s: Cannot open the file\n", source);
            break;
        case FILE_NOT_FOUND:
            fprintf(stderr, "Error! The file: %s Not found\n", source);
            break;
        case CREATE_FILE:
            fprintf(stderr, "Error! File: %s: Cannot create the file\n", source);
            break;
        case NO_FILE_INSERTED:
            fprintf(stderr, "Error! No file was inserted\n");
            break;
        case EXTRANEOUS_TEXT:
            fprintf(stderr, "Error! File: %s, Line: %d: Extraneous text\n", source, lineNumber);
            break;
        case LINE_DEVIATION:
            fprintf(stderr, "Error! File: %s, Line: %d: Line too long\n", source, lineNumber);
            break;
        case MACRO_NAME_DEVIATION:
            fprintf(stderr, "Error! File: %s, Line: %d: MACRO name contains more characters than the maximum\n", source, lineNumber);
            break;
        case LABEL_NAME_DEVIATION:
            fprintf(stderr, "Error! File: %s, Line: %d: Label name contains more characters than the maximum\n", source, lineNumber);
            break;
        case TEXT_OUT_OF_APOSTROPHES:
            fprintf(stderr, "Error! File: %s, Line: %d: String is out of \" \" \n", source, lineNumber);
            break;
        case NO_CLOSING_APOSTROPHES:
            fprintf(stderr, "Error! File: %s, Line: %d: No closing \" at the and of the string\n", source, lineNumber);
            break;
        case ALREADY_DEFINED_LABEL:
            fprintf(stderr, "Error! File: %s, Line: %d: Label already defined\n", source,lineNumber);
            break;
        case LABEL_BEFORE_EX_EN:
            fprintf(stderr, "Warning! File: %s, Line: %d: Label before extern or entry\n",source, lineNumber);
            break;
        case UNDEFINED_MACRO_DECLARATION:
            fprintf(stderr, "Error! File: %s, Line: %d: Undefined macro declaration\n", source, lineNumber);
            break;
        case COMMAND_UNDEFINED:
            fprintf(stderr, "Error! File: %s, Line: %d: Undefined command\n", source, lineNumber);
            break;
        case UNDECLARED_LABEL_OPERAND:
            fprintf(stderr, "Error! File: %s, Line: %d: label operand is never declared\n", source, lineNumber);
            break;
        case INVALID_DATA_TYPE:
            fprintf(stderr, "Error! File: %s, Line: %d: Invalid data type\n", source, lineNumber);
            break;
        case INVALID_MACRO_NAME:
            fprintf(stderr, "Error! File: %s, Line: %d: Invalid macro name\n",source, lineNumber);
            break;
        case INVALID_LABEL_NAME:
            fprintf(stderr, "Error! File: %s, Line: %d: Invalid label name\n",source, lineNumber);
            break;
        case INVALID_OPERAND:
            fprintf(stderr, "Error! File: %s, Line: %d: Invalid operand \n",source, lineNumber);
            break;
        case MISSING_LABEL:
            fprintf(stderr, "Error! File: %s, Line: %d: Missing label\n",source, lineNumber);
            break;
        case MISSING_COMMA:
            fprintf(stderr, "Error! File: %s, Line: %d: Missing comma\n",source, lineNumber);
            break;
        case MISSING_OPERAND:
            fprintf(stderr, "Error! File: %s, Line: %d: Missing operand\n",source, lineNumber);
            break;
        case MULTIPLE_COMMAS:
            fprintf(stderr, "Error! File: %s, Line: %d: Multiple commas\n",source, lineNumber);
            break;
        case ILLEGAL_COMMA:
            fprintf(stderr, "Error! File: %s, Line: %d, Illegal comma\n", source, lineNumber);
            break;
        default:
            break;
    }
}


void freeFunction(FILE **source_file, FILE **dest_file, char **sourceName, char **destName, mcro **macroHead,
                  symbols **symbolHead, missingMemoryInfo **infoHead) {
    if (source_file != NULL && *source_file != NULL) {
        fclose(*source_file);
    }
    if (dest_file != NULL && *dest_file != NULL) {
        fclose(*dest_file);
    }
    if (sourceName != NULL && *sourceName != NULL) {
        free(*sourceName);
    }
    if (destName != NULL && *destName != NULL) {
        free(*destName);
    }
    if (macroHead != NULL && *macroHead != NULL) {
        freeMacros(*macroHead);
    }
    if(symbolHead != NULL && *symbolHead != NULL){
        freeSymbolTable(*symbolHead);
    }
    if(infoHead != NULL && *infoHead != NULL) {
        freeMissingMemoryInfoList(*infoHead);
    }
}
