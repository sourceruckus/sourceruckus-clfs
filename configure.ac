# -*- mode: autoconf -*-
AC_PREREQ(2.65)
AC_INIT([Source Ruckus], [3.0-dev], [michael.d.labriola@gmail.com], sourceruckus)

# figure out our host system
#
# NOTE: We use AC_CANONICAL_BUILD for this, but it gets slightly
#       different results than CLFS' sed script.  But it doesn't
#       depend on the MACHTYPE variable being set correctly (it wasn't
#       on my SUSE netbook).
#
#       CLFS_HOST=$(echo ${MACHTYPE} | sed -e 's/-[^-]*/-cross/')
#
#       That gives you 'i486' for build_cpu on my AMD A-series box,
#       whereas AC_CANONICAL_BUILD results in 'i686'.  Doesn't seem to
#       be hurting anything so far...
AC_CANONICAL_BUILD
CLFS_HOST=$build_cpu-cross-$build_os

# check for automake >= 1.13
#
# NOTE: This is fatal.  Unlike the other checks, which are deciding what
#       to build in bootstrap-early, a recent version of automake is
#       NEEDED in order to even bootstrap the source trees for the
#       bootstrap-early packages.
#
# NOTE: We now use AM_EXTRA_RECURSIVE_TARGETS, which was added in v1.13.
#       This is also what's driving our autoconf >= 2.65 requirement
#       (autoconf 1.13 requires autoconf 2.65).
#
# NOTE: We set 'foreign' to allow for GNU Make extensions in our automake
#       files
AM_INIT_AUTOMAKE(1.13 foreign)

# We don't directly need libtool here, but leaving this out may cause
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


# NOTE: no need to check if this is mounted... it has to be
AC_ARG_WITH(ruckus-dev,
  AS_HELP_STRING([--with-ruckus-dev=DEV], [/ruckus is a seperate partition, as apposed to it being on the installation partition.]),
  [ruckus_dev=$withval])
AC_SUBST(RUCKUS_DEV, $ruckus_dev)

AC_ARG_WITH(ruckus-path,
  AS_HELP_STRING([--with-ruckus-path=PATH], [Path to ruckus src tree on specified device [default: /]]),
  [ruckus_path=$withval])
AC_SUBST(RUCKUS_PATH, $ruckus_path)

AC_ARG_WITH(install-dev,
  AS_HELP_STRING([--with-install-dev=DEV], [Install on specified DEV, which must be mounted and writeable by build user.]),
  [install_dev=$withval
   # check that it's mounted
   mtab=`grep "^$install_dev" /proc/mounts`
   AS_IF([test -z "$mtab"],
     [AC_MSG_ERROR([
		*************************************************************
		The installation device must already exist and be mounted.
		Note that if you're using LVM, you may need to specify as
		/dev/mapper/VG-LV (i.e., if /dev/VG/LV doesn't work).

                You specified: --with-install-dev=$install_dev
		*************************************************************])
     ])
   install_fstype=`echo $mtab | awk '{print $5}'`
   # if CLFS wasn't specified, set it to mountpoint of install_dev
   mp=`echo $mtab | awk '{print $2}'`
   AS_IF([test -z "$CLFS"], [CLFS=$mp])
  ])
AC_SUBST(CLFS_DEV, $install_dev)
AC_SUBST(CLFS_FSTYPE, $install_fstype)


# FIXME: isn't there a better way to do this? some m4 or ac macro?
# FIXME: need to pass this variable along so we can pass device to domU
abs_srcdir=$(cd $srcdir && pwd)

AS_IF([test -z "$BUILDROOT"],
  [BUILDROOT=$abs_srcdir/.build])

AS_IF([test -z "$PREFIX_EARLY"],
  [PREFIX_EARLY=$prefix])

AS_IF([test "$PREFIX_EARLY" = "NONE" ],
  [PREFIX_EARLY=$BUILDROOT/bootstrap-early])

AS_IF([test -z "$PREFIX_CROSS"],
  [PREFIX_CROSS=/cross-tools])

AS_IF([test -z "$PREFIX_FINAL"],
  [PREFIX_FINAL=/tools])

AS_IF([test -z "$CLFS"],
  [CLFS=$BUILDROOT/baby])

AC_ARG_VAR(BUILDROOT,
  [The build dir (default: $abs_srcdir/.build)])

AC_ARG_VAR(PREFIX_EARLY,
  [Prefix for bootstrap-early (default: $BUILDROOT/bootstrap-early)])

AC_ARG_VAR(PREFIX_CROSS,
  [Prefix for bootstrap-cross (default: /cross-tools)])

AC_ARG_VAR(PREFIX_FINAL,
  [Prefix for bootstrap-final (default: /tools)])

AC_ARG_VAR(CLFS,
  [Location of baby filesystem during build (default: mountpoint of $install_dev)])


# check that $CLFS is writeable
mkdir -p $CLFS 2>/dev/null
AS_IF([touch $CLFS/.foo 2>/dev/null],
  [rm $CLFS/.foo],
  [AC_MSG_ERROR([
		*************************************************************
		The installation partition must be writeable by the build
		user.

                You specified: $CLFS
		*************************************************************])
  ])


# sanity checks
#
# NOTE: install_dev is required unless --with-stage1-xen-only, CLFS
#       cannot be unset at this point, and we need to check the validity
#       of ruckus_dev if provided.

AS_IF([test -z "$install_dev" -a "$stage1_xen_only" != "yes"],
  [AC_MSG_ERROR([
		*************************************************************
		You must specify an Install Device via --with-install-dev.
		*************************************************************])
  ])

# check that $CLFS/ruckus exists if ruckus_dev wasn't specified
AS_IF([test "$CLFS/ruckus" != "$abs_srcdir" -a -z "$ruckus_dev"],
  [AC_MSG_ERROR([
		*************************************************************
		The srcdir must be at /ruckus of the installation
		partition (e.g., /path/to/partition/ruckus) unless a
		seperate partition is being used (via --with-ruckus-dev
		and --with-ruckus-path).

                You specified: --with-install-dev=$install_dev
		Mountpoint: $CLFS
		Srcdir: $abs_srcdir
		*************************************************************])
  ])

# if user specified ruckus_dev, it could be wrong...
AS_IF([test -n "$ruckus_dev"],
  [mtab=`grep "^$ruckus_dev" /proc/mounts`
   mp=`echo $mtab | awk '{print $2}'`$ruckus_path
   mp=$(cd $mp && pwd)
   AS_IF([test "$mp" != "$abs_srcdir"],
     [AC_MSG_ERROR([
		*************************************************************
		Ruckus Device was specified, but doesn't match reality.

                You specified: --with-ruckus-dev=$ruckus_dev
                Mountpoint: $mp
                Srcdir: $abs_srcdir
		*************************************************************])
     ])
   ruckus_fstype=`echo $mtab | awk '{print $5}'`
  ])
AC_SUBST(RUCKUS_FSTYPE, $ruckus_fstype)



# FIXME: make --enable-stage2-xen the default

# FIXME: autodetect install_dev and ruckus_dev based on location of src
#        tree...?



# xen checks
#
# NOTE: By default we're going to build Xen, at the very least for use in
#       the final system.  However, recent Xen versions only support a 64bit
#       hypervisor.  We can still build a 32bit userland and dom0 kernel,
#       but the hypervisor will need to be cross-compiled.
#
#       If we're bulding for a 32bit architecture, we need to throw a warning
#       about cross-compiling the hypervisor for x86_64 1st.  We can
#       accomplish this very easily by reconfiguring for x86_64 and building
#       xen in bootstrap-stage1, but not deleting the hypervisor when making
#       clean, so when we go to build for i686 (which just silently doesn't
#       build a hypervisor), we'll still have the x86_64 one laying around.
#
#       So, if bulding for i686, we'll check for a built x86_64 hypervisor
#       in .build, and unless the user supplied --disable-xen, we'll display
#       a warning message and do some cross-compiling magic to build it if
#       it's missing.
#
# NOTE: We'll also have to keep the x86_64 hypervisor around for the
#       final build phase, at which point we'll be rebuilding the i686
#       userland tools.
#
# host 32bit, building 32bit, stage2xen: final, stage1 (64bit), early
# host 32bit, building 32bit: final, stage1 (64bit)
# host 32bit, building 64bit, stage2xen: final, stage1, early
# host 32bit, building 64bit: final, stage1
#
# host 64bit, building 32bit, stage2xen: final, early
# host 64bit, building 32bit: final, early
# host 64bit, building 64bit, stage2xen: final, early
# host 64bit, building 64bit: final
#
# so on a 32bit host, ./configure --with-arch=i686 ... will print a warning
# during configure.  It will then automatically ./configure
# --with-arch=x86_64 --disable-multilib ... --stage1-xen-only, build through
# stage1 (which will ONLY include Xen), then make clean and ./configure
# --with-arch=i686 ... again, then build more.
#
# FIXME: did i get this right?
#
# FIXME: what if host system already has a perfectly usable 64bit Xen >=
#        4.4 installation?  then this is a waste of time and effort...
#
# FIXME: i think i ALWAYS want to build xen.gz if xen is
#        enabled... otherwise we have no way of guaranteing that the
#        pre-built hypervisor installed in stage2 is the correct version
#        (i.e., it could be from the host system or be an old build).  I
#        was originally skipping the detour if PREFIX_EARLY/boot/xen.gz
#        already existed...
#
#
# NOTE: if stage2 xen is disabled, stage2 is going to require bare-metal
#       booting, probably facilited by lots of manual work (as root).
AC_ARG_ENABLE(stage2-xen,
  AS_HELP_STRING([--disable-stage2-xen], [Generate a xen domU config file for booting the stage2 builder]),
  [stage2_xen=$enableval],
  [stage2_xen=yes])

AC_ARG_ENABLE(xen,
  AS_HELP_STRING([--disable-xen], [Disable building Xen in hostprep and final system.]),
  [xen_enabled=$enableval
   AS_IF([test "$xen_enabled" != "yes"], [stage2_xen=no])
  ],
  [xen_enabled=yes
   AS_IF([test "$build_arch" != x86_64],
     [AC_MSG_WARN([
		*************************************************************
		Xen requires a 64bit hypervisor.  The build will go on a
		detour to cross-compile the hypervisor automatically.  This
		entails reconfiguring, building the cross-compilation
		toolchain, cross-compiling xen, removing the
		cross-compilation toolchain, reconfiguring back to the
		original config, and carrying on with the build.  Log files
		may look confusing, and it may take longer than expected,
		but it does work.

                If you don't really care about Xen, you can disable it in
                both the bootstrap and final systems w/ the --disable-xen
                flag.
                *************************************************************])
     ])
  ])

AC_SUBST(XEN_ENABLED, $xen_enabled)
AC_SUBST(STAGE2_XEN, $stage2_xen)

# FIXME: we probably want an automake conditional for building the final
#        stage xen package.  for example, if we disable xen and build32
#        bit, the final build stage xen package will silently not build
#        the xen hypervisor... making it pretty useless.
AM_CONDITIONAL([XEN_ENABLED], [test x$xen_enabled = xyes])

# xen detour
#
AC_ARG_WITH(stage1-xen-only,
  AS_HELP_STRING([--with-stage1-xen-only], [Just build the Xen hypervisor in bootstrap-stage1.  This is usefull when you just need to cross-compile the Xen hypervisor for x86_64.]),
  stage1_xen_only=$withval,
  stage1_xen_only=no)

# only makes sense for x86_64, xen_enabled
AS_IF([test "$stage1_xen_only" = "yes"],
  [AS_IF([test "$xen_enabled" != "yes" -o "$build_arch" != "x86_64"],
    [AC_MSG_ERROR([
		*************************************************************
                Setting --stage1-xen-only only makes sense if Xen is
                enabled and build_arch is x86_64.

		Xen Enabled: $xen_enabled
		Arch: $build_arch
		*************************************************************])
    ])
  ])

AC_SUBST(STAGE1_XEN_ONLY, $stage1_xen_only)
AM_CONDITIONAL([STAGE1_XEN_ONLY], [test x$stage1_xen_only = xyes])


# flags for installation and kickoff of stage2
#
# --with-install-part=/dev/sdXN
#
#     Install bootstrap system into the specified partition.  This is
#     going to assume that the partition and filesystem are already
#     created and it WILL NOT install a bootloader.  This is perfect for
#     when you want to boot the resulting bootstrap system as a virtual
#     machine or dual boot it on the build system.
#
#     NOTE: This will require that our source tree already resides at
#           /ruckus of the specified partition's filesystem.  (Unless
#           we've also supplied --enable-ruckus-part=/dev/sdXN, in which
#           case that will be mounted at /ruckus of the install
#           partition).
#
#
# --enable-stage2-xen
#
#    Generate a xen domU config file for booting into the stage2 bootstrap
#    builder.  This can only be used along with --enable-install-part and will
#    cause bootstrap-early to check for the Xen4 Hypervisor and a kernel w/
#    pvops_dom0 support, triggering their compilation if needed.
#
# FIXME: we don't check for pvops_dom0... we do build tools and
#        cross-compile hypervisor, though.  prior to actually using xen,
#        system bootloader config will need to be edited, any
#        distro-provided xen services need to be disabled, and system will
#        have to be rebooted.  after all that, the following (as root)
#        will give you usable xl commands:
#
#            export toplevel=/path/to/ruckus
#            . $toplevel/utils/bootstrap-early/functions
#
#
# --enable-stage2-auto
#
#    Bootstrap system will automatically kick off the stage2 builder.
#
#    NOTE: It sounds like not such a good idea... you probably want to
#          manually kick it off.  This way nothing can blow up your source
#          tree w/out you explicitly kicking off the command.
#
#          However!  If it takes 2-ish hours to get to this point, and we
#          eventually want to automate the whole build, you want want to
#          have to come back 2 hrs later and kick off the stage2 builder.
#          With this flag (and maybe another one to automagically boot as
#          a domU or reboot), we'll be able to automate the entire build
#          process to a single command to kick-off and come back when it's
#          all done.
#




AC_ARG_ENABLE(stage2-auto,
  AS_HELP_STRING([--enable-stage2-auto], [Bootstrap system will automatically kick off the stage2 builder]),
  [stage2_auto=$enableval],
  [stage2_auto=no])
AC_SUBST(STAGE2_AUTO, $stage2_auto)


AC_ARG_VAR(INIT_SUBMOD_ARGS, Arguments to be passed to init_submodules)
AC_ARG_VAR(GET_SOURCES_ARGS, Arguments to be passed to get_sources)


AM_EXTRA_RECURSIVE_TARGETS([hostprep bootstrap bootstrap-install bootstrap-boot])


# FIXME: check for git version w/ submodule --init --recursive?
#        init_submodules may not work right if these flags are missing...


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


# check for glib
#
# NOTE: pkg-config has an internal glib snapshot that can be used if glib
#       is missing on the host system, but it adds needless compile time
#       (and might not build propperly on some host systems).
#
# NOTE: It looks strange to use pkg-config to check for a build dependency
#       of pkg-config... but it works.  And I was having a hard time doing
#       non-pkg-config lib and header checks for libglib and glib.h.
#
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


# check for extlinux >= 4.x for ext4 support
#
# FIXME: really only needed if we're not doing stage2_xen
AS_IF([test "$stage2_xen" != "yes"],
AC_CACHE_CHECK([for extlinux >= 4.0], [ac_cv_path_EXTLINUX],
  [AC_PATH_PROGS_FEATURE_CHECK([EXTLINUX], [extlinux],
    [[result=`$ac_path_EXTLINUX --version 2>&1 | awk '{print $2}' | awk '{tmp=split($NF,v,"."); if (v[1] < 4) print "no"; else if (v[1] == 4 && v[2] < 0) print "no2"; else print "yes"}'`
     test "x$result" = "xyes" && ac_cv_path_EXTLINUX=$ac_path_EXTLINUX ac_path_EXTLINUX_found=:
    ]],

    [ac_cv_path_EXTLINUX=no
     p_early+=" syslinux"
    ])
  ])
)

# FIXME: if stage2_xen, check for xen hypervisor and useland, add to p_early
#        if needed
#
# FIXME: if stage2_xen, check for pvops_dom0 kernel, maybe add to p_early if
#        needed?
#
# FIXME: if stage2_xen, check that we're actually booted into Xen.  this might
#        require root...? not sure. check /proc/xen?
#
# FIXME: do i really want to tackle building Xen in bootstrap-early?  Maybe
#        just xen itself (i.e., don't check for other xen deps? and just
#        print a warning regarding kernel and being booted into xen?)
#
xen_detour=no
AS_IF([test "$xen_enabled" != "no"],
  [# We've got a few extra deps for Xen to check for
   #
   # NOTE: as86, ld86, bcc, and iasl or only needed for x86* builds.  That's
   #       really all we're supporting for now, but this was pretty easy to
   #       copy-n-tweak from xen's tools/configure.ac, so I did.
   case "$build_arch" in
     i[[3456]]86|x86_64)
       AC_PATH_PROG([AS86], [as86])
       AC_PATH_PROG([LD86], [ld86])
       AC_PATH_PROG([BCC], [bcc])
       AS_IF([test -z "$AS86" -o -z "$LD86" -o -z "$BCC"], [p_early+=" dev86"])
       AC_PATH_PROG([IASL], [iasl])
       AS_IF([test -z "$IASL"], [p_early+=" acpica"])
       ;;
   esac

   AC_CHECK_LIB([aio], [io_setup], [], [p_early+=" libaio"])

   p_early+=" xen"

   AS_IF([test "$build_cpu" != "x86_64"],
     [case "$build_arch" in
        *64)
          ;;
        *)
          xen_detour=yes
          ;;
      esac
     ])
  ]
)
AC_SUBST(XEN_DETOUR, $xen_detour)
AM_CONDITIONAL([XEN_DETOUR], [test x$xen_detour = xyes])


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

		ln -fs $CLFS$PREFIX_CROSS $PREFIX_CROSS
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

		ln -fs $CLFS$PREFIX_FINAL $PREFIX_FINAL
		*************************************************************])
  ])





AC_SUBST(p_early)
AC_SUBST(p_stage1)
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
  packages/syslinux/Makefile
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
echo "  Xen Enabled..........: $xen_enabled"
echo "  Xen Detour...........: $xen_detour"
echo "  Stage1 Xen Only......: $stage1_xen_only"
echo "  Stage2 Xen...........: $stage2_xen"
echo "  Stage2 Auto..........: $stage2_auto"
echo "  Install Device.......: $install_dev"
echo "  Ruckus Device........: $ruckus_dev"
echo "  Ruckus Path..........: $ruckus_path"
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


# FIXME: we need bootstrap-early to be OUTSIDE of CLFS if we're going to
#        boot via xen.  otherwise, when we umount $CLFS prior to xl
#        create, we'll also lose our xen userland...
#
# FIXME: We probably also need to make sure that all the src/ sourcetrees
#        have been bootstrapped prior to booting into the stage2 builder
#        w/out the bootstrap-early content...  And we probably need to
#        build bootstrap-early prior to even attempting to bootstrap all
#        the src submodules.
#
#        This can be accomplished by iterating through all the packages/*
#        subidrs and calling autoreconf_generic in sourcecode_dir if
#        SOURCE_URL, Makefile, nor configure exists in them (with a few
#        exceptions, I'm sure).  This should be the 1st thing the
#        bootstrap target does (now that hostprep is seperate).
#
# FIXME: Using --prefix for PREFIX_EARLY.  However, this means that we now
#        have to worry about root perms for bootstrap-early's install
#        commands...  /usr/local is the default, after all...
#
#        Default value is $BUILD_ROOT/bootstrap-early... if you override
#        it, it's up to you to ensure write permissions.
