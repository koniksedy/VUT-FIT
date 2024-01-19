CO=sn-article

edit:
	code *.tex *.bib

open:
	xdg-open $(CO).pdf

normostrany:
	echo "scale=2; `detex -n $(CO).tex | sed s/"^ *"/""/ | sed s/"^	*"/""/ | wc -c`/1800;" | bc
