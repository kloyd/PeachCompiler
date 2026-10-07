OBJECTS= ./build/compiler.o \
		./build/cprocess.o \
		./build/helpers/vector.o \
		./build/helpers/buffer.o \
		./build/lex_process.o \
		./build/token.o \
		./build/lexer.o 


INCLUDES = -I./

all: ${OBJECTS}
	gcc main.c ${INCLUDES} ${OBJECTS} -g -o ./main

./build/compiler.o: ./compiler.c 
	gcc ./compiler.c -o ./build/compiler.o -g -c 

./build/lex_process.o: ./lex_process.c 
	gcc ./lex_process.c -o ./build/lex_process.o -g -c 

./build/lexer.o: ./lexer.c 
	gcc ./lexer.c -o ./build/lexer.o -g -c 

./build/token.o: ./token.c 
	gcc ./token.c -o ./build/token.o -g -c 

./build/cprocess.o: ./cprocess.c 
	gcc ./cprocess.c -o ./build/cprocess.o -g -c 

./build/helpers/vector.o: ./helpers/vector.c 
	gcc ./helpers/vector.c -o ./build/helpers/vector.o -g -c 
	
./build/helpers/buffer.o: ./helpers/buffer.c 
	gcc ./helpers/buffer.c -o ./build/helpers/buffer.o -g -c 

clean:
	rm ./main
	rm -rf ${OBJECTS}