doxygen
copy docs\LatexORMakefile docs\doxygen\latex
cd docs\doxygen\latex
del Makefile
rename LatexORMakefile Makefile
make 
cd ..\..\..
copy docs\doxygen\latex\refman.pdf .
del server-reference-manual.pdf
rename refman.pdf server-reference-manual.pdf