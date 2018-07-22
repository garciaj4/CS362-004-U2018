CFLAGS = -Wall -fpic -coverage -lm -std=c99
	
runTest: testme.c
		gcc -o myTest testme.c -g $(CFLAGS)
		./myTest &> randomtestresults.out
		gcov testme.c >> randomtestresults.out
		cat testme.c.gcov >> randomtestresults.out

clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so *.out myTest