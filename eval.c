 #include <math.h>
#include <string.h>
#include "MEM.h"
#include "DBG.h"
#include "zrjfun.h"

static ZF_Value
eval_boolean_expression(ZF_Boolean boolean_value)
{
    ZF_Value   v;

    v.type = ZF_BOOLEAN_VALUE;
    v.u.boolean_value = boolean_value;

    return v;
}

static ZF_Value
eval_int_expression(int int_value)
{
    ZF_Value   v;

    v.type = ZF_INT_VALUE;
    v.u.int_value = int_value;

    return v;
}

static ZF_Value
eval_double_expression(double double_value)
{
    ZF_Value   v;

    v.type = CRB_DOUBLE_VALUE;
    v.u.double_value = double_value;

    return v;
}

static ZF_Value
eval_string_expression(CRB_Interpreter *inter, char *string_value)
{
    CRB_Value   v;

    v.type = CRB_STRING_VALUE;
    v.u.string_value = crb_literal_to_crb_string(inter, string_value);

    return v;
}

static CRB_Value
eval_null_expression(void)
{
    CRB_Value   v;

    v.type = CRB_NULL_VALUE;

    return v;
}

static void
refer_if_string(CRB_Value *v)
{
    if (v->type == CRB_STRING_VALUE) {
        crb_refer_string(v->u.string_value);
    }
}

static void
release_if_string(CRB_Value *v)
{
    if (v->type == CRB_STRING_VALUE) {
        crb_release_string(v->u.string_value);
    }
}