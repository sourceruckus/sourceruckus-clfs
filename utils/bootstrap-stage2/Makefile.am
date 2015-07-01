EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=init

# FIXME: is this even needed?

# NOTE: Each individual package being built just simply does a re-install
#       if .build dir already exists.
#bootstrap-local:
#	@echo bootstrapping in bootstrap-stage2
#	env -i ruckus_srcdir=$(abs_top_srcdir) ruckus_builddir=$(abs_top_builddir) $(srcdir)/go

clean-local:
	rm -rf $(builddir)/.build
