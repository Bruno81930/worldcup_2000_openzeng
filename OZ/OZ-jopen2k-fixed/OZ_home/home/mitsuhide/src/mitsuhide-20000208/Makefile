TARGETS 	= lib/libYowAI.a bin/mitsuhide

TARGET : $(TARGETS)

lib/libYowAI.a:
	@(cd src ; make)
	cp src/*.h include

bin/mitsuhide:
	@(cd mitsuhide ; make)
	mv mitsuhide/mitsuhide bin/mitsuhide

clean :
	@(cd src ; make clean)
	@(cd mitsuhide ; make clean)
	rm -f include/*.h lib/* bin/mitsuhide *core *bak *~
