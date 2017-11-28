# COMPILER=/opt/microchip/mplabc18/v3.40/bin/mcc18
COMPILER=gcc
# linker libraries
LIBS=
# What you want the executable to be named
EXE_NAME=generic
OPTIMIZE=
WARNINGS=

# header files to be accounted for
INCLUDES=board.h bool.h comms.h lcd.h targeting.h
# .c files
FILES=board lcd


%.o: %.c $(INCLUDES)
	$(COMPILER) $(WARNINGS) $(OPTIMIZE) $^ $(LIBS)

main: main.o $(addsuffix .o,$(FILES))
	$(COMPILER) $(WARNINGS) -o $(EXE_NAME) $^ $(LIBS)

board_test: board_test.c board.c board.h bool.h
	$(COMPILER) $(WARNINGS) -o board_testing $^ $(LIBS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(EXE_NAME)
	rm -f board_testing
