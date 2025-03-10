cmake_minimum_required(VERSION 3.16..3.31)

project(
    umler
    VERSION 1.0
    LANGUAGES C
)

add_executable(umler src/main.c src/lexer.c src/parser.c src/list.c)
