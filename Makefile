all:
	cd src && make

clean:
	cd src && make clean

test:
	cd t && make

archive: clean
	RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/repl-src-$$RELEASE.tar.gz"; \
	cd .. && \
	  tar czf archive/repl-src-$$RELEASE.tar.gz \
	  --exclude=\.git repl/

