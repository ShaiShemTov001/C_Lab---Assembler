#include "data_handler.h"
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "utils.h"
#include "stdlib.h"
#include "conversions.h"

directives directive_identifier(char **line) {
    char* ptr = NULL, *directive = NULL;
    char lineCpy[MAX_LINE_LENGTH + 1] = {'\0'};

    strcpy(lineCpy, *line); /* Copy content of line to lineCpy */
    ptr = lineCpy; /* Set ptr to point to lineCpy */
    ptr = skip_spaces(ptr); /* Move ptr to the first character in line */
    directive = strtok(ptr, " \t"); /* Tokenize ptr to get the directive */
    *line = skip_spaces(*line); /* Skip spaces in the original line */
    if (strncmp(directive, ".data", strlen(".data")) == 0) {
        *line = *line + strlen(directive); /* Move line pointer past the directive */
        return DATA;
    } else if (strncmp(directive, ".string", strlen(".string")) == 0) {
        *line = *line + strlen(directive);
        return STRING;
    } else if (strncmp(directive, ".entry", strlen(".entry")) == 0) {
        *line = *line + strlen(directive);
        return ENTRY;
    } else if (strncmp(directive, ".extern", strlen(".extern")) == 0) {
        *line = *line + strlen(directive);
        return EXTERN;
    }
    return NONE_DIRECTIVE; /* If none of the directives match */
}

void dataInsertion(int *DC, char *line, machine_word **array, int type, errors *errorNum, int memoryNum) {
    machine_word mWord;
    data_word dWord;
    int num = 0, before_DC = *DC; /*Save the value so that if we find error we revert the changes to DC*/
    char* pointer = NULL;

    if(type == STRING){
        /*if the string is empty, initiate null char '\0' in memory to end of the string*/
        if(*line == '\0') {
            dWord.data = 0;
            mWord = create_machine_word_with_data_word(&dWord);
            (*array)[(*DC)] = mWord;
            (*DC)++;
            return;
        }
        /* Check if the line starts with a double quote */
        if(*line != '"') {
            if(*line == ',') *errorNum = ILLEGAL_COMMA;
            else *errorNum = TEXT_OUT_OF_APOSTROPHES;
            (*DC) = before_DC;
            return;
        }
        line++; /*skipping the first quote */
        /* Iterate until the closing double quote is found */
        while(*line != '"' && *line != '\0' && *line != '\n') {
            if(memoryNum >= MAX_MEMORY_SIZE){
                *errorNum = MEMORY_FULL;
                return;
            }
            /* Store the character as unsigned char data and create machine word*/
            dWord.data = (unsigned char)*line;
            mWord = create_machine_word_with_data_word(&dWord);
            (*array)[(*DC)] = mWord;
            (*DC)++;
            line++;
        }
        if(*line != '"') {/* If the first character of 'line' is not a double quote */
            *errorNum = NO_CLOSING_APOSTROPHES;
            (*DC) = before_DC;
            return;
        }
        line++; /*skipping the second quote */
        line = skip_spaces(line);
        if (*line != '\0' && *line != '\n') { /* If 'line' is not at the end or newline character */
            *errorNum = EXTRANEOUS_TEXT; /* Set 'errorNum' to indicate extraneous text */
            (*DC) = before_DC;
            return;
        }
        /*Initiate null char '\0' in memory to end of the string*/
        dWord.data = 0;
        mWord = create_machine_word_with_data_word(&dWord);
        (*array)[(*DC)] = mWord;
        (*DC)++;
    }/*End of .String handler*/

        /*Numbers type data*/
    else {
        /*To check the last char in the line we need to remove '\n' to use strlen()-1 */
        if(line[strlen(line)-1] == '\n'){
            line[strlen(line)-1] = '\0';
        }
        /*checking validation commas in two situation - line without number and line with numbers  */
        if (*line == ',') {
            *errorNum = ILLEGAL_COMMA;
            (*DC) = before_DC;
            return;
        }
        if (strlen(line) == 0 || line[strlen(line) - 1] == ',') { /* If line is empty or ends with a comma */
            *errorNum = MISSING_OPERAND; /* Set 'errorNum' to indicate missing operand */
            (*DC) = before_DC; /* Restore 'DC' to its previous value */
            return;
        }
        /* Iterate until the end of line or newline character,if we get to the while - we have numbers in the line */
        while (*line != '\0') { /* Loop while the character in 'line' is not null terminator */
            if (memoryNum >= MAX_MEMORY_SIZE) { /* If memoryNum exceeds maximum size */
                *errorNum = MEMORY_FULL; /* Set 'errorNum' to indicate memory full */
                return;
            }

            if (*line == ',') { /* If the character is a comma */
                if (*(line - 1) == ',')
                    *errorNum = MULTIPLE_COMMAS;
                else
                    *errorNum = ILLEGAL_COMMA;
                (*DC) = before_DC;
                return;
            }
            if (!isdigit(*line) && *line != '+' && *line != '-') { /* If character is not a digit, '+' or '-' */
                *errorNum = INVALID_DATA_TYPE;
                (*DC) = before_DC;
                return;
            }
            num = strtol(line, &pointer, 10); /* Convert string to integer */
            dWord.data = (int) num; /* Store the integer as data */
            mWord = create_machine_word_with_data_word(&dWord);
            (*array)[(*DC)] = mWord;
            (*DC)++;
            line = skip_spaces(pointer);
            if (*line == ',') { /* If the character is a comma */
                line++; /* Move to the next character */
                continue;
            }
            if (*line != ',' && isdigit(*line)) { /* If the character is not a comma and is a digit */
                *errorNum = MISSING_COMMA;
                (*DC) = before_DC;
                return;
            }else {
                continue;
            }
        }
    }
}

