#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int READ_BUF_SIZE = 1024;

void print_usage();

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file");
        return EXIT_FAILURE;
    }

    Lexer *lexer = create_lexer();

    char buf[READ_BUF_SIZE];
    while (fgets(buf, READ_BUF_SIZE, file)) {
        lexer_input(lexer, buf, strlen(buf));

        while (lexer_next(lexer)) {
            char *value = strndup(lexer->token.start, lexer->token.len);
            printf("'%s'\t(%s)\n", value, token_type_str(lexer->token.type));
            free(value);
        }
    }

    fclose(file);
    free_lexer(lexer);
    return EXIT_SUCCESS;
}

void print_usage() {
    const char *usage = "umler v1.0\n"
        "\n"
        "Usage:\n"
        "umler FILENAME\n";

    printf("%s\n", usage);
}
