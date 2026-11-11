#include "redc-utils.c"

def(hi_func) {
    printf("hello\n");
}

def(init) {

    __crtptr Relex command(hi) call(hi_func);

}