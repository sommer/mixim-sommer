.PHONY: all makefiles clean cleanall doxy

# if out/config.py exists, we can also create command line scripts for running simulations
ADDL_TARGETS =
ifeq ($(wildcard out/config.py),)
else
    ADDL_TARGETS += run debug memcheck
endif

# default target
all: src/Makefile $(ADDL_TARGETS)
	@cd src && $(MAKE)

# command line scripts
run debug memcheck: % : src/scripts/%.in.py out/config.py
	@echo "Creating script \"./$@\""
	@head -n1 "$<" > "$@"
	@cat out/config.py >> "$@"
	@tail -n+2 "$<" >> "$@"
	@chmod a+x "$@"

# legacy
makefiles:
	@echo
	@echo '====================================================================='
	@echo 'Warning: make makefiles has been deprecated in favor of ./configure'
	@echo '====================================================================='
	@echo
	./configure
	@echo
	@echo '====================================================================='
	@echo 'Warning: make makefiles has been deprecated in favor of ./configure'
	@echo '====================================================================='
	@echo

clean: src/Makefile
	cd src && $(MAKE) clean
	rm -f run debug memcheck
	rm -rf src/protobuf

cleanall: src/Makefile
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile
	rm -f run debug memcheck

src/Makefile:
	@echo
	@echo '====================================================================='
	@echo '$@ does not exist.'
	@echo 'Please run "./configure" or use the OMNeT++ IDE to generate it.'
	@echo '====================================================================='
	@echo
	@exit 1

out/config.py:
	@echo
	@echo '====================================================================='
	@echo '$@ does not exist.'
	@echo 'Please run "./configure" to generate it.'
	@echo '====================================================================='
	@echo
	@exit 1

# autogenerated documentation
doxy:
	doxygen doxy.cfg

doxyshow: doxy
	xdg-open doc/doxy/index.html

