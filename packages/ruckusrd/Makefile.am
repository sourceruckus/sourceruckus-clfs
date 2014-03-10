pkgname=$(notdir $(abs_srcdir))
sourcecode_dir=$(top_srcdir)/src/$(pkgname)
sourcecode=$(wildcard $(sourcecode_dir)/*.tar.*)

dist_noinst_DATA=${sourcecode}

pkgvers=$(basename $(basename $(subst $(sourcecode_dir)/$(pkgname)-,,$(sourcecode))))

pkgrev=1

description=blah blah
extra_content=
features=

notes_template=supergeneric

# Ok, DO NOT put --prefix or --libdir in configure_opts.
#
# FIXME: should i check for this?
configure_opts=--foo
prefix=/usr
libdir=
extra_install=
extra_sections=


$(if $(findstring --prefix,$(configure_opts)),$(error CANNOT HAVE --prefix IN configure_opts.  USE prefix VARIABLE INSTEAD))


all:
	@echo pkg: ${pkgname}
	@echo sourcecode: ${sourcecode}
	@echo sourcecode_dir: ${sourcecode_dir}
	@echo pkgvers: ${pkgvers}

${sourcecode}: ${sourcecode_dir}/Makefile
	make -C ${sourcecode_dir} dist

${sourcecode_dir}/Makefile:
	cd ${sourcecode_dir} && ./autogen.sh
