pkgname=$(shell basename $(abs_srcdir))
sourcecode_dir=${top_srcdir}/src/${pkgname}
sourcecode=$(sourcecode_dir)/*.tar.*

dist_noinst_DATA=${sourcecode}

all:
	@echo pkg: ${pkgname}
	@echo sourcecode: ${sourcecode}
	@echo sourcecode_dir: ${sourcecode_dir}

${sourcecode}: ${sourcecode_dir}/Makefile
	make -C ${sourcecode_dir} dist

${sourcecode_dir}/Makefile:
	cd ${sourcecode_dir} && ./autogen.sh
