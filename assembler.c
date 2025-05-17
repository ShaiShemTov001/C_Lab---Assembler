#include "pre_assembler.h"
#include "utils.h"
#include "first_pass.h"
#include "second_pass.h"
#include "write_files.h"

/*Processor initialize all the variables that we need for more than one function in this project, for more control
 * about the memory we allocated and needs to be free we work with pointers and send them from and to here.
 * @param - const char *source - the file we process */
void processor (const char *source);

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {/*argc must be greater then 1*/
        printError(NO_FILE_INSERTED, 0, "");
        exit(1);
    }
    for(i = 1; i < argc; i++){
        processor(argv[i]);
        /*this is the main loop that contain the file names from the user that needs to be compiled (could be more than one)
         * from here we will send the files one by one to validate and make the operation*/
    }
    return 0;
}

void processor (const char *source){
    status assemblerStatus = SUCCESSFULL;
    bool printFlag = TRUE;  /* Flag to control printing */
    char *destName = NULL;  /* Destination name */
    char *sourceName = NULL;  /* Source name */
    mcro *newMcro = NULL;  /* Pointer to a mcro structure */
    mcro *mcroHead = NULL;  /* Pointer to the head of a linked list */
    FILE* destFile = NULL;  /* Destination file pointer */
    FILE* sourceFile = NULL;  /* Source file pointer */
    int IC = SAVED_MEMORY; /*Instruction counter*/
    int DC = 0; /*directive counter*/
    symbols *symbolHead = NULL;  /* Pointer to the head of a linked list of symbols */
    missingMemoryInfo *infoHead = NULL;  /* Pointer to the head of a linked list of missing memory information */
    /*The data image */
    machine_word dataArray[MAX_MEMORY_SIZE] = {0};
    machine_word *dataArrayPtr = dataArray;
    /*The code image - first fill only with instruction, then we fill all the data we stored in dataArray */
    machine_word memoryImage[MAX_MEMORY_SIZE] = {0};
    machine_word *memoryImagePtr = memoryImage;
    /* Construct the source and destination file names */
    destName = (char*) mallocWithPrint(strlen(source) + SUFFIX);
    if(destName == NULL){
        exit(1);
    }

    sourceName = (char*) mallocWithPrint(strlen(source) + SUFFIX);
    if (sourceName == NULL){
        free(destName);
        exit(1);
    }
    strcpy(sourceName, source);
    strcat(sourceName, ".as"); /* add the suffix .as to the end of the filename. */
    strcpy(destName, source);
    strcat(destName, ".am"); /* add the suffix .am to the end of the filename. */

    /* Open the source file in read mode */
    sourceFile = fopen(sourceName, "r");
    if (sourceFile == NULL) {
        printError(FILE_NOT_FOUND, 0, sourceName);
        freeFunction(NULL, NULL, &sourceName, &destName, NULL, NULL, NULL);
        exit(1);
    }
    /* Open the destination file in write mode */
    destFile = fopen(destName, "w");
    if (destFile == NULL) {
        printError(CREATE_FILE, 0, sourceName);
        freeFunction(&sourceFile, NULL, &sourceName, &destName, NULL, NULL, NULL);
        exit(1);
    }
    /*Calling the pre assembler function to process the file  */
    assemblerStatus = pre_assembler(sourceName, &sourceFile, &newMcro, &destFile, &mcroHead);
    if(assemblerStatus == FATAL_ERROR){
        freeFunction(&sourceFile, &destFile, &sourceName, &destName, &mcroHead, NULL, NULL);
        exit(1);
    }
    if(assemblerStatus == SYNTAX_ERROR){
        printFlag = FALSE;
    }
    /*Free memory for the first pass*/
    freeFunction(&sourceFile, &destFile, &sourceName, NULL, &mcroHead, NULL, NULL);
    sourceName = destName;/* sourceName - .am*/
    destName = NULL;
    sourceFile = fopen(sourceName, "r");
    if(sourceFile == NULL){
        free(sourceName);
        exit(1);
    }
    /*Calling the first pass to create the memory image. */
    assemblerStatus = firstPass(&sourceFile, &sourceName, &symbolHead, &infoHead, &dataArrayPtr, &memoryImagePtr,
                                &DC, &IC);

    if(assemblerStatus == FATAL_ERROR){
        freeFunction(&sourceFile, NULL, &sourceName, NULL, NULL, &symbolHead,
                     &infoHead);
        exit(1);
    }
    if(assemblerStatus == SYNTAX_ERROR){
        printFlag = FALSE;
    }
    freeFunction(&sourceFile, NULL, NULL, NULL, NULL, NULL,
                 NULL);
    /*Calling the second pass the complete the memory image */
    assemblerStatus = secondPass(memoryImagePtr, sourceName, infoHead, symbolHead);
    if(assemblerStatus == SYNTAX_ERROR){
        printFlag = FALSE;
    }
    destName = sourceName;
    sourceName = NULL;
    /*No error occurred - print the .ob .ent .ext files */
    if(printFlag == TRUE){
        strcpy(strrchr(destName, '.'), ".ent");/*adds suffix .ent to sourceName*/
        destFile = fopen(destName, "w"); /* Open the source file in write mode */
        if (destFile == NULL) {
            /* Error handling if the source file cannot be opened */
            printError(OPENING_FILE, 0, destName);
            freeFunction(NULL, NULL, NULL, &destName, NULL, &symbolHead, &infoHead);
            exit(1);
        }
        printEntryToFile(symbolHead, destName, &destFile);
        fclose(destFile);

        strcpy(strrchr(destName, '.'), ".ext");/*adds suffix .ext to sourceName*/
        destFile = fopen(destName, "w");  /* Open the source file in write mode */
        if (destFile == NULL) {
            /* Error handling if the source file cannot be opened */
            printError(OPENING_FILE, 0, destName);
            freeFunction(NULL, NULL, NULL, &destName, NULL, &symbolHead, &infoHead);
            exit(1);
        }
        printExternToFile(symbolHead, infoHead, destName, &destFile);
        fclose(destFile);

        strcpy(strrchr(destName, '.'), ".ob");/*adds suffix .ob to sourceName*/
        destFile = fopen(destName, "w"); /* Open the source file in write mode */
        if (destFile == NULL) {
            /* Error handling if the source file cannot be opened */
            printError(OPENING_FILE, 0, destName);
            freeFunction(NULL, NULL, NULL, &destName, NULL, &symbolHead, &infoHead);
            exit(1);
        }
        printMemoryImage(memoryImage, IC, DC, destName, &destFile);
        freeFunction(NULL, &destFile, NULL, &destName, NULL, &symbolHead, &infoHead);

    }else {
        freeFunction(NULL, NULL, NULL, &destName, NULL, &symbolHead, &infoHead);
        return;
    }
}
