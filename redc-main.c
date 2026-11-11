#include "redc-utils.c"

#if defined(SOURCE)
#include SOURCE
#endif

int main (const int argc, const char * argv[]) {

    // Init
    try (redcBufferInit(), code) { printf("could not initialize redc Buffer : %llu\n", lluof(code)); return -1; }
    try (redcRelExInit(), code) { printf("could not initialize redc RelEx : %llu\n", lluof(code)); return -1; }
    try (redcParseInit(), code) { printf("could not initialize redc Parse : %llu\n", lluof(code)); return -1; }
    try (redcCRTInit(), code) { printf("could not initialize redc CRT : %llu\n", lluof(code)); return -1; }
    try (redcUtilsInit(), code) { printf("could not initialize redc Utils : %llu\n", lluof(code)); return -1; }

    #if defined(SOURCE)
    RBF_init();
    #endif

    crt_run_loop() till_forever;

    return 0;
}



