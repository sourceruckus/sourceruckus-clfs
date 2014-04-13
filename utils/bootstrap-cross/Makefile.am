EXTRA_DIST=
EXTRA_DIST+=README
EXTRA_DIST+=go
EXTRA_DIST+=patches

bootstrap-local-xen:
	pushd $(top_srcdir) && \
	mv config.status config.status.bak && \
	./configure --with-arch=x86_64 --disable-multilib \
	    --with-stage1-xen-only && \
	make bootstrap && \
	make clean && \
	mv config.status.bak config.status && \
	./config.status && \
	popd



# NOTE: Each individual package being built just simply does a re-install
#       if .build dir already exists.
bootstrap-local:
	env -i ./go

clean-local:
	rm -rf .build
