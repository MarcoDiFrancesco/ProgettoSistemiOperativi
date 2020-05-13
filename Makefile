# To build siletly: make -s
MAKE=make -s
# C compilaton flags
FLAGS=-std=gnu90 -I.

default:
	@$(MAKE) help

help:
	@cat src/help.txt

build:
	@mkdir -p bin
	@$(MAKE) bin/analyzer
	@$(MAKE) bin/report
	@$(MAKE) bin/main
	@echo "Files successfully built"

clean:
	@rm -rf bin/*
	@echo "Cleaned successfully"

ADEPS=src/analyzer/test.c src/analyzer/test.h
bin/analyzer: src/analyzer/main.c $(ADEPS)
	gcc -o $@ $< $(ADEPS) $(FLAGS)

MDEPS=src/read_file/read_file.c src/read_file/read_file.h
bin/main: src/main/main.c $(MDEPS)
	gcc -o $@ $< $(MDEPS) $(FLAGS)

RDEPS=src/report/result.c src/report/result.h
bin/report: src/report/main.c $(RDEPS)
	gcc -o $@ $< $(RDEPS) $(FLAGS)
