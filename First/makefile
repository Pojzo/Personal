pwcheck: pwcheck.c
	gcc -std=c99 -Wall -Wextra -Werror -o pwcheck pwcheck.c 

test: test.c
	gcc -std=c99 -o test test.c

test1:
	./pwcheck 1 1 <example.txt >output.txt && ./test test1.txt output.txt

test2: 
	./pwcheck 2 3 <example.txt >output.txt && ./test test2.txt output.txt

test3:
	./pwcheck 3 2 <example.txt >output.txt && ./test test3.txt output.txt

test4:
	./pwcheck 4 2 <example.txt >output.txt && ./test test4.txt output.txt

test5:
	./pwcheck 2 4 --stats <example.txt >output.txt && ./test test5.txt output.txt

clean:
	rm -f output.txt
