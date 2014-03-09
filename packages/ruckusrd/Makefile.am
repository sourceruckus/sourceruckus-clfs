pkgname=$(shell basename $(abs_srcdir))
sourcecode_dir=${top_srcdir}/src/${pkgname}
sourcecode=$(sourcecode_dir)/*.tar.*
dist_noinst_DATA=${sourcecode}

# FIXME: why can't i scarf this out of $sourcecode?
pkgvers=0.10.2

pkgrev=1

description=blah blah
extra_content=
features=

notes_template=supergeneric
configure_opts=
prefix=/usr
libdir=
extra_install=
extra_sections=



all:
	@echo pkg: ${pkgname}
	@echo sourcecode: ${sourcecode}
	@echo sourcecode_dir: ${sourcecode_dir}
	@echo pkgvers: ${pkgvers}
	@echo ${sourcecode} | sed 's|^${sourcecode_dir}/${pkgname}-\(.*\)\.tar\..*|\1|'

${sourcecode}: ${sourcecode_dir}/Makefile
	make -C ${sourcecode_dir} dist

${sourcecode_dir}/Makefile:
	cd ${sourcecode_dir} && ./autogen.sh
