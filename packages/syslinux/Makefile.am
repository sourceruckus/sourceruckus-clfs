pkgname=$(shell basename $(abs_srcdir))
sourcecode_dir=${top_srcdir}/src/${pkgname}
sourcecode=$(sourcecode_dir)/*.tar.*

dist_noinst_DATA=${sourcecode}

all:
	@echo pkg: ${pkgname}
	@echo sourcecode: ${sourcecode}
	@echo sourcecode_dir: ${sourcecode_dir}

# FIXME: The following targets would actually work if we had a Makefile in
#        src/syslinux that had a dist target... but I wasn't planning on
#        sticking Makefiles in those dirs...
#
# FIXME: Maybe we could conditionally include a makefile that has these
#        rules defined if sourcecode_dir DOESN'T have a SOURC_URL file...?
#
#${sourcecode}: ${sourcecode_dir}/Makefile
#	make -C ${sourcecode_dir} dist
#	echo pkg: ${pkgname}
#
#${sourcecode_dir}/Makefile:
#	cd ${sourcecode_dir} && ./autogen.sh
