EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=functions
EXTRA_DIST+=go


# NOTE: Each individual package being built just simply does a re-install
#       if .build dir already exists.
#
# FIXME: True.  However, some packages' install targets may fail if files
#        already exist (e.g., pkg-config hard link)
#
#        i fixed my pkg-config...
bootstrap-local:
	env -i ./go


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
