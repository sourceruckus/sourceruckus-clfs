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


# NOTE: XEN_DETOUR means we need to reconfigure to 64bit, make hostprep,
#       and reconfigure to 32bit.
if XEN_DETOUR
hostprep-local: hostprep-local-64bit-detour
endif

# NOTE: STAGE1_XEN_ONLY means we're IN THE MIDDLE of the Xen Detour
#       mentioned above, which means we're going to build bootstrap-cross
#       during hostprep.
if STAGE1_XEN_ONLY
hostprep-local:
	env -i ./go
endif


# NOTE: If we've already built/installed the cross-compilation toolchain,
#       great, don't do it over again.  This is important because we don't
#       want to force a rebuild of the cross toolchain if we're kicking
#       bootstrap back off after fixing a problem in bootstrap-stage1.
bootstrap-local:
	[ -f .build/ALL_DONE ] && echo nothing to build || env -i ./go || exit 1


clean-local:
	rm -rf .build
	rm -rf .build-detour
