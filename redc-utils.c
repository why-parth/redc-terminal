#if !defined REDC_UTILS_C
#define REDC_UTILS_C

#include <stdio.h>
#define REDC_BUFFER_NAMESPACE
#define REDC_RELEX_NAMESPACE
#define REDC_PARSE_NAMESPACE
#define REDC_CRT_NAMESPACE
#include "redc-crt.h"


#ifdef _WIN32
#define REDC_SEP "\\"
#define REDC_EXT ".exe"
#else
#define REDC_SEP "/"
#define REDC_EXT ""
#endif


def(stk_func) {
    show_buffer;
}

def(relex_func) {
    show_relex_list;
}

def(exit_func) {
    exit(0);
}

def(source_func) {
    REDC_CHAR * src = strof(getb(1, Str));

    // GCC Compilation
    char cmd[1024];
    sprintf( cmd, "gcc " "."REDC_SEP"redc-main.c -DSOURCE='%s' -o .redcrt0", src );
    int exit_code = system(cmd);

    if (!exit_code) { exit(64); }

}

def(clear_func) {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

REDC_INT redcUtilsInit(void) {

    __crtptr Relex command(stk) call(stk_func);
    __crtptr Relex command(relex) call(relex_func);
    __crtptr Relex command(exit) call(exit_func);
    __crtptr Relex command(clear) call(clear_func);

    __crtptr Relex command(source) take(Str) call(source_func);

    return 1;
}


#endif


