#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

struct compile_process *compile_process_create(const char *filename, const char *filename_out, int flags)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Failed to open %s\n", filename);
        return NULL;
    }

    FILE *out_file = NULL;
    if (filename_out)
    {
        out_file = fopen(filename_out, "w");
        if (!out_file)
        {
            printf("Failed to open output %s\n", filename_out);
            return NULL;
        }
    }

    struct compile_process *process = calloc(1, sizeof(struct compile_process));
    process->flags = flags;
    process->cfile.fp = file;
    process->ofile = out_file;

    return process;
}

// pull next character from file and return
char compile_process_next_char(struct lex_process *lex_process)
{
    struct compile_process *compiler = lex_process->compiler;
    compiler->pos.col += 1;
    char c = getc(compiler->cfile.fp);
    if (c == '\n')
    {
        compiler->pos.line += 1;
        compiler->pos.col = 1;
    }

    return c;
}

// peek next character, do not advance file position.
char compile_process_peek_char(struct lex_process *lex_process)
{
    struct compile_process *compiler = lex_process->compiler;
    char c = getc(compiler->cfile.fp);
    ungetc(c, compiler->cfile.fp);

    return c;
}

// push character back to file
void compile_process_push_char(struct lex_process *lex_process, char c)
{
    struct compile_process *compiler = lex_process->compiler;
    ungetc(c, compiler->cfile.fp);
}