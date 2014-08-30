all:
	g++ -I../Includes -c *.cpp
	ar cr renderer.a *.o
	cp renderer.a ../bin

clean:
	rm -f *.o
	rm -f *.a
