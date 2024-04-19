# the name of the output
CO=excel-paper

#CHANGED_FILES_OTHER =literature.bib
CHANGED_FILES_OTHER+=$(wildcard figs/*.pdf) $(wildcard *.bib) $(wildcard *.bbl) $(wildcard *.sty) $(wildcard *.bst)

# List of TeX files
#TEX_FILES=$(wildcard *.tex)

###############################################################################
#                                 Rules                                       #
###############################################################################

.PHONY: all clean

all: $(CO).pdf

$(CO).pdf: $(CO).tex $(CHANGED_FILES_OTHER)
#	pdflatex -synctex=1  $(CO)
#	bibtex $(CO)
#	pdflatex $(CO)
#	pdflatex $(CO)
	rubber --unsafe --synctex --short --pdf $<
#	rubber-info --errors $<
#	rubber-info --check $<

edit:
	code $(CO).tex *.bib

open:
	xdg-open $(CO).pdf

clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out $(CO).lof $(CO).pdf *~ *rubbercache* *synctex* *.fdb_latexmk *.fls
