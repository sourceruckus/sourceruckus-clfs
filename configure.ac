# -*- mode: autoconf -*-

AC_INIT([Source Ruckus], [3.0-dev], [michael.d.labriola@gmail.com], sourceruckus)

AM_INIT_AUTOMAKE

# check for needed utils on the host system
#
# NOTE: This is the list of packages to be built/installed as a part of
#       the bootstrap-early stage.
p_early=

# add rsync so we can use it to copy source trees for packages that don't
# support out-of-tree builds
AC_PATH_PROG(RSYNC, [rsync])
AS_IF([test "$RSYNC" != ""], [], [p_early+=" rsync"])

# gzip is needed so we can ensure tar can extract from tar.gz files
#
# FIXME: lots of the following types of warnings, then a failure on my
#        netbook:
#
#        cc1: warning: command line option "-Wabi" is valid for C++/ObjC++
#        but not for C
AC_PATH_PROG(GZIP, [gzip])
AS_IF([test "$GZIP" != ""], [], [p_early+=" gzip"])

# bzip2 is also needed to make sure we can extract all our source tarballs
AC_PATH_PROG(BZIP2, [bzip2])
AS_IF([test "$BZIP2" != ""], [], [p_early+=" bzip2"])

# xz is needed to build syslinux (and potentially for uncompressing
# source tarballs, patches, etc)
AC_PATH_PROG(XZ, [xz])
AS_IF([test "$XZ" != ""], [], [p_early+=" xz"])

# tar is needed to make sure that we have a new enough version to extract
# tarballs compressed with xz and lzma
#
# FIXME: actually check for xz support!  easiest way might be to create a
#        tar.xz, tar,bz2, etc, file and assume that if can create you can
#        extract.
#
# FIXME: tar --bzip2, --xz, --gzip
AC_PATH_PROG(TAR, [tar])
AS_IF([test "$TAR" != ""],

  # we have tar, make sure it has gzip support
  [AC_CACHE_CHECK([whether $ac_cv_path_TAR supports gzip],
     [my_cv_tar_gzip],
     [my_cv_tar_gzip=no
      tar --gzip -c $srcdir/README >/dev/null 2>&1 && my_cv_tar_gzip=yes])
   AS_IF([test $my_cv_tar_gzip != yes],
     [p_early+=" tar"],

     # our tar supports gzip, check for bzip2
     [AC_CACHE_CHECK([whether $ac_cv_path_TAR supports bzip2],
        [my_cv_tar_bzip2],
        [my_cv_tar_bzip2=no
         tar --bzip2 -c $srcdir/README >/dev/null 2>&1 && my_cv_tar_bzip2=yes])
      AS_IF([test $my_cv_tar_bzip2 != yes],
        [p_early+=" tar"],

        # now check for xz support
        [AC_CACHE_CHECK([whether $ac_cv_path_TAR supports xz],
           [my_cv_tar_xz],
           [my_cv_tar_xz=no
            tar --xz -c $srcdir/README >/dev/null 2>&1 && my_cv_tar_xz=yes])
         AS_IF([test $my_cv_tar_xz != yes],
           [p_early+=" tar"],
           [])
        ])
     ])
  ],

  # no tar
  [p_early+=" tar"])

# help2man is needed to build man pages for libtool and texinfo after
# running autoreconf
AC_PATH_PROG(HELP2MAN, [help2man])
AS_IF([test "$HELP2MAN" != ""], [], [p_early+=" help2man"])

# might also want to make sure that the host system has a usable ncurses
# (and ncurses-devel) library.  otherwise we can't run the 'menuconfig'
# target for kernel and busybox configuration
#
# NOTE: Both the kernel and busybox fail to look for ncurses anywhere
#       other than in /usr... so in order to actually make use of
#       this, their source trees will need to be patched to point to
#       our bootstrap-early libraries.
#
# NOTE: texinfo also requires ncurses to compile
#
# FIXME: print a WARNING
#
# FIXME: check for both lib and headers
AC_CHECK_LIB(ncurses, [initscr],
  # lib found, check for header
  [AC_CHECK_HEADER(curses.h, [], [p_early+=" ncurses"])],
  [p_early+=" ncurses"])

# latest m4 needs a pretty recent version of texinfo (>= 4.13)
#
# FIXME: binutils needs special sed hackage to build w/ texinfo 5.x
#
# FIXME: texinfo 5.1 requires automake >= 1.13 and ncurses.  actually, it
#        requires aclocal-1.13 and automake-1.13... so having automake 1.14
#        doesn't help at all...
#
# FIXME: keeping this at 4.13 for now to avoid above issues.
#AC_PATH_PROG(TEXINFO, [texinfo])
#AS_IF([test "$TEXINFO" != ""], [], [p_early+=" texinfo"])

# install these to make sure the rest of the build is done using the GNU
# build tools we test against
#
# NOTE: This is the bootstrap black whole mentioned elsewhere.  I have
#       sucessfully worked around it a few different ways.
#
#       - Installed automake-1.12.4 from source.tar.xz in /scrap/staging and
#         tweaked environment vars appropriately (e.g., PATH,
#         LD_LIBRARY_PATH).
#
#       - My netbook already had automake 1.11.1 installed and configured
#         properly in /usr/local (had done it via source.tar.xz a while
#         back).  Of course, it ALSO had autoconf, m4, gettext, libtool,
#         make, pkg-config, and bison of similar vintage installed... so
#         just installing automake v1.11.1 might not be enough.
#
# NOTE: I've placed requirements in comments for each of these, for future
#       reference.
p_early+=" m4" # automake >= 1.11.6, autoconf >= 2.62, help2man >= 1.29, makeinfo >= 4.13, xz
p_early+=" autoconf" # m4 >= 1.4.16
p_early+=" automake" # autoconf >= 2.69
p_early+=" gettext"
p_early+=" libtool" # warned about libtool.m4 bootstrap failure
p_early+=" pkg-config" # glib bits need libtool
p_early+=" make" # automake >= 1.11.1, gettext >= 0.18.1, pkg-config

# FIXME: p_early+=" gperf"
# FIXME: flex?
# FIXME: graphviz?
# FIXME: valgrind?
p_early+=" bison"

# to ensure that we don't inadvertantly use a modified version with
# non-standard extensions to build our filesystems
#
# NOTE: e2fsprogs > v1.41.7 may not compile on systems with really old kernel
#       headers.  This can be worked around by hacking it's misc/Makefile.in to
#       not compile e4defrag.  It can also be worked around by rewinding the
#       e2fsprogs submodule to v1.41.7 prior to building bootstrap-early, then
#       fast forwarding it back to the modern day.  ;-)
p_early+=" e2fsprogs"

# for ruckusrd?
AC_PATH_PROG(CPIO, [cpio])
AS_IF([test "$CPIO" != ""], [], [p_early+=" cpio"])

# check for ruckusrd >= 0.10.0
#AC_PATH_PROG(RUCKUSRD, [ruckusrd])
#AS_IF([test "$RUCKUSRD" != ""], [], [p_early+=" ruckusrd"])

# check for extlinux >= 4.x for ext4 support
#AC_PATH_PROG(SYSLINUX, [syslinux])
#AS_IF([test "$SYSLINUX" != ""], [], [p_early+=" syslinux"])

AC_SUBST(p_early)


#AC_CONFIG_FILES(Makefile, src/Makefile, packages/Makefile, utils/Makefile)
AC_CONFIG_FILES([
  Makefile
  utils/Makefile
  utils/bootstrap-early/config
])

AC_OUTPUT
