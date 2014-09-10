EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=go2
EXTRA_DIST+=config
EXTRA_DIST+=patches


if STAGE1_XEN_ONLY
hostprep-local:
	env -i ./go
endif

# NOTE: Each individual package being built just simply does a re-install
#       if .build dir already exists.
bootstrap-local:
	env -i ./go

bootstrap-install-local:
	env -i ./go2

clean-local:
	rm -rf .build
	rm -rf .build-detour
