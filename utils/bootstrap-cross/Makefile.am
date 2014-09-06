EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=patches

.PHONY: bootstrap-local-xen
# FIXME: does mv .build to .build-detour hide the hypervisor from us...?
bootstrap-local-xen:
	pushd $(top_srcdir) && \
	grep -m1 "./configure" config.log | sed 's|.*configure ||' \
	    > .previous_flags && \
	./configure `cat .previous_flags` \
	    --with-arch=x86_64 --disable-multilib --with-stage1-xen-only && \
	make bootstrap && \
	rm -rf $(CLFS)/{tools,cross-tools} && \
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
	rm -rf .build
	rm -rf .build-detour
