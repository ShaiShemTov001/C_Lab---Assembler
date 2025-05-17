# C_Lab-Assembler

This project is an assembler, written in the C programming language, for an imaginary CPU with a 12-bit memory word size. This was an assignment for the C programming language course at the Open University.

**Authors:** Liron Ariel and Shai Shem Tov

**Project Description:**

The purpose of this project was to develop an assembler that translates assembly language code into machine code for a specified, simplified computer architecture. The assembler supports a custom assembly language with features including:

* Instructions with 0, 1, or 2 operands
* Addressing modes: immediate, direct, and register direct
* Directives for data allocation (`.data`, `.string`), external symbol declaration (`.extern`), and entry point declaration (`.entry`)
* Macros

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

**Assembly Language Features (as per the assignment):**

* **Memory:** 12-bit memory words, 1024 memory locations (0-1023)
* **Registers:** 8 general-purpose registers (`r0` - `r7`), 12-bits wide
* **Instructions:**
    * Two-operand instructions: `mov`, `cmp`, `add`, `sub`, `lea`
    * One-operand instructions: `not`, `clr`, `inc`, `dec`, `jmp`, `bne`, `red`, `prn`, `jsr`
    * Zero-operand instructions: `rts`, `stop`
* **Addressing Modes:** Immediate (1), Direct (3), Register Direct (5)
* **Directives:**
    * `.data` -  Defines data values 
    * `.string` - Defines a string
    * `.entry` -  Declares an entry point
    * `.extern` - Declares an external symbol
* **Macros:** Macro definition and expansion are supported (no nested macros)
* **Statements:** The assembly language consists of different types of statements:
    * Empty lines
    * Comment lines (starting with `;`)
    * Directives
    * Instructions
