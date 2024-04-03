lib = -fopenmp

task2_1: task2_1.c
		gcc -o $@ task2_1.c $(lib)

task2_2: task2_2.c
	gcc -o  $@ task2_2.c -lm $(lib) 

task2_3: task2_3.cpp
	g++ -o task3_guided task2_3.cpp $(lib) -DTYPEGUIDED
	g++ -o task3_static task2_3.cpp $(lib) -DTYPESTATIC
	g++ -o task3_dynamic task2_3.cpp $(lib) -DTYPEDYNAMIC
