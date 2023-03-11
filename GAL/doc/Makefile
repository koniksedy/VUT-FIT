# Makefile
# Makefile pro dokumentaci k dokumentaceu do předmětu GAL
# Autoři: Bc. Jan Bíl
#         Bc. Michal Šedý
# Poslední úprava: 30.10.2022

CO=dokumentace

all: $(CO).pdf

pdf: $(CO).pdf

$(CO).ps: $(CO).dvi
	dvips $(CO)

$(CO).pdf: clean
	pdflatex $(CO)
	-bibtex $(CO)
	pdflatex $(CO)
	pdflatex $(CO)

$(CO).dvi: $(CO).tex $(CO).bib
	latex $(CO)
	-bibtex $(CO)
	latex $(CO)
	latex $(CO)

clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out $(CO).lof $(CO).ptc
	rm -f $(CO).pdf
	rm -f *~
	rm -f $(CO).fdb_latexmk $(CO).synctex.gz $(CO).fls

rename:
	mv $(CO).tex $(NAME).tex
	mv $(CO)-kapitoly.tex $(NAME)-kapitoly.tex
	mv $(CO)-bibliografie.bib $(NAME)-bibliografie.bib
	mv $(CO)-prilohy.tex $(NAME)-prilohy.tex
	sed -i "s/$(CO)-kapitoly/$(NAME)-kapitoly/g" $(NAME).tex
	sed -i "s/$(CO)-bibliografie/$(NAME)-bibliografie/g" $(NAME).tex
	sed -i "s/$(CO)-prilohy/$(NAME)-prilohy/g" $(NAME).tex
	sed -i "s/$(CO)/$(NAME)/g" Makefile

# Pozor, vlna neresi vse (viz popis.txt)
vlna:
	vlna -l $(CO)-*.tex

# Spocita normostrany / Count of standard pages
normostrany:
	echo "scale=2; `detex -n $(CO)*.tex | sed s/"^ *"/""/ | sed s/"^	*"/""/ | wc -c`/1800;" | bc


edit:
	code $(CO)-kapitoly.tex

open:
	xdg-open $(CO).pdf
