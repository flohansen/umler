#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "list.h"
#include <stddef.h>
#include <stdio.h>

#define READ_BUF_SIZE 1024

typedef enum {
    NODE_TYPE_CLASS,
} NodeType ;

typedef struct {
    NodeType type;
    char *name;
    size_t name_len;
    MemberList *members;
} ClassNode;

typedef struct {
    NodeType type;
    union {
        ClassNode classNode;
    } data;
} Node;

typedef struct {
    FILE *file;
    char buf[READ_BUF_SIZE];
    Lexer *lexer;
} Parser;

typedef struct {
    Node **items;
    size_t cap;
    size_t len;
} NodeList;

Parser *create_parser(FILE *file);
void free_parser(Parser *parser);
NodeList *parser_parse(Parser *parser);
void parser_advance(Parser *parser);
int parser_expect_token(Parser *parser, TokenType type, Token *token);
int parser_expect_value(Parser *parser, const char *value);
int parser_check_token(Parser *parser, TokenType type);
int parser_check_value(Parser *parser, const char *value);

Node *parser_parse_class(Parser *parser);
void node_print(Node *node);
void free_node(Node *node);

NodeList *create_node_list(size_t cap);
void node_list_append(NodeList *list, Node *node);
void free_node_list(NodeList *list);

#endif //PARSER_H
