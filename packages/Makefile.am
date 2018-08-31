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

# FIXME: these were just for systemd...
#
#SUBDIRS += attr
#SUBDIRS += acl

# bb
#
#SUBDIRS += sed

SUBDIRS += pkg-config # pkg-config-lite?
SUBDIRS += ncurses

# bb
#
#SUBDIRS += shadow
#SUBDIRS += util-linux
#SUBDIRS += procps
#SUBDIRS += e2fsprogs

# FIXME: this was just for systemd
#
#SUBDIRS += libcap

# bb
#
#SUBDIRS += coreutils

SUBDIRS += iana-etc
SUBDIRS += libtool

# FIXME: some equivelent stuff is in bb... but we may need the real deal
#        (unless newer version of bb is way better).
#
#SUBDIRS += iproute2

# bb
#
#SUBDIRS += bzip2

# FIXME: what is this...?  GNU Database Manager?  what needs this?  not
#        systemd...  anything?  can we leave this out?
#
#SUBDIRS += gdbm

SUBDIRS += perl
SUBDIRS += readline
SUBDIRS += autoconf
SUBDIRS += automake

# bb
#
#SUBDIRS += bash

# FIXME: maybe in newer bb?  doesn't look like it...  but maybe we don't
#        need it?  i seem to recall kernel makefiles needing it at some
#        point...
#
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

# FIXME: this was just for systemd
#
#SUBDIRS += gperf

# bb
#
#SUBDIRS += grep

SUBDIRS += groff

# bb
#
#SUBDIRS += less
#SUBDIRS += gzip

# FIXME: portions of these are in bb... can we live w/out the missing
#        bits?
#
#SUBDIRS += iputils
#SUBDIRS += kbd

# FIXME: what needed this?  not systemd...  can we remove this?
#
#SUBDIRS += libpipeline

# FIXME: portions of these are in bb... can we live w/out the missing
#        bits?
#
#SUBDIRS += man-db

SUBDIRS += make
SUBDIRS += xz

# FIXME: these were just for systemd
#
#SUBDIRS += expat
#SUBDIRS += perl_xml_parser
#SUBDIRS += intltool

# bb
#
#SUBDIRS += kmod
#SUBDIRS += patch
#SUBDIRS += psmisc

# FIXME: CLFS SYSVINIT adds these...  can we leave them out if we use bb
#        init?
#
#SUBDIRS += libestr
#SUBDIRS += libee
#SUBDIRS += rsyslog
#SUBDIRS += sysvinit

# FIXME: reevaluate this... might really be better off with mdev/eudev and
#        bb init
#
#SUBDIRS += systemd
#SUBDIRS += dbus

# bb
#
#SUBDIRS += tar
SUBDIRS += texinfo

# FIXME: CLFS SYSVINIT added this...  gonna try to stick with bb mdev for
#        now.
#
# SUBDIRS += eudev

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
#        that have a make target to create squashfs images and adjust
#        filesystems instead of building/installing packages?
#
# SUBDIRS += finalize-base
# SUBDIRS += ... more packages ...
# SUBDIRS += finalize-server
#



# FIXME: make some generic make rules to include for building srp from
#        content, brp from srp, installing brp, etc, for each of the
#        SUBDIR .am files to use.
#
# srp --build=foo.notes --src=/path/to/src --copysrc
# srp --build=foo.notes --src=foo.tar.xz --extra=/path/to/extra/files
#
#       I don't think we can really do "generic" make rules.  Let's start
#       by hand-writing build rules per-package.
#
# srp -b /ruckus/packages/tzdata/tzdata.notes --src /ruckus/src/tzdata/tzdata2014d.tar.gz
# srp -b /ruckus/packages/man-pages/man-pages.notes --src /ruckus/src/man-pages --gitsrc
