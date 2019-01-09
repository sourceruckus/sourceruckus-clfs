SUBDIRS=

hostprep-local:
	$(INSTALL_DATA) $(builddir)/ruckus-builder.env $(PREFIX_HOSTPREP)/


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
# install domU config and kernel in PREFIX_HOSTPREP so we can get at them w/
# CLFS_DEV and RUCKUS_DEV unmounted.
bootstrap-install-local:
	$(INSTALL_DATA) $(builddir)/ruckus-builder.cfg \
	    $(PREFIX_HOSTPREP)/etc/xen/ruckus-builder-$(CLFS_ARCH_STRING).cfg
	$(MKDIR_P) $(PREFIX_HOSTPREP)/boot/$(CLFS_ARCH_STRING)
	$(INSTALL_DATA) $(PREFIX_TOOLS)/boot/* \
	    $(PREFIX_HOSTPREP)/boot/$(CLFS_ARCH_STRING)

endif

endif
