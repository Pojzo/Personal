pwcheck: pwcheck.c
	gcc -std=c99 -Wall -Wextra -Werror -o pwcheck pwcheck.c 

test: ./tests/test.c
	gcc -std=c99 -o ./tests/test ./tests/test.c
	
run: pwcheck.c
	./pwcheck 2 2 --stats

test1:
	@./pwcheck 1 1 <example.txt >output.txt 
	@./tests/test ./tests/test1.txt output.txt 1

test2: 
	@./pwcheck 2 3 <example.txt >output.txt
	@./tests/test ./tests/test2.txt output.txt 2 

test3:
	@./pwcheck 3 2 <example.txt >output.txt 
	@./tests/test ./tests/test3.txt output.txt 3

test4:
	@./pwcheck 4 2 <example.txt >output.txt 
	@./tests/test ./tests/test4.txt output.txt 4

test5:
	@./pwcheck 2 4 --stats <example.txt >output.txt
	@./tests//test ./tests/test5.txt output.txt 5

testall:
	echo Testing all inputs
	@make test1 
	@make test2 
	@make test3 
	@make test4 
	@make test5

clean:
	@rm -f output.txt

indent:
	indent -linux pwcheck.c 

