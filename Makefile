
all: lint_conf

lint_conf: lint_conf.cpp
	g++ -Wall -o lint_conf lint_conf.cpp

vimtest: lint_conf
	./lint_conf < example.ios
	./lint_conf example.ios

clean:
	rm -f lint_conf

