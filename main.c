#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "stack.h"

// Writes the header of the .h file
void write_header(FILE *outfile, char* filename, char* begin) {
    fprintf(outfile, "%s", begin);
    // Writes the name of the file in uppercases
    int i = 0;
    while (filename[i] != '.') {
        fprintf(outfile, "%c", toupper(filename[i]));
        i++;
    }
    fprintf(outfile, "_H_\n\n");
}

// Writes the footer of the .h file
void write_footer(FILE *outfile) {
    fprintf(outfile, "#endif");
}

// Reads a line and update the stack if it reads a { or a }
void read_line(char* line, struct stack **stack, int i) {
    char previous = '/';
    while (line[i] != '\0') {
        if (line[i] == '{')
            push_value(stack, true);
        else if (line[i] == '}')
            push_value(stack, false);
        if (previous == '/' && (line[i] == '/' || line[i] == '*'))
            break;
        previous = line[i];
        i++;
    }
}

// Reads all the lines of the .c file
void readlines(char* filename, struct stack *stack, FILE *outfile) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int i;
    int previous_length = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // For all the lines
    while ((read = getline(&line, &len, fp)) != -1) {
        previous_length = length(stack);
        i = 0;
        // Consumes spaces and tabs
        while (line[i] == ' ')
            i++;

        // If the line is not a comment
        if (line[i] == '#')
            fprintf(outfile, "%s", line);
        if (line[i] != '/')
            read_line(line, &stack, i);

        // If this is a function's name
        if (previous_length == 0 && length(stack) == 1) {
            int j = 0;
            // Copies all the line but the { and remplaces it with a ;
            while (line[j] != '{') {
                fprintf(outfile, "%c", line[j]);
                j++;
            }
            fprintf(outfile, ";\n\n");
        }
    }

    fclose(fp);
    if (line)
        free(line);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Bad usage: ./hgen file.c file.h\n");
        exit(1);
    }

    // Getting the name of the .h file
    char* cfile = argv[1];
    char* filename = argv[2];

    FILE *outfile = fopen(filename, "w");

    // Writing the full header of the .h file
    write_header(outfile, filename, "#ifndef _");
    write_header(outfile, filename, "#define _");

    // Defining the empty stack
    struct stack *stack = NULL;

    // Reading all the lines of the .c file
    readlines(cfile, stack, outfile);

    free_stack(stack);

    write_footer(outfile);

    fclose(outfile);

    return 0;
}
