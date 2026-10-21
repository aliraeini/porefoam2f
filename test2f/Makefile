
all:  ; echo "usage: make test -j2"
clean:;

tstdata=$(wildcard *.py) $(wildcard *.mhd) ref
tsts = $(wildcard *.sh)
# tsts = voxcylinder.sh  voxcylinderFSF.sh
# tsts = voxcylCFMesh.sh  voxcylCFMeshFSF.sh

$(info tsts: ${tsts})

msSrc ?= $(abspath ../..)

USE_msTEST=1
include  ${msSrc}/script/Makefile.in
