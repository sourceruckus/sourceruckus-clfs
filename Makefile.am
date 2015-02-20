SUBDIRS=
SUBDIRS+=packages
SUBDIRS+=utils

EXTRA_DIST=
EXTRA_DIST+=MIRRORS
EXTRA_DIST+=get_sources
EXTRA_DIST+=init_submodules


.PHONY: download
download:
	./init_submodules @INIT_SUBMOD_ARGS@
	./get_sources @GET_SOURCES_ARGS@

clean-local:
	rm -rf .build
	rm -f .previous_flags
