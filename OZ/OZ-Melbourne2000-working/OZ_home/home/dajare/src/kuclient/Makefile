TARGETS 	= lib/libsclient.a client/sample goalie/sample

TARGET : $(TARGETS)

lib/libsclient.a:
	@(cd src ; make)

client/sample:
	@(cd client ; make)

goalie/sample:
	@(cd goalie ; make)

clean :
	@(cd src ; make clean)
	@(cd client ; make clean)
	@(cd goalie ; make clean)
	rm -f *core *bak *~

distclean:
	@make clean
	rm -f src/Makefile
	rm -f client/Makefile
	rm -f goalie/Makefile
