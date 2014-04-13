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
	[ ! -f .build/ALL_DONE ] && env -i ./go || echo nothing to build


# NOTE: We remove the .build directory for the other bootstrap stages
#       during clean.  We don't want to do that here, though, because
#       we want to be able to configure 64bit, build through stage1
#       xen, make clean, configure 32bit, and make bootstrap w/out
#       rebuilding bootstrap-early.
#
# NOTE: Distclean is probably not the most correct place to remove
#       .build, but if we don't do it there and we don't do it in
#       clean, it'll never get done.
clean-local:
distclean-local:
	rm -rf .build
