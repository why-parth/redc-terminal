#include <stdio.h>
#define REDC_BUFFER_NAMESPACE
#define REDC_RELEX_NAMESPACE
#include "redc-relex.h"

def(sub) {
    printf("this function subtracts");
}

def(return_function) {
    printf("returning..."); 
}

def(source_c_file) {
    printf("the c file has been sourced!");
}

int main (const int argc, const char * argv[]) {

    try (redcBufferInit(), code) { printf("could not initialize redc buffer : %llu\n", lluof(code)); return -1; }
    try (redcRelExInit(), code) { printf("could not initialize redc relex : %llu\n", lluof(code)); return -1; }

    Relex command(source) take(Str) call(source_c_file);
    Relex command(main) take(Int) take(Str) call(return_function);
    
    push(Int) 45;
    push(Str) "This is a string";

    show_buffers;
    show_relex_list;

    return 0;
}



