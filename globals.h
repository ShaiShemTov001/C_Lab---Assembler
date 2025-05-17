#ifndef C_LAB_PROJECT_OPENU_2023_GLOBALS_H
#define C_LAB_PROJECT_OPENU_2023_GLOBALS_H

/*Maximum length of a single source line */
#define MAX_LINE_LENGTH 81
/* Maximum length of a label name* */
#define MAX_LABEL_LENGTH 32
/* Maximum length of a macro name* */
#define MAX_MACRO_LENGTH 32
/*Maximum memory size */
#define MAX_MEMORY_SIZE 1024
/*Memory initial boot - save space for computer memory size 100 memory words*/
#define SAVED_MEMORY 100

#define DEFAULT_ENTRY_MEMORY -1

#define DEFAULT_EXTERN_MEMORY -2

#define SUFFIX 5

/*Boolean (t/f) definition */
typedef enum booleans {
    FALSE = 0, TRUE = 1
} bool;

typedef enum opcode {
    ERROR = -1,
    MOV = 0, CMP, ADD, SUB, NOT, CLR,
    LEA, INC, DEC, JMP, BNE, RED, PRN, JSR,
    RTS, STOP
}opcode;

typedef enum ARE{
    A = 0, /* 00 */
    R = 2, /* 10 */
    E = 1 /* 01 */
}ARE;

typedef enum address {
    IMMEDIATE_ADDR =1, /* Immediate address */
    DIRECT_ADDR = 3, /* Direct address */
    REGISTER_ADDR = 5, /*Register direct address */
    NONE_ADDR = 0
}address;

typedef enum sentence_types{
    EMPTY = 0,  /* White spaces line " " "\t" "\n" */
    COMMENT,  /* Comment line starts with ; */
    DIRECTIVE,  /* Directive line (data, string, extern, entry) */
    INSTRUCTION,  /* Command line  */
    LABEL,       /* SYMBOLS */
    NONE       /*Initiate value */
} sentenceTypes;

typedef enum directives {
    DATA = 1,
    STRING,
    ENTRY,
    EXTERN,
    NONE_DIRECTIVE
}directives;

typedef struct commandVariables {
    char commandPtr[MAX_LINE_LENGTH];
    opcode command;
    char operand1[MAX_LINE_LENGTH];
    char operand2[MAX_LINE_LENGTH];
    address operand1address;
    address operand2address;
} commandVariables;

typedef struct data_word{
    unsigned int data : 12;
}data_word;

typedef struct first_word{
    unsigned int ARE : 2; /*bytes 0-1*/
    unsigned int dest_operand : 3;/*bytes 2-4*/
    unsigned int opcode : 4;/*bytes 5-8*/
    unsigned int src_operand : 3;/*bytes 9-11*/
}first_word;


typedef struct imdt_drct_word{
    unsigned int data : 10;
    unsigned int ARE : 2;
}imdt_drct_word;

typedef struct reg_word{
    unsigned int dest_reg : 5;
    unsigned int src_reg : 5;
    unsigned int ARE : 2;
}reg_word;

typedef struct machine_word{
    unsigned int mWord : 12;
}machine_word;

typedef struct base64{
    char first ;
    char last ;
}base64;

typedef enum errors{
    NO_ERROR = 0,

    /*Memory Errors */
    MEMORY_FULL,
    FAILED_ALLOCATED_MEMORY,
    /*File Errors */
    OPENING_FILE,
    FILE_NOT_FOUND,
    CREATE_FILE,
    NO_FILE_INSERTED,

    /*Text Errors*/
    EXTRANEOUS_TEXT,
    LINE_DEVIATION,
    MACRO_NAME_DEVIATION,
    LABEL_NAME_DEVIATION,
    TEXT_OUT_OF_APOSTROPHES,
    NO_CLOSING_APOSTROPHES,

    /*Already \ Undefined \ Undeclard Errors */
    ALREADY_DEFINED_LABEL,
    LABEL_BEFORE_EX_EN,
    UNDEFINED_MACRO_DECLARATION,
    COMMAND_UNDEFINED,
    UNDECLARED_LABEL_OPERAND,

    /*Invalidations Error */
    INVALID_DATA_TYPE,
    INVALID_MACRO_NAME,
    INVALID_LABEL_NAME,
    INVALID_OPERAND,
    /*Missing Errors */
    MISSING_LABEL,
    MISSING_COMMA,
    MISSING_OPERAND,

    /*Comma Errors */
    MULTIPLE_COMMAS,
    ILLEGAL_COMMA
}errors;

typedef enum{
    SUCCESSFULL,
    SYNTAX_ERROR,
    FATAL_ERROR
} status;


typedef struct MACRO {
    char name[MAX_MACRO_LENGTH];
    char *content;
    struct MACRO *next;
} mcro;

typedef struct SYMBOLS{
    char name[MAX_LABEL_LENGTH];
    int memory;
    int type;
    bool isCode;
    struct SYMBOLS *next;
} symbols;

/*This struct is held the info about symbols we found as operand in the file and need to encoding them in the second pass */
typedef struct MEMORYINFO {
    int memoryToFill;
    char operand[MAX_LABEL_LENGTH];
    int lineNumber;
    struct MEMORYINFO *next;
}missingMemoryInfo;



#endif
