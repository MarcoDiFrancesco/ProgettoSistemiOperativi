FLAGS=-I.

help:
	@cat src/help.txt

build:
	@mkdir -p bin
	@make bin/analyzer
	@make bin/createfile
	@make bin/main
	@make bin/report
	@echo "Files susccessfully built"

clean:
	@# force option is used to fail silently, avoiding "No such file or directory"
	@rm -rf bin
	@echo "Cleaned susccessfully"

ADEPS=src/analyzer/test.c src/analyzer/test.h
bin/analyzer: src/analyzer/main.c $(ADEPS)
	gcc -o $@ $< $(ADEPS) $(FLAGS)

CDEPS=
bin/createfile: src/createfile/main.cpp $(CDEPS)
	g++ -o $@ $<

MDEPS=
bin/main: src/main/main.c $(MDEPS)
	gcc -o $@ $< $(MDEPS) $(FLAGS)

RDEPS=
bin/report: src/report/main.c $(RDEPS)
	gcc -o $@ $< $(RDEPS) $(FLAGS)
