#include <stdio.h> // for printf, scanf etc
#include <stdlib.h>// for exit function
#include <string.h>// for using string operations
#define SIZE 100//stack size

typedef struct stack
{
	int top;//describes the value index
	int st[SIZE];//stack array FILO is used
}stack;

int variables[26];//the variables which is given in the expressions
int isValid[26];//valid bit for every variable to understand which variable is used


void initialize(stack *stack)
{
	stack->top=0;//initializing the top value of stack
}

int isEmpty(stack *stack)
{
	if (stack->top==0)
		return 1;// is there any element in the stack?
	else
		return 0;//if it is not 0
}

int isFull(stack *stack)
{
	if (stack->top==SIZE)
		return 1;// is the stack carrying the max element?
	else
		return 0;// or not
}
int lookFirst(stack *stack)
{
	int item;//keep the pop value
	if (isEmpty(stack))
	{
		return INT_MAX;//if it is full return macro INT_MAX which is 2,147,483,647
	}
	else
	{
		stack->top--;// receiving a value from top of stack
		item = stack->st[stack->top];
		stack->top++;// leave it just like you take!!
		return item;
	}
}
void push(int value, stack *stack)
{
	if (isFull(stack))
	{
		printf("Stack is Full");//if stack is full we can't put any value to stack
	}
	else
	{
		stack->st[stack->top]=value;
		stack->top++;// putting the value to stack
	}
}

int pop(stack *stack)
{
	if (isEmpty(stack))
	{
		return -1;//if it is empty it returns -1 to implement other functions
	}
	else
	{
		stack->top--;// receiving a value from top of stack
		return stack->st[stack->top];
	}
}

int getPrecedence(int operation)
{
	switch(operation)
	{
		case 0: return 0; break;// for + process
		case 1: return 0; break;// for - process
		case 2: return 1; break;// for * process
		case 3: return 1; break;// for / process
		default: return -1;break;//for turning the other operations
	}
}


int operationToInt(char operation)
{
	switch(operation)
	{
		case '+': return 0; break;// our stack is implemented with integer
		case '-': return 1; break;//so we need use the symbols to
		case '*': return 2; break;//some integer values
		case '/': return 3; break;
		case '(': return 4; break;
		case ')': return 5; break;
	}
}

char* intToOperation(int operation)
{
	switch(operation)
	{	
		case 0: return "+"; break;//in writing postfix notation or stack operations
		case 1: return "-"; break;//we should convert the integer values to operation
		case 2: return "*"; break;//equivalent
		case 3: return "/"; break;
		case 4: return "("; break;
		case 5: return ")"; break;
		default: return ""; break;//of course if it's empty we write nothing
		
	}
}

char *allocateMemory()
{
	char *place=(char *) malloc (255*sizeof(char));//we want a place for writing string
	if (!place)
	{
		printf("Not Enough Space");//if the place could not given to us we write a message to user
		exit(0);//and exit the program.
	}
	return place;//else we return the pointer
}

void printExpression(char *postfix, stack *op)
{
	stack temp1;//temporary stack for writing the correct order
	int item;// keep pop value
	
	
	initialize(&temp1); // initialize the temporary stack for use
	
	printf("%-20s|",postfix); // first write the postfix notation after write the stack values
	if (isEmpty(op) == 1) // if the stack is empty we don't need to do the other parts
	{
		printf("Stack Is Empty");
	}
	else
	{
		while ( !isEmpty(op))//pop the value from stack till its empty
		{
			item= pop(op);
			push(item, &temp1);//and push it to another stack to get the reverse order
		}
		
		while((item=pop(&temp1)) != -1)
		{
			printf("%s",intToOperation(item));//in here we write the value with parsin to operation
			push(item, op);// after that to not distort the sequence we push the item to original stack again
		}
	}
	printf("\n");
}

void printValues (stack *op)
{
	stack temp1;//to configure the order
	int item;// keep for pop value
	
	initialize(&temp1);// initializing the stack
	
	
	while ( !isEmpty(op))// till it is empty
	{					 // we pop item in the original stack
		item= pop(op);	 // and we push it to the stack
		push(item, &temp1);// that we use for temporary
	}
	
	while((item=pop(&temp1)) != -1)
	{
		push(item, op);//in here we pop the value from temporary stack
		printf("%d ",item); //and we write it console also we push it to original stack, too
	}
	printf("\n");
}

char * infixToPostfix(char *infix)
{
	
	stack operations;// create the signs with the correct priorty
	int postOperation; // temporary variable to move from stack to postfix notation
	
	initialize(&operations);//first initialize the stack
	infix[strlen(infix)-2]=0;//get rid of from ; and \n
	char *postfix=allocateMemory();//request permission from system
	strcpy(postfix,"");//reset the space with 0
	char *part = strtok(infix, " ");//split the infix to their parts
	
	printf("\n  POSTFIX FORM         STACK VALUES\n");
	while( part)
	{
		if ((part[0] >= 'a' && part[0] <= 'z') || (part[0] >= '0' && part[0] <= '9')) 
		{//if its a variable or a constantwe add this one to postfix immediately
			strcat(postfix, part); //adding functions
			strcat(postfix, " ");
			printExpression(postfix, &operations); //print the expression which is made
		}
		else if (part[0] == '(')
		{
			push(operationToInt(part[0]), &operations);//if its an open parentheses we push to the stack
			printExpression(postfix, &operations); //print the expression which is made
		}
		else if (part[0] == ')')//if its a close parentheses we pop from the stack 'till we'll get the open parentheses
		{
			while (lookFirst(&operations) != operationToInt('('))//look the first value from stack to find the open paranthesess
			{
				postOperation = pop(&operations); //pop value from stack and move it to a variable for simple understanding
				strcat(postfix, intToOperation(postOperation)); // add the operation symbol to postfix notation
				strcat(postfix, " ");// adding space for postfix
				printExpression(postfix, &operations); //print the expression which is made
			}
			pop(&operations);// be freed with (
			printExpression(postfix, &operations); //print the expression which is made
		}
		else
		{//means that we got a priority issue and we need to fix it
			while (getPrecedence(lookFirst(&operations)) >= getPrecedence(operationToInt(part[0])))
			{//while the priority is bigger we add the postfix notation
				postOperation = pop(&operations);
				strcat(postfix, intToOperation(postOperation));
				strcat(postfix, " ");// adding space for postfix
				printExpression(postfix, &operations); //print the expression which is made
			}
			push(operationToInt(part[0]), &operations);
			printExpression(postfix, &operations); //print the expression which is made
		}
		part = strtok(NULL, " "); // parse the string part by part
	}
	
	while (!isEmpty(&operations))// do this till the stack is going to be empty
	{
		postOperation = pop(&operations); //pop value from stack and move it to a variable for simple understanding
		strcat(postfix, intToOperation(postOperation)); // add the operation symbol to postfix notation
		strcat(postfix, " ");// adding space for postfix
		printExpression(postfix, &operations);//for print the expression which is building
	}

	return postfix;//finally we return the postfix notation	
}

int compute(int operand1, int operand2, char process)
{
	switch(process)
	{
		case '+': return operand1+operand2; break;// this switch case structure helps me
		case '-': return operand1-operand2; break;// to compute the op1 and op2 with
		case '*': return operand1*operand2; break;// the process sign
		case '/': return operand1/operand2; break;
	}
}

int Implement(char *postfix)
{
	int i;//loop variable
	stack operands;//used for writing the postfix notation to processed value
	initialize(&operands);
	printf("\nPostfix Form: %s\n",postfix);
	
	for (i=0;i<26;i++)
	{
		if (isValid[i])//for final step we write the variables which are used cause we define 26 variables
			printf("%c <- %d ",i+'a', variables[i]);
	}
	
	printf("\n*****Process Steps In Stack****\n");
	char *part = strtok(postfix, " "); // shredding the postfix string
	
	while (part)//do this while get the null from parsing the postfix expression
	{
		if ((part[0] >= 'a' && part[0] <= 'z') || (part[0] >= 'A' && part[0] <= 'Z')) // it can be variable
		{
			push(variables[part[0]-'a'], &operands);//push the variable to stack
			printValues(&operands);//printing the steps of process
		}
		else if (part[0] >= '0' && part[0] <= '9') // or a constant
		{
			push(atoi(part), &operands);//atoi is used for get the number value of char*
			printValues( &operands);//printing the steps of process
		}
		else // or an operation
		{
			push(compute(pop(&operands), pop(&operands), part[0]), &operands); // pushing the trancsaction between op1 and op2
			printValues(&operands);//printing the steps of process
		}
		part = strtok(NULL, " ");// shredding the postfix with null
	}
	return pop(&operands);//last is the result of the computation
	
}

void readFromFileAndCalculate(char *fileName)
{
	FILE *fp= fopen(fileName, "r"); // for get the operations
	char process[100]; // recieving transaction from file to Ram
	char *postfix;//hold a pointer to configure the lines
	if(!fp)
	{
		printf("File with Name: %s Could not Open",fileName);//if it isnt opens we write a message to user
		exit(0);//if the file can not open we exit the program.
	}
	
	while (!feof(fp))
	{
		strcpy(process, "");// resetting the process string
		fgets(process, 100, fp);
		printf("Operation %s", process);//first we write the infix notation of the line
		printf("\n");
		postfix = infixToPostfix(&process[4]);//infix to postfix processing is happens
		variables[process[0]-'a'] = Implement(postfix);//postfix notation to processed value
		isValid[process[0]-'a'] = 1;// we are validing the value array also to use the variable a,b,c... again
		printf("\nResult\n");
		printf("%c <- %d\n\n", process[0], variables[process[0]-'a']);//for result the line we put the variable value to command line
	}
	fclose(fp);
}

int main(int argc, char *argv[]) {
	char fileName[26]; // gets the relative path from user
	int i;// loop variable
	
	printf("Enter The FULL File Name (ENTER RELATIVE PATH)=");
	scanf("%s",fileName); // taking the input file name from user
	readFromFileAndCalculate(fileName);// this one reads and calculates the lines of expressions
	printf("\n*****Final Values*****\n");
	
	for (i=0;i<26;i++)
	{
		if (isValid[i])//for final step we write the variables which are used cause we define 26 variables
			printf("%c <- %d\n",i+'a', variables[i]);
	}
	return 0;
}
