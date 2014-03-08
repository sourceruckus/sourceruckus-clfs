# -*- mode: autoconf -*-
AC_PREREQ(2.62)
AC_INIT([Source Ruckus], [3.0-dev], [michael.d.labriola@gmail.com], sourceruckus)

AC_CANONICAL_BUILD
CLFS_HOST=$build_cpu-cross-$build_os

# check for automake >= 1.11.1
#
# NOTE: Cyclic dependencies of doom this way come.  Not having at least
#       automake 1.11 DEFINATELY is a problem... I think having >= 1.11.1
#       is OK, but I'm not even sure of that.  :-/
#
# NOTE: This is fatal.  Unlink the other checks, which are deciding what
#       to build in bootstrap-early, a recent version of automake is
#       NEEDED in order to even bootstrap the source trees for the
#       bootstrap-early packages.
#
# NOTE: We set 'foreign' to allow for GNU Make extensions in our automake
#       files
AM_INIT_AUTOMAKE(1.11.1 foreign)

# We don't direcetly need libtool here, but leaving this out may cause
# source trees inside the src dir to fail to bootstrap unless we add
# AC_CONFIG_AUX_DIR calls to each of them.  By default, automake and
# autoconf will walk up the directory tree looking for auxilary files
# instead of making new ones... and it looks like if ANY of the aux files
# are found at ../.., it assumes they're ALL there.
LT_INIT


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
AC_ARG_WITH(arch,
  AS_HELP_STRING([--with-arch=ARCH], [Final target architecture of build (valid: m4_unquote(m4_split(valid_archs))) [default=default_arch]]),
  build_arch=$withval,
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

# FIXME: probably not going to be used anywhere...
AM_CONDITIONAL([MULTILIB],
  [test x$build_multilib = xyes -a x$build_arch = xx86_64])


# set variables based on build_arch
AS_IF([test "$build_arch" = "i686"],
  [ # x86
   CLFS_ARCH=i386
   BUILD32=
   BUILD64=
   CLFS_TARGET32=
   build_multilib=no
  ],
  [ # x86_64
   CLFS_ARCH=x86_64
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
BUILDROOT=$abs_srcdir/.build
PREFIX_EARLY=$BUILDROOT/bootstrap-early
PREFIX_CROSS=/cross-tools
PREFIX_FINAL=/tools
CLFS=$BUILDROOT/baby
AC_ARG_VAR(BUILDROOT, The build dir)
AC_ARG_VAR(PREFIX_EARLY, Prefix for bootstrap-early)
AC_ARG_VAR(PREFIX_CROSS, Prefix for bootstrap-cross)
AC_ARG_VAR(PREFIX_FINAL, Prefix for bootstrap-final)
AC_ARG_VAR(CLFS, Location of baby filesystem during build)


# FIXME: add flags for installation and kickoff of stage2
#
# ./configure --with-arch=i686 --enable-install-dev=/dev/sdf
#
# --enable-install-part=/dev/sdXN
#
#     Install bootstrap system into the specified partition.  This is
#     going to assume that the partition and filesystem are already
#     created and it WILL NOT install a bootloader.  This is perfect for
#     when you want to boot the resulting bootstrap system as a virtual
#     machine.
#
# --enable-install-dev=/dev/sdX
#
#     Install bootstrap system onto the specified block device.  ALL DATA
#     ON DEVICE WILL BE LOST, because the sandbox stage will re-partition
#     and create filesystems as needed.  The bootstrap_install stage will
#     install and configure the bootloader.
#
# --enable-install-raid0=/dev/md99:/dev/sda,/dev/sdb,/dev/sdc,...
#
#     Just like --enable-install-dev, except that multiple block devices
#     are used in a RAID0 array to get better disk performance.
#
#     FIXME: If we want to use RAID0 for stage2 build, we'll need a
#            separate RAID1 /boot partition.  Should we make the user
#            specify two md devs via configure?  Autodetect next available
#            minor md number?
#
# --enable-auto-stage2
#
#    Bootstrap system will automatically kick off the stage2 builder.
#
#    FIXME: I'm not so sure this is a good idea... Sure, we can do it, but
#           you probably want to manually kick it off.  This way nothing
#           can blow up your source tree w/out you explicitly kicking off
#           the command.
#
# --with-fstype=ext2
#
#    Specify fstype for bootable bootstrap system (created in sandbox
#    stage).  Valid options are ext2, ext3, ext4.  You almost certainly
#    want ext2 to avoid journaling overhead and improve disk performance,
#    but I'd like to benchmark this before hardcoding it.
#
#
# NOTE: Only a single --enable-install* option can be supplied.  Default
#       is --disable-install.
#
# NOTE: If configured w/out an --enable-install* option, the sandbox and
#       install targets will be disabled.

# FIXME: add ext4-no-jounal?
AC_DEFUN([valid_fstypes], [ext2 ext3 ext4])
m4_define(default_fstype, ext2)
AC_ARG_WITH(fstype,
  AS_HELP_STRING([--with-fstype=FSTYPE], [Filesystem type for bootstrap system (valid: m4_unquote(m4_split(valid_fstypes))) [default=default_fstype]]),
  build_fstype=$withval,
  build_fstype=default_fstype)

# check for propper fstype usage
build_fstype_is_valid=no
for x in valid_fstypes; do
  AS_IF([test "$build_fstype" = "$x"],
    [build_fstype_is_valid=yes
     break
    ])
done

AS_IF([test "$build_fstype_is_valid" != "yes"],
  [AC_MSG_ERROR([Invalid fstype '$build_fstype' specified (valid: m4_unquote(m4_split(valid_fstypes)))])])

AC_SUBST(BOOTSTRAP_FSTYPE, $build_fstype)


install_type=none
AC_ARG_ENABLE(install,
  AS_HELP_STRING([--disable-install], [Disable sandbox and install targets (enable via specific --enable-install-* options) [default=disabled]]),
  build_install=$enableval,
  build_install=no)

AC_ARG_ENABLE(install-part,
  AS_HELP_STRING([--enable-install-part=PARTITION], [Install into specified PARTITION.]),
  [install_part=$enableval
   install_type=part
   build_install=yes
  ],
  install_part=no)

AC_ARG_ENABLE(install-dev,
  AS_HELP_STRING([--enable-install-dev=BLOCKDEV], [Install into specified BLOCKDEV.  ALL DATA ON BLOCKDEV WILL BE DESTROYED!]),
  [install_dev=$enableval
   install_type=dev
   build_install=yes
  ],
  install_dev=no)

AC_ARG_ENABLE(install-raid0,
  AS_HELP_STRING([--enable-install-raid0=DISK_LIST], [Install into RAID0 device created from comma-delimited DISK_LIST.  ALL DATA ON DISKS WILL BE DESTROYED!]),
  [install_raid0=$enableval
   install_type=raid0
   build_install=yes
  ],
  install_raid0=no)

# FIXME: handle multiple --enable-install flags

# FIXME: handle --enable-install-* w/out arg (install_dev == yes)




# FIXME: let's get this all sorted out in my head... build order!
#
# FIXME: instead of creating our final device at the end and then copying
#        the source tree in, why don't we create it at the beginning and
#        just build in it from the get go?
#
#        Because, we'd have to do all the RAID magic prior to running
#        init_submodules and get_sources... which I was planning on
#        eventually sticking in autogen.sh.  So it would have to happen
#        before configure.
#
#
# --> ./autogen.sh --enable-install-md=/dev/sda,...
#
#     Will run autoreconf, configure, etc.  Nothing special here.
#
# --> sudo make sandbox
#
#     Only needed if an --enable-install option was supplied.  Will create
#     raid device(s), filesystem(s), mount them, move our whole tree over
#     into the sanbox.  (should we mv or cp?)
#
# --> cd sandbox (if you made one)
# --> make download
#
#     Run init_submodules, get_sources.  (This could be done before
#     sandbox, but will be much faster to do it afterwords.)
#
# --> make bootstrap
#
#     Build bootstrap-early, bootstrap-cross, bootstrap-stage1.
#
# --> sudo make bootstrap_install
#
#     Do the prep work for booting the bootstrap system (requires root).
#     This includes chmoding, bootloader installation, etc.
#
# ... boot it ..
#
# --> cd /ruckus
# --> make all
#
#     Iterate through all the packages, building and installing each one.
#     There will be some special work to be done (e.g., compile toolchain
#     tweaking, dumping installation images), but should result in a
#     bootable IS09660 installer image containing install scripts and
#     rootfs images.  All opensource source code should be included in an
#     additional IS09660 image for easy GPL compliance.
#
#     FIXME: The dist target should get plumbed up so that it creates
#            a tarball of all the required sources to build a
#            system... So we need to make sure that the build system
#            is robust enough to not fail if certain packages get left
#            out.
#
#     FIXME: This stage has really only been half-baked so far... ;-)





# FIXME: check for git version w/ submodule --init --recursive?  if
#        running autoconf happens after submodule work, it's too
#        late... perhaps we should add this check to autogen prior to
#        autoconf?



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
#
# FIXME: might want to check for -g incremental support, although, if it's
#        got xz support, I can't imagine it wouldn't have -g...
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


# check for glib
#
# NOTE: pkg-config has an internal glib snapshot that can be used if glib
#       is missing on the host system, but it adds needless compile time
#       (and might not build propperly on some host systems).
#
# FIXME: why not just use pkg-config for this check?  glib-2.0
#
#AC_CHECK_LIB([glib-2.0], [g_malloc],
#  # lib found, check for header
#  [AC_CHECK_HEADER(glib.h, [has_glib=yes], [has_glib=no], [-Iglib-2.0])],
#  [has_glib=no])
#
# FIXME: was having a hard time getting glib.h to work... shying away from
#        using the pkg-config macros because of the autotools cyclic deps
#        probem...
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES(GLIB, [glib-2.0], [has_glib=yes], [has_glib=no])
AC_SUBST(HOST_HAS_GLIB, $has_glib)


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


# bc
#
# NOTE: CLFS added bc to cross-tools (and final system) on 2013-07-23... I
#       remember seing something on the mailing list... but I don't really
#       remember for sure why it was added.  I think it had something to do
#       with prepping the linux headers.
#
# FIXME: I don't understand why this isn't a host system requirement as
#        apposed to a cross-tools item... I suspect it ended up in
#        cross-tools for CLFS because it's so easy/quick to install, why
#        bother checking host system.  But we've got bootstrap-early to take
#        care of that...
#
# FIXME: oldest version I've tested with is 1.06...
AC_PATH_PROG(BC, [bc])
AC_ARG_VAR(BC, Path to bc binary)
AS_IF([test "$BC" != ""], [], [p_early+=" bc"])


# for ruckusrd
AC_PATH_PROG(CPIO, [cpio])
AC_ARG_VAR(CPIO, Path to cpio binary)
AS_IF([test "$CPIO" != ""], [], [p_early+=" cpio"])


# check for ruckusrd >= 0.10
#
# FIXME: instead of checking for a certain version here, why don't I just
#        always install what we've got controlled... it'll only take a we
#        bit extra and then we're guaranteed to be using the right
#        version.
#
# FIXME: need to actually add a submodule for ruckusrd and srp
AC_CACHE_CHECK([for ruckusrd >= 0.10], [ac_cv_path_RUCKUSRD],
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


# check for CLFS symlinks...
#
# FIXME: Put a decent description of the CLFS /tools and /cross-tools
#        symlinks in here...
AC_MSG_CHECKING([for the CLFS $PREFIX_CROSS symlink])
prefix_cross_link=`readlink $PREFIX_CROSS 2>/dev/null`
AS_IF([test "$prefix_cross_link" = "$CLFS$PREFIX_CROSS"],
  [AC_MSG_RESULT($prefix_cross_link)],
  [AC_MSG_RESULT(missing)
   AC_MSG_ERROR([
		*************************************************************
		Please create the CLFS $PREFIX_CROSS symlink by executing
		the following command as root, then rerun configure:

		ln -s $CLFS$PREFIX_CROSS $PREFIX_CROSS
		*************************************************************])
  ])

AC_MSG_CHECKING([for the CLFS $PREFIX_FINAL symlink])
prefix_final_link=`readlink $PREFIX_FINAL 2>/dev/null`
AS_IF([test "$prefix_final_link" = "$CLFS$PREFIX_FINAL"],
  [AC_MSG_RESULT($prefix_final_link)],
  [AC_MSG_RESULT(missing)
   AC_MSG_ERROR([
		*************************************************************
		Please create the CLFS $PREFIX_FINAL symlink by executing
		the following command as root, then rerun configure:

		ln -s $CLFS$PREFIX_FINAL $PREFIX_FINAL
		*************************************************************])
  ])





AC_SUBST(p_early)
AC_SUBST(CLFS_HOST)
AC_SUBST(CLFS_TARGET)
AC_SUBST(CLFS_TARGET32)
AC_SUBST(BUILD32)
AC_SUBST(BUILD64)
AC_SUBST(CLFS_ARCH)


# FIXME: can i include other files and append to AC_CONFIG_FILES
#        dynamically? maintaining another list of all the packages to
#        be built is gonna get ugly...
AC_CONFIG_FILES([
  Makefile
  packages/Makefile
  packages/ruckusrd/Makefile
  utils/Makefile
  utils/bootstrap-early/Makefile
  utils/bootstrap-early/config
  utils/bootstrap-cross/Makefile
  utils/bootstrap-stage1/Makefile
  utils/bootstrap-stage2/Makefile
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
echo "Bootstrap Early Packages:"
echo " $p_early"
echo
echo "Options:"
echo "  Final Target Arch....: $build_arch"
echo "  Multilib.............: $build_multilib"
echo "  Bootstrap fstype.....: $build_fstype"
echo "  Install Type.........: $install_type"
case "$install_type" in
  part)
    echo "  Install Partition....: $install_part"
    ;;
  dev)
    echo "  Install Device.......: $install_dev"
    ;;
  raid0)
    echo "  Install Device.......: $install_raid0"
    ;;
esac
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
echo "  BUILDROOT............: $BUILDROOT"
echo "  PREFIX_EARLY.........: $PREFIX_EARLY"
echo "  PREFIX_CROSS.........: $PREFIX_CROSS"
echo "  PREFIX_FINAL.........: $PREFIX_FINAL"
echo "  CLFS.................: $CLFS"
echo
