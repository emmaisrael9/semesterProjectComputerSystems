#include <stdio.h>
#include <string.h>
/**
 * @author Emma Israel
 */

#define MAX_LINES 2600  //Maximum space we need in the array ( in the case input has 52 letters and fsm has 50 states)

/**
 * Necessary methods
 */
int *processDefinitionFile(char *defFile, int *array1);
int *processInputFile(char *inputFile,int *array2);
int compareArrays(char *defFile, char *inputFile,int *array1, int *array2);
int numberOfLines(char *file);

/**
 * Debugger methods
 */
void debugger(char *file1, char *file2, int *array1,  int *array2);
int *processDefinitionFile1(char *defFile, int *array1);
int *processInputFile1(char *inputFile, int *array2);
int programWorking1(char *defFile, char *inputFile,int *array1, int *array2, int step, int input);

/**
 * Test method
 */
int checkNotEmptyFile(char *fileName);


int main( int numArgs, char *argFile[]){
    // Variables for files name:
    char *defFile;
    char *inputFile;
    // Initialize arrays to fill with content of the files
    int definitionArray[MAX_LINES * 3]; //must store three element for each line in the file so MAX size times 3
    int inputArray[MAX_LINES];
    int i = strcmp(argFile[1],"-d");
    // Run debugger and initialize the files name to the right argument of main:
    if (argFile[3] && i==0){ // if the main has a third argument, start the debugger and initialize variables
        defFile = argFile[2];
        inputFile = argFile[3];
        debugger(defFile, inputFile, definitionArray,inputArray);
        return 0;
    }
    else { //
        defFile = argFile[1];
        inputFile = argFile[2];
    }
    if (checkNotEmptyFile(defFile)!=0 || checkNotEmptyFile(inputFile)!=0){
        return 0;
    }

    compareArrays(defFile, inputFile, definitionArray, inputArray);

}

/**
 * Read the definition file, and scrape necessary data.
 */
int *processDefinitionFile(char *defFile, int *defArray) {
    FILE *fileOpen = fopen(defFile, "r");
    // Initialize necessary variables:
    int numbLines = numberOfLines(defFile);
    int count = 0;
    int firstElem = 0;
    char secondElem = '0';
    int thirdElem = 0;
    printf("processing FSM definition file %s\n",defFile);
    while (fscanf(fileOpen, "%d:%c>%d", &firstElem, &secondElem, &thirdElem) == 3) {
        defArray[count] = firstElem;
        if (secondElem>='a'&& secondElem>='z'||secondElem>='A'&& secondElem>='Z'){
            defArray[count + 1] = secondElem;
        }
        else{
            printf("ERROR: wrong character at line %d, expected an letter as input. Program crashed\n", count/3);
            return 0;
        }
        defArray[count + 2] = thirdElem;
        count += 3; // add 3 to counter since we are parsing 3 elems at a time.
    }
    if ((count/3)!=numbLines){
        printf("ERROR: wrong character inserted, expected an number as original state. Program crashed\n");
        return 0;
    }
    fclose(fileOpen);
    printf("FSM has %d transitions\n",numbLines );
    return defArray;
}

/**
 * Count number of lines in the arguement file.
 */
int numberOfLines(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char fileText[MAX_LINES];
    int counter = 0;
    while (fgets(fileText, MAX_LINES - 1, file) != NULL) {
        counter += 1;
    }
    fclose(file);
    return counter;
}


/**
 * Return an array with all the elements from the line in text file.
 */
int *processInputFile(char *inputFile, int *array2) {
    printf("processing FSM input file %s\n", inputFile);
    FILE *file2Open = fopen(inputFile, "r");
    char inputArray[MAX_LINES];
    int count = 0;
    while (fgets(inputArray, MAX_LINES, file2Open) != NULL) {
        array2[count] = inputArray[0];
        if (!(array2[count]>='a'&& array2[count]>='z'||array2[count]>='A'&& array2[count]>='Z')){
            printf("ERROR: wrong character at line %d, expected an letter as input. Program crashed\n", count);
            return 0;
        }
        count += 1;
    }
    return array2;
}

/**
 * Match the combination of the input file elements and the current state with the coressponding pattern in definition file and update the state.
 */
int compareArrays(char *defFile, char *inputFile, int *array1, int *array2) {
    int *definitionArray = processDefinitionFile(defFile, array1); //array containing the definition useful data
    int *currentChar = processInputFile(inputFile, array2);//array containing the input characters
    int linesInInput = numberOfLines(inputFile); //variables keeping track of the number of lines in each file
    int linesInDef = numberOfLines(defFile);
    int currentState = 0; //variables to indicate previous state and the state to update it to
    int updatedState = 0;
    int errorTracker=0; //variable used to keep track of the evolution of the state changes
    int hasBeenUpdated=0; // works with error tracker to keep track of whether or not the state has been changed
    int i=0; //initialize i here for the print statement for the last printf after the loop to work
    for (i = 0; i < linesInInput; i++) {
        for (int j = 0; j < linesInDef*3; j=j+3) {
            if (currentChar[i] == definitionArray[j + 1] && currentState == definitionArray[j]) {
                updatedState = definitionArray[j + 2];
                errorTracker+=1;
            }
        }
        if (errorTracker==hasBeenUpdated){
            printf("ERROR: input %c and state %d combination not defined in the definition of the FSM, fsm stops after %d steps \n",currentChar[i],currentState, errorTracker);
            return 0;
        }
        printf("at step %d, input %c transitions FSM from state %d to state %d\n", i, currentChar[i],
               currentState, updatedState);
        hasBeenUpdated+=1;
        currentState = updatedState;
    }
    printf("after %d steps, state machine finished successfully at state %d\n", i, currentState);
    return 0;
}
// TEST CODE ///////////////////////////////////////////////////////////////////////////////////////////////////////

int checkNotEmptyFile(char *fileName){
    FILE *file = fopen(fileName, "r");
    char inputArray[MAX_LINES];
    fseek(file, 0, SEEK_END); // goto end of file
    if (ftell(file) == 0){
        printf("ERROR: file %s is empty\n", fileName);
        fclose(file);
        return 1;
    }
    else {

        fclose(file);
        return 0;
    }
}

// DEBUGGER ////////////////////////////////////////////////////////////////////////////////////////////////////////

void debugger(char *file1, char *file2, int *array1, int *array2){
    int linesInput=numberOfLines(file2);
    int linesDef= numberOfLines(file1);
    int step=0;
    char input='0';
    printf("FSM debugger> ");
    scanf("%c", &input);
    while (step<linesInput) {
        if (input == 'p') {
            step -= 1;
            int *array = processDefinitionFile1(file1, array1);
            printf("processing FSM definition file %s\n", file1);
            printf("FSM has %d transitions\n", numberOfLines(file1));
            programWorking1(file1, file2, array1, array2, step, 1);//function that returns the state at a moment t
            for (int i = 0; i < (linesDef * 3); i += 3) {
                printf("transition %d: state %d with input %c goes to state %d\n", (i / 3) + 1, array[i], array[i + 1],
                       array[i + 2]);
            }
            step += 1;
        }
        else if (input == 'n') {
            programWorking1(file1, file2, array1, array2, step, 0);//function that returns the state at a moment t
            step += 1;
        }
        printf("FSM debugger> ");
        scanf("%c", &input);
    }
}
int *processDefinitionFile1(char *defFile, int *array1) {
    FILE *file1Open = fopen(defFile, "r");
    fclose(file1Open);
    FILE *fileOpen = fopen(defFile, "r");
    int countt=0;
    int firstElem=0;
    char secondElem='0';
    int thirdElem=0;

    while (fscanf(fileOpen, "%d:%c>%d", &firstElem, &secondElem, &thirdElem) == 3) {
        array1[countt] = firstElem;
        if (secondElem>='a'&& secondElem>='z'||secondElem>='A'&& secondElem>='Z'){
            array1[countt + 1] = secondElem;
        }
        array1[countt + 2] = thirdElem;
        countt+=3;
    }
    fclose(file1Open);
    return array1;
}

int programWorking1(char *file1, char *file2, int *array1, int *array2, int step, int input ) {
    int *definitionArray = processDefinitionFile1(file1, array1); //array containing the definition useful data
    int *currentChar = processInputFile1(file2, array2);//array containing the input characters
    int count2 = numberOfLines(file2);
    int count1 = numberOfLines(file1);
    int currentState = 0;
    int errorTracker=0;
    int updatedState = 0;
    int count=0;
    int i=0;

    for (i = 0; i < count2; i++) {// using
        for (int j = 0; j < count1*3; j=j+3) {//using line count of definition file
            if (j == 0) {
                if (currentChar[0] == definitionArray[1] && currentState == definitionArray[0]) {
                    updatedState = definitionArray[2];
                    errorTracker+=1;
                }
            }
            else if (currentChar[i] == definitionArray[j + 1] && currentState == definitionArray[j]) {
                updatedState = definitionArray[j + 2];
                errorTracker+=1;
            }
        }
        if ( count==step && input==0){
            printf("at step %d, input %c transitions FSM from state %d to state %d\n",count, currentChar[count], currentState, updatedState);
        }
        if(step==-1 && count==step+1 && input==1 ){
            printf("The FSM is currently in state %d\n", currentState);
        }
        if( step!=-1 && count==step && input==1 ){
            printf("The FSM is currently in state %d\n", updatedState);
        }
        currentState = updatedState;
        count+=1;
    }
    if (step==count2-1 && input ==0){
        printf("after %d steps, state machine finished successfully at state %d\n",count, currentState);
    }
    return currentState;
}

int *processInputFile1(char *inputFile, int *array2){
    FILE *file = fopen(inputFile, "r");
    char inputArray[MAX_LINES];
    int count = 0;
    // Iterate through all lines in input file and put each input in an array: Have to redefine this comment
    while (fgets(inputArray, MAX_LINES, file)!= NULL) {
        if ((inputArray[0]>='A' && inputArray[0]<='Z') ||inputArray[0]>='a' && inputArray[0]<='z'){
            array2[count++] = inputArray[0];
        }
    }
    return array2;
}
