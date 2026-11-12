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

#define __crtptr redc__relex_global_crt_flag = 1;

REDC_INT redcParseInit(void);
REDC_INT redcCRTRun(REDC_INT _Log, ...);

REDC_INT redcCRTInit(void) {
    redcCRTBufferPointer = redcBuffer;
}

#define REDC_CRT_RELINE_X printf(REDC_COLOR_CONDITION "\033[1s\033[1A x \033[1u" REDC_COLOR_NONE)

#define REDC_CRT_RUN_SHOW_TRUE if (_Log) printf( REDC_COLOR_NAMES "true\n" REDC_COLOR_NONE)
#define REDC_CRT_RUN_SHOW_FALSE if (_Log) printf( REDC_COLOR_CONDITION "false\n" REDC_COLOR_NONE)
#define REDC_CRT_RUN_SHOW_FALSE_EXIT {                                                                                          \
    REDC_CRT_RUN_SHOW_FALSE;                                                                                                    \
    if (_Tried == 1) _FirstEnd = _Node ? _Node : redcBufferLast;                                                                \
    goto try_next;                                                                                                              \
}                                                                                                                               \

REDC_INT redcCRTRun(REDC_INT _Log, ...) {
    
    struct redcRelExNode * _RelEx = NULL;
    struct redcBufferNode * _Node = REDC_CRT_POINTER->Next, * _Prev = REDC_CRT_POINTER;
    struct redcBufferNode * _Before = NULL, * _Start = NULL, * _Stop = NULL, * _FirstEnd = NULL;

    REDC_INT _Tried = 0, _Matched = 0;
    REDC_FNV _Fnv = 0;
    REDC_INT _RelEx_i;

    for ( ; _Node ; (_Prev = _Node), REDC_JUMP(_Node) ) {
        
        if (_Node->DType == Fnv) {
            _Tried = 0; _Matched = 0;
            _Fnv = _Node->DValue.Fnv;
            _RelEx_i = REDC_RELEX_FULL_SEARCH(_Fnv);


            _Before = _Prev;
            _Start = _Node;
            _FirstEnd = NULL;

            while (!_Matched && _RelEx_i) {
                _Tried++;
                _Node = _Start;
                _Prev = _Before;
                _RelEx = redcRelExGetNode(_RelEx_i);

                if (_Log) {
                    printf("relex : ");
                    show_relex(_RelEx);
                    putchar('\n');
                }

                struct redcRelExNode * _Check = _RelEx->Next->Next;

                while (_Check) {
                    if (_Log) redcShowRelExNode(_Check), putchar(' ');

                    // Checking if the rule holds
                    switch (_Check->TType) {

                        case Take:
                        REDC_JUMP(_Prev);
                        REDC_JUMP(_Node);
                        if (!_Node) { REDC_CRT_RUN_SHOW_FALSE_EXIT }

                        if (_Node->DType == _Check->TData) { REDC_CRT_RUN_SHOW_TRUE; }
                        else REDC_CRT_RUN_SHOW_FALSE_EXIT;
                        
                        break;

                        case Upto:

                        for (REDC_INT redc_i = 0; redc_i < _Check->TData; redc_i++) {
                            REDC_JUMP(_Prev);
                            REDC_JUMP(_Node);
                            if (!_Node) REDC_CRT_RUN_SHOW_FALSE_EXIT
                        }

                        REDC_CRT_RUN_SHOW_TRUE;

                        break;

                        case Till:

                        for ( REDC_JUMP(_Prev), REDC_JUMP(_Node); _Node && _Node->DType != _Check->TData; REDC_JUMP(_Prev), REDC_JUMP(_Node));
                        if (!_Node) REDC_CRT_RUN_SHOW_FALSE_EXIT

                        REDC_CRT_RUN_SHOW_TRUE;

                        break;

                        case Call:

                        if (!_Check->TData) REDC_CRT_RUN_SHOW_FALSE_EXIT
                        REDC_CRT_RUN_SHOW_TRUE;
                        
                        ((redc__func_ptr)(_Check->TPtr))();

                        break;
                        
                    }

                    _Check = _Check->Next;
                }

                _Matched = 1;
                try_next:
                if (!_Matched) _RelEx_i = redcRelExIs_Fnv(_Fnv, _Log);
            }

            if (!_Tried) continue;

            if (!_Matched) {
                REDC_CRT_RELINE_X;
                REDC_CRT_POINTER = _FirstEnd;
                return 0;
            }

            _Stop = _Node->Next;

            if (_RelEx) {
                if (_RelEx->__crt_flag == 1) {

                    _Before = REDC_CRT_POINTER;
                    while (_Before->Next != _Start) REDC_JUMP(_Before);

                    _Before->Next = _Stop;

                    if (_Stop == NULL) redcBufferLast = _Before;

                    while ( _Start != _Stop ) {
                        struct redcBufferNode * _Next = _Start->Next;
                        free(_Start);
                        redcBufferLength--;
                        _Start = _Next;
                    }

                    _Node = _Before;
                }
                else {
                    REDC_CRT_POINTER = _Node;
                }
            }
            
        }

    }

    return 1;
}


#if defined (REDC_CRT_NAMESPACE)

#define crt_run(...) redcCRTRun(__VA_OPT__(__VA_ARGS__, ) 0)

#define crt_get(...) redcCRTIndent_0(__VA_ARGS__), redcCRTGet, redcParseParseString(redcCRTInput, redcCRTInputLen - 1, 0), free(redcCRTInput), redcCRTInput = NULL, redc__crt_input_cap = 0 
#define crt_loop(...) for (REDC_INT redc__crt_loop_temp = 1 ; redc__crt_loop_temp;) do { if (redc__crt_loop_temp) redc__crt_loop_temp = 0; else crt_get(__VA_ARGS__); } while
#define crt_run_loop(...) for (REDC_INT redc__crt_loop_temp = 1 ; redc__crt_loop_temp;) do { if (redc__crt_loop_temp) redc__crt_loop_temp = 0; else { crt_get(__VA_ARGS__); crt_run(); } } while

#define till_no_input REDC_CRT_CONDITION_NO_INPUT
#define till_forever (1)

#endif

#endif

#endif