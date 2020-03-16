#define _GNU_SOURCE
#include <ctype.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Displays an error message if there is an invalid sequence of {} in the input
// file
void error_message_brackets() {
    // There is an invalid sequence of {}
    fprintf(stderr,
            ">>> The file comports an invalid sequence of curly brackets\n");
    exit(EXIT_FAILURE);
}

// Writes the header of the .h file
void write_header(FILE* outfile, char* filename, char* begin) {
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
void write_footer(FILE* outfile) {
    fprintf(outfile, "#endif");
}

// Reads a line and update the stack if it reads a { or a }
void read_line(char* line, int* par_count, int i) {
    char previous = '/';
    while (line[i] != '\0') {
        if (*par_count < 0) {
            error_message_brackets();
        }
        if (line[i] == '{')
            *par_count += 1;
        else if (line[i] == '}')
            *par_count -= 1;
        if (previous == '/' && (line[i] == '/' || line[i] == '*'))
            break;
        previous = line[i];
        i++;
    }
}

// Copies the structure to the h file
void copy_struct(FILE* infile, FILE* outfile) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    fprintf(outfile, "\n");
    while ((read = getline(&line, &len, infile)) != -1) {
        if (!strcmp(line, "}; */\n") || !strcmp(line, "};*/\n")) {
            // If we are at the end of the structure
            fprintf(outfile, "};\n\n");
            break;
        } else {
            fprintf(outfile, "%s", line);
        }
    }
    free(line);
}

// Reads all the lines of the .c file
void readlines(char* filename, int par_count, FILE* outfile) {
    FILE* fp;
    char* line = NULL;
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
        is_struct = is_struct || !strcmp(line, "/* [raw]\n");
        if (is_struct) {
            // If we have to copy the struct
            if (par_count != 0) {
                error_message_brackets();
            }
            copy_struct(fp, outfile);
            is_struct = false;
        } else {
            previous_length = par_count;
            i = 0;
            // Consumes spaces and tabs
            while (line[i] == ' ')
                i++;

            // If it's not a header
            if (line[i] == '#')
                fprintf(outfile, "%s", line);
            // If the line is not a comment
            if (line[i] != '/')
                read_line(line, &par_count, i);
            // If this is a function's name
            if (previous_length == 0 && par_count == 1) {
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
int main(int argc, char* argv[]) {
    char* cfile;
    char* filename;
    if (argc <= 1) {
        printf("Bad usage: %s [file.c] [file.h]\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        cfile = argv[i];
        filename = malloc((strlen(cfile) + 1) * sizeof(char));
        strcpy(filename, cfile);
        filename[strlen(cfile) - 1] = 'h';
        filename[strlen(cfile)] = '\0';

        // Getting the name of the .h file
        FILE* outfile = fopen(filename, "w");
        // Writing the full header of the .h file
        write_header(outfile, basename(filename), "#ifndef _");
        write_header(outfile, basename(filename), "#define _");

        // Defining the empty stack
        int par_count = 0;

        // Reading all the lines of the .c file
        readlines(cfile, par_count, outfile);

        // If the count of {} is invalid
        if (par_count != 0) {
            error_message_brackets();
        }

        write_footer(outfile);

        fclose(outfile);

        free(filename);
    }

    return 0;
}
