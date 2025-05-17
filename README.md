# C_Lab-Assembler
This project is an assignment in C programing language course at the Open university.
The propose of this project it to build an assembler for imaginary CPU and 12bits memory size.
This project was done by Liron Ariel - 316382712 and Shai Sem Tov - 207187709.

Files : 
assembler - main function definition argc & argv processing, single file processing
pre_assembler - the pre processor, spread the macros that declared in the file
first_pass - contain function for processing a single, and encoding it to the memory image
second_pass - contain function for completing the memory image by rplacing labels by it's address
code_handler - contain function for process instruction line.
data_handler - contain function for process directive lines.
globals.h - contain type and constant definitions.
linked_list - contain function to manage the linked list we used.
conversions - contain fuction that creates machine_word according to the operand type.
utils - contain general proposed , project wide function.
writefiles - contains methods for writing output files: *.ob, *.ext and *.ent

at all the h. files you can find more detail about evey function
