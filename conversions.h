#ifndef C_LAB_PROJECT_OPENU_2023_CONVERSIONS_H
#define C_LAB_PROJECT_OPENU_2023_CONVERSIONS_H
#include "globals.h"

/*Binary machine code - It extracts the least significant num_bits from value,
 * shifts the existing mWord in the structure to the left by num_bits positions,
 * and then combines the shifted mWord with the extracted bits from value,
 * updating the mWord field with the new value.
 * @param - unsigned int value - the value we want to convert(decimal)
 * @param - int num_bits - specifying the number of least significant bits to use for encoding value.
 * @param - machine_word **data - the struct that held the converts
 * */
void binary_machine_code(unsigned int value, int num_bits, machine_word *data);

/*Create_machine_word with first word creates machine word according to the struct first_word
 *@param - first_word *data - struct first_word
 *@return - machine_word
 * */
machine_word create_machine_word_with_first_word(first_word* data);

/*Create_machine_word with reg word creates machine word according to the struct reg_word
 *@param - reg_word *data - struct reg_word
 *@return - machine_word
 * */
machine_word create_machine_word_with_reg_word(reg_word* data);

/*Create_machine_word with immediate - direct word creates machine word according to the struct imdt_drct_word
 *@param - imdt_drct_word *data - struct imdt_drct_word
 *@return - machine_word
 * */
machine_word create_machine_word_with_imdt_drct_word(imdt_drct_word* data);

/*Create_machine_word with data word creates machine word according to the struct data_word
 *@param - data_word *data - struct data_word
 *@return - machine_word
 * */
machine_word create_machine_word_with_data_word(data_word* data);

/*Create first word from commandVariables creates first word according to the struct commandVariables
 *@param - commandVariables *content - struct commandVariables
 *@return - struct of commandVariables converts to first word
 * */
first_word create_first_word_from_command_variables(commandVariables *content);

/*Self use function help us to see the binary conversion to machine_word
 * */
void print_binary_representation(machine_word* data);

/*Convert12to64 converts machine word value to base 64 representation according to base64 struct
 * @param - machine_word word to convert
 * @return base64 representation of a given machine word
 * */
base64 convertMachineWordToBase64(machine_word wordToConvert);

#endif
