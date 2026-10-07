#include "compiler.h"
#include "helpers/vector.h"
#include <stdarg.h>
#include <stdlib.h>

struct lex_process_functions compiler_lex_functions = {
    .next_char = compile_process_next_char, .peek_char = compile_process_peek_char, .push_char = compile_process_push_char};

void compiler_error(struct compile_process *compiler, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, " on line %i, col %i in file %s\n", compiler->pos.line, compiler->pos.col, compiler->pos.filename);

    exit(-1);
}

void compiler_warning(struct compile_process *compiler, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    fprintf(stderr, " on line %i, col %i in file %s\n", compiler->pos.line, compiler->pos.col, compiler->pos.filename);
}

void print_tokens(struct lex_process *lex_process) {
    printf("Token List:-\n");
    int vector_size = vector_count(lex_process->token_vec);
    for (int i = 0; i < vector_size; i++) {
        struct token *token = vector_at(lex_process->token_vec, i);
        printf("%d: ", i);
        if (token->type == TOKEN_TYPE_NUMBER) {
            printf("(Number): %llu\n", token->llnum);
        } else if (token->type == TOKEN_TYPE_STRING) {
            printf("(String) %s\n", token->sval);
        } else if (token->type == TOKEN_TYPE_OPERATOR) {
            printf("(Operator) %s\n", token->sval);
        } else if (token->type == TOKEN_TYPE_SYMBOL) {
            printf("(Symbol) %c\n", token->cval);
        } else if (token->type == TOKEN_TYPE_IDENTIFIER) {
            printf("(Identifier) %s\n", token->sval);
        } else if (token->type == TOKEN_TYPE_KEYWORD) {
            printf("(Keyword) %s\n", token->sval);
        } else if (token->type == TOKEN_TYPE_NEWLINE) {
            printf("(Newline)\n");
        } else if (token->type == TOKEN_TYPE_COMMENT) {
            printf("(Comment) %s\n", token->sval);
        }
    }
}

int compile_file(const char *filename, const char *out_filename, int flags) {

    struct compile_process *process = compile_process_create(filename, out_filename, flags);
    if (!process) {
        return COMPILER_FAILED_WITH_ERRORS;
    }

    // Perform Lexical Analysis -> Tokens
    struct lex_process *lex_process = lex_process_create(process, &compiler_lex_functions, NULL);
    if (!lex_process) {
        return COMPILER_FAILED_WITH_ERRORS;
    }

    if (lex(lex_process) == LEXICAL_ANALYSIS_ALL_OK) {
        print_tokens(lex_process);
    } else {
        return COMPILER_FAILED_WITH_ERRORS;
    }

    // Perform parsing -> AST

    // Perform code generation. -> ASM out

    return COMPILER_FILE_COMPILED_OK;
}
