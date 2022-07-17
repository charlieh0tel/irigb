main: main.cc IrigBEncoder.o

IrigBEncoder.o: IrigBEncoder.cc IrigBEncoder.h

style:
	clang-format --style="{BasedOnStyle: Google, ColumnLimit: 120}" -i *.cc *.h
.PHONY: style

clean:
	-rm -f main
.PHONY: clean
