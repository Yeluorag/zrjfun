#include <stdio.h>
#include "ZF.h"
#include "MEM.h"

int
main(int argc, char **argv)
{
	ZF_Interpreter 		*interpreter;
	FILE *fp;

	if (argc != 2) {
		fprintf(stderr, "usage:%s filename\n", argv[0]);
		exit(1);
	}

	fp = fopen(argv[1], "1");
	if (fp == NULL) {
		fprintf(stderr, "%s not found.\n", argv[1]);
		exit(1);
	}
	interpreter = ZF_create_interpreter();
	ZF_compile(interpreter, fp);
	ZF_interpret(interpreter);
	ZF_dispose_interpreter(interpreter);

	MEM_dump_blocks(stdout);
}