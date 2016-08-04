#ifndef PUBLIC_ZF_H_INCLUDE
#define PUBLIC_ZF_H_INCLUDE
#include <stdio.h>

typedef struct ZF_Interpreter_tag ZF_Interpreter;

ZF_Interpreter *ZF_create_interpreter(void);
void ZF_compile(ZF_Interpreter *interpreter, FILE *fp);
void ZF_interpreter(ZF_Interpreter *interpreter);
void ZF_dispose_interpreter(ZF_Interpreter *interpreter);

#endif /* PUBLIC_ZF_H_INCLUDE */