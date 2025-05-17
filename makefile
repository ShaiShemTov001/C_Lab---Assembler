all: assembler

assembler: assembler.o utils.o linked_lists.o pre_assembler.o first_pass.o data_handler.o code_handler.o conversions.o second_pass.o write_files.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o utils.o linked_lists.o pre_assembler.o first_pass.o data_handler.o code_handler.o conversions.o second_pass.o write_files.o

assembler.o: assembler.c utils.h globals.h linked_lists.h pre_assembler.h first_pass.h data_handler.h code_handler.h conversions.h second_pass.h write_files.h
	gcc -ansi -Wall -pedantic -c assembler.c -o assembler.o

utils.o: utils.c utils.h
	gcc -ansi -Wall -pedantic -c utils.c

linked_lists.o: linked_lists.c linked_lists.h
	gcc -ansi -Wall -pedantic -c linked_lists.c

pre_assembler.o: pre_assembler.c pre_assembler.h
	gcc -ansi -Wall -pedantic -c pre_assembler.c

first_pass.o: first_pass.c first_pass.h
	gcc -ansi -Wall -pedantic -c first_pass.c

data_handler.o: data_handler.c data_handler.h
	gcc -ansi -Wall -pedantic -c data_handler.c

code_handler.o: code_handler.c code_handler.h
	gcc -ansi -Wall -pedantic -c code_handler.c

conversions.o: conversions.c conversions.h
	gcc -ansi -Wall -pedantic -c conversions.c

second_pass.o: second_pass.c second_pass.h
	gcc -ansi -Wall -pedantic -c second_pass.c

write_files.o: write_files.c write_files.h
	gcc -ansi -Wall -pedantic -c write_files.c

clean:
	rm -f *.o
