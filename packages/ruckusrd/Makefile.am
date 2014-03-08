# FIXME: pkgname is blank... ${shell or ${exec or something like that...?
pkgname=${dirname ${srcdir}}


sourcecode_dir=${top_srcdir}/src/ruckusrd
sourcecode=${sourcecode_dir}/ruckusrd-0.10.2.tar.bz2

dist_noinst_DATA=${sourcecode}

${sourcecode}: ${sourcecode_dir}/Makefile
	make -C ${sourcecode_dir} dist
	echo pkg: ${pkgname}

${sourcecode_dir}/Makefile:
	cd ${sourcecode_dir} && ./autogen.sh
