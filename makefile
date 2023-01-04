main:main.o GA.o
	g++ -O3 main.o GA.o -o GAWSA.out

main.o:main.cpp
	g++ -c main.cpp

GA.o:GA.cpp
	g++ -c GA.cpp

clean:
	rm *.o GAWSA.out