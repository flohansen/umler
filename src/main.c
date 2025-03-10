#include "parser.h"
#include <stddef.h>
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
    NodeList *nodes = parser_parse(parser);

    for (size_t i = 0; i < nodes->len; ++i) {
        Node *node = nodes->items[i];
        node_print(node);
    }

    free_node_list(nodes);

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
