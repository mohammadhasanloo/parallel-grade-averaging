all: meanCalculator.out class.out student.out course.out

meanCalculator.out: meanCalculator.o
	g++ meanCalculator.o -o meanCalculator.out

meanCalculator.o: meanCalculator.cpp meanCalculator.h
	g++ -c meanCalculator.cpp -o meanCalculator.o

class.out: class.o
	g++ class.o -o class.out

class.o: class.cpp class.h
	g++ -c class.cpp -o class.o

student.out: student.o
	g++ student.o -o student.out

student.o: student.cpp student.h
	g++ -c student.cpp -o student.o

course.out: course.o
	g++ course.o -o course.out

course.o: course.cpp course.h
	g++ -c course.cpp -o course.o

clean:
	rm -rf *.out
	rm -r -f *.o