# -*- mode: autoconf -*-
AC_PREREQ(2.62)
AC_INIT([Source Ruckus], [3.0-dev], [michael.d.labriola@gmail.com], sourceruckus)

AC_CANONICAL_BUILD
CLFS_HOST=$build_cpu-cross-$build_os

# NOTE: We do NOT use AC_CANONICAL_HOST or AC_CANONICAL_TARGET here, for
#       the following reasons:
#
#       1. We want to OVERRIDE the --host value AUTOMATICALLY based on the
#          results of config.guess.
#
#       2. Passing --host or --target into this configure script will mean
#          something DIFFERENT than what's documented in the autoconf
#          manual (i.e., When I say "target", it's really what should be
#          passed into --host for some packages).  We're not configuring a
#          cross-compiler here, we're configuring a system for building
#          lots of things including a cross-compiler...
#
#       3. I don't want to make/let the user specify the whole triplet.
#
#       4. I just don't want to confuse myself... ;-)
#
AC_DEFUN([valid_archs], [i686 x86_64])
m4_define(default_arch, x86_64)
AC_ARG_ENABLE(arch,
  AS_HELP_STRING([--enable-arch=ARCH], [Final target architecture of build (valid: m4_unquote(m4_split(valid_archs))) [default=default_arch]]),
  build_arch=$enableval,
  build_arch=default_arch)

CLFS_TARGET=$build_arch-ruckus-linux-gnu


# allow disabling multilib for pure64 builds
AC_ARG_ENABLE(multilib,
  AS_HELP_STRING([--enable-multilib], [Enable multilib [default=auto]]),
  build_multilib=$enableval,
  build_multilib=yes)

# check for propper arch usage
build_arch_is_valid=no
for x in valid_archs; do
  AS_IF([test "$build_arch" = "$x"],
    [build_arch_is_valid=yes
     break
    ])
done

AS_IF([test "$build_arch_is_valid" != "yes"],
  [AC_MSG_ERROR([Invalid arch '$build_arch' specified (valid: m4_unquote(m4_split(valid_archs)))])])

CLFS_ARCH=$build_arch

# set variables based on build_arch
AS_IF([test "$build_arch" = "i686"],
  [ # x86
   BUILD32=
   BUILD64=
   CLFS_TARGET32=
   build_multilib=no
  ],
  [ # x86_64
   AS_IF([test "$build_multilib" = "yes"],
     [ # x86_64 multilib
      BUILD32="-m32"
      BUILD64="-m64"
      CLFS_TARGET32="i686-ruckus-linux-gnu"
     ],
     [ # pure64
      BUILD32=
      BUILD64="-m64"
      CLFS_TARGET32=
     ])
  ])


abs_srcdir=$(cd $srcdir && pwd)
INSTALLROOT=$abs_srcdir/..
BUILDROOT=$abs_srcdir/.build
PREFIX_EARLY=$BUILDROOT/bootstrap-early
PREFIX_CROSS=/cross-tools
PREFIX_FINAL=/tools
AC_ARG_VAR(INSTALLROOT, Root of our final rootfs)
AC_ARG_VAR(BUILDROOT, The build dir)
AC_ARG_VAR(PREFIX_EARLY, Prefix for bootstrap-early)
AC_ARG_VAR(PREFIX_CROSS, Prefix for bootstrap-cross)
AC_ARG_VAR(PREFIX_FINAL, Prefix for bootstrap-final)


AM_INIT_AUTOMAKE

# FIXME: probably not going to be used anywhere...
AM_CONDITIONAL([MULTILIB],
  [test x$build_multilib = xyes -a x$target_arch = xx86_64])


# check for needed utils on the host system
#
# NOTE: This is the list of packages to be built/installed as a part of
#       the bootstrap-early stage.
p_early=

# add rsync so we can use it to copy source trees for packages that don't
# support out-of-tree builds
AC_PATH_PROG(RSYNC, [rsync])
AC_ARG_VAR(RSYNC, Path to rsync binary)
AS_IF([test "$RSYNC" != ""], [], [p_early+=" rsync"])


# gzip is needed so we can ensure tar can extract from tar.gz files
#
# FIXME: lots of the following types of warnings, then a failure on my
#        netbook (SUSE Enterprise 11?):
#
#        cc1: warning: command line option "-Wabi" is valid for C++/ObjC++
#        but not for C
AC_PATH_PROG(GZIP, [gzip])
AC_ARG_VAR(GZIP, Path to gzip binary)
AS_IF([test "$GZIP" != ""], [], [p_early+=" gzip"])


# bzip2 is also needed to make sure we can extract all our source tarballs
AC_PATH_PROG(BZIP2, [bzip2])
AC_ARG_VAR(BZIP2, Path to bzip2 binary)
AS_IF([test "$BZIP2" != ""], [], [p_early+=" bzip2"])


# xz is needed to build syslinux (and potentially for uncompressing
# source tarballs, patches, etc)
AC_PATH_PROG(XZ, [xz])
AC_ARG_VAR(XZ, Path to xz binary)
AS_IF([test "$XZ" != ""], [], [p_early+=" xz"])


# make sure that we have a version of tar that can extract tarballs
# compressed with gzip, bzip2, and xz
AC_PATH_PROG(TAR, [tar])
AC_ARG_VAR(TAR, Path to tar binary (must support gzip, bzip2, and xz))
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
AC_ARG_VAR(HELP2MAN, Path to help2man binary)
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
need_ncurses=no
AC_CHECK_LIB(ncurses, [initscr],
  # lib found, check for header
  [AC_CHECK_HEADER(curses.h, [], [need_ncurses=yes])],
  [need_ncurses=yes])

AS_IF([test "$need_ncurses" != "no"],
  [p_early+=" ncurses"
   AC_MSG_WARN([
		*************************************************************
		Your host system lacks a usable ncurses library.  We will
		build and install one via bootstrap-early, but you may
		have to patch packages (e.g., busybox, linux kernel) to
		look anywhere other than /usr for it.  You might be better
		off installing ncurses by hand (as root) with
		--prefix=/usr and re-running this configure script.
		*************************************************************])
  ])


# latest m4 needs a pretty recent version of texinfo (>= 4.13)
#
# FIXME: binutils needs special sed hackage to build w/ texinfo 5.x
#
# FIXME: texinfo 5.1 requires automake >= 1.13 and ncurses.  actually, it
#        requires aclocal-1.13 and automake-1.13... so having automake 1.14
#        doesn't help at all...
#
# FIXME: keeping this at 4.13 for now to avoid above issues.
#
# FIXME: does this nasty puke macro even work if version is 4.13a?
#
AC_CACHE_CHECK([for texinfo >= 4.13], [ac_cv_path_TEXINFO],
  [AC_PATH_PROGS_FEATURE_CHECK([TEXINFO], [makeinfo],
    [[result=`$ac_path_TEXINFO --version | head -n1 | awk '{tmp=split($NF,v,"."); if (v[1] < 4) print "no"; else if (v[1] == 4 && v[2] < 13) print "no2"; else print "yes"}'`
     test "x$result" = "xyes" && ac_cv_path_TEXINFO=$ac_path_TEXINFO ac_path_TEXINFO_found=:
    ]],

    [ac_cv_path_TEXINFO=no
     p_early+=" texinfo"
    ])
  ])


# gettext needs gperf to bootstrap propperly
#
# NOTE: As far as I can tell, any version will do
AC_PATH_PROG(GPERF, [gperf])
AC_ARG_VAR(GPERF, Path to gperf binary)
AS_IF([test "$GPERF" != ""], [], [p_early+=" gperf"])


# always install these to make sure the rest of the build is done using
# the GNU build tools we test against
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
#       reference.  (although, i think they're outdated again...)
#
p_early+=" m4" # automake >= 1.11.6, autoconf >= 2.62, help2man >= 1.29, makeinfo >= 4.13, xz
p_early+=" autoconf" # m4 >= 1.4.16
p_early+=" automake" # autoconf >= 2.69
p_early+=" gettext"
p_early+=" libtool" # warned about libtool.m4 bootstrap failure
p_early+=" pkg-config" # glib bits need libtool
p_early+=" make" # automake >= 1.11.1, gettext >= 0.18.1, pkg-config

# FIXME: gperf?
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


# for ruckusrd
AC_PATH_PROG(CPIO, [cpio])
AC_ARG_VAR(CPIO, Path to cpio binary)
AS_IF([test "$CPIO" != ""], [], [p_early+=" cpio"])


# check for ruckusrd >= 0.10.0
AC_CACHE_CHECK([for ruckusrd >= 0.10.0], [ac_cv_path_RUCKUSRD],
  [AC_PATH_PROGS_FEATURE_CHECK([RUCKUSRD], [ruckusrd],
    [[result=`$ac_path_RUCKUSRD --version | cut -b2- | awk '{tmp=split($NF,v,"."); if (v[1] < 0) print "no"; else if (v[1] == 0 && v[2] < 10) print "no2"; else print "yes"}'`
     test "x$result" = "xyes" && ac_cv_path_RUCKUSRD=$ac_path_RUCKUSRD ac_path_RUCKUSRD_found=:
    ]],

    [ac_cv_path_RUCKUSRD=no
     p_early+=" ruckusrd"
    ])
  ])


# check for extlinux >= 4.x for ext4 support
AC_CACHE_CHECK([for extlinux >= 4.0], [ac_cv_path_EXTLINUX],
  [AC_PATH_PROGS_FEATURE_CHECK([EXTLINUX], [extlinux],
    [[result=`$ac_path_EXTLINUX --version 2>&1 | awk '{print $2}' | awk '{tmp=split($NF,v,"."); if (v[1] < 4) print "no"; else if (v[1] == 4 && v[2] < 0) print "no2"; else print "yes"}'`
     test "x$result" = "xyes" && ac_cv_path_EXTLINUX=$ac_path_EXTLINUX ac_path_EXTLINUX_found=:
    ]],

    [ac_cv_path_EXTLINUX=no
     p_early+=" syslinux"
    ])
  ])

AC_SUBST(p_early)
AC_SUBST(INSTALLROOT)
AC_SUBST(CLFS_HOST)
AC_SUBST(CLFS_TARGET)
AC_SUBST(CLFS_TARGET32)
AC_SUBST(BUILD32)
AC_SUBST(BUILD64)
AC_SUBST(CLFS_ARCH)


#AC_CONFIG_FILES(Makefile, src/Makefile, packages/Makefile, utils/Makefile)
AC_CONFIG_FILES([
  Makefile
  utils/Makefile
  utils/bootstrap-early/config
])

AC_OUTPUT

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE_STRING"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options Summary:"
echo
echo "Bootstrap Early Packages: $p_early"
echo
echo "Options:"
echo "  Final Target Arch....: $build_arch"
echo "  Multilib.............: $build_multilib"
echo
echo "CLFS Variables:"
echo "  CLFS_HOST............: $CLFS_HOST"
echo "  CLFS_TARGET..........: $CLFS_TARGET"
echo "  CLFS_TARGET32........: $CLFS_TARGET32"
echo "  CLFS_ARCH............: $CLFS_ARCH"
echo "  BUILD32..............: $BUILD32"
echo "  BUILD64..............: $BUILD64"
echo
echo "Other Variables:"
echo "  INSTALLROOT..........: $INSTALLROOT"
echo "  BUILDROOT............: $BUILDROOT"
echo "  PREFIX_EARLY.........: $PREFIX_EARLY"
echo "  PREFIX_CROSS.........: $PREFIX_CROSS"
echo "  PREFIX_FINAL.........: $PREFIX_FINAL"
echo
