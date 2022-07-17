main: main.cc IrigBFrame.o

IrigBFrame.o: IrigBFrame.cc IrigBFrame.h IrigSymbol.h IrigTime.h

style:
	clang-format --style="{BasedOnStyle: Google, ColumnLimit: 120}" -i *.cc *.h
.PHONY: style

clean:
	-rm -f main *.o
.PHONY: clean
