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
bootstrap-local:
	[ ! -f .build/ALL_DONE ] && ( env -i ./go || exit 1 ) || echo nothing to build


clean-local:
	rm -rf .build
	rm -rf .build-detour
