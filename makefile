default: basm bam

basm: dependencies/token.c dependencies/List/list.c dependencies/Errors/Errors.c Basm/*.c
	gcc $^ -g -o $@

bam: dependencies/Stack/stack.c dependencies/token.c Bam/*.c
	gcc $^ -g -o $@

test: default
	@echo "Running tests..."
	./basm Examples/factorial.asm
	./bam b.out
	./basm Examples/fibonacci.asm
	./bam b.out
	./basm Examples/rule110.asm
	./bam b.out

clean:
	rm -f basm bam