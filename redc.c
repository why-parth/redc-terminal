#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#define SEP "\\"
#define EXT ".exe"
#define DEL "del"
#define REN "ren"
#define ESE 0
#else
#define SEP "/"
#define EXT ""
#define DEL "rm"
#define REN "mv"
#define ESE 8
#endif

int main (const int argc, const char * argv[]) {
    
    // GCC Compilation
    char cmd[1024];
    sprintf(
        cmd,
        "gcc " "."SEP"redc-main.c " "%s%s%s" " -o .redcrt", 
        (argc == 2) ? "-DSOURCE='\"" : "", (argc == 2) ? *(argv + 1) : "", (argc == 2) ? "\"'" : ""
    );
    int exit_code = system(cmd);
    if (exit_code) return exit_code;

    // return
    crt:
    exit_code = system( "."SEP".redcrt"EXT );
    exit_code = exit_code >> ESE;

    switch (exit_code) {
        case 0:
        return 0;
        break;

        case 64:
        system( DEL " .redcrt " );
        system( REN " .redcrt0 .redcrt " );
        goto crt;
        break;

        default:
        return exit_code;
    }

}



