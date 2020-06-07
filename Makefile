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
	@$(MAKE) bin/main
	@$(MAKE) bin/counter
	@$(MAKE) bin/report
	@echo "Files successfully built"

clean:
	@rm -rf bin/*
	@echo "Cleaned successfully"

# Analyzer
ADEPS=src/analyzer/analyzer.c src/analyzer/analyzer.h
# Main
MDEPS=src/main/main_functions.c src/main/main_functions.h
# Report
RDEPS=src/report/report.c src/report/report.h
# Counter
CDEPS=src/analyzer/counter/counter.c src/analyzer/counter/counter.h

bin/analyzer: src/analyzer/main.c $(ADEPS) $(MDEPS)
	$(COMPILER) -o $@ $< $(ADEPS) $(MDEPS) $(FLAGS)

bin/main: src/main/main.c $(MDEPS)
	$(COMPILER) -o $@ $< $(MDEPS) $(FLAGS)

bin/report: src/report/main.c $(RDEPS)
	$(COMPILER) -o $@ $< $(RDEPS) $(FLAGS)

bin/counter:src/analyzer/counter/main.c $(CDEPS)
	$(COMPILER) -o $@ $< $(CDEPS) $(FLAGS)
