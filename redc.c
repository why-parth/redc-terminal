#include <stdio.h>
#define REDC_BUFFER_NAMESPACE
#include "redc-buffer.h"


int main (const int argc, const char * argv[]) {
    redcBufferInit();

    push(Int) 45;
    push(Str) "This is a string";
    push(Char) 'a';

    show;

    pop(1);

    show;

    return 0;
}



