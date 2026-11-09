#include <stdio.h>
#define REDC_BUFFER_NAMESPACE
#define REDC_RELEX_NAMESPACE
#include "redc-parse.h"

def(sub) {
    printf("this function subtracts");
}

def(return_function) {
    printf("returning..."); 
}

def(source_c_file) {
    printf("the c file has been sourced!");
}

def(compile) {
    printf( REDC_COLOR_NAMES "compiled!" REDC_COLOR_NONE "\n" );
}

int main (const int argc, const char * argv[]) {

    try (redcBufferInit(), code) { printf("could not initialize redc buffer : %llu\n", lluof(code)); return -1; }
    try (redcRelExInit(), code) { printf("could not initialize redc relex : %llu\n", lluof(code)); return -1; }
    try (redcParseInit(), code) { printf("could not initialize redc parse : %llu\n", lluof(code)); return -1; }

    Relex command(source) take(Str) call(source_c_file);
    Relex command(main) take(Int) take(Str) call(return_function);
    Relex command(gcc) take(Str) call(compile);
    
    // push(Int) 45;
    // push(Str) "This is a string";

    // show_buffers;
    // show_relex_list;

    // redcRelExSource();

    // redcRelExSource();
    redcRelExIs_Fnv( redc__str_to_fnv("source") );






    // redcParsePollString
    // ("gcc path = 'this is the path  ->  here' compile main.c nl = \"\\n\"");

    putchar('\n');

    return 0;
}



