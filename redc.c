#include <stdio.h>
#define REDC_BUFFER_NAMESPACE
#include "redc-buffer.h"


void add (void) {
    printf("This function adds");
}

def(sub) {
    printf("this function subtracts");
}

int main (const int argc, const char * argv[]) {
    redcBufferInit();

    push(Int) 45;
    push(Str) "This is a string";
    push(Char) 'a';
    push_func(sub);

    // push_func(add);

    show;

    pop(1);

    show;

    return 0;
}



