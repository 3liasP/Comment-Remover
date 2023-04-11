#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Error: could not open input file '%s'\n", filename);
        return 1;
    }

    char output_filename[MAX_LINE_LENGTH];
    snprintf(output_filename, MAX_LINE_LENGTH, "%s_no_comments.c", filename);
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        printf("Error: could not open output file '%s'\n", output_filename);
        fclose(input_file);
        return 1;
    }

    int in_block_comment = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        char *p = line;
        char c = *p;

        while (c != '\0') {
            if (!in_block_comment && c == '/') {
                char next_c = *(p+1);

                if (next_c == '/') {
                    break;  // start of inline comment, stop processing line
                } else if (next_c == '*') {
                    in_block_comment = 1;  // start of block comment
                    p++;  // skip over '*'
                    c = *(p+1);  // get next character
                }
            } else if (in_block_comment && c == '*') {
                char next_c = *(p+1);

                if (next_c == '/') {
                    in_block_comment = 0;  // end of block comment
                    p++;  // skip over '/'
                    c = *(p+1);  // get next character
                }
            } else if (!in_block_comment) {
                fputc(c, output_file);
            }

            p++;
            c = *p;
        }

        if (!in_block_comment) {
            fputc('\n', output_file);
        }
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}
