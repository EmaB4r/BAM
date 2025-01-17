default:
	gcc dependencies/token.c dependencies/List/list.c dependencies/Errors/Errors.c Basm/*.c -g -o basm
	gcc dependencies/Stack/stack.c dependencies/token.c Bam/*.c -g -o bam