#ifndef PRIVATE_ZRJFUN_H_INCLUDE
#define PRIVATE_ZRJFUN_H_INCLUDE
#include <stdio.h>
#include "MEM.h"
#include "ZF.h"
#include "ZF_dev.h"

#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

#define MESSAGE_ARGUMENT_MAX	(256)
#define LINE_BUF_SIZE			(1024)

typedef enum {
	PARSE_ERR = 1,
	CHARACTER_INVALID_ERR,
	FUNCTION_MULTIPLE_DEFINE_ERR,
	COMPILE_ERROR_COUNT_PLUS_1
} CompileError;

typedef enum {
	VARIABLE_NOT_FOUND_ERR = 1,
	FUNCTION_NOT_FOUND_ERR,
	ARGUMENT_TOO_MANY_ERR,
	ARGUMENT_TOO_FEW_ERR,
	NOT_BOOLEAN_TYPE_ERR,
	MINUS_OPERAND_TYPE_ERR,
	BAD_OPERAND_TYPE_ERR,
	NOT_BOOLEAN_OPERATOR_ERR,
	FOPEN_ARGUMENT_TYPE_ERR,
	FCLOSE_ARGUMENT_TYPE_ERR,
	FGETS_ARGUMENT_TYPE_ERR,
	FPUTS_ARGUMENT_TYPE_ERR,
	NOT_NULL_OPERATOR_ERR,
	DIVISION_BY_ZERO_ERR,
	GLOBAL_VARIABLE_NOT_FOUND_ERR,
	GLOBAL_STATEMENT_IN_TOPLEVEL_ERR,
	BAD_OPERATOR_FOR_STRING_ERR,
	RUNTIME_ERROR_COUNT_PLUS_1
} RuntimeError;

typedef enum {
	INT_MESSAGE_ARGUMENT = 1,
	DOUBLE_MESSAGE_ARGUMENT,
	STRING_MESSAGE_ARGUMENT,
	CHARACTER_MESSAGE_ARGUMENT,
	POINTER_MESSAGE_ARGUMENT,
	MESSAGE_ARGUMENT_END
} MessageArgumentType;

typedef struct {
	char *format;
} MessageFormat;

typedef struct Expression_tag Expression;

typedef enum {
	BOOLEAN_EXPRESSION = 1,
	INT_EXPRESSION,
	DOUBLE_EXPRESSION,
	STRING_EXPRESSION,
	IDENTIFIER_EXPRESSION,
	ASSIGN_EXPRESSION,
	ADD_EXPRESSION,
	SUB_EXPRESSION,
	MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    EQ_EXPRESSION,
    NE_EXPRESSION,
    GT_EXPRESSION,
    GE_EXPRESSION,
    LT_EXPRESSION,
    LE_EXPRESSION,
    LOGICAL_AND_EXPRESSION,
    LOGICAL_OR_EXPRESSION,
    MINUS_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
    NULL_EXPRESSION,
    EXPRESSION_TYPE_COUNT_PLUS_1
} ExpressionType;

#define dkc_is_math_operator(operator) \
	((operator) == ADD_EXPRESSION) || (operator) == SUB_EXPRESSION\
	|| (operator) == MUL_EXPRESSION || (operator) == DIV_EXPRESSION\
	|| (operator) == MOD_EXPRESSION)

#define dkc_is_compare_operator(operator) \
  ((operator) == EQ_EXPRESSION || (operator) == NE_EXPRESSION\
   || (operator) == GT_EXPRESSION || (operator) == GE_EXPRESSION\
   || (operator) == LT_EXPRESSION || (operator) == LE_EXPRESSION)

#define dkc_is_logical_operator(operator) \
  ((operator) == LOGICAL_AND_EXPRESSION) || (operator) == LOGICAL_OR_EXPRESSION)

typedef struct ArgumentList_tag {
	Expression *expression;
	struct ArgumentList_tag *next;
} ArgumentList;

typedef struct {
	 char 		*variable;
	 Expression *operand;
} AssignExpression;     /* assign expr */

typedef struct {
	Expression *left;
	Expression *right;
} BinaryExpression;     /* binary expr */

typedef struct { 		/* func call */
	char 				*identifier;
	ArgumentList 		*argument;
} FunctionCallExpression;  

struct Expression_tag {
	ExpressionType type;
	int line_number;
	union {
		ZF_Boolean				boolean_value;
		int 					int_value;
		double 					double_value;
		char 					*string_value;
		char 					*identifier;
		AssignExpression    	assign_expression;
		BinaryExpression     	binary_expression;
		Expression  			*minus_expression;
		FunctionCallExpression  function_call_expression;
	} u;
};

typedef Statement_tag Statement;

typedef struct StatementList_tag {
	Statement 	*statement;
	struct StatementList_tag 	*next;
} StatementList;

typedef struct {		/* program block */
	StatementList 		*statement_list;
} Block;

typedef struct IdentifierList_tag {
	char 		*name;
	struct IdentifierList_tag	*next;
} IdentifierList;

typedef struct { 		/* **???** */
	IdentifierList 		*identifier_list;
} GlobalStatement;

typedef struct Elsif_tag {
	Expression  		*condition;
	Block 				*block;
	struct Elsif_tag 	*next;
} Elsif;

typedef struct {
	Expression 	*condition;
	Block 		*then_block;
	Elsif 		*elsif_list;
	Block 		*else_block;
} IfStatement;

typedef struct {
	Expression 	*condition;
	Block 		*block;
} WhileStatement;

typedef struct {
	Expression 	*init;
	Expression 	*condition;
	Expression 	*post;
	Block 		*block;
} ForStatement;

typedef struct {
	Expression 	*return_value;
} ReturnStatement;

typedef enum {
	EXPRESSION_STATEMENT = 1,
	GLOBAL_STATEMENT,
	IF_STATEMENT,
	WHILE_STATEMENT,
	FOR_STATEMENT,
	RETURN_STATEMENT,
	BREAK_STATEMENT,
	CONTINUE_STATEMENT,
	STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

struct Statement_tag {
	StatementType 		type;
	int 				line_number;
	union {
		Expression 		*expression_s;
		GlobalStatement global_s;
		IfStatement 	if_s;
		WhileStatement 	while_s;
		ForStatement 	for_s;
		ReturnStatement return_s;
	} u;
};

typedef sturct ParameterList_tag {
	char		*name;		/* variable name */
	struct ParameterList_tag *next; /* pointer for list */
} ParameterList;

typedef enum {
	ZRJFUN_FUNCTION_DEFINITION = 1,		/* ZF function */
	NATIVE_FUNCTION_DEFINITION			/* C  function */
} FunctionDefinitionType;

typedef struct FunctionDefinition_tag {
	char				*name;		/* function name */
	FunctionDefinitionType	type;	/* function type */
	union {
		struct {
			ParameterList	*parameter;	/* argu define */
			Block			*block;		/* function main body */
		} zrjfun_f;
		struct {
			ZF_NativeFunctionProc 	*proc;
		} native_f;
	} u;
	struct FunctionDefinition_tag	*next;	/* for list */
} FunctionDefinition;

typedef struct Variable_tag {
	char		*name;	/* varible name */
	ZF_Value	value;	/* varible value */
	struct Variable_tag	*next;	/* pointer to next variable */
} Variable;

typedef enum { 			/* what this statement do */
	NORMAL_STATEMENT_RESULT = 1,
	RETURN_STATEMENT_RESULT,
	BREAK_STATEMENT_RESULT,
	CONTINUE_STATEMENT_RESULT,
	STATEMENT_RESULT_TYPE_COUNT_PLUS_1
} StatementResultType;

typedef struct {
	StatementResultType type;
	union {
		ZF_Value 		return_value;
	} u;
} StatementResult;

typedef struct GlobalVariableRef_tag {
	Variable 	*variable;			/* variable list */
	sturct GlobalVariableRef_tag *next;
} GlobalVariableRef;

typedef struct {  	/* local variables in running func */
	Variable 	*variable;
	GlobalVariableRef 	*globale_variable;
} LocalEnvironment;

struct ZF_String_tag {
	int 		ref_count;
	char 		*string;
	ZF_Boolean 	is_literal;  /* 是否为字面常量 */
};

typedef struct {	/* string pool */
	ZF_String 	*strings;
} StringPool;

struct ZF_Interpreter_tag {  /* interpreter */
	MEM_Storage			interpreter_storage;
	MEM_Storage			execute_storage;
	Variable			*variable;
	FunctionDefinition	*function_list;		/* function define list */
	StatementList		*statement_list;	/* function statement list */
	int 				current_line_number;
};


/* create.c */
void zf_function_define(char *identifier, ParameterList *parameter_list,
						Block *block);
ParameterList *zf_create_parameter(char *identifier);
ParameterList *zf_chain_parameter(ParameterList *list,
									char *identifier);
ArgumentList *zf_create_argument_list(Expression *expression);
ArgumentList *zf_chain_argument_list(ArgumentList *list, Expression *expr);
StatementList *zf_create_statement_list(Statement *statement);
StatementList *zf_chain_statement_list(StatementList *list,
										Statement *statement);
Expression *zf_alloc_expression(ExpressionType type);
Expression *zf_create_assign_expression(char *variable,
											Expression *operand);
Expression *zf_create_binary_expression(ExpressionType operator,
										Expression *left,
										Expression *right);
Expression *zf_create_minus_expression(Expression *operand);
Expression *zf_create_identifier_expression(char *identifier);
Expression *zf_create_function_call_expression(char *func_name,
												ArgumentList *argument);
Expression *zf_create_boolean_expression(ZF_Boolean value);
Expression *zf_create_null_expression(void);
Statement *zf_create_global_identifier(char *identifier);
IdentifierList *zf_create_global_identifier(char *identifier);
IdentifierList *zf_chain_identifier(IdentifierList *list, char *identifier);
Statement *zf_create_if_statement(Expression *condition,
									Block *then_block, Elsif *elsif_list,
									Block *else_block);
Elsif *zf_chain_elsif_list(Elsif *list, Elsif *add);
Elsif *zf_create_elsif(Expression *expr, Block *block);
Statement *zf_create_while_statement(Expression *condition, Block *block);
Statement *zf_create_for_statement(Expression *init, Expression *cond,
									Expression *post, Block *block);
Block *zf_create_block(StatementlList *statement_list);
Statement *zf_create_expression_statement(Expression *expression);
Statement *zf_create_return_statement(Expression *expression);
Statement *zf_create_break_statement(void);
Statement *zf_create_continue_statement(void);

/* string.c */
char *zf_create_identifier(char *str);
void zf_open_string_literal(void);
void zf_add_string_literal(int letter);
void zf_reset_string_literal_buffer(void);
void *zf_close_string_literal(void);

/* execute.c */
StatementResult
zf_execute_statement_list(ZF_Interpreter *inter,
						LocalEnvironment *env, StatementList *list);

/* eval.c */
ZF_Value zf_eval_binary_expression(ZF_Interpreter *inter,
								LocalEnvironment *env,
								ExpressionType operator,
								Expression *left, Expression *right);
ZF_Value zf_eval_minus_expression(ZF_Interpreter *inter,
								LocalEnvironment *env, Expression *operand);
ZF_Value zf_eval_expression(ZF_Interpreter *inter,
						LocalEnvironment *env, Expression *expr);

/* string_pool.c */
ZF_String *zf_literal_to_zf_string(ZF_Interpreter *inter, char *str);
void zf_refer_string(ZF_String *str);
void zf_release_string(ZF_String *str);
ZF_String *zf_search_zf_string	(ZF_Interpreter inter, char *str);
ZF_String *zf_create_zrjfun_string(ZF_Interpreter *inter, char *str);								

/* util.c */
ZF_Interpreter *zf_get_current_interpreter(void);
void zf_set_current_interpreter(ZF_Interpreter *inter);
void *zf_malloc(size_t size);
void *zf_execute_malloc(ZF_Interpreter *inter, size_t size);
Variable *zf_search_local_variable(LocalEnvironment *env,
									char *identifier);
Variable *
zf_search_global_variable(ZF_Interpreter *inter, char *identifier);
void zf_add_local_variable(LocalEnvironment *env,
							char *identifier, ZF_Value *value);
ZF_NativeFucntionProc *
zf_search_native_function(ZF_Interpreter *inter, char *name);
FunctionDefinition *zf_search_function(char *name);
char *zf_get_operator_string(ExpressionType type);

/* error.c */
void zf_compile_error(CompileError id, ...);
void zf_runtime_error(int line_number, RuntimeError id, ...);

/* native.c */
CRB_Value crb_nv_print_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
CRB_Value crb_nv_fopen_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
CRB_Value crb_nv_fclose_proc(CRB_Interpreter *interpreter,
                             int arg_count, CRB_Value *args);
CRB_Value crb_nv_fgets_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
CRB_Value crb_nv_fputs_proc(CRB_Interpreter *interpreter,
                            int arg_count, CRB_Value *args);
void crb_add_std_fp(CRB_Interpreter *inter);

#endif /* PRIVATE_CROWBAR_H_INCLUDED */











