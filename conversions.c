#include "conversions.h"
#include "globals.h"

void binary_machine_code(unsigned int value, int num_bits, machine_word *data) {
    /*This line calculates a mask that consists of num_bits set to 1 and all other bits set to 0.*/
    unsigned int mask = (1 << num_bits) - 1;
    /*Combines the shifted mWord and the masked value using bitwise OR*/
    (data)->mWord = ((data)->mWord << num_bits) | (value & mask);
}

machine_word create_machine_word_with_first_word(first_word* data) {
    machine_word word;
    word.mWord = 0;
    /* Convert the 'src_operand' to binary and store it in the machine_word. */
    binary_machine_code(data->src_operand, 3, &word);
    /* Convert the 'opcode' to binary and store it in the machine_word. */
    binary_machine_code(data->opcode, 4, &word);
    /* Convert the 'dest_operand' to binary and store it in the machine_word. */
    binary_machine_code(data->dest_operand, 3, &word);
    /* Convert the 'ARE' to binary and store it in the machine_word. */
    binary_machine_code(data->ARE, 2, &word);
    /* Return the pointer to the created machine_word. */
    return word;
}

machine_word create_machine_word_with_reg_word(reg_word* data) {
    machine_word word;
    word.mWord = 0;
    /* Convert the 'src_reg' to binary and store it in the machine_word. */
    binary_machine_code(data->src_reg, 5, &word);
    /* Convert the 'dest_reg' to binary and store it in the machine_word. */
    binary_machine_code(data->dest_reg, 5, &word);
    /* Convert the 'ARE' to binary and store it in the machine_word. */
    binary_machine_code(data->ARE, 2, &word);
    /* Return the pointer to the created machine_word. */
    return word;
}

machine_word create_machine_word_with_imdt_drct_word(imdt_drct_word* data) {
    machine_word word;
    word.mWord = 0;
    /* Convert the 'data' to binary and store it in the machine_word. */
    binary_machine_code(data->data, 10, &word);
    /* Convert the 'ARE' to binary and store it in the machine_word. */
    binary_machine_code(data->ARE, 2, &word);
    /* Return the pointer to the created machine_word. */
    return word;
}

machine_word create_machine_word_with_data_word(data_word* data) {
    machine_word word;
    word.mWord = 0;
    /* Convert the 'data' to binary and store it in the machine_word. */
    binary_machine_code(data->data, 12, &word);
    /* Return the pointer to the created machine_word. */
    return word;
}

first_word create_first_word_from_command_variables(commandVariables* content) {
    first_word fWord;
    /* Initialize the first_word content using the fields from 'content'. */
    fWord.opcode = content->command;
    fWord.src_operand = content->operand1address;
    fWord.dest_operand = content->operand2address;
    /* Set 'ARE' field to A in command line. */
    fWord.ARE = A;
    /* Return the pointer to the created first_word. */
    return fWord;
}

base64 convertMachineWordToBase64(machine_word wordToConvert) {
    base64 result;
    const char base64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    /* Create the first 6-bit representation using the base64 alphabet */
    result.last = base64_alphabet[wordToConvert.mWord & 0x3F];
    /* Shift the input 6 bits to the right to get the remaining 6 bits */
    wordToConvert.mWord >>= 6;
    /* Create the last 6-bit representation using the base64 alphabet */
    result.first = base64_alphabet[wordToConvert.mWord & 0x3F];
    return result;
}





