# To build siletly: make -s
MAKE=make
# C compiler
COMPILER=gcc
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

# Analyzer
ADEPS=src/analyzer/analyzer.c src/analyzer/analyzer.h
bin/analyzer: src/analyzer/main.c $(ADEPS)
	$(COMPILER) -o $@ $< $(ADEPS) $(FLAGS)

# Main
MDEPS=src/main/read_file.c src/main/file_check.c src/main/main.h
bin/main: src/main/main.c $(MDEPS)
	$(COMPILER) -o $@ $< $(MDEPS) $(FLAGS)

# Report
RDEPS=src/report/result.c src/report/result.h
bin/report: src/report/main.c $(RDEPS)
	$(COMPILER) -o $@ $< $(RDEPS) $(FLAGS)
