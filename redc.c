// So that header files also declare the showing functions
#include <stdio.h>

// For a better namespace
#define REDC_BUFFER_NAMESPACE
#define REDC_RELEX_NAMESPACE
#define REDC_PARSE_NAMESPACE
#define REDC_CRT_NAMESPACE

// redc-parse.h (it icluded redc-relex.h and redc-buffer.h)
#include "redc-crt.h"

// Defining functions
def(add_func) {
    printf("Numbers added!\n");
}

def(source_func) {
    printf("C file is sourced!\n");
}

def(compile_func) {
    printf("C file is compiled!\n");
}

int main (const int argc, const char * argv[]) {

    // Init
    try (redcBufferInit(), code) { printf("could not initialize redc Buffer : %llu\n", lluof(code)); return -1; }
    try (redcRelExInit(), code) { printf("could not initialize redc RelEx : %llu\n", lluof(code)); return -1; }
    try (redcParseInit(), code) { printf("could not initialize redc Parse : %llu\n", lluof(code)); return -1; }
    try (redcCRTInit(), code) { printf("could not initialize redc CRT : %llu\n", lluof(code)); return -1; }

    // All RelEx (Relation Expressions)
    Relex command(source) take(Str) call(source_func);
    Relex command(gcc) take(Str) call(compile_func);
    __crtptr Relex command(add) take(Int) take(Int) call(add_func);
    Relex command(take_3) upto(3) call(add_func);
    Relex command(till_str) till(Str) call(add_func);

    // // Showing each RelEx
    // show_relex_list;

    // // Showing the buffer (must be empty)
    // show_buffer;

    // // Passing an expression
    // parse( "add 45 67");
    
    // // Showing the buffer (must have some data in it)
    // show_buffer;

    // /* The parsed data is not acted upon yet. */

    /* CRT is coming along pretty nice! */

    // 'redc__parse_buffer_str_max_len' is set to 8 for demonstation.

    printf("Alpha Run of the redc's CRT integration!\n");

    crt_loop() till_no_input;

    show_buffer;
    
    crt_run();
    
    show_buffer;
    
    putchar('\n');

    return 0;
}



