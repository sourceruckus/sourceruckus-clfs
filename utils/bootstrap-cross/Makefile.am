EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=patches

.PHONY: bootstrap-local-xen
bootstrap-local-xen:
	pushd $(top_srcdir) && \
	grep -m1 "./configure" config.log | sed 's|.*configure ||' \
	    > .previous_flags && \
	./configure --with-arch=x86_64 --disable-multilib \
	    --with-stage1-xen-only --disable-install && \
	make bootstrap && \
	make clean && \
	./configure `cat .previous_flags` && \
	rm .previous_flags && \
	popd


# Add the xen hook as a dependent of the local hook if needed
if NEED_CROSS_COMPILED_XEN
bootstrap-local: bootstrap-local-xen
else
bootstrap-local:
endif
	env -i ./go

clean-local:
	rm -rf .build
