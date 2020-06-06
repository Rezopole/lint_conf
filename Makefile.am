## {
## }


ACLOCAL_AMFLAGS = -I m4

vimtest: lint_conf
	./lint_conf < example.ios
	./lint_conf example.ios

bin_PROGRAMS = lint_conf
man1_MANS = lint_conf.1

lint_conf_SOURCES = lint_conf.cpp
lint_conf_CXXFLAGS = @CXXFLAGS@


clean-local:
	:

distclean-local:
	:
	rm -f m4/*.m4
	rm -f config.h.in~
	rm -rf debian
	rm -f Makefile
	rm -f lint-conf*.deb



