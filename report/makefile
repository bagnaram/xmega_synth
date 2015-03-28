# Makefile for LaTeX files

LATEX = pdflatex
BIBTEX  = bibtex
MAKEINDEX = makeindex
PDF = full_report


all: 
	$(LATEX) $(PDF).tex
	killall -HUP mupdf

bib:
	$(BIBTEX) $(PDF).aux

view:
	mupdf $(PDF).pdf

clean-all:
	rm -f *.dvi *.log *.bak *.aux *.bbl *.blg *.idx *.ps *.eps *.pdf *.toc *.out *~

clean:
	rm -f *.log *.bak *.aux *.bbl *.blg *.idx *.toc *.out *~
