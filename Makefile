a.out:  test.c list.h
	gcc -Wall $< -o $@

clean:
	rm a.out
