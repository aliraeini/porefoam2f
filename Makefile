
msSrc ?= ..

ifneq "${OPT}" ".exe"
all:  ; $(MAKE) -f  ${msSrc}/script/Makefile.in  recurseMake USE_msRecurse=1
else
all: ; @echo skipping ${CURDIR}
endif

clean:; $(MAKE) -f  ${msSrc}/script/Makefile.in  recurseClean USE_msRecurse=1


test: ;
	cd test2f     && $(MAKE) test
	cd test2fCrnr && $(MAKE) test
