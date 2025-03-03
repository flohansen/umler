#include "lexer.h"
#include <stdlib.h>
#include <string.h>

Lexer *create_lexer() {
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    memset(lexer, 0, sizeof(Lexer));
    return lexer;
}

void free_lexer(Lexer *lexer) {
    free(lexer);
}

void lexer_input(Lexer *lexer, const char *input, size_t len) {
    lexer->input = input;
    lexer->len = len;
    lexer->cursor = 0;
}

int lexer_next(Lexer *lexer) {
    char c;

    do {
        if (!lexer_next_char(lexer, &c)) {
            return LEXER_EOF;
        }
    } while (is_whitespace(c));

    if (is_alpha(c)) {
        lexer_parse_symbol(lexer);
    } else if (c == '{') {
        lexer_parse_char(lexer, TOKEN_TYPE_BRACES_OPEN);
    } else if (c == '}') {
        lexer_parse_char(lexer, TOKEN_TYPE_BRACES_CLOSE);
    } else if (c == '[') {
        lexer_parse_char(lexer, TOKEN_TYPE_BRACKETS_OPEN);
    } else if (c == ']') {
        lexer_parse_char(lexer, TOKEN_TYPE_BRACKETS_CLOSE);
    } else if (c == '(') {
        lexer_parse_char(lexer, TOKEN_TYPE_PARENTHESES_OPEN);
    } else if (c == ')') {
        lexer_parse_char(lexer, TOKEN_TYPE_PARENTHESES_CLOSE);
    } else if (c == '+') {
        lexer_parse_char(lexer, TOKEN_TYPE_PLUS);
    } else if (c == '-') {
        lexer_parse_char(lexer, TOKEN_TYPE_MINUS);
    } else if (c == '~') {
        lexer_parse_char(lexer, TOKEN_TYPE_TILDE);
    } else {
        lexer_parse_char(lexer, TOKEN_TYPE_CHAR);
    }

    return LEXER_SUCCESS;
}

int lexer_peek_char(Lexer *lexer, char *c) {
    if (lexer->cursor >= lexer->len) {
        return LEXER_EOF;
    }

    *c = lexer->input[lexer->cursor];
    return LEXER_SUCCESS;
}

int lexer_next_char(Lexer *lexer, char *c) {
    int result = lexer_peek_char(lexer, c);
    if (!result) {
        return result;
    }

    lexer->cursor++;
    return LEXER_SUCCESS;
}

void lexer_parse_symbol(Lexer *lexer) {
    lexer->token.type = TOKEN_TYPE_SYMBOL;
    lexer->token.start = lexer->input + lexer->cursor - 1;
    lexer->token.len = 1;

    while (1) {
        char c;
        if (!lexer_peek_char(lexer, &c)) {
            break;
        }

        if (!is_alphanumeric(c) && c != '_') {
            break;
        }

        lexer->cursor++;
        lexer->token.len++;
    }
}

void lexer_parse_char(Lexer *lexer, TokenType type) {
    lexer->token.type = type;
    lexer->token.start = lexer->input + lexer->cursor - 1;
    lexer->token.len = 1;
}

const char *token_type_str(TokenType type) {
    switch (type) {
        case TOKEN_TYPE_SYMBOL:
            return "SYMBOL";
        case TOKEN_TYPE_BRACES_OPEN:
            return "BRACES OPEN";
        case TOKEN_TYPE_BRACES_CLOSE:
            return "BRACES CLOSE";
        case TOKEN_TYPE_BRACKETS_OPEN:
            return "BRACKETS OPEN";
        case TOKEN_TYPE_BRACKETS_CLOSE:
            return "BRACKETS CLOSE";
        case TOKEN_TYPE_PARENTHESES_OPEN:
            return "PARENTHESES OPEN";
        case TOKEN_TYPE_PARENTHESES_CLOSE:
            return "PARENTHESES CLOSE";
        case TOKEN_TYPE_PLUS:
            return "PLUS";
        case TOKEN_TYPE_MINUS:
            return "MINUS";
        case TOKEN_TYPE_TILDE:
            return "TILDE";
        default:
            return "CHAR";
    }
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_numeric(char c) {
    return c >= '0' && c <= '9';
}

bool is_alphanumeric(char c) {
    return is_alpha(c) || is_numeric(c);
}
