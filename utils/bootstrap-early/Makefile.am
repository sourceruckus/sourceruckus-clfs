EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=functions
EXTRA_DIST+=go


# NOTE: Each individual package being built just simply does a re-install
#       if .build dir already exists.
#
# NOTE: True.  However, some packages' install targets may fail if
#       files already exist (e.g., pkg-config hard link).  I patched
#       my pkg-config tree for this.
#
# NOTE: To avoid further breakage, to save time, and to make sure we
#       don't mess up a perfectly fine build, I've rigged this up to
#       only build to completion a single time.
#
hostprep-local:
	[ -f $(builddir)/.build/ALL_DONE ] && echo nothing to build || \
	    env -i ruckus_srcdir=$(top_srcdir) ruckus_builddir=$(top_builddir) \
	    $(srcdir)/go || exit 1

clean-local:
	rm -rf $(builddir)/.build
	rm -rf $(builddir)/.build-detour
