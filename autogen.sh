#!/bin/sh

autoreconf -v --install --symlink || exit 1

if test -z "$NOCONFIGURE"; then
    ./configure "$@"
fi
