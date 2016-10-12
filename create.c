
#include "MEM.h"
#include "DBG.h"
#include "zrjfun.h"

void
zf_function_define(char *identifier, ParameterList *parameter_list,
					Block *block)
{
	FunctionDefinition *f;
	ZF_Interpreter *inter;

	if (zf_search_function(identifier)) {
		zf_compile_error(FUNCTION_MULTIPLE_DEFINE_ERR,
						STRING_MESSAGE_ARGUMENT, "name", identifier,
						MESSAGE_ARGUMENT_END);
		return;
	}
	inter = zf_get_current_interpreter();

	f = zf_malloc(sizeof(FunctionDefinition));
	f->name = identifier;
	f->type = ZRJFUN_FUNCTION_DEFINITION;
	f->u.zrjfun_f.parameter = parameter_list;
	f->u.zrjfun_f.block = block;
	f->next = inter->function_list;
	inter->function_list = f;
}

ParameterList *
zf_create_parameter(char *identifier)
{
	ParameterList 		*p;

	p = zf_malloc(sizeof(ParameterList));
	p->name = identifier;
	p->next = NULL;

	return p;
}

ParameterList *
zf_chain_parameter(ParameterList *list, char *identifier)
{
	ParameterList *pos;

	for (pos = list; pos->next; pos = pos->next)
		;
	pos->next = zf_create_parameter(identifier);

	return list;
}

ArgumentList *
zf_create_argument_list(Expression *expression)
{
	ArgumentList *al;

	al = zf_malloc(sizeof(ArgumentList));
	al->expression = expression;
	al->next = NULL;

	return al;
}

ArgumentList *
zf_chain_argument_list(ArgumentList *list, Expression *expr)
{
	ArgumentList *pos;

	for (pos = list; pos->next; pos = pos->next)
		;
	pos->next =zf_create_argument_list(expr);

	return list;
}

StatementList *
zf_create_statement_list(Statement *statement)
{
	StatementList *sl;

	sl = zf_malloc(sizeof(StatementList));
	sl->statement = statement;
	sl->next = NULL;

	return sl;
}

StatementList *
zf_chain_statement_list(StatementList *list, Statement *statement)
{
	StatementList *pos;

	if (list == NULL)
		return zf_create_statement_list(statement);

	for (pos = list; pos->next; pos = pos->next)
		;
	pos->next = zf_create_statement_list(statement);

	return list;
}

Expression *
zf_alloc_expression(ExpressionType type)
{
	Expression *exp;

	exp = zf_malloc(sizeof(Expression));
	exp->type = type;
	exp->line_number = zf_get_current_interpreter()->current_line_number;

	return exp;
}

Expression *
zf_create_assign_expression(char *variable,  Expression *operand)
{
	Expression *exp;

	exp = zf_alloc_expression(ASSIGN_EXPRESSION);
	exp->u.assign_expression.variable = variable;
	exp->u.assign_expression.operand = operand;

	return exp;
}

static Expression
convert_value_to_expression(ZF_Value *v)
{
	Expression expr;

	if (v->type == ZF_INT_VALUE) {
		expr.type = INT_EXPRESSION;
		expr.u.int_value = v->u.int_value;
	} else if (v->type == ZF_DOUBLE_VALUE) {
		expr.type = DOUBLE_EXPRESSION;
		expr.u.double_value = v->u.double_value;
	} else {
		DBG_assert(v->type == ZF_BOOLEAN_VALUE,
					("v->type..%d\n", v->type));
		expr.type = BOOLEAN_EXPRESSION;
		expr.u.boolean_value = v->u.boolean_value;
	}
	return expr;
}

Expression *
zf_create_binary_expression(ExpressionType operator,
							Expression *left, Expression *right)
{
	if ((left->type == INT_EXPRESSION 
		|| left->type == DOUBLE_EXPRESSION)
		&& (right->type == INT_EXPRESSION
			|| right->type == DOUBLE_EXPRESSION)) {
		ZF_Value v;
		v = zf_eval_binary_expression(zf_get_current_interpreter(),
									NULL, operator, left, right);
		/* Overwriting left hand expression. */
		*left = convert_value_to_expression(&v);

		return left;		/* 常量折叠 - constant folding */
	} else {
		Expression *exp;
		exp = zf_alloc_expression(operator);
		exp->u.binary_expression.left = left;
		exp->u.binary_expression.right = right;
		return exp;
	}
}

Expression *
zf_create_minus_expression(Expression *operand)
{
	if (operand->type == INT_EXPRESSION
		|| operand->type == DOUBLE_EXPRESSION) {
		ZF_Value 		v;
		v = zf_eval_minus_expression(zf_get_current_interpreter(),
									NULL, operand);

		*operand = convert_value_to_expression(&v);
		return operand;
	} else {
		Expression 		*exp;
		exp = zf_alloc_expression(MINUS_EXPRESSION);
		exp->u.minus_expression = operand;
		return exp;
	}
}

Expression *
zf_create_identifier_expression(char *identifier)
{
	Expression *exp;

	exp = zf_alloc_expressioin(IDENTIFIER_EXPRESSION);
	exp->u.identifier = identifier;

	return exp;
}

Expression *
zf_create_function_call_expression(char *func_name, ArgumentList *argument)
{
	Expression *exp;

	exp = zf_alloc_expressioin(FUNCTION_CALL_EXPRESSION);
	exp->u.function_call_expression.identifier = func_name;
	exp->u.function_call_expression.argument = argument;

	return exp;
}

Expression *
zf_create_boolean_expression(ZF_Boolean value)
{
	Expression *exp;

	exp = zf_alloc_expressioin(BOOLEAN_EXPRESSION);
	exp->u.boolean_value = value;

	return exp;
}

Expression *
zf_create_null_expression(void)
{
	Expression *exp;

	exp = zf_alloc_expression(NULL_EXPRESSION);

	return exp;
}

static Statement *
alloc_statement(StatementType type)
{
	Statement *st;

	st = zf_malloc(sizeof(Statement));
	st->type = type;
	st->line_number = zf_get_current_interpreter()->current_line_number;

	return st;
}

Statement *
zf_create_global_statement(IdentifierList *identifier_list)
{
	Statement *st;

	st = alloc_statement(GLOBAL_STATEMENT);
	st->u.global_s.identifier_list = identifier_list;

	return st;
}

IdentifierList *
zf_create_global_identifier(char *identifier)
{
	IdentifierList *i_list;

	i_list = zf_malloc(sizeof(IdentifierList));
	i_list->name = identifier;
	i_list->next = NULL;

	return i_list;
}

IdentifierList *
zf_chain_identifier(IdentifierList *list, char *identifier)
{
	IdentifierList *pos;

	for (pos = list; pos->next; pos = pos->next)
		;
	pos->next = zf_create_global_identifier(identifier);

	return list;
}

Statement *
zf_create_if_statement(Expression *condition,
						Block *then_block, Elsif *elsif_list,
						Block *else_block)
{
	Statement *st;

	st = alloc_statement(IF_STATEMENT);
	st->u.if_s.condition = condition;
	st->u.if_s.then_block = then_block;
	st->u.if_s.elsif_list = elsif_list;
	st->u.if_s.else_block = else_block;

	return st;
}

Elsif *
zf_chain_elsif_list(Elsif *list, Elsif *add)
{
	Elsif *pos;

	for (pos = list; pos->next; pos = pos->next)
		;
	pos->next = add;

	return list;
}

Elsif *
zf_create_elsif(Expression *expr, Block *block)
{
	Elsif *ei;

	ei = zf_malloc(sizeof(Elsif));
	ei->condition = expr;
	ei->block = block;
	ei->next = NULL;

	return ei;
}

Statement *
zf_create_while_statement(Expression *condition, Block *block)
{
	Statement *st;

	st = alloc_statement(WHILE_STATEMENT);
	st->u.while_s.condition = condition;
	st->u.while_s.block = block;

	return st;
}

Statement *
zf_create_for_statement(Expression *init, Expression *cond,
						Expression *post, Block *block)
{
	Statement *st;
	st = alloc_statement(FOR_STATEMENT);
	st->u.for_s.init = init;
	st->u.for_s.condition = cond;
    st->u.for_s.post = post;
    st->u.for_s.block = block;

    return st;
}

Block *
zf_create_block(StatementList *statement_list)
{
	Block *block;

	block = zf_malloc(sizeof(Block));
	block->statement_list = statement_list;

	return block;
}

Statement *
zf_create_expression_statement(Expression *expression)
{
	Statement *st;

	st = alloc_statement(EXPRESSION_STATEMENT);
	st->u.expression_s = expression;

	return st;
}

Statement *
zf_create_return_statement(Expression *expression)
{
	Statement *st;

	st = alloc_statement(RETURN_STATEMENT);
	st->u.return_s.return_value = expression;

	return st;
}

Statement *zf_create_break_statement(void)
{
	return alloc_statement(BREAK_STATEMENT);
}

Statement *zf_create_continue_statement(void)
{
	return alloc_statement(CONTINUE_STATEMENT);
}












