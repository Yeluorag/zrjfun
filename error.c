#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "MEM.h"
#include "DBG.h"
#include "crowbar.h"

extern char *yytext;
extern MessageFormat zf_compile_error_message_format[]; /* in error_message.c */
extern MessageFormat zf_runtime_error_message_format[];

typedef struct {
	char  		*string;
} VString;

void
zf_runtime_error(int line_number, RuntimeError id, ...)
{
	
}