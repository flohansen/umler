#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void print_usage();

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file\n");
        return EXIT_FAILURE;
    }
    
    Parser *parser = create_parser(file);

    while (!parser_check_token(parser, TOKEN_TYPE_EOF)) {
        if (parser_check_value(parser, "class")) {
            Node *node = parser_parse_class(parser);
            if (!node) {
                printf("Error: Could not parse class node\n");
                return EXIT_FAILURE;
            }

            node_print(node);
            free_node(node);
        }
    }

    fclose(file);
    return EXIT_SUCCESS;
}

void print_usage() {
    const char *usage =
        "umler v1.0\n"
        "\n"
        "Usage:\n"
        "umler FILENAME\n";

    printf("%s\n", usage);
}
