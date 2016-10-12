#ifndef PUBLIC_ZF_DEV_H_INCLUDED
#define PUBLIC_ZF_DEV_H_INCLUDED
#include "ZF.h"

typedef enum {
	ZF_FALSE = 0,
	ZF_TRUE = 1
} ZF_Boolean;

typedef struct ZF_String_tag ZF_String;

typedef struct {
	char 		*name;
} ZF_NativePointerInfo;

typedef enum {
	ZF_BOOLEAN_VLAUE = 1,
	ZF_INT_VALUE,
	ZF_DOUBLE_VALUE,
	ZF_STRING_VALUE,
	ZF_NATIVE_POINTER_VALUE,		/* 原生指针型 */
	ZF_NULL_VALUE
} ZF_ValueType;

typedef struct {
	ZF_NativePointerInfo 	*info;
	void					*pointer;
} ZF_NativePointer;

typedef struct {
	ZF_ValueType 	type;
	union {
		ZF_Boolean 			boolean_value;
		int 				int_value;
		double 				double_value;
		ZF_String 			string_value;
		ZF_NativePointer 	native_pointer;
	} u;
} ZF_Value;

typedef ZF_Value ZF_NativeFunctionProc(ZF_Interpreter *interpreter,
									int arg_count, ZF_Value *args);

void ZF_add_native_function(ZF_Interpreter *interpreter,
									char *name, ZF_NativeFunctionProc *proc);
void ZF_add_global_variable(ZF_Interpreter *inter,
									char *identifier, ZF_Value *value);


#endif /* PUBLIC_ZF_DEV_H_INCLUDED */