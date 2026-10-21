

all: msApp

msSrc ?= $(abspath ../..)

USE_MINIFOAMX3=1
OFCs=$(shell find | grep -F .C | sed s:./::)
OFincs =   meshTools finiteVolume transportModels/incompressible transportModels
OFlibs =   meshTools finiteVolume incompressibleTransportModels interfaceProperties
msCFLAGS += -DNoRepository

# msCFLAGS += -I../interFacePropsBCs -I../interFacePropsBCs/alphaContactAngle
USE_msMAKE=1
include  ${msSrc}/script/Makefile.in
