all: *.a
	g++ fs.a -o FILESYSTEM

*.a: *.o *.h
	ar rvs fs.a *.o *.h

*.o: *.cpp
	g++ -c *.cpp

clean:
	rm *.o
