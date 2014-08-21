#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* in2post(char* equation);					//Converts an infix expression to postfix
int postEval(char* postfix);						//Evaluates a postfix expression and return the result
void push(char* value, char** stack);			//Push string to a stack
char* pop(char** stack);							//Pop and return the top element of a stack
char* top(char** stack);							//Returns the top element of a stack
int topIndex = -1;									//Global variable used to index a stack


int main( int argc, char* argv) {
	char* infix = "5 *  ( 2 + 10 )  /  ( 8 + 2 ) ";
	printf("This is the infix %s\n", infix);
	char* postfix = in2post(infix);
	printf("This is the post fix %s\n", postfix);
	int result = postEval(postfix);
	printf("This is the answer to the equation %d\n", result);	
	return 0;
}

char* in2post(char* equation){
	char* copy = strdup(equation);										//Initialize a copy of the original equation
	char** equationArray = (char**)malloc(256*sizeof(char*));	//Initialize an array of strings to hold 256 strings
	char* p = strtok(copy," ");											//Tokenizer to delimit a space
	int i = 0;																	//Iterator
	int arraySize = 0;														//Keep track of the size of the array of strings
	char* postfix = (char*)malloc(128*sizeof(char));				//Initialize a variable to hold the postfix expression with suitable size

	//Creates an array containing the delimited
	while(p != NULL) {
		equationArray[i++] = p;
		p = strtok(NULL, " ");
		arraySize++;
	}

	char* stack[arraySize];				//Creates a stack with enough size to hold all the postfix operator/operands

	//Iterate through each operand and operator for infix to postfix conversion
	for( i = 0; i < arraySize; i++){
		//Determine if the element is an operator
		if(strcmp(equationArray[i], "+") == 0 || strcmp(equationArray[i],"-") == 0 || strcmp(equationArray[i], "*") == 0 || strcmp(equationArray[i], "/") == 0
			  	|| strcmp(equationArray[i], "(") == 0 || strcmp(equationArray[i], ")") == 0) {
			//Push to stack if there are no elements in the stack
			if(topIndex == -1) {
				push(equationArray[i], stack);
			}
			//If it's plus or minus, it will concatenate the last element of
			//the stack as long as the priority is the same of higher.
			else if(strcmp(equationArray[i], "(") == 0) {
				push(equationArray[i], stack);
			}	
			else if(strcmp(equationArray[i], ")") == 0) {
				while(strcmp(top(stack), "(") != 0) {
					strcat(postfix, pop(stack));
					strcat(postfix, " ");
				}
				pop(stack);
			}
			else if(strcmp(equationArray[i], "+") == 0 || strcmp(equationArray[i], "-") == 0){
				while(strcmp(top(stack), "+") == 0 || strcmp(top(stack), "-") ==0 || strcmp(top(stack), "*") == 0 || strcmp(top(stack),"/") == 0 ) {
					strcat(postfix, pop(stack));
					strcat(postfix, " ");
				}
				//Pushes most recent operator to stack
				push(equationArray[i], stack);
			}
			//If it's a multiplication or division symbol, it keeps
			//concatenating the top stack elements as longa s it has the same
			//precendence.
			else if(strcmp(equationArray[i], "*") == 0 || strcmp(equationArray[i], "/") == 0) {
				while(strcmp(top(stack), "*") == 0 || strcmp(top(stack), "/") == 0 ) { 
					strcat(postfix, pop(stack));
					strcat(postfix, " ");
				}
				//Pushes most recent operator to stack
				push(equationArray[i], stack);
			}
		}
		//Concatenate to the postfix expression if element is an operand
		else {
			strcat(postfix, equationArray[i]);
			strcat(postfix, " ");
		}
	}
	//Concatenates all the remaining elements in the stack to the final
	//postfix expression
	while(topIndex != -1) {
		strcat(postfix, pop(stack));
		strcat(postfix, " ");
	}

	//Free memory used to hold copy
	free(copy);
	
	//Return the final postfix expression
	return postfix;
}

int postEval(char* postfix) {
	char* copy = strdup(postfix);											//Initialize a string to hold a copy of the postfix expression
	char** equationArray = (char**)malloc(257*sizeof(char*));	//Initialize an array of strings to hold the delimited postfix expressoin
	char* p = strtok(copy, " ");											//Tokenizes the first space
	int i = 0;																	//Iterator
	int arraySize = 0;														//Variable to keep track of the size of the array of strings


	//Delimits the string by the spaces and set the array equal to each
	//element
	while(p != NULL) {
		equationArray[i++] = p;
		p = strtok(NULL, " ");
		arraySize++;
	}

	char* stack[arraySize*3];		//Initialize a stack with enough size to hold the delimited array
	char str[15];					//Initialize a string to for conversion from integer to string

	//Iterate through each operator and operand in the postfix array
	for(i = 0; i < arraySize; i++) {
		//Determine if the element is an operator
		if(strcmp(equationArray[i], "+") == 0 || strcmp(equationArray[i], "-") == 0 || strcmp(equationArray[i], "*") == 0 || strcmp(equationArray[i], "/") == 0) {
			int first = atoi(pop(stack));		//Convert the top element of the stack to an integer
			int second = atoi(pop(stack));	//Convert the secodn top element of the stack to an integer
			int result;								//Initialize an int to hold the result of the two top elements
			//Add the two numbers
			if(strcmp(equationArray[i], "+") == 0) {
				result = second + first;
			}
			//Subtract the two numbers
			else if(strcmp(equationArray[i], "-") == 0) {
				result = second - first;
			}
			//Multiply the two numbers
			else if(strcmp(equationArray[i], "*") == 0) {
				result = second*first;
			}
			//divide the two numbers
			else if(strcmp(equationArray[i], "/") == 0) { 
				result = second/first;
			}
			//Convert the result back into string 
			sprintf(str, "%d", result);

			//Initialize a new string to hold the value of the result
			char* newStr = (char*) malloc((strlen(str) + 1)*sizeof(char));	
			strcpy(newStr, str);

			//Push the result back onto the stack
			push(newStr, stack);
		}
		//It not an operator, push the element to the stack
		else {
			push(equationArray[i], stack);
		}
	}	
	//Free the memory of copy
	free(copy);

	//Return the last element of the stack, which contains the answer, as an
	//intger
	return atoi(pop(stack));
}

void push(char* value, char** stack) {
	//Increments indexing to keep track of the top element
	topIndex++;
	//Set the top element in the stack to the value passed in
	stack[topIndex] = value;

}

char* pop(char** stack) {
	//Check if the stack does not containg any element
	if( topIndex == -1) {
		return "-1";
	}

	//Initialize a variable to hold the top element of the stack
	char* element = stack[topIndex];

	//Decrement the top index
	topIndex--;

	//Return the top element
	return element;

}
char* top(char** stack) {
	//Returns the top element in the stack
	return stack[topIndex];
}
