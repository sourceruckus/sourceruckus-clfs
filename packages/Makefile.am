SUBDIRS =

# BEGIN CLFS Chapter 10 - Installing Basic System Software
#
# Deviations for CLFS will be noted appropriately.
#
SUBDIRS += man-pages
SUBDIRS += tzdata

# NOTE: glibc assumes ownership of all headers in /usr/include under the
#       assumption that they're the kernel headers previously installed in
#       bootstrap-stage2.  don't put anything before glibc that installs
#       files in /usr/include
#
# FIXME: double-check that man-pages and tzdata don't install files in
#        /usr/include
#
# NOTE: glibc has a post-install script that correlates w/ CLFS's
#       "Adjusting the Toolchain" step.  This will modify the specs for
#       /tools*/bin/gcc to compile against the newly installed glibc
#       instead of the bootstrap version in /tools*/lib.  Don't attempt to
#       build/install glibc outside the bootstrap environment.  Bad things
#       will happen.
#
SUBDIRS += glibc

SUBDIRS += m4
SUBDIRS += gmp
SUBDIRS += mpfr
SUBDIRS += mpc
SUBDIRS += isl
SUBDIRS += cloog
SUBDIRS += zlib
SUBDIRS += flex
SUBDIRS += bison
SUBDIRS += binutils
SUBDIRS += gcc

SUBDIRS += attr
SUBDIRS += acl

# bb
#
#SUBDIRS += sed

SUBDIRS += pkg-config # pkg-config-lite?
SUBDIRS += ncurses

# bb
#
#SUBDIRS += shadow
#SUBDIRS += util-linux

SUBDIRS += procps # procps-ng?

# bb
#
#SUBDIRS += e2fsprogs
SUBDIRS += libcap

# bb
#
#SUBDIRS += coreutils
SUBDIRS += iana-etc
SUBDIRS += libtool
SUBDIRS += iproute2

# bb
#
#SUBDIRS += bzip2

SUBDIRS += gdbm
SUBDIRS += perl
SUBDIRS += readline
SUBDIRS += autoconf
SUBDIRS += automake

# bb
#
#SUBDIRS += bash

SUBDIRS += bc

# bb
#
#SUBDIRS += diffutils

SUBDIRS += file
SUBDIRS += gawk

# bb
#
#SUBDIRS += findutils

SUBDIRS += gettext
SUBDIRS += gperf

# bb
#
#SUBDIRS += grep

SUBDIRS += groff
SUBDIRS += less

# bb
#
#SUBDIRS += gzip

SUBDIRS += iputils
SUBDIRS += kbd
SUBDIRS += libpipeline
SUBDIRS += man-db
SUBDIRS += make
SUBDIRS += xz
SUBDIRS += expat
SUBDIRS += xml_parser
SUBDIRS += intltool

# bb
#
#SUBDIRS += kmod
#SUBDIRS += patch
SUBDIRS += psmisc

# FIXME: reevaluate this... might really be better off with mdev/eudev and
#        bb init
#
#SUBDIRS += systemd

SUBDIRS += dbus

# bb
#
#SUBDIRS += tar
SUBDIRS += texinfo

# bb
#
#SUBDIRS += vim

# NOTE: we use syslinux instead of grub and do magic tricks with ruckusrd.
#
#SUBDIRS += grub
SUBDIRS += ruckusrd
SUBDIRS += syslinux

SUBDIRS += kernel
SUBDIRS += kernel-firmware

# END CLFS Chapter 10 - Installing Basic System Software
#
# Beyond this point, we add what we want and deviate wildly from CLFS.


# FIXME: how should we plumb in the image separation?  maybe fake packages
# that have a make target to create squashfs images and adjust filesystems
# instead of building/installing packages?
#
# SUBDIRS += finalize-base
# SUBDIRS += ... more packages ...
# SUBDIRS += finalize-server
#
