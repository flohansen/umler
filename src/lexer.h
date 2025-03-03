#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>

#define LEXER_EOF 0;
#define LEXER_SUCCESS 1;

typedef enum {
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_BRACES_OPEN,
    TOKEN_TYPE_BRACES_CLOSE,
    TOKEN_TYPE_BRACKETS_OPEN,
    TOKEN_TYPE_BRACKETS_CLOSE,
    TOKEN_TYPE_PARENTHESES_OPEN,
    TOKEN_TYPE_PARENTHESES_CLOSE,
    TOKEN_TYPE_PLUS,
    TOKEN_TYPE_MINUS,
    TOKEN_TYPE_TILDE,
    TOKEN_TYPE_CHAR,
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    size_t len;
} Token;

typedef struct {
    const char *input;
    size_t len;
    size_t cursor;
    Token token;
} Lexer;

Lexer *create_lexer();
void free_lexer(Lexer *lexer);
void lexer_input(Lexer *lexer, const char *input, size_t len);
int lexer_next(Lexer *lexer);
int lexer_peek_char(Lexer *lexer, char *c);
int lexer_next_char(Lexer *lexer, char *c);
void lexer_parse_symbol(Lexer *lexer);
void lexer_parse_char(Lexer *lexer, TokenType type);

const char *token_type_str(TokenType type);

bool is_whitespace(char c);
bool is_alpha(char c);
bool is_numeric(char c);
bool is_alphanumeric(char c);

#endif
