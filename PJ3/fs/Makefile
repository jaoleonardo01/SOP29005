all: *.a
	g++ fs.a -o FILESYSTEM

*.a: *.o *.h
	ar rvs fs.a *.o *.h

*.o: *.cpp
	g++ -c -w -std=c++11 *.cpp

clean:
	rm *.o FILESYSTEM
