EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=patches

.PHONY: hostprep-local-64bit-detour
hostprep-local-64bit-detour:
	pushd $(top_srcdir) && \
	grep -m1 "./configure" config.log | sed 's|.*configure ||' \
	    > .previous_flags && \
	./configure `cat .previous_flags` \
	    --with-arch=x86_64 --disable-multilib --with-stage1-xen-only && \
	make hostprep && \
	rm -rf $(CLFS)/{tools,cross-tools} && \
	mv utils/bootstrap-cross/.build{,-detour} && \
	mv utils/bootstrap-stage1/.build{,-detour} && \
	./configure `cat .previous_flags` && \
	rm .previous_flags && \
	popd


# NOTE: If Xen is enabled, we build x86_64 cross-compilation toolchain in
#       hostprep (so we can cross-compile the 64bit hypervisor).  If we're
#       building for 32bit, though, we have to go on a reconfiguration
#       detour.
if XEN_ENABLED
if XEN_DETOUR
hostprep-local: hostprep-local-64bit-detour
else
hostprep-local:
	env -i ./go
endif
endif

# NOTE: If we've already built/installed the cross-compilation toolchain,
#       great, don't do it over again.
bootstrap-local:
	[ -f .build/ALL_DONE ] && echo nothing to build || env -i ./go || exit 1


clean-local:
	rm -rf .build
	rm -rf .build-detour
