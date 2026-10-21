
all:  ; echo "usage: make test -j2"
clean:;


tsts = $(wildcard *.mhd) $(wildcard *.py) $(wildcard *.sh)
# tsts =  throatTri.sh throatStar.sh


$(info tsts: ${tsts})

msSrc ?= $(abspath ../..)

USE_msTEST=1
include  ${msSrc}/script/Makefile.in
