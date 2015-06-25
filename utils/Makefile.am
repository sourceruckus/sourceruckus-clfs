SUBDIRS=

hostprep-local:
	$(INSTALL_DATA) ruckus-builder.env $(PREFIX_EARLY)/


if !HOSTPREP_ONLY

SUBDIRS+=bootstrap-early
SUBDIRS+=bootstrap-cross
SUBDIRS+=bootstrap-stage1
SUBDIRS+=bootstrap-stage2
#SUBDIRS+=build
#
# FIXME: We should remove the build dir and move it's NOTES into some
#        other file...

if STAGE2_XEN
# install domU config and kernel in PREFIX_EARLY so we can get at them w/
# CLFS_DEV and RUCKUS_DEV unmounted.
bootstrap-install-local:
	$(INSTALL_DATA) ruckus-builder.cfg \
	    $(PREFIX_EARLY)/etc/xen/ruckus-builder-$(CLFS_ARCH_STRING).cfg
	$(INSTALL_DATA) $(CLFS)/boot/* $(PREFIX_EARLY)/boot/$(CLFS_ARCH_STRING)

endif

endif
