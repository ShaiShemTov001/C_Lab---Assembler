#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "code_handler.h"
#include "globals.h"
#include "linked_lists.h"
#include "utils.h"
#include "conversions.h"

void command_validation(char *line, commandVariables **data, errors *errorNum) {
    opcode command = ERROR;  /* Command value by the typedef */
    char linePtr[MAX_LINE_LENGTH] = {'\0'}; /*Pointer to the first label and, or, command */
    int operandType = NONE;   /* Initiate value for operandType */
    strcpy(linePtr, line);
    /*Calling the code_extractor function */
    code_extractor(linePtr, &(*data), errorNum);
    if(*errorNum > 0) return; /*Error found go back to the first pass*/
    command = (*data)->command;
    /*Checks the operands by the command type if valid insert the type to the commandVariables struct */
    switch (command) {
        /*Command with two operands*/
        case MOV:
        case ADD:
        case SUB:
        case CMP:
        case LEA:
            operandType = check_operand((*data)->operand1,errorNum);
            (*data)->operand1address = operandType;
            if ((command == LEA && operandType != DIRECT_ADDR) || operandType == ERROR) {
                /* Check if the command is LEA and the operand type is not DIRECT_ADDR, or if the operand type is ERROR */
                *errorNum = INVALID_OPERAND;
                return;
            }
            operandType = check_operand((*data)->operand2, errorNum);
            (*data)->operand2address = operandType;
            if (command != CMP || operandType == ERROR) {
                /* Check if the command is not CMP or if the operand type is ERROR */
                if (operandType == IMMEDIATE_ADDR || operandType == ERROR) {
                    /* Check if the operand type is IMMEDIATE_ADDR or ERROR */
                    *errorNum = INVALID_OPERAND;
                    return;
                }
            }
            return; /*Finish process the line*/
            /*Commands with one operand */
        case NOT :
        case CLR :
        case INC :
        case DEC :
        case JMP :
        case BNE :
        case RED :
        case JSR :
        case PRN :
            operandType = check_operand((*data)->operand2, errorNum);
            (*data)->operand2address = operandType;
            if (command != PRN || operandType == ERROR) {
                /* Check if the command is not PRN or if the operand type is ERROR */
                if (operandType == IMMEDIATE_ADDR || operandType == ERROR) {
                    /* Check if the operand type is IMMEDIATE_ADDR or ERROR */
                    *errorNum = INVALID_OPERAND;
                    return;
                }
            }
            return; /*Finish process the line */
            /*Command with no operands */
            /*Stop and rts already check in the code extractor if invalid error has been sent */
        case STOP:
        case RTS:
        default:
            return;
    }
}

opcode command_num(char *ptr) {
    if (strcmp("mov", ptr) == 0) return MOV; /* Decimal code - 0 */
    else if (strcmp("cmp", ptr) == 0) return CMP;/* Decimal code - 1 */
    else if (strcmp("add", ptr) == 0) return ADD;/* Decimal code - 2 */
    else if (strcmp("sub", ptr) == 0) return SUB;/* Decimal code - 3 */
    else if (strcmp("not", ptr) == 0) return NOT;/* Decimal code - 4 */
    else if (strcmp("clr", ptr) == 0) return CLR;/* Decimal code - 5 */
    else if (strcmp("lea", ptr) == 0) return LEA;/* Decimal code - 6 */
    else if (strcmp("inc", ptr) == 0) return INC;/* Decimal code - 7 */
    else if (strcmp("dec", ptr) == 0) return DEC;/* Decimal code - 8 */
    else if (strcmp("jmp", ptr) == 0) return JMP;/* Decimal code - 9 */
    else if (strcmp("bne", ptr) == 0) return BNE;/* Decimal code - 10 */
    else if (strcmp("red", ptr) == 0) return RED;/* Decimal code - 11 */
    else if (strcmp("prn", ptr) == 0) return PRN;/* Decimal code - 12 */
    else if (strcmp("jsr", ptr) == 0) return JSR; /*Decimal code - 13 */
    else if (strcmp("rts", ptr) == 0) return RTS;/*Decimal code - 14 */
    else if (strcmp("stop", ptr) == 0) return STOP;/*Decimal code - 15 */
    else return ERROR; /* Unknown command */
}

address check_operand(char *sourceOperand, errors *errorNum) {
    char *operand = sourceOperand;
    /*Address type of operand (1,3,5) initial to NONE_ADDR as default */
    address operandAddType = NONE_ADDR;
    /*Checks if register operand is valid */
    int registerNumber = 0;
    if (*operand == '@') {
        operand++;
        if (*operand != 'r') {
            /* Check if the next character is 'r' */
            *errorNum = INVALID_OPERAND;
            return ERROR;
        }
        operand++;
        registerNumber = strtol(operand, &operand, 10);
        /* Convert the operand to an integer using strtol */
        if (registerNumber < 0 || registerNumber > 7) {
            /* Check if the register number is within the valid range */
            *errorNum = INVALID_OPERAND;
            return ERROR;
        }
        /*Register is valid, operand type is register address */
        operandAddType = REGISTER_ADDR;
        /*Checks if operand is number - immediate address*/
    } else if (*operand == '+' || *operand == '-' || isdigit(*operand)) {
        if (*operand == '+' || *operand == '-')
            operand++;
        while (isdigit(*operand) && *operand != '\0') {
            /* Check if the current character is a digit */
            operand++;
        }
        if (!(isdigit(*operand)) && *operand != '\0') {
            /* Check if the current character is not a digit and not the end of the operand */
            *errorNum = INVALID_OPERAND;
            return ERROR;
        }
        /*Operand is valid, operand type is immediate address */
        operandAddType = IMMEDIATE_ADDR;
        /*Checks if the operand is Label - direct address */
    } else{
        if (invalidNameCheck(operand)) {
            /* Check if the operand has an invalid name */
            *errorNum = INVALID_OPERAND;
            return ERROR;
        }
        operand++;
        while (isalnum(*operand))
            operand++;
/* Continue iterating while the current character is alphanumeric */
        if (!isalnum(*operand) && *operand != '\0') {
            /* Check if the current character is not alphanumeric and not the end of the operand */
            *errorNum = INVALID_OPERAND;
            return ERROR;
        }else
            /*Operand is valid, operand type is direct address */
            operandAddType =  DIRECT_ADDR;
    }
    return operandAddType;
}

void code_extractor(char *line, commandVariables **data, errors *errorNum) {
    char command[MAX_LINE_LENGTH] = "";  /* Stores the command */
    char rest[MAX_LINE_LENGTH] = "";  /* Stores the rest of the line */
    char operand1[MAX_LINE_LENGTH] = "";  /* Stores the first operand */
    char operand2[MAX_LINE_LENGTH] = "";  /* Stores the second operand */
    opcode commandNum = 0;  /* Stores the command number */
    /*Scanning the line and "cut" it to tokens to check if the amount of operand is as same as the command demands*/
    int scanned = sscanf(line, " %[^\t\n ] %[^\t\n ] %[^\t\n ] %s", command, operand1, operand2, rest);
    (*data)->operand1address = 0; /* Set the operand 1 type in the data structure to 0. */
    (*data)->operand2address = 0; /* Set the operand 2 type in the data structure to 0. */
    (*data)->command = ERROR;
    /* Call the 'command_num' function to get the command number based on the command string. */
    commandNum = command_num(command);
/* Get the command number based on the command string */
    if(commandNum == ERROR){
        /* Check if the command number is ERROR */
        *errorNum = COMMAND_UNDEFINED;
        return;
    }
    /*No operand commands */
    if (scanned == 1) {
        if (commandNum == RTS || commandNum == STOP) {
            /* If there's only one scanned item, and it corresponds to RTS or STOP command, proceed with processing. */
            strcpy((*data)->commandPtr, command); /* Copy the command to the data structure. */
            (*data)->command = command_num(command); /* Set the command number in the data structure. */
            operand1[0] = '\0'; /* Clear operand1 since it should not be used in this case. */
            operand2[0] = '\0'; /* Clear operand2 since it should not be used in this case. */
            strcpy((*data)->operand1, operand1); /* Copy the empty operand1 to the data structure. */
            strcpy((*data)->operand2, operand2); /* Copy the empty operand2 to the data structure. */
            return;
        } else{
            /* If there's only one scanned item again, it means there's a missing operand after the command. */
            *errorNum = MISSING_OPERAND;
            return;
        }
    }
    /* If there are more than one scanned items, it means there is extra text after the STOP or RTS command.*/
    if (commandNum == RTS || commandNum == STOP) {
        *errorNum = EXTRANEOUS_TEXT;
        return;
    }
    if (scanned == 2) {
        if(commandNum == MOV || commandNum == ADD || commandNum == SUB || commandNum == LEA || commandNum == CMP){
            /*Scanned two tokens and this is a command that requires two operands we have missing operand error */
            *errorNum = MISSING_OPERAND;
            return;
        }else {
            if(*operand1 == ',' || *(operand1 + strlen(operand1)-1) == ','){
                /*If we scanned two tokens and the second one starts with comma,
                 * or ends with comma that means we have illegal comma  */
                *errorNum = ILLEGAL_COMMA;
                return;
            }
        }
        /* If two items are scanned and this not one of the commands above it means there's only one operand. */
        strcpy((*data)->commandPtr, command); /* Copy the command to the data structure.*/
        (*data)->command = command_num(command); /* Set the command number in the data structure.*/
        operand2[0] = '\0'; /* Clear operand2 since it should not be used in this case.*/
        strcpy((*data)->operand1, operand2); /* Copy the empty operand2 to the data structure.*/
        strcpy((*data)->operand2, operand1); /* Copy the operand to operand1 in the data structure.*/
        return;
    }

    if (scanned == 3) {
        if(!(commandNum == MOV || commandNum == ADD || commandNum == SUB || commandNum == LEA || commandNum == CMP)){
            /*If we scanned three tokens and this isn't a two operand type of command this is incompatible command error */
            if(*operand2 == ','){
                *errorNum = ILLEGAL_COMMA;
            }
            else *errorNum = EXTRANEOUS_TEXT;
            return;
        }else {
            /*Checks that we have the correct amount of commas */
            if (*operand1 == ',') {
                /* Check if the first operand is a comma */
                *errorNum = ILLEGAL_COMMA;
                return;
            }
            if (*operand2 == ',') {
                /* Check if the second operand is a comma */
                if (*(operand2 + 1) == ',') {
                    /* Check if there are multiple consecutive commas */
                    *errorNum = MULTIPLE_COMMAS;
                    return;
                } else if (*skip_spaces(operand2 + 1) == '\n' || *skip_spaces(operand2 + 1) == '\0') {
                    /* Check if the operand after the comma is missing */
                    *errorNum = MISSING_OPERAND;
                    return;
                }
            }
            if (*operand2 != ',') {
                /* Check if the second operand is not a comma */
                *errorNum = MISSING_COMMA;
                return;
            }
        }
        /* If three items are scanned, it means there are two operand and no rest.*/
        strcpy((*data)->commandPtr, command); /* Copy the command to the data structure.*/
        (*data)->command = command_num(command); /* Set the command number in the data structure.*/
        strcpy((*data)->operand1, operand1); /* Copy operand 1 to the data structure.*/
        strcpy((*data)->operand2, (operand2+1)); /* Copy operand 2 to the data structure.*/
        return;
    }
    /*If there are more than four scanned items, it means there is extra text after the command and arguments.*/
    if (scanned == 4) {
        if(*rest == ','){
            *errorNum = ILLEGAL_COMMA;
        }
        else {
            *errorNum = EXTRANEOUS_TEXT;
        }
        return;
    }
}

status code_insertion(int *IC, commandVariables *instructionContent, machine_word *array, missingMemoryInfo **head,
                      missingMemoryInfo **current, int lineNumber, int memoryNum) {
    first_word fWord;
    machine_word mWord;
    reg_word rWord;
    imdt_drct_word imdrWord;
    fWord = create_first_word_from_command_variables(instructionContent);
    mWord = create_machine_word_with_first_word(&fWord);
    (array)[*IC] = mWord;  /* Store 'mWord' in array at index '*IC' */
    (*IC)++;               /* Increment value pointed to by 'IC' */


    /*Command line is from reg to reg - only add one line*/
    if (instructionContent->operand1address == REGISTER_ADDR && instructionContent->operand2address == REGISTER_ADDR){
        if(memoryNum >= MAX_MEMORY_SIZE)
            return FATAL_ERROR;
        /*We skip '@' and 'r' to insert the number of the reg into the struct*/
        rWord.src_reg = atoi(instructionContent->operand1 + 2); /* Extract source register from operand1 and convert to int */
        rWord.dest_reg = atoi(instructionContent->operand2 + 2); /* Extract destination register from operand2 and convert to int */
        rWord.ARE = A;  /* Set ARE field in rWord */
        mWord = create_machine_word_with_reg_word(&rWord); /* Create machine word using rWord */
        (array)[*IC] = mWord;
        (*IC)++;
    }
        /*Check what kind of address type any operand gets and add one code line for each operand */
    else {
        if (instructionContent->operand1address == IMMEDIATE_ADDR) {
            if (memoryNum >= MAX_MEMORY_SIZE) /* Check if memoryNum exceeds maximum size */
                return FATAL_ERROR;
            imdrWord.data = atoi(instructionContent->operand1); /* Convert operand1 to integer and store in imdrWord data */
            imdrWord.ARE = A;  /* Set ARE field in imdrWord */
            mWord = create_machine_word_with_imdt_drct_word(&imdrWord); /* Create machine word using imdrWord */
            (array)[*IC] = mWord;
            (*IC)++;
        } else if (instructionContent->operand1address == DIRECT_ADDR) {
            if (*head == NULL) {
                /*create a missingMemoryInfo with the label we found as an operand */
                if ((*head = createMissingMemoryInfo(*IC, lineNumber, instructionContent->operand1)) == NULL)
                    return FATAL_ERROR;
                else {
                    *current = *head;
                }
            } else {
                if (((*current)->next = createMissingMemoryInfo(*IC, lineNumber, instructionContent->operand1)) == NULL) /* If creating missing memory info fails */
                    return FATAL_ERROR;
                *current = (*current)->next; /* Update 'current' to point to the next node */
            }
            (*IC)++;
        }else if (instructionContent->operand1address == REGISTER_ADDR) { /* If operand1 address mode is register */
            if (memoryNum >= MAX_MEMORY_SIZE)  /* Check if memoryNum exceeds maximum size */
                return FATAL_ERROR;
            /*We skip '@' and 'r' to insert the number of the reg into the struct*/
            rWord.src_reg = atoi(instructionContent->operand1 + 2); /* Extract source register from operand1 and convert to int */
            rWord.dest_reg = 0; /* Set destination register to 0 */
            rWord.ARE = A; /* Set ARE field in rWord */
            mWord = create_machine_word_with_reg_word(&rWord); /* Create machine word using rWord */
            (array)[*IC] = mWord;
            (*IC)++;
        }

        if(instructionContent->operand2address == IMMEDIATE_ADDR){
            if (memoryNum >= MAX_MEMORY_SIZE) /* If memoryNum exceeds maximum size */
                return FATAL_ERROR; /* Return a fatal error */
            imdrWord.data = atoi(instructionContent->operand2); /* Convert operand2 to int and store in imdrWord data */
            imdrWord.ARE = A; /* Set ARE field in imdrWord */
            mWord = create_machine_word_with_imdt_drct_word(&imdrWord); /* Create machine word using imdrWord */
            (array)[*IC] = mWord;
            (*IC)++;
        }
        else if(instructionContent->operand2address == DIRECT_ADDR){
            /*create a missingMemoryInfo with the label we found as an operand */
            if (*head == NULL) { /* If 'head' points to NULL (empty list) */
                *head = createMissingMemoryInfo(*IC, lineNumber, instructionContent->operand2); /* Create missing memory info and assign to 'head' */
                if (*head == NULL) { /* If creation of missing memory info fails */
                    return FATAL_ERROR;
                } else { /* If creation succeeds */
                    *current = *head; /* Update 'current' to point to 'head' */
                }
                *current = *head;
            }
            else{
                if(((*current) -> next = createMissingMemoryInfo(*IC, lineNumber, instructionContent->operand2)) == NULL) return FATAL_ERROR;
                *current = (*current) -> next;
            }
            (*IC) ++;
        }
        else if(instructionContent -> operand2address == REGISTER_ADDR){
            if (memoryNum >= MAX_MEMORY_SIZE) /* If memoryNum exceeds maximum size */
                return FATAL_ERROR;
            rWord.src_reg = 0;
            /*We skip '@' and 'r' to insert the number of the reg into the struct*/
            rWord.dest_reg = atoi(instructionContent->operand2 + 2); /* Extract destination register from operand2 and convert to int */
            rWord.ARE = A; /* Set ARE field in rWord */
            mWord = create_machine_word_with_reg_word(&rWord); /* Create machine word using rWord */
            (array)[*IC] = mWord;
            (*IC)++;
        }
    }
    return SUCCESSFULL;
}

