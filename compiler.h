#ifndef PEACHCOMPILER_H
#define PEACHCOMPILER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define S_EQ(str, str2) (str && str2 && (strcmp(str, str2) == 0))

struct pos {
    int line;
    int col;
    const char *filename;
};

#define NUMERIC_CASE                                                                                                               \
    case '0':                                                                                                                      \
    case '1':                                                                                                                      \
    case '2':                                                                                                                      \
    case '3':                                                                                                                      \
    case '4':                                                                                                                      \
    case '5':                                                                                                                      \
    case '6':                                                                                                                      \
    case '7':                                                                                                                      \
    case '8':                                                                                                                      \
    case '9'

#define OPERATOR_CASE_EXCLUDING_DIVISION                                                                                           \
    case '+':                                                                                                                      \
    case '-':                                                                                                                      \
    case '*':                                                                                                                      \
    case '>':                                                                                                                      \
    case '<':                                                                                                                      \
    case '^':                                                                                                                      \
    case '%':                                                                                                                      \
    case '!':                                                                                                                      \
    case '=':                                                                                                                      \
    case '~':                                                                                                                      \
    case '|':                                                                                                                      \
    case '&':                                                                                                                      \
    case '(':                                                                                                                      \
    case '[':                                                                                                                      \
    case ',':                                                                                                                      \
    case '.':                                                                                                                      \
    case '?'

#define SYMBOL_CASE                                                                                                                \
    case '{':                                                                                                                      \
    case '}':                                                                                                                      \
    case ':':                                                                                                                      \
    case ';':                                                                                                                      \
    case '#':                                                                                                                      \
    case '\\':                                                                                                                     \
    case ')':                                                                                                                      \
    case ']'

enum { 
    LEXICAL_ANALYSIS_ALL_OK, 
    LEXICAL_ANALYSIS_INPUT_ERROR 
};

enum {
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE
};

enum {
    NUMBER_TYPE_NORMAL,
    NUMBER_TYPE_LONG,
    NUMBER_TYPE_FLOAT,
    NUMBER_TYPE_DOUBLE
};

struct token {

    int type;
    int flags;
    struct pos pos;

    union {
        char cval;          // char
        const char *sval;   // string
        unsigned int inum;  // int
        unsigned long lnum; // long
        unsigned long long llnum;
        void *any;
    };

    struct token_number {
        int type;
    } num;
     
    // true if there is whitespace between the token and next token.
    bool whitespace;

    const char *between_brackets;
};

struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process *process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process *process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process *process, char c);

struct lex_process_functions {
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;
};

struct lex_process {
    struct pos pos;
    struct vector *token_vec;
    struct compile_process *compiler;

    /**
     * ((50))
     */
    int current_expression_count;
    struct buffer *parenthesis_buffer;
    struct lex_process_functions *function;

    // allows private data lexer does not understand.
    void *private;
};

enum { COMPILER_FILE_COMPILED_OK, COMPILER_FAILED_WITH_ERRORS };

struct compile_process {
    // compile flags
    int flags;

    struct pos pos;
    struct compile_process_input_file {
        FILE *fp;
        const char *abs_path;
    } cfile;

    // A vector of tokens from lexical analysis.
    struct vector *token_vec;
    FILE *ofile;
};

int compile_file(const char *filename, const char *out_filename, int flags);
struct compile_process *compile_process_create(const char *filename, const char *filename_out, int flags);

char compile_process_next_char(struct lex_process *lex_process);
char compile_process_peek_char(struct lex_process *lex_process);
void compile_process_push_char(struct lex_process *lex_process, char c);

struct lex_process *lex_process_create(struct compile_process *compiler, struct lex_process_functions *functions, void *private);
void lex_process_free(struct lex_process *process);
void *lex_process_private(struct lex_process *process);
struct vector *lex_process_tokens(struct lex_process *process);

int lex(struct lex_process *process);

/** 
 * @brief Builds tokens for the input string
 * 
 * @param compiler
 * @param str
 * @return struct lex_process*
 */
struct lex_process *tokens_build_for_string(struct compile_process *compiler, const char *str);

void compiler_error(struct compile_process *compiler, const char *msg, ...);
void compiler_warning(struct compile_process *compiler, const char *msg, ...);

bool token_is_keyword(struct token *token, const char *value);

#endif