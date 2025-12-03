# makefile for building the program. Each of these can be run from the command line like "make hello.out".
# "make clean" deletes the exectuable to build again 
# "make test" builds the main file and then runs the test script. This is what the autograder uses
# 
# Note to students: You dont need to fully understand this! 

# Source files - all modules are now compiled together
SOURCES = main.c resistor.c rc_circuit.c circuit_analyzer.c signal_generator.c ai_chat.c helpers.c

# Detect OS and set executable name
ifeq ($(OS),Windows_NT)
    EXECUTABLE = main.exe
    RM = del /Q
    NULL_DEVICE = nul
else
    EXECUTABLE = main.out
    RM = rm -f
    NULL_DEVICE = /dev/null
endif

main.out: $(EXECUTABLE)

$(EXECUTABLE):
	gcc $(SOURCES) -o $(EXECUTABLE) -lm -Wall -Wextra

clean:
	-$(RM) main.exe main.out 2>$(NULL_DEVICE)

test: clean $(EXECUTABLE)
	bash test.sh
