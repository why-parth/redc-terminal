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

#if defined (REDC_CRT_NAMESPACE)

#define crt_get(...) redcCRTIndent_0(__VA_ARGS__), redcCRTGet, redcParseParseString(redcCRTInput, redcCRTInputLen - 1, 0), free(redcCRTInput), redcCRTInput = NULL, redc__crt_input_cap = 0 
#define crt_loop(...) for (REDC_INT redc__crt_loop_temp = 1 ; redc__crt_loop_temp;) do { if (redc__crt_loop_temp) redc__crt_loop_temp = 0; else crt_get(__VA_ARGS__); } while

#define till_no_input REDC_CRT_CONDITION_NO_INPUT

#endif

#endif

#endif