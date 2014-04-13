EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=go2
EXTRA_DIST+=config
EXTRA_DIST+=patches


# NOTE: Each individual package being built just simply does a re-install
#       if .build dir already exists.
bootstrap-local:
	env -i ./go

clean-local:
	rm -rf .build
