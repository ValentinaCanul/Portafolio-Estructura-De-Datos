#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Stack for double values (Used for numbers in postfix evaluation)
struct stack {
    double* array;
    int top;
    int maxSize;
};
typedef struct stack Stack;

// Stack for char values (used for operators in infix-to-postfix conversion)
typedef struct {
    char* array;
    int top;
    int maxSize;
} CharStack;

// Create a new CharStack for operators
CharStack* createCharStack(int size) {
    CharStack* s = (CharStack*)malloc(sizeof(CharStack));
    s->array = (char*)malloc(size * sizeof(char));
    s->top = -1;
    s->maxSize = size;
    return s;
}

// Check if CharStack is empty
int isCharStackEmpty(CharStack* s) { return s->top == -1; }

// Push a character (operator or parenthesis) onto CharStack
void pushChar(CharStack* s, char value) { s->array[++s->top] = value; }

// Pop a character from CharStack
char popChar(CharStack* s) { return isCharStackEmpty(s) ? '\0' : s->array[s->top--]; }

// Peek at the top character of CharStack
char peekChar(CharStack* s) { return isCharStackEmpty(s) ? '\0' : s->array[s->top]; }

// Free memory used by CharStack
void deleteCharStack(CharStack* s) { free(s->array); free(s); }

// Get what operator we have for infix-to-postfix conversion
int Operator(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Convert infix equation to postfix
void infixToPostfix(const char* infix, char* postfix) {
    CharStack* ops = createCharStack(strlen(infix)); // Stack for operators
    int j = 0; // Index for postfix output
    for (int i = 0; infix[i]; i++) {
        if (isspace(infix[i])) continue; // Skip spaces
        if (isdigit(infix[i])) {
            // Copy multi-digit numbers to output
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' '; // Add space after each number
            i--; // Adjust index after inner loop
        } else if (infix[i] == '(') {
            pushChar(ops, infix[i]); // Push '(' to stack
        } else if (infix[i] == ')') {
            // Pop operators until '(' is found
            while (!isCharStackEmpty(ops) && peekChar(ops) != '(') {
                postfix[j++] = popChar(ops);
                postfix[j++] = ' '; // Add space after each operator
            }
            if (!isCharStackEmpty(ops)) popChar(ops); // Remove '('
        } else if (strchr("+-*/", infix[i])) {
            // Pop operators with higher or equal precedence
            while (!isCharStackEmpty(ops) && Operator(peekChar(ops)) >= Operator(infix[i])) {
                postfix[j++] = popChar(ops);
                postfix[j++] = ' ';
            }
            pushChar(ops, infix[i]); // Push current operator
        }
    }
    // Pop any remaining operators
    while (!isCharStackEmpty(ops)) {
        postfix[j++] = popChar(ops);
        postfix[j++] = ' ';
    }
    postfix[j] = '\0'; // Null-terminate output
    deleteCharStack(ops); // Free operator stack
}

// Evalute Postfix
double evaluatePostfix(const char* expr){
    Stack s;
    s.array = (double*)malloc(100* sizeof(double)); 
    s.top = -1; 
    s.maxSize=100;

    for (int i=0; expr[i]; i++){
        if (isspace (expr[i])) continue;
        if (isdigit(expr[i])){
            double num=0;
            while(isdigit(expr[i])){
                num= num * 10 + (expr[i] - '0');
                i++;
            }
            i--;
            s.array[++s.top]=num;
        }
        else if (strchr("+-*/", expr[i])){
            double b = s.array[s.top--];
            double a= s.array[s.top--];
            switch (expr[i]){
                case '+': s.array[++s.top] = a + b; break;
                case '-': s.array[++s.top] = a - b; break;
                case '*': s.array[++s.top] = a * b; break;
                case '/': s.array[++s.top] = a / b; break;
            }
        }
    }

    double result = s.array[s.top];
    free(s.array);
    return result;
}

// Read equations from file, convert to postfix, and store in array
int readEquations(const char* path, char*** outArray) {
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Could not open file: %s\n", path);
        return 0;
    }
    // Read entire file into buffer
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(len + 1);
    fread(buffer, 1, len, file);
    buffer[len] = '\0';
    fclose(file);

    // Count number of equations (by ';')
    int count = 0;
    for (int i = 0; buffer[i]; i++) if (buffer[i] == ';') count++;
    char** equations = (char**)malloc(count * sizeof(char*));

    // Split buffer into equations and process each
    int eqIdx = 0;
    char* start = buffer;
    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == ';') {
            int eqLen = &buffer[i] - start;
            char* eq = (char*)malloc(eqLen + 1);
            strncpy(eq, start, eqLen);
            eq[eqLen] = '\0';

            // Convert infix to postfix
            char* postfix = (char*)malloc(eqLen * 3); // Allocate enough space
            infixToPostfix(eq, postfix);

            // Format output string
            char* formatted = (char*)malloc(strlen(postfix) + 12);
            sprintf(formatted, "Ecuation: %s;", postfix);

            equations[eqIdx++] = formatted; // Store result
            free(eq);
            start = &buffer[i + 1]; // Move to next equation
        }
    }
    free(buffer);
    *outArray = equations; // Output array of strings
    return count; // Return number of equations
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <path_to_equations_file>\n", argv[0]);
        return 1; // Exit 1, which is an error
    }
    char** results;
    int n = readEquations(argv[1], &results); 

    FILE* out=fopen("exp_postfijas.txt", "w"); //archivo de salida

    for (int i = 0; i < n; i++) {
        char* expr= strchr(results[i], ':');
        if (expr) expr += 2; //apunta a parte post fija

        double res = evaluatePostfix(expr);

        fprintf(out, "%s Result: %.2f\n", results[i], res);//imprime en un archivo
        printf("%s Result: %.2f\n", results[i], res);

        free(results[i]);
    }
    fclose(out);
    free(results); // Free array of pointers
    return 0;
}
