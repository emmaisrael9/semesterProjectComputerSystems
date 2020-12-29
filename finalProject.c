#include <stdio.h>
/**
 * Author Emma Israel 
 */
#define MAX_LINES 2600 //Maximum space we need in the array ( in the case input has 52 letters and fsm has 50 states) 

int numberOfLines (char *file );
char *processDefinitionFile (char *file1, char *array1);
char *processInputFile (char *file,char *array2 );
int programWorking (char *file1, char *file2,char *array1, char* array2);

/**
 * Test functions 
 */
int userErrorCheck(char *file1,char *file2);
int checkNotEmptyFile(char *file);

/**
 * Debugger Functions
 */
void debugger(char *file1, char *file2, char *array1, char *array2);
char *processDefinitionFile1(char *defFile, char *array1);
int programWorking1(char *defFile, char *inputFile,char *array1, char *array2, int step);
char *processInputFile1(char *inputFile,char *array2);

int main( int numArgs, char *argFile[]){
  //initialize my variables
  char *file1; 
  char *file2;
  char array1[MAX_LINES * 3]; //array used to store the definition file 
  char array2[MAX_LINES]; //array used to store the inputs
  if (argFile[3]){
    file1 = argFile[2];
    file2 = argFile[3];
    debugger(file1, file2, array1, array2);
  }
  else {
    file1 = argFile[1];
    file2 = argFile[2];
  }
  int userErrors=userErrorCheck(file1,file2);
  if (userErrors!=0){ // if the number returned by global check is other than 0, there is an error and the program stops
    return 0;
  }
  programWorking(file1,file2,array1, array2);
}


/**
 * Read the definition file, and scrape necessary data.
 */
char *processDefinitionFile(char *defFile, char *array1){
  FILE *file = fopen(defFile, "r");
  char textPerLine[MAX_LINES];
  int count = 0;
  printf("processing FSM definition file %s\n", defFile);
  // Iterate through each line to find the three necesary chars per line:
  while (fgets(textPerLine, MAX_LINES, file) != NULL){
    if (textPerLine[0]>=48 && textPerLine[0]<=57){
      array1[count] = textPerLine[0];}
    else {printf("ERROR: wrong character at line %d, expected an digit as origin state . Program crashed\n", (count/3)+1);
      return 0;
    }
    if ((textPerLine[2]>='A' && textPerLine[2]<='Z') ||(textPerLine[2]>='a' && textPerLine[2]<='z')){
      array1[count + 1] = textPerLine[2];
    }
    else { printf("ERROR: wrong character at line %d, expected an letter as input. Program crashed\n", (count/3)+1);
      return 0;
    }
    if (textPerLine[0]>=48 && textPerLine[0]<=57){
    array1[count + 2] = textPerLine[4];
    }
    else {printf("ERROR: wrong character at line %d, expected an digit as updated state. Program crashed\n", (count/3)+1);
      return 0;
    }
    count += 3;
  }
  fclose(file);
  return array1;
}
  
/**
 * Return an array with all the elements from the line in text file.
 */ 
char *processInputFile(char *inputFile,char *array2){
  printf("processing FSM input file %s\n", inputFile);
  FILE *file = fopen(inputFile, "r");
  char inputArray[MAX_LINES];
  int count = 0;
  // Iterate through all lines in input file and put each input in an array: Have to redefine this comment
  while (fgets(inputArray, MAX_LINES, file)!= NULL) {
    if (inputArray[1]){
      printf("ERROR: wrong input format at line %d, expected only one character. Program crashed\n", count);
      return 0;
    }
    if ((inputArray[0]>='A' && inputArray[0]<='Z') ||inputArray[0]>='a' && inputArray[0]<='z'){
    array2[count++] = inputArray[0];
    }
    else {
    printf("ERROR: wrong character in inputs at line %d, expected an letter as input. Program crashed\n", count);
    return 0;
  }
}
  fclose(file);
  return array2;
}

/**
 * Count number of lines in the arguement file.
 */
int numberOfLines(char *definitionFile) {
  FILE *file= fopen(definitionFile, "r");
  char textPerLine[MAX_LINES];
  int linesInFile = 0;
  // Iterate through lines in file to find total count:
  while(fgets(textPerLine, MAX_LINES-1, file) != NULL) { 
    linesInFile += 1;
  }
  fclose(file);
  return linesInFile;
}


/**
 * Match the input file with the coressponding definition file and update the state. 
 */
int programWorking(char *defFile, char *inputFile,char *array1, char *array2){
  if ((processDefinitionFile(defFile, array1)==0) || (processInputFile(inputFile, array2)==0) ){
    return 0;
  }
  char *definitionArray = processDefinitionFile(defFile, array1); //array containing the definition file useful data
  char *currentChar = processInputFile(inputFile, array2);//array containing the input file characters
  int linesInDefFile = numberOfLines(defFile);
  int linesInInputFile = numberOfLines(inputFile);
  printf("FSM has %d transitions\n", linesInDefFile);
  // Initialize necessary local variables:
  char currentState = '0';
  int errorTracker=0; 
  char updatedState = '0';
  int dimension = linesInDefFile*3;
  int count = 0; // each position in a 1d array.
  char twodArray[linesInDefFile][3];  
  // Convert the one dimensional array to a 2d array to easily compare input and definition arrays:
  for (int i = 0; i < linesInDefFile; i++){
    for (int j = 0; j < 3; j++){
      twodArray[i][j] = definitionArray[count+j];
    }
    count+=3; //loop through b 3 times each time. add 3 each time to make up for the 3 loops.
  }
  // Iterate through 2d array to compare 3 significant characters per line in the input and definition arrays:
  count = 0; // reset count to now represent each step.
  for (int i = 0; i < linesInInputFile;i++) {
    for (int j = 0; j < linesInDefFile;j++) {
      if (currentChar[count] == twodArray[j][1] && currentState == twodArray[j][0]){
        updatedState = twodArray[j][2];
        errorTracker +=1;
      }
      }
    if (errorTracker==count){
      printf("ERROR: input and state combination not defined in the definition of the FSM, fsm stops after %d steps \n", errorTracker);
      return 0;
    }
    printf("at step %d, input %c transitions FSM from state %c to state %c\n",count, currentChar[count], currentState, updatedState);
    count += 1;
    currentState = updatedState;
  }
  printf("after %d steps, state machine finished successfully at state %c\n",count, currentState);
}

//////////////////////////////////////////////// begining of the test codes ////////////////////////////////////////////////////////////


int userErrorCheck(char *file1, char *file2){
  int errorCounter;
  errorCounter=(checkNotEmptyFile(file1))+(checkNotEmptyFile(file2));
  return errorCounter;
}

int checkNotEmptyFile(char *fileName){
  FILE *file = fopen(fileName, "r");
  fseek(file, 0, SEEK_END); // go to end of file
  if (ftell(file) == 0){
    printf("ERROR: file %s is empty\n", fileName);
     return 1;
  }
    else {
      return 0;
    }
}

int testProcessdefinitionFile(char *file,char *array){
  char* outputArray=processDefinitionFile(file,array);//case 1 : see if processDefinitionFile works with a one line file
  int size=sizeof outputArray;
  if (size == 3){printf("processDefinition passed the dimension test for one line long file!\n");}
  else {printf("processDefinition failed the dimension test for one line long file Array size is: %d instead of 3\n", size);}
  for (int i=0;i<size;i++){
    printf("element: %c\n",outputArray[i]);
  }
  return 0;
}

//////////////////////////////////////////////////////////////////begining of the debugger code ///////////////////////////////////////////////////////////////////

void debugger(char *file1, char *file2, char *array1, char *array2){
  int linesInput=numberOfLines(file2);
  int linesDef= numberOfLines(file1);
  int step=0;
  char input;
  int cur;
  printf("FSM debugger> ");
  char *array=processDefinitionFile1(file1, array1);
  while (step<linesInput){
    int state=0;
    scanf("%c", &input);
    

    if (input=='p'){
        printf("processing FSM definition file %s\n",file1);
	printf("FSM has %d transitions\n", numberOfLines(file1));
	printf("The FSM is currently in state %d\n", state);
        for (int i=0;i<(linesDef*3);i+=3){
	  printf("transition %d: state %c with input %c goes to state %c\n",(i/3)+1,array1[i],array1[i+1],array1[i+2]);
	    printf("FSM debugger> ");
	}
    }

    if (input=='n'){
      state =programWorking1(file1,file2,array1,array2,step);//function that returns the state at a moment t 
      step+=1;
        printf("FSM debugger> ");
    }
  }
}

char *processDefinitionFile1(char *defFile, char *array1){
  FILE *file = fopen(defFile, "r");
  char textPerLine[MAX_LINES];
  int count = 0;
  // Iterate through each line to find the three necesary chars per line:
  while (fgets(textPerLine, MAX_LINES, file) != NULL){
      array1[count] = textPerLine[0];
      array1[count + 1] = textPerLine[2];
    array1[count + 2] = textPerLine[4];
    count += 3;
  }
  fclose(file);
  int maxLines=numberOfLines(defFile);
  return array1;
}

int programWorking1(char *defFile, char *inputFile,char *array1, char *array2, int step){
  char *definitionArray = processDefinitionFile1(defFile, array1); //array containing the definition file useful data
  char *currentChar = processInputFile1(inputFile, array2);//array containing the input file characters
  int linesInDefFile = numberOfLines(defFile);
  int linesInInputFile = numberOfLines(inputFile);
  // Initialize necessary local variables:
  char currentState = '0';
  int errorTracker=0; 
  char updatedState = '0';
  int dimension = linesInDefFile*3;
  int count = 0; // each position in a 1d array.
  char twodArray[linesInDefFile][3];  
  // Convert the one dimensional array to a 2d array to easily compare input and definition arrays:
  for (int i = 0; i < linesInDefFile; i++){
    for (int j = 0; j < 3; j++){
      twodArray[i][j] = definitionArray[count+j];
    }
    count+=3; //loop through b 3 times each time. add 3 each time to make up for the 3 loops.
  }
  // Iterate through 2d array to compare 3 significant characters per line in the input and definition arrays:
  count = 0; // reset count to now represent each step.
  for (int i = 0; i < linesInInputFile;i++) {
    for (int j = 0; j < linesInDefFile;j++) {
      if (currentChar[count] == twodArray[j][1] && currentState == twodArray[j][0]){
        updatedState = twodArray[j][2];
      }
    }
    if (count==step){
       printf("at step %d, input %c transitions FSM from state %c to state %c\n",count, currentChar[count], currentState, updatedState);
    }
      
    count += 1;
    currentState = updatedState;
    
  }
  if (step==linesInInputFile){
    printf("after %d steps, state machine finished successfully at state %c\n",count, currentState);
  }
  return updatedState;
}

char *processInputFile1(char *inputFile,char *array2){
  FILE *file = fopen(inputFile, "r");
  char inputArray[MAX_LINES];
  int count = 0;
  // Iterate through all lines in input file and put each input in an array: Have to redefine this comment
  while (fgets(inputArray, MAX_LINES, file)!= NULL) {
    if ((inputArray[0]>='A' && inputArray[0]<='Z') ||inputArray[0]>='a' && inputArray[0]<='z'){
    array2[count++] = inputArray[0];
    }
  fclose(file);
  return array2;
  }
}
