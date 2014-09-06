#!/bin/sh

# FIXME: probably shouldn't symlink... once we boot into the stage2
#        environment, the symlinks will be broken...
autoreconf -v --install || exit 1

if test -z "$NOCONFIGURE"; then
    ./configure "$@"
fi
