#if !defined _REDC_PARSE_H
#define _REDC_PARSE_H

#include "./redc-relex.h"

#include <stdio.h>

// source      this 


REDC_INT redc__parse_sep;
REDC_INT redc__parse_sep_default;

REDC_FNV redc__parse_global_token_fnv;

#define REDC_PARSE_GTF_RESET redc__parse_global_token_fnv = REDC_FNV_OFFSET
#define REDC_PARSE_GTF_PUSH (redc__parse_global_token_fnv *= REDC_FNV_PRIME), redc__parse_global_token_fnv ^= 

REDC_INT redcParseInit(void);
REDC_INT redcParsePollString(const REDC_CHAR * _String);
REDC_INT redcParseTokenFNVHandle(void);

REDC_INT redcParseInit(void) {
    redc__parse_sep = ' ';
    redc__parse_sep_default = ' ';
    REDC_PARSE_GTF_RESET;
}

#define REDC__parse_halt_sep_reduction(_Start, _End) if (redc__parse_sep && c == _Start) (printf(REDC_COLOR_STRINGS)), (redc__parse_sep = 0), resume_sep = _End; else
#define REDC__parse_resume_sep_reduction if (!redc__parse_sep && c == resume_sep) (redc__parse_sep = redc__parse_sep_default), resume_sep = 0

REDC_INT redcParseTokenFNVHandle(void) {
    
}

REDC_INT redcParsePollString(const REDC_CHAR * _String) {

    REDC_INT sep_done = 1;
    REDC_INT resume_sep = 0;
    REDC_INT escape = 0; 
    REDC_INT pushed = 1;

    for ( REDC_CHAR c = *_String, * c_ptr = (REDC_CHAR *)_String ; c ; c = *(++c_ptr) ) {
        if (escape) goto handle_escape;
        if (sep_done && c == redc__parse_sep) { continue; }
        sep_done = 0;

        if (c == '\\') { escape = 1; continue; }

        REDC__parse_halt_sep_reduction('"', '"')
        REDC__parse_halt_sep_reduction('\'', '\'')
        REDC__parse_resume_sep_reduction;

        if (redc__parse_sep == c && !sep_done) { pushed = 0; sep_done = 1; }
        if (resume_sep == 0) printf(REDC_COLOR_NONE);

        if ( !redc__parse_sep || (redc__parse_sep && c != redc__parse_sep) )
        REDC_PARSE_GTF_PUSH c;

        if (!pushed) {
            redcParseTokenFNVHandle();
            REDC_PARSE_GTF_RESET;
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

    redcParseTokenFNVHandle();
    REDC_PARSE_GTF_RESET;

}



#endif