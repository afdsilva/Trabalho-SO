# Trabalho SO - Tabela Hash Concorrente
# André Felipe da Silva
# Matricula: 11107587
# e-mail: 	afdsilva@inf.ufpel.edu.br
# ultimo update: 08.01.2014

DATA=08.01.2014
TRABALHO=trabalho-so
EXECUTABLE=teste
BASE_FILENAME=$(TRABALHO)\[$(DATA)\]
REVISION=1

FLAGS=-lpthread
OBJECTS=Mutex.o CondVar.o Thread.o libhash.o teste.o

all: $(OBJECTS)
	g++ -Wall $(OBJECTS) -o $(EXECUTABLE) $(FLAGS)
teste2: Mutex.o CondVar.o Thread.o libhash.o teste2.o
	g++ -Wall Mutex.o CondVar.o Thread.o libhash.o teste2.o -o teste2 $(FLAGS)
Mutex.o: src/Mutex.h src/Mutex.cpp 
	g++ -c src/Mutex.cpp
CondVar.o: src/CondVar.h src/CondVar.cpp
	g++ -c src/CondVar.cpp
Thread.o: src/Thread.h src/Thread.cpp
	g++ -c src/Thread.cpp
libhash.o: src/libhash.h src/libhash.cpp
	g++ -c src/libhash.cpp
run:
	./$(EXECUTABLE)
teste.o: src/teste.cpp
	g++ -c src/teste.cpp
teste2.o: src/teste.cpp
	g++ -c src/teste2.cpp
clean:
ifneq ($(wildcard *.o),)
	rm *.o
endif
ifeq ($(wildcard $(EXECUTABLE)),$(EXECUTABLE))
	rm $(EXECUTABLE)
endif
