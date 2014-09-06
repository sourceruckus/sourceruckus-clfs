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
	rm -rf .build-detour
	rm -f .previous_flags

# FIXME: should we also clean these up?
#
#	rm -rf $(CLFS)/tools
#	rm -rf $(CLFS)/cross-tools
#
#        I'm thinking not, because we could accidentally toast already
#        installed stuff... although, once we're done, it'll be owned by
#        root so I won't be able to do it...
