#if !defined _REDC_PARSE_H
#define _REDC_PARSE_H

#include "./redc-relex.h"

#include <stdio.h>

// source      this 


REDC_INT redc__parse_sep;
REDC_INT redc__parse_sep_default;

REDC_FNV redc__parse_global_token_fnv;
REDC_CHAR * redc__parse_global_string;
REDC_INT redc__parse_global_string_n;
REDC_INT redc__parse_global_integer;
REDC_INT redc__parse_global_fraction;

#define REDC_PARSE_GTF_RESET redc__parse_global_token_fnv = REDC_FNV_OFFSET
#define REDC_PARSE_GTF_PUSH (redc__parse_global_token_fnv *= REDC_FNV_PRIME), redc__parse_global_token_fnv ^= 

enum redcRelExTType redc__parse_t_type;
enum redcBufferDType redc__parse_d_type;

REDC_INT redcParseInit(void);
REDC_INT redcParsePollString(const REDC_CHAR * _String);
REDC_INT redcParseBufferHandle(void);

REDC_INT redcParseInit(void) {
    redc__parse_sep = ' ';
    redc__parse_sep_default = ' ';
    REDC_PARSE_GTF_RESET;
    redc__parse_t_type = 0;
    redc__parse_global_integer = 0;
    redc__parse_global_fraction = 0;
}

#define REDC__parse_halt_sep_reduction(_Start, _End) if (redc__parse_sep && c == _Start) (printf(REDC_COLOR_STRINGS)), (redc__parse_sep = 0), resume_sep = _End; else
#define REDC__parse_resume_sep_reduction if (!redc__parse_sep && c == resume_sep) (redc__parse_sep = redc__parse_sep_default), resume_sep = 0

REDC_INT redcParseBufferHandle(void) {
    // Push values on the basis of how they are typed in the terminal.
    // String, Int, Float, else FNV

    if (redc__parse_d_type == Fnv) { push(Fnv, redc__parse_global_string_n) redc__parse_global_token_fnv; REDC_BUFFER_PUSH_PTR redc__parse_global_string; }
    else if (redc__parse_d_type == Int) push(Int) redc__parse_global_integer;
    else if (redc__parse_d_type == Float) push(Float) dblof(redc__parse_global_integer) / redc__parse_global_fraction;
    else if (redc__parse_d_type == Str) push(Str, redc__parse_global_string_n) redc__parse_global_string;
    
}

REDC_INT redcParsePollString(const REDC_CHAR * _String) {

    REDC_INT sep_done = 1;
    REDC_INT resume_sep = 0;
    REDC_INT escape = 0; 
    REDC_INT pushed = 1;

    REDC_CHAR * string_start = NULL;
    REDC_INT is_float = 0;

    REDC_CHAR c = *_String;
    REDC_CHAR * c_ptr = (REDC_CHAR *)_String;

    for ( ; c ; c = *(++c_ptr) ) {
        if (escape) goto handle_escape;
        if (sep_done && c == redc__parse_sep) { continue; }
        sep_done = 0;

        if (!string_start) string_start = c_ptr;

        if (c == '\\') { escape = 1; continue; }

        REDC__parse_halt_sep_reduction('"', '"')
        REDC__parse_halt_sep_reduction('\'', '\'')
        REDC__parse_resume_sep_reduction;

        if (redc__parse_sep == c && !sep_done) { pushed = 0; sep_done = 1; }
        if (resume_sep == 0) printf(REDC_COLOR_NONE);

        if ( !redc__parse_sep || (redc__parse_sep && c != redc__parse_sep) ) {
            REDC_PARSE_GTF_PUSH c;

            if (is_float) {
                if (!redc__parse_global_fraction) redc__parse_global_fraction = 10;
                else redc__parse_global_fraction *= 10;
            }
            if (c == '.') is_float = 1;
            else {
                redc__parse_global_integer *= 10;
                redc__parse_global_integer += c - '0';
            }
        }

        if (!pushed) {
            redc__parse_d_type = REDC_STR_DTYPE(string_start, lluof(c_ptr - string_start));
            redc__parse_global_string = string_start;
            redc__parse_global_string_n = lluof(c_ptr - string_start);
            redcParseBufferHandle();
            REDC_PARSE_GTF_RESET;
            string_start = 0;
            redc__parse_global_integer = 0;
            redc__parse_global_fraction = 0;
            is_float = 0;
            pushed = 1;
        }

        putchar(c);



        continue;
        handle_escape:
        escape = 0;
        switch (c) {
            // Not Handled Yet
        }
    }

    redc__parse_d_type = REDC_STR_DTYPE(string_start, lluof(c_ptr - string_start));
    redc__parse_global_string = string_start;
    redc__parse_global_string_n = lluof(c_ptr - string_start);
    redcParseBufferHandle();
    REDC_PARSE_GTF_RESET;
    string_start = 0;

}


#if defined (REDC_PARSE_NAMESPACE)

#define parse redcParsePollString

#endif


#endif