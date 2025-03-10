#include "parser.h"
#include "lexer.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser *create_parser(FILE *file) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    memset(parser, 0, sizeof(Parser));
    parser->file = file;
    parser->lexer = create_lexer();

    fgets(parser->buf, READ_BUF_SIZE, parser->file);
    lexer_input(parser->lexer, parser->buf, strlen(parser->buf));
    lexer_next(parser->lexer);

    return parser;
}

void free_parser(Parser *parser) {
    free_lexer(parser->lexer);
    free(parser);
}

int parser_expect_token(Parser *parser, TokenType type, Token *token) {
    if (parser->lexer->token.type != type) {
        return 0;
    }

    *token = parser->lexer->token;
    parser_advance(parser);
    return 1;
}

void parser_advance(Parser *parser) {
    if (!lexer_next(parser->lexer)) {
        if (!fgets(parser->buf, READ_BUF_SIZE, parser->file)) {
            parser->lexer->token.type = TOKEN_TYPE_EOF;
            return;
        }

        lexer_input(parser->lexer, parser->buf, strlen(parser->buf));

        if (!lexer_next(parser->lexer)) {
            parser_advance(parser);
        }
    }
}

int parser_expect_value(Parser *parser, const char *value) {
    if (!parser_check_value(parser, value)) {
        return 0;
    }

    parser_advance(parser);
    return 1;
}

int parser_check_token(Parser *parser, TokenType type) {
    return parser->lexer->token.type == type;
}

int parser_check_value(Parser *parser, const char *value) {
    return strncmp(parser->lexer->token.start, value, strlen(value)) == 0;
}

Node *parser_parse_class(Parser *parser) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }

    memset(node, 0, sizeof(Node));

    if (!parser_expect_value(parser, "class")) {
        printf("unexpected token value\n");
        free(node);
        return NULL;
    }

    Token token;
    if (!parser_expect_token(parser, TOKEN_TYPE_SYMBOL, &token)) {
        printf("unexpected token\n");
        free(node);
        return NULL;
    }

    node->type = NODE_TYPE_CLASS;
    node->data.classNode.type = NODE_TYPE_CLASS;
    node->data.classNode.name = strndup(token.start, token.len);
    node->data.classNode.name_len = token.len;
    node->data.classNode.members = create_member_list(1);

    if (!parser_expect_token(parser, TOKEN_TYPE_BRACES_OPEN, &token)) {
        printf("unexpected token\n");
        free_node(node);
        return NULL;
    }

    if (!parser_expect_token(parser, TOKEN_TYPE_BRACES_CLOSE, &token)) {
        while (parser_check_token(parser, TOKEN_TYPE_OPERATOR)) {
            parser_advance(parser);

            MemberNode member;
            memset(&member, 0, sizeof(MemberNode));

            switch (token.start[0]) {
                case '+':
                    member.type = METHOD_TYPE_PUBLIC;
                    break;
                case '-':
                    member.type = METHOD_TYPE_PRIVATE;
                    break;
            }

            if (!parser_expect_token(parser, TOKEN_TYPE_SYMBOL, &token)) {
                free_node(node);
                return NULL;
            }

            member.name = strndup(token.start, token.len);
            member.name_len = token.len;

            if (!parser_expect_value(parser, ":")) {
                free_node(node);
                return NULL;
            }

            if (!parser_expect_token(parser, TOKEN_TYPE_SYMBOL, &token)) {
                free_node(node);
                return NULL;
            }

            member.ret = strndup(token.start, token.len);
            member.ret_len = token.len;

            if (!parser_expect_value(parser, ";")) {
                free_node(node);
                return NULL;
            }

            member_list_append(node->data.classNode.members, member);
        }

        if (!parser_expect_token(parser, TOKEN_TYPE_BRACES_CLOSE, &token)) {
            free_node(node);
            return NULL;
        }
    }

    return node;
}

void node_print(Node *node) {
    switch (node->type) {
        case NODE_TYPE_CLASS: {
            printf("Class Node\n");
            printf("  Name: %s\n", node->data.classNode.name);
            printf("  Members: %zu\n", node->data.classNode.members->len);

            for (size_t i = 0; i < node->data.classNode.members->len; ++i) {
                MemberNode item = node->data.classNode.members->items[i];
                printf("    - %s %s %s\n", member_type_str(item.type), item.ret, item.name);
            }

            break;
        }
    }
}

void free_node(Node *node) {
    switch (node->type) {
        case NODE_TYPE_CLASS: {
            free(node->data.classNode.name);

            for (size_t i = 0; i < node->data.classNode.members->len; ++i) {
                MemberNode item = node->data.classNode.members->items[i];
                free(item.name);
                free(item.ret);
            }

            free_member_list(node->data.classNode.members);
            break;
        }
    }

    free(node);
}
