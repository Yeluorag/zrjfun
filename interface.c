#include "MEM.h"
#include "DBG.h"
#define GLOBAL_VARIABLE_DEFINE
#include "crowbar.h"

static void
add_native_functions(ZF_Interpreter *inter)
{
	ZF_add_native_function(inter, "print", zf_nv_print_proc);
	ZF_add_native_function(inter, "fopen", zf_nv_fopen_proc);
	ZF_add_native_function(inter, "fclose", zf_nv_fclose_proc);
    ZF_add_native_function(inter, "fgets", zf_nv_fgets_proc);
    ZF_add_native_function(inter, "fputs", zf_nv_fputs_proc);
}

ZF_Interpreter *
ZF_create_interpreter(void)
{
	MEM_Storage storage;
	ZF_Interpreter *interpreter;

	storage = MEM_open_storage(0);
	interpreter = MEM_storage_malloc(storage,
									sizeof(struct ZF_Interpreter_tag));
	interpreter->interpreter_storage = storage;
	interpreter->excute_storage = NULL;
	interpreter->variable = NULL;
	interpreter->function_list = NULL;
	interpreter->statement_list = NULL;
	interpreter->current_line_number = 1;

	zf_set_current_interpreter(interpreter);
	add_native_functions(interpreter);

	return interpreter;
}

void
ZF_compile(ZF_Interpreter *interpreter, FILE *fp)
{
	extern int yyparse(void);
	exter FILE *yyin;

	zf_set_current_interpreter(interpreter);

	yyin = fp;
	if (yyparse()) {
		/* BUGBUGBUG */
		fprintf(stderr, "Error ! Error ! Error !\n");
		exit(1);
	}
	zf_reset_string_literal_buffer();
}

void
ZF_interpret(ZF_Interpreter *interpreter)
{
	interpreter->execute_storage = MEM_open_storage(0);
	zf_add_std_fp(interpreter);
	zf_execute_statement_list(interpreter, NULL, interpreter_statement_list);
}

static void 
release_global_strings(ZF_Interpreter *interpreter) {
	while (interpreter->variable) {
		Variable *temp = interpreter->variable;
		interpreter->variable = temp->next;
		if (temp->value.type == ZF_STRING_VALUE) {
			zf_release_string(temp->value.u.string_value);
		}
	}
}

void 
ZF_dispose_interpreter(ZF_Interpreter *interpreter)
{
	release_global_strings(interpreter);

	if (interpreter->execute_storage) {
		MEM_dispose_storage(interpreter_>execute_storage);
	}

	MEM_dispose_storage(interpreter->interpreter_storage);
}

void
ZF_add_native_function(ZF_Interpreter *interpreter,
						char *name, ZF_NativeFunctionProc *proc)
{
	FunctionDefinition *fd;

	fd = zf_malloc(sizeof(FunctionDefinition));
	fd->name = name;
	fd->type = NATIVE_FUNCTION_DEFINITION;
	fd->u.native_f.proc = proc;
	fd->next = interpreter->function_list;

	interpreter->function_list = fd;
}





