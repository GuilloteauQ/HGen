#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <libgen.h>

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

// Copies the structure to the h file
void copy_struct(FILE* infile, FILE *outfile) {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, infile)) != -1) {
        if (!strcmp(line, "}; */\n") || !strcmp(line, "};*/\n")) {
            // If we are at the end of the structure
            fprintf(outfile, "}; \n\n");
            break;
        } else {
            fprintf(outfile, "%s", line);
        }
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
    bool is_struct = false;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // For all the lines
    while ((read = getline(&line, &len, fp)) != -1) {
        is_struct = is_struct || !strcmp(line, "/* [struct]\n");
        if (is_struct) {
            // If we have to copy the struct
            assert (length(stack) == 0);
            copy_struct(fp, outfile);
            is_struct = false;
        } else {
            previous_length = length(stack);
            i = 0;
            // Consumes spaces and tabs
            while (line[i] == ' ')
                i++;

            // If it's not a header
            if (line[i] == '#')
                fprintf(outfile, "%s", line);
            // If the line is not a comment
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
    }

    fclose(fp);
    if (line)
        free(line);
}

// Main function
int main(int argc, char *argv[]) {

    char* cfile;
    char* filename;
    switch (argc) {
        case 3:
            cfile = argv[1];
            filename = argv[2];
            break;
        case 2:
            cfile = argv[1];
            filename = malloc((strlen(cfile) + 1) * sizeof(char));
            strcpy(filename, cfile);
            filename[strlen(cfile) - 1] = 'h';
            filename[strlen(cfile)] = '\0';
            break;
        default:
            printf("Bad usage: ./hgen file.c file.h\n");
            exit(1);
            break;
    }


    // Getting the name of the .h file
    FILE *outfile = fopen(filename, "w");
    // Writing the full header of the .h file
    write_header(outfile, basename(filename), "#ifndef _");
    write_header(outfile, basename(filename), "#define _");

    // Defining the empty stack
    struct stack *stack = NULL;

    // Reading all the lines of the .c file
    readlines(cfile, stack, outfile);

    free_stack(stack);

    write_footer(outfile);

    fclose(outfile);

    if (argc == 2) {
        free(filename);
    }

    return 0;
}
