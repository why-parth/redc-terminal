#if !defined _REDC_BUFFER_H
#define _REDC_BUFFER_H

#include <stdlib.h>
#include <stdint.h>
#include <float.h>

#define REDC_TRY(_InitFunction, ...) for ( REDC_INT __VA_OPT__(__VA_ARGS__,) redc_exit = 1; redc_exit; redc_exit = 0 ) if (!( __VA_OPT__(__VA_ARGS__ =) _InitFunction))

#define REDC_IMPLEMENT

#define REDC__merge_token_2(a, b) a##b
#define REDC__merge_2(a, b) REDC__merge_token_2(a, b)

#define REDC_MERGE REDC__merge_2

#define REDC_INT __int128_t
#define REDC_FLOAT __float128
#define REDC_CHAR __u_char

#define intof(_Value) ((int)(_Value))
#define lluof(_Value) ((unsigned long long int)(_Value))
#define fltof(_Value) ((float)(_Value))
#define dblof(_Value) ((double)(_Value))

#define ischar == (REDC_CHAR)

#define REDC_FNV_OFFSET 0xcbf29ce484222325LLU
#define REDC_FNV_PRIME 0x00000100000001b3LLU

#define REDC_FNV __uint64_t

#define REDC_STR REDC_CHAR *

typedef void (* redc__func_ptr)(void);
#define REDC_FUNC redc__func_ptr

typedef struct {
    REDC_FUNC ptr;
    REDC_CHAR * name;
} REDC_FUNCTION_DATA;

#define REDC_BUFFER_FUNCTION(_Name)                                                 \
    void REDC_MERGE(RBF_, _Name) (void);                           \
    REDC_FUNCTION_DATA _Name = {REDC_MERGE(RBF_, _Name), #_Name};  \
    void REDC_MERGE(RBF_, _Name) (void)

union redcBufferDValue {
    REDC_CHAR Char;
    REDC_INT Int;
    REDC_FLOAT Float;
    REDC_FNV Fnv;
    REDC_STR Str;
    REDC_FUNC Func;
};

enum redcBufferDType {
    Char, Int, Float, Fnv, Str, Func
};

REDC_CHAR * redc__buffer_dtype_name[] = { "Char", "Int", "Float", "Fnv", "Str", "Func" };
#define REDC_BUFFER_DTYPE_NAME(_DType) (redc__buffer_dtype_name[_DType])

struct redcBufferNode {
    enum redcBufferDType DType;
    union redcBufferDValue DValue;
    struct redcBufferNode * Next;
    void * __ptr;
};

#define REDC_JUMP(_Node) (_Node) = (_Node->Next)

#define REDC_BUFFER_NEW_NODE(...) (__VA_OPT__(__VA_ARGS__ =) (struct redcBufferNode *)malloc(sizeof(struct redcBufferNode)))
#define REDC_BUFFER_DATA(_Node, _Type, _Value) (_Node->DType = _Type), _Node->DValue._Type = _Value
#define REDC_BUFFER_TERMINATE(_Node) _Node->Next = NULL
#define REDC_BUFFER_PTR(_Node, _Value) _Node->__ptr = _Value 

struct redcBufferNode * redcBuffer;
struct redcBufferNode * redcBufferLast;
REDC_INT redcBufferLength;

REDC_INT redcBufferInit(void);

REDC_INT redcBufferPush_Int(REDC_INT _Value);
REDC_INT redcBufferPush_Char(REDC_CHAR _Value);
REDC_INT redcBufferPush_Float(REDC_FLOAT _Value);
REDC_INT redcBufferPush_Str(REDC_STR _Value);
REDC_INT redcBufferPush_Fnv(REDC_FNV _Value);

REDC_FNV redc__str_to_fnv(const REDC_CHAR * _String);

REDC_INT redc__buffer_push_func(REDC_FUNC _Value, REDC_CHAR * _Name);
REDC_INT redc__buffer_push_func_data(REDC_FUNCTION_DATA * _Value);

#define redcBufferPush_Func(_Value) _Generic( (_Value),                     \
    REDC_FUNC : redc__buffer_push_func((REDC_FUNC)&_Value, #_Value),        \
    default : redc__buffer_push_func_data((REDC_FUNCTION_DATA *)&_Value)    \
)

#if defined (REDC_IMPLEMENT)

/* Init Buffer */

REDC_INT redcBufferInit(void) {
    if (!(redcBuffer = REDC_BUFFER_NEW_NODE())) return 0;
    redcBuffer->DType = Int;
    redcBuffer->DValue.Int = 0;
    redcBuffer->Next = NULL;
    redcBufferLast = redcBuffer;
    redcBufferLength = 0;
    return 1;
}

/* Pushing function(s) */

REDC_INT redcBufferPush_Char(REDC_CHAR _Value) {
    if (!REDC_BUFFER_NEW_NODE(redcBufferLast->Next)) return 0;
    REDC_JUMP(redcBufferLast);
    REDC_BUFFER_DATA(redcBufferLast, Char, _Value);
    REDC_BUFFER_TERMINATE(redcBufferLast);
    redcBufferLength++;
    return 1;
}

REDC_INT redcBufferPush_Int(REDC_INT _Value) {
    if (!REDC_BUFFER_NEW_NODE(redcBufferLast->Next)) return 0;
    REDC_JUMP(redcBufferLast);
    REDC_BUFFER_DATA(redcBufferLast, Int, _Value);
    REDC_BUFFER_TERMINATE(redcBufferLast);
    redcBufferLength++;
    return 1;
}

REDC_INT redcBufferPush_Float(REDC_FLOAT _Value) {
    if (!REDC_BUFFER_NEW_NODE(redcBufferLast->Next)) return 0;
    REDC_JUMP(redcBufferLast);
    REDC_BUFFER_DATA(redcBufferLast, Float, _Value);
    REDC_BUFFER_TERMINATE(redcBufferLast);
    redcBufferLength++;
    return 1;
}

REDC_INT redcBufferPush_Fnv(REDC_FNV _Value) {
    if (!REDC_BUFFER_NEW_NODE(redcBufferLast->Next)) return 0;
    REDC_JUMP(redcBufferLast);
    REDC_BUFFER_DATA(redcBufferLast, Fnv, _Value);
    REDC_BUFFER_TERMINATE(redcBufferLast);
    redcBufferLength++;
    return 1;
}

REDC_INT redcBufferPush_String(REDC_STR _Value) {
    if (!REDC_BUFFER_NEW_NODE(redcBufferLast->Next)) return 0;
    REDC_JUMP(redcBufferLast);
    REDC_BUFFER_DATA(redcBufferLast, Str, _Value);
    REDC_BUFFER_TERMINATE(redcBufferLast);
    redcBufferLength++;
    return 1;
}

REDC_INT redc__buffer_push_func(REDC_FUNC _Value, REDC_CHAR * _Name) {
    if (!REDC_BUFFER_NEW_NODE(redcBufferLast->Next)) return 0;
    REDC_JUMP(redcBufferLast);
    REDC_BUFFER_DATA(redcBufferLast, Func, _Value);
    REDC_BUFFER_PTR(redcBufferLast, _Name);
    REDC_BUFFER_TERMINATE(redcBufferLast);
    redcBufferLength++;
    return 1;
}

REDC_INT redc__buffer_push_func_data(REDC_FUNCTION_DATA *_Value) {
    if (!REDC_BUFFER_NEW_NODE(redcBufferLast->Next)) return 0;
    REDC_JUMP(redcBufferLast);
    REDC_BUFFER_DATA(redcBufferLast, Func, _Value->ptr);
    REDC_BUFFER_PTR(redcBufferLast, _Value->name);
    REDC_BUFFER_TERMINATE(redcBufferLast);
    redcBufferLength++;
    return 1;
}

REDC_FNV redc__str_to_fnv(const REDC_CHAR * _String) {
    if (!_String) return 0;
    REDC_FNV ret = REDC_FNV_OFFSET;
    REDC_CHAR * ptr = (REDC_CHAR *)_String;
    do {
        ret *= REDC_FNV_PRIME;
        ret ^= *ptr;
    } while (*++ptr);
    return ret;
}

// One simple interface to push to the redcBuffer
#define REDC_BUFFER_PUSH(_DType) redcBufferLength++, ((redcBufferLast = (redcBufferLast->Next = REDC_BUFFER_NEW_NODE()))->Next = NULL), (redcBufferLast->__ptr = NULL), (redcBufferLast->DType = _DType), redcBufferLast->DValue._DType =


/* Deletion Functions */

REDC_INT redcBufferDelete(struct redcBufferNode * _Buffer, REDC_INT _Index) {
    if (_Index - 1 >= redcBufferLength) {
        printf( "segmentation fault\t\t(index does not exist)\n" );
        exit(-1);
    }
    
    struct redcBufferNode * _A = _Buffer;
    
    while (_Index--) {
        if (_Buffer->Next || !_Index) (_A = _Buffer), _Buffer = _Buffer->Next;
        else return 0;
    }

    struct redcBufferNode * _B = _Buffer->Next;
    redcBufferLength--;
    if (_Buffer) free(_Buffer);
    _A->Next = _B;
    return 1;
}

REDC_INT redcBufferDeleteRange(struct redcBufferNode * _Buffer, REDC_INT _Index, REDC_INT _Count) {
    if (_Count && (_Index + _Count - 2 >= redcBufferLength)) {
        printf( "segmentation fault\t\t(index does not exist)\n" );
        exit(-1);
    }

    struct redcBufferNode * _A = _Buffer;
    struct redcBufferNode * _B;

    while (_Index--) {
        if (_Buffer->Next || !_Index) (_A = _Buffer), _Buffer = _Buffer->Next;
        else return 0;
    }

    while (_Count--) {
        if (_Buffer->Next || !_Count) (_B = _Buffer), _Buffer = _Buffer->Next;
        else return 0;
        redcBufferLength--;
        if (_B) free(_B);
    }
    
    _A->Next = _Buffer;
    return 1;
}

// One simple interface to delete single or ranged index(es)

#define REDC__buffer_delete_overload_ redcBufferDelete
#define REDC__buffer_delete_overload_0 redcBufferDeleteRange
#define REDC_BUFFER_DELETE(_Index, ...) REDC__merge_token_2(REDC__buffer_delete_overload_, __VA_OPT__(0))(redcBuffer, _Index + 1 __VA_OPT__(, __VA_ARGS__))


/* If stdio.h is included */

#if defined _STDIO_H

#define REDC_BG_RGB_STRING(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m"
#define REDC_BG_G_STRING(w) "\033[48;2;" #w ";" #w ";" #w "m"
#define REDC_BG_CLEAR_STRING "\033[49m"

#define REDC_BG_RGB(r, g, b) printf(REDC_BG_RGB_STRING(r, g, b))
#define REDC_BG_G(w) printf(REDC_BG_G_STRING(w))
#define REDC_BG_CLEAR printf(REDC_BG_CLEAR_STRING)

#define REDC__bg_overload_ REDC_BG_G
#define REDC__bg_overload_0 REDC_BG_RGB
#define REDC_BG(r, ...) REDC__merge_token_2(REDC__bg_overload_, __VA_OPT__(0))(r __VA_OPT__(, __VA_ARGS__))

/* Display function(s) */

void redcShowBufferNode(struct redcBufferNode * _Node);

enum { Dark, Light } redc__lineshade;
REDC_INT redc__lineno = 0;

#define REDC_RESET_LINENO redc__lineno = 0
#define REDC_NO_LINENO redc__lineno = -1
#define REDC_COLOR_NUMBERS "\033[38;2;150;75;200m"
#define REDC_COLOR_STRINGS "\033[38;2;225;150;100m"
#define REDC_COLOR_DTYPES "\033[38;2;100;150;200m"
#define REDC_COLOR_NAMES "\033[38;2;170;240;100m"
#define REDC_COLOR_WHITE "\033[38;2;250;250;250m"
#define REDC_COLOR_NONE "\033[39m"

void redcShowBufferNode(struct redcBufferNode * _Node) {
    
    REDC_INT redc_i = 0;
    
    if (redc__lineshade == Light) REDC_BG_G(30); else REDC_BG_G(40);
    redc_i += (redc__lineno == -1) ? printf("      ?") : printf(" %llu", lluof(redc__lineno));
    redc_i += printf("%*c ", intof(12 - redc_i), ' ');
    
    if (redc__lineshade == Light) REDC_BG_G(35); else REDC_BG_G(45);
    redc_i += printf(" %s", REDC_BUFFER_DTYPE_NAME(_Node->DType) );
    redc_i += printf("%*c ", intof(20 - redc_i), ' ');
    
    REDC_BG_CLEAR;
    if (_Node->DType == Char) redc_i += printf(         REDC_COLOR_STRINGS " '%c'", _Node->DValue.Char);
    else if (_Node->DType == Int) redc_i += printf(     REDC_COLOR_NUMBERS " %lld", lluof(_Node->DValue.Int));
    else if (_Node->DType == Float) redc_i += printf(   REDC_COLOR_NUMBERS " %f", dblof(_Node->DValue.Float));
    else if (_Node->DType == Fnv) redc_i += printf(     REDC_COLOR_NUMBERS " %lu", _Node->DValue.Fnv);
    else if (_Node->DType == Str) redc_i += printf(     REDC_COLOR_STRINGS " \"%s\"", _Node->DValue.Str);

    else if (_Node->DType == Func) redc_i += printf(
        REDC_COLOR_DTYPES " void " REDC_COLOR_NAMES "%s" REDC_COLOR_WHITE "(" REDC_COLOR_DTYPES "void" REDC_COLOR_WHITE ");", _Node->__ptr ? (REDC_CHAR *)_Node->__ptr : (REDC_CHAR *)"?"
    );
    
    printf(REDC_COLOR_NONE);
    
    if (redc__lineshade == Light) redc__lineshade = Dark; else redc__lineshade = Light;
    
    putchar('\n');
    redc__lineno++;


}

#define REDC_BUFFER_SHOW redcShowBuffer(redcBuffer->Next)

void redcShowBuffer(struct redcBufferNode * _Buffer) {

    REDC_INT redc_i = 0;

    redc_i += printf(" #");
    redc_i += printf("%*c ", intof(12 - redc_i), ' ');

    redc_i += printf(" DType");
    redc_i += printf("%*c ", intof(20 - redc_i), ' ');

    redc_i += printf(" DValue");

    putchar('\n');

    REDC_RESET_LINENO;
    
    if (!_Buffer) {
        if (redc__lineshade == Light)
        printf( REDC_BG_G_STRING(30) " \033[3mnull        " REDC_BG_G_STRING(35) " null   " REDC_BG_CLEAR_STRING " null\033[0m\n");
        else
        printf( REDC_BG_G_STRING(40) " \033[3mnull        " REDC_BG_G_STRING(45) " null   " REDC_BG_CLEAR_STRING " null\033[0m\n");
    }

    else while (_Buffer) {

        redcShowBufferNode(_Buffer);

        _Buffer = _Buffer->Next;


    }

    REDC_NO_LINENO;

}

#endif

#undef REDC_IMPLEMENT
#endif



#if defined (REDC_BUFFER_NAMESPACE)

#define try REDC_TRY

#define push REDC_BUFFER_PUSH
#define pop REDC_BUFFER_DELETE
#define show_buffers REDC_BUFFER_SHOW
#define def REDC_BUFFER_FUNCTION
#define push_func redcBufferPush_Func

#endif

#endif