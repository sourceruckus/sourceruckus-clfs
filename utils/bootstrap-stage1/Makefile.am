EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=go2
EXTRA_DIST+=config
EXTRA_DIST+=patches


# NOTE: This means we're IN THE MIDDLE of a Xen Detour (i.e., we've
#       already reconfigured to 64bit and built up the cross-compilation
#       toolchain).  In this case, we build during hostprep.
if STAGE1_XEN_ONLY
hostprep-local:
	env -i $(srcdir)/go
endif

# NOTE: Each individual package being built just simply does a re-install
#       if .build dir already exists.
bootstrap-local:
	env -i $(srcdir)/go

bootstrap-install-local:
	env -i $(srcdir)/go2

clean-local:
	rm -rf $(builddir)/.build
	rm -rf $(builddir)/.build-detour
