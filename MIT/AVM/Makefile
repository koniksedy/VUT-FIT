# usage: make all
.PHONY: all
all: clean prezentace.pdf

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

open: prezentace.pdf
	xdg-open prezentace.pdf

edit:
	code slides_en.tex prezentace.tex

clean:
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.vrb *.pdf *.synctex.gz *.fdb_latexmk *.fls
