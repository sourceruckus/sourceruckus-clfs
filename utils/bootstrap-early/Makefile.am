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
