EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=patches

.PHONY: bootstrap-local-xen
bootstrap-local-xen:
	pushd $(top_srcdir) && \
	grep -m1 "./configure" config.log | sed 's|.*configure ||' \
	    > .previous_flags && \
	./configure --with-arch=x86_64 --disable-multilib --prefix=$(prefix) \
	    --with-stage1-xen-only --disable-install && \
	make bootstrap && \
	mv utils/bootstrap-cross/.build{,-detour} && \
	mv utils/bootstrap-stage1/.build{,-detour} && \
	./configure `cat .previous_flags` && \
	rm .previous_flags && \
	popd


# Add the xen hook as a dependent of the local hook if needed
if STAGE1_XEN_DETOUR
bootstrap-local: bootstrap-local-xen
else
bootstrap-local:
endif
	env -i ./go

clean-local:
	rm -rf .build*
