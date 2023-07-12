# makefile for compilation
CO=2023-xsedym02

all: $(CO).pdf

$(CO).pdf: clean
	pdflatex $(CO)
	bibtex $(CO)
	pdflatex $(CO)
	pdflatex $(CO)

rename:
ifneq ("$(wildcard $(CO)-text.tex)","")
	sed -i "s/$(CO)/$(NAME)/g" $(CO)-text.tex
	mv $(CO)-text.tex $(NAME)-text.tex
endif
ifneq ("$(wildcard $(CO)-text-en.tex)","")
	sed -i "s/$(CO)/$(NAME)/g" $(CO)-text-en.tex
	mv $(CO)-text-en.tex $(NAME)-text-en.tex
endif
	sed -i "s/$(CO)/$(NAME)/g" $(CO)-bib.bib
	sed -i "s/$(CO)/$(NAME)/g" $(CO).tex
	mv $(CO).tex $(NAME).tex
	mv $(CO)-bib.bib $(NAME)-bib.bib
	sed -i "s/$(CO)/$(NAME)/g" Makefile

clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out $(CO).lof $(CO).ptc
	rm -f $(CO).pdf
	rm -f *~
	rm -f $(CO).fdb_latexmk
	rm -f $(CO).fls
	rm -f $(CO).synctex.gz

pack:
	tar czvf $(CO).tar.gz *.tex *.bib *.bst ./images/* $(CO).pdf Makefile

vlna:
	vlna -l *.tex

# Spocita normostrany / Count of standard pages
normostrany:
	echo "scale=2; `detex -n *.tex | wc -c`/1800;" | bc

open:
	xdg-open $(CO).pdf

edit:
	code *.tex *.bib

