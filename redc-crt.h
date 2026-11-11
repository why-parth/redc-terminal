#if !defined REDC_C_RUN_TIME_H
#define REDC_C_RUN_TIME_H

#include "redc-parse.h"

#if defined _STDIO_H

REDC_CHAR * redcCRTInput;
REDC_INT redc__crt_input_cap;
REDC_INT redcCRTInputLen;

#define redcCRTGet redcCRTInputLen = getline(str_ptrof(&redcCRTInput), size_t_ptrof(&redc__crt_input_cap), stdin)
#define redcCRTIndent_0(...) printf( REDC_COLOR_TERMINAL "%s" REDC_COLOR_NONE, __VA_OPT__(__VA_ARGS__,) " > ")

#define REDC_CRT_CONDITION_NO_INPUT (redcCRTInputLen != 1)


struct redcBufferNode * redcCRTBufferPointer;
#define REDC_CRT_POINTER redcCRTBufferPointer

// #define __crtptr 

REDC_INT redcParseInit(void);
REDC_INT redcCRTRun(REDC_INT _Log);

REDC_INT redcCRTInit(void) {
    redcCRTBufferPointer = redcBuffer;
}

REDC_INT redcCRTRun(REDC_INT _Log) {

    for ( struct redcBufferNode * _Node = REDC_CRT_POINTER ; _Node ; _Node = _Node->Next ) {
        if (_Node->DType == Fnv) {
            REDC_INT _RelEx_i = redcRelExIs_Fnv(_Node->DValue.Fnv, _Log);
            struct redcRelExNode * _RelEx = redcRelExGetNode(_RelEx_i);

            if (_Log) {
                printf("relex : ");
                show_relex(_RelEx);
                putchar('\n');
            }

            struct redcBufferNode * _Check = _Node->Next;
            while (_Check) {
                // redcShowRele;
            }
            
        }
    }

}


#if defined (REDC_CRT_NAMESPACE)

#define crt_get(...) redcCRTIndent_0(__VA_ARGS__), redcCRTGet, redcParseParseString(redcCRTInput, redcCRTInputLen - 1, 0), free(redcCRTInput), redcCRTInput = NULL, redc__crt_input_cap = 0 
#define crt_loop(...) for (REDC_INT redc__crt_loop_temp = 1 ; redc__crt_loop_temp;) do { if (redc__crt_loop_temp) redc__crt_loop_temp = 0; else crt_get(__VA_ARGS__); } while

#define till_no_input REDC_CRT_CONDITION_NO_INPUT

#endif

#endif

#endif