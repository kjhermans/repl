all:
	echo "Do 'sudo make test'"

test:
	cd t && make

archive:
	RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/repl-src-$$RELEASE.tar.gz"; \
	cd .. && \
	  tar czf archive/repl-src-$$RELEASE.tar.gz \
	  --exclude=\.git repl/

