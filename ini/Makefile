# $Id: makefile
FILES=IMemoryPool.c IObject.c IValue.c IKeyValue.c ISection.c INIEnvironment.c IBuffer.c ILexer.c IEval.c
#-Wno-incompatible-pointer-types-discards-qualifiers
WARNS=-Wno-implicit-int
EXT=-O2 -g
CMPILER=gcc

all:
	bash format.sh
	$(CMPILER) ISample.c -o si3_tests $(EXT) $(FILES) $(WARNS)
