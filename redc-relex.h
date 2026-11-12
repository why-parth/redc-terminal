#if !defined _REDC_RELATION_EXPRESSION_H
#define _REDC_RELATION_EXPRESSION_H

#include "./redc-buffer.h"

// Register command() take() upto() till() call() ;
// each of these functions continue the state and unless register is called again, 
// the continues state is the absolute saved values in the registered commands.

// RelEx Token Types
enum redcRelExTType {
    Command,
    Take,  // == 7
    Upto,  // <= 5
    Till, // is int
    Call
};

#define REDC_COLOR_CONDITION "\033[38;2;240;80;40m"

REDC_CHAR * redc__relex_ttype_name[] = {
    REDC_COLOR_NAMES "command" REDC_COLOR_NONE,
    REDC_COLOR_CONDITION "take" REDC_COLOR_NONE,
    REDC_COLOR_CONDITION "upto" REDC_COLOR_NONE,
    REDC_COLOR_CONDITION "till" REDC_COLOR_NONE,
    REDC_COLOR_WHITE "call" REDC_COLOR_NONE
};

REDC_CHAR * redc__relex_info_color[] = { REDC_COLOR_NAMES, REDC_COLOR_DTYPES, REDC_COLOR_NUMBERS, REDC_COLOR_DTYPES, REDC_COLOR_NAMES };

#define REDC_RELEX_TTYPE_NAME(_TType) (redc__relex_ttype_name[_TType])
#define REDC_RELEX_INFO_COLOR(_TType) (redc__relex_info_color[_TType])

// RelEx Node : This makes up the full RelEx, each token is a link and an addition information (void *) can also be connected to it
struct redcRelExNode {
    enum redcRelExTType TType;
    struct redcRelExNode * Next;
    REDC_INT TData;
    REDC_CHAR * __t_data_name;
    REDC_FNV __t_name_fnv;
    REDC_INT __crt_flag;
    void * TPtr;
};

// RelEx List : This list stores all the RelEx
struct redcRelExListNode {
    struct redcRelExNode * RelEx;
    struct redcRelExListNode * Next;
};

REDC_INT redc__relex_global_crt_flag;

struct redcRelExListNode * redcRelExList;
struct redcRelExListNode * redcRelExListLast;
struct redcRelExNode * redcRelExLast;
REDC_INT redcRelExListLength;

// Interface
#define REDC_RELEX_NEW_NODE(...) (__VA_OPT__(__VA_ARGS__ =) (struct redcRelExNode *)malloc(sizeof(struct redcRelExNode)))
#define REDC_RELEX_PUSH(_TType, ...) ((redcRelExLast = (redcRelExLast->Next = REDC_RELEX_NEW_NODE()))->Next = NULL), (redcRelExLast->TData = lluof(__VA_ARGS__)), (redcRelExLast->__t_name_fnv = redc__str_to_fnv(__VA_OPT__(#__VA_ARGS__) "")),(redcRelExLast->__t_data_name = __VA_OPT__(#__VA_ARGS__) ""), (redcRelExLast->TType = _TType), redcRelExLast->TPtr

#define REDC_RELEX_SYMBOL(_TData, ...)  REDC_MERGE(redc__temp_, __LINE__); ((redcRelExLast = (redcRelExLast->Next = REDC_RELEX_NEW_NODE()))->Next = NULL), (redcRelExLast->TData = redc__str_to_fnv(__VA_OPT__(#_TData))), (redcRelExLast->__t_name_fnv = redc__str_to_fnv(__VA_OPT__(#_TData) "")),(redcRelExLast->__t_data_name = #_TData __VA_OPT__( " (" #__VA_ARGS__ ")") ""), (redcRelExLast->TType = Command), redcRelExLast->TPtr

#define REDC_RELEX_APPEND redcRelExListLength++, (redcRelExListLast->Next = (struct redcRelExListNode *)malloc(sizeof(struct redcRelExListNode))), (redcRelExListLast = redcRelExListLast->Next), ((redcRelExListLast->RelEx = REDC_RELEX_NEW_NODE())->Next = NULL), (redcRelExListLast->RelEx->__crt_flag = redc__relex_global_crt_flag), (redcRelExListLast->RelEx->TType = 0), (redcRelExListLast->RelEx->TData = 0), (redcRelExListLast->RelEx->TPtr = NULL), (redcRelExLast = redcRelExListLast->RelEx), redc__relex_global_crt_flag = 0



#define REDC_RELEX_TERMINATE(_Node) _Node->Next = NULL

REDC_INT redcRelExInit(void);

REDC_INT redcRelExInit(void) {

    if (redcRelExListLast = (redcRelExList = (struct redcRelExListNode *)malloc(sizeof(struct redcRelExListNode)))) {
        redcRelExList->Next = NULL;
        redcRelExListLength = 0;
    } else return 0;

    if (!(redcRelExList->RelEx = REDC_RELEX_NEW_NODE())) return 0;

    redcRelExListLast = redcRelExList;
    redcRelExLast = NULL;
    REDC_RELEX_TERMINATE(redcRelExListLast->RelEx);
    REDC_RELEX_TERMINATE(redcRelExListLast);

    redc__relex_global_crt_flag = 0;
    return 1;
}

// Display section

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

#define REDC_RELEX_NODE_STRING REDC_BG_G_STRING(50) " %s%s " REDC_COLOR_NONE REDC_BG_CLEAR_STRING
#define REDC_RELEX_NODE_FORMAT(_Node) redc__relex_info_color[_Node->TType], redc__relex_ttype_name[_Node->TType]
#define REDC_RELEX_NODE_FSTRING(_Node) REDC_RELEX_NODE_STRING, REDC_RELEX_NODE_FORMAT(_Node)
#define redcShowRelExNode(_Node) printf( REDC_RELEX_NODE_FSTRING(_Node) )

void redcShowRelEx(struct redcRelExNode * _Node);
void redcShowRelEx(struct redcRelExNode * _Node);

REDC_INT redcRelExIs_Str(const REDC_CHAR * _String, REDC_INT _Log);
REDC_INT redcRelExIs_Fnv(REDC_FNV _Fnv, REDC_INT _Log);

struct redcRelExNode * redcRelExGetNode(REDC_INT _Index);

void redcRelExSource(void);

#define REDC_SHOW_RELEX(_RelEx) redcShowRelEx(_RelEx->Next)
void redcShowRelEx(struct redcRelExNode * _Node) {

    if (redc__lineshade == Light) REDC_BG_G(30); else REDC_BG_G(35);
    
    while (_Node) {

        printf(" %s%s " REDC_COLOR_NONE, REDC_RELEX_NODE_FORMAT(_Node));
        
        if (_Node->TData) {
            printf("%s", REDC_RELEX_INFO_COLOR(_Node->TType));
            
            switch (_Node->TType) {

            
                case Command:
                printf("%s ", _Node->__t_data_name);
                break;

                case Take:
                case Till:
                printf("%s ", REDC_BUFFER_DTYPE_NAME(_Node->TData));
                break;
                
                case Upto:
                printf("%llu ", lluof(_Node->TData));
                break;
                
                case Call:
                printf("%s" REDC_COLOR_WHITE "() ", _Node->__t_data_name);
                break;

            }

            printf(REDC_COLOR_NONE);
        }
        
        REDC_JUMP(_Node);
        
    }

    REDC_BG_CLEAR;
    
    if (redc__lineshade == Light)  redc__lineshade = Dark; else redc__lineshade = Light;
}

#define REDC_SHOW_RELEX_LIST redcShowRelExList(redcRelExList->Next)
void redcShowRelExList(struct redcRelExListNode * _Node) {

    REDC_INT redc_i = 0;
    redc__lineno = 0;

    printf(" #            RelEx\n");

    if (!_Node) printf( REDC_BG_G_STRING(60) " \033[31;3mnull       \033[0m  \033[3mnull\033[0m\n");
    else while (_Node) {

        if (redc__lineshade == Light) REDC_BG_G(24); else REDC_BG_G(26);
        printf("\033[31;1m");
        redc_i = printf(" re-%llu", lluof(redc__lineno));
        REDC_COLOR_NONE;
        printf("\033[39;22m");
        
        printf("%*c" REDC_BG_CLEAR_STRING " ", intof(12 - redc_i), ' ');
        
        redcShowRelEx(_Node->RelEx->Next);
        
        REDC_JUMP(_Node);
        putchar('\n');
        redc__lineno++;
        
    }

}

#endif

REDC_INT redcRelExIs_Str(const REDC_CHAR * _String, REDC_INT _Log) {
    
    struct redcRelExListNode * _start = redcRelExList->Next;

    const REDC_CHAR * _Cmp = _start->RelEx->Next->__t_data_name;
    REDC_INT i = 0;

    REDC_INT ret = 0;

    if (_Log) printf("searching : %s\nin", _String);

    while (_start) {
        _Cmp = _start->RelEx->Next->__t_data_name;

        ret++;
        i = 0;
        for (; _String[i] && _Cmp[i]; i++) {
            if (_Cmp[i] != _String[i]) break;
        }

        if (_Cmp[i] == _String[i]) { if (_Log) printf("\t \033[3m-> \033[4m%s\033[24;3m (found at %llu)\033[0m\n", _Cmp, lluof(ret)); return ret; }
        else if (_Log) printf("\t    %s\n", _Cmp);

        _start = _start->Next;
    }

    if (_Log) printf("not found!\n");
    return 0;
}

REDC_INT redcRelExIs_Fnv(REDC_FNV _Fnv, REDC_INT _Log) {
    static struct redcRelExListNode * _start = NULL;
    static REDC_INT ret = 0;

    if (!ret || !_Fnv) { _start = redcRelExList->Next; ret = 0; }

    const REDC_CHAR * _Name;
    REDC_INT i = 0;
    REDC_FNV _Cmp;

    if (_Log) printf("searching an fnv :\nin");

    while (_start) {
        ret++;

        _Name = _start->RelEx->Next->__t_data_name;
        _Cmp = _start->RelEx->Next->__t_name_fnv;

        if (_Fnv == _Cmp) { if (_Log) printf("\t \033[3m-> \033[4m%s\033[24;3m (found at %llu)\033[0m\n", _Name, lluof(ret)); _start = _start->Next; return ret; }
        else if (_Log) printf("\t    %s\n", _Name);

        _start = _start->Next;
    }

    _start = redcRelExList->Next;
    ret = 0;

    if (_Log) printf("not found!\n");
    return 0;
}

#define REDC_RELEX_SEARCH(_Fnv) (redcRelExIs_Fnv(_Fnv, 0))
#define REDC_RELEX_FULL_SEARCH(_Fnv) (redcRelExIs_Fnv(0, 0), redcRelExIs_Fnv(_Fnv, 0))

struct redcRelExNode * redcRelExGetNode(REDC_INT _Index) {
    struct redcRelExListNode * ret = redcRelExList;
    while (_Index--) {
        ret = ret->Next;
        if (!ret) return NULL;
    }
    return ret->RelEx;
}

void redcRelExSource(void) {

    struct redcRelExListNode * _start = redcRelExList->Next;

    while (_start) {

        _start->RelEx->Next->TData = redc__str_to_fnv(_start->RelEx->Next->__t_data_name);
        
        _start = _start->Next;
    }

}


#if defined (REDC_RELEX_NAMESPACE)

#define Relex REDC_RELEX_APPEND; REDC_INT
#define Overload REDC_RELEX_APPEND;
#define command(_Cmd) _Cmd ; REDC_RELEX_PUSH(Command, _Cmd)
#define take(_DType) ; REDC_RELEX_PUSH(Take, _DType)
#define upto(_Count) ; REDC_RELEX_PUSH(Upto, _Count)
#define till(_DType) ; REDC_RELEX_PUSH(Till, _DType)

#define symbol(_TSymbol, ...) REDC_RELEX_SYMBOL(_TSymbol __VA_OPT__(, __VA_ARGS__))

#define REDC__call(_Function) ; REDC_RELEX_PUSH(Call, _Function)
#define call(_Function) REDC__call(REDC_MERGE(RBF_, _Function)) = REDC_MERGE(RBF_, _Function)

#define show_relex(_RelEx) REDC_SHOW_RELEX(_RelEx)
#define show_relex_list REDC_SHOW_RELEX_LIST

#define relex_search REDC_RELEX_SEARCH
#define relex_full_search REDC_RELEX_FULL_SEARCH

#endif



#endif