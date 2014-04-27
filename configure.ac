# -*- mode: autoconf -*-
AC_PREREQ(2.62)
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


# FIXME: isn't there a better way to do this? some m4 or ac macro?
abs_srcdir=$(cd $srcdir && pwd)
BUILDROOT=$abs_srcdir/.build
#PREFIX_EARLY=$BUILDROOT/bootstrap-early
PREFIX_EARLY=$prefix
PREFIX_CROSS=/cross-tools
PREFIX_FINAL=/tools
CLFS=$BUILDROOT/baby
AC_ARG_VAR(BUILDROOT, The build dir)
AC_ARG_VAR(PREFIX_EARLY, [Prefix for bootstrap-early (i.e., --prefix)])
AC_ARG_VAR(PREFIX_CROSS, Prefix for bootstrap-cross)
AC_ARG_VAR(PREFIX_FINAL, Prefix for bootstrap-final)
AC_ARG_VAR(CLFS, Location of baby filesystem during build)


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
AC_ARG_ENABLE(xen,
  AS_HELP_STRING([--disable-xen], [Disable building Xen in bootstrap and final system.]),
  xen_enabled=$enableval,
  [AS_IF([test "$build_arch" != x86_64],
     [# need to check for prebuilt x86_64 hypervisor
      AS_IF([test -f $PREFIX_EARLY/boot/xen.gz],
        xen_enabled=yes,
        [AC_MSG_WARN([
		*************************************************************
		Xen requires a 64bit hypervisor.  The build will go on a
		detour to cross-compile the hypervisor automatically.  This
		entails reconfiguring, building the cross-compilation
		toolchain, cross-compiling xen, removing the
		cross-compilation toolchain, reconfiguring back to the
		original config, and carrying on with the build.  Log files
		may look confusing, and it may take longer than expected,
		but it does work.  ;-)

                If you don't really care about Xen, you can disable it in
                both the bootstrap and final systems w/ the --disable-xen
                flag.
                *************************************************************])
	 xen_enabled=yes
        ])
     ],
     xen_enabled=yes)
  ])

# FIXME: we probably want an automake conditional for building the final
#        stage xen package.  for example, if we disable xen and build32
#        bit, the final build stage xen package will silently not build
#        the xen hypervisor... making it pretty useless.
AM_CONDITIONAL([XEN_ENABLED], [test x$xen_enabled = xyes])

AC_ARG_WITH(stage1-xen-only,
  AS_HELP_STRING([--with-stage1-xen-only], [Just build the Xen hypervisor in bootstrap-stage1.  This is usefull when you just need to cross-compile the Xen hypervisor for x86_64.]),
  stage1_xen_only=$withval,
  stage1_xen_only=no)

# FIXME: make sure xen is enabled?  error if disabled.
AC_SUBST(STAGE1_XEN_ONLY, $stage1_xen_only)

# flags for installation and kickoff of stage2
#
# --enable-install-part=/dev/sdXN
#
#     Install bootstrap system into the specified partition.  This is
#     going to assume that the partition and filesystem are already
#     created and it WILL NOT install a bootloader.  This is perfect for
#     when you want to boot the resulting bootstrap system as a virtual
#     machine or dual boot it on the build system.
#
#     NOTE: This will require that our source tree already resides at
#           /ruckus of the specified partition's filesystem.
#
# --enable-install-dev=/dev/sdX
#
#     Install bootstrap system onto the specified block device.  ALL DATA
#     ON DEVICE WILL BE LOST, because the sandbox stage will re-partition
#     and create filesystems as needed.  The bootstrap_install stage will
#     install and configure the bootloader.
#
#     FIXME: that really only makes sense for removable media...
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
# FIXME: I'm pretty sure I'm going to remove the -raid0 and -dev
#        installation flags and put that code into a standalone build
#        wrapper script.  The driver behind this is: boot off of LiveCD,
#        clone wrapper script, execute it.  The script can then set up the
#        HDDs appropriately and clone the whole ruckus tree into them
#        AFTERWARDS.  Otherwise, we have to be able to clone the whole
#        ruckus tree on the LiveCD, which may not work (i.e., not enough
#        drive space?)
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
# --with-fstype=ext2
#
#    Specify fstype for bootable bootstrap system (created in sandbox
#    stage).  Valid options are ext2, ext3, ext4.  You almost certainly
#    want ext2 to avoid journaling overhead and improve disk performance,
#    but I'd like to benchmark this before hardcoding it.
#
# FIXME: fstype might get swallowed up into build script along with -raid0
#        and -dev sandbox targets.
#
# NOTE: Only a single --enable-install* option can be supplied.  Default
#       is --disable-install.
#
# FIXME: default SHOULD be to require either a valid --enable-install-part
#        or --disable-install.
#
# NOTE: If configured w/out an --enable-install* option, the sandbox and
#       install targets will be disabled.
#
# FIXME: still true?

install_type=none
AC_ARG_ENABLE(install,
  AS_HELP_STRING([--disable-install], [Disable sandbox and install targets (enable via specific --enable-install-* options) [default=enabled]]),
  build_install=$enableval,
  build_install=yes)

AC_ARG_ENABLE(install-part,
  AS_HELP_STRING([--enable-install-part=PARTITION], [Install into specified PARTITION.]),
  [install_part=$enableval
   install_type=part
   build_install=yes
   # check that partition is mounted
   mtab=`grep "^$install_part" /proc/mounts`
   AS_IF([test -z "$mtab"],
     [AC_MSG_ERROR([
		*************************************************************
		The installation partition must already exist and be
		mounted.  Note that if you're using LVM, you need to
		specify as /dev/mapper/VG-LV (i.e., /dev/VG/LV won't
		work).

                You specified: --enable-install-part=$install_part
		*************************************************************])
     ])
   # check that $top_srcdir is at /ruckus of partition
   mp=`echo $mtab | awk '{print $2}'`
   AS_IF([test "$mp/ruckus" != "$abs_srcdir"],
     [AC_MSG_ERROR([
		*************************************************************
		The srcdir must be at /ruckus of the installation
		partition (e.g., /path/to/partition/ruckus).

                You specified: --enable-install-part=$install_part
		Mountpoint: $mp
		Srcdir: $abs_srcdir
		*************************************************************])
     ])
   # check that $toplevel/.. is writeable
   #
   # FIXME: i might not need this if we're going to leave CLFS pointing at
   #        .build/baby until boot-prep.
   AS_IF([touch $mp/.foo],
     [rm $mp/.foo],
     [AC_MSG_ERROR([
		*************************************************************
		The installation partition must be writeable by the build
		user.

                You specified: --enable-install-part=$install_part
		*************************************************************])
     ])
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

AS_IF([test "$build_install" = "yes" -a "$install_type" = "none"],
  [AC_MSG_ERROR([
		*************************************************************
		You must either specify a valid installation partition via
		the --enable-install-part option or disable installation
		entirely via the --disable-install option.
		*************************************************************])
  ])

# FIXME: handle multiple --enable-install flags

# FIXME: handle --enable-install-* w/out arg (install_dev == yes)

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




AC_ARG_ENABLE(stage2-xen,
  AS_HELP_STRING([--enable-stage2-xen], [Generate a xen domU config file for booting the stage2 builder]),
  [stage2_xen=$enableval
   AS_IF([test "$install_type" != "part" -a "$install_type" != "none"],
     AC_MSG_ERROR([
		*************************************************************
		The stage2 Xen option is incompatible with any install type
		other than 'part' or 'none'.  You chose '$install_type'.
		*************************************************************]))
   # check for xen_enabled
  ],
  [stage2_xen=no])


AC_ARG_ENABLE(stage2-auto,
  AS_HELP_STRING([--enable-stage2-auto], [Bootstrap system will automatically kick off the stage2 builder]),
  [stage2_auto=$enableval],
  [stage2_auto=no])



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
#     NOTE: bootstrap-early, bootstrap-cross, bootstrap-stage1, clean,
#           reconfig, bootstrap-cross, bootstrap-stage1 if we're building
#           on or for 32bit
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


AC_ARG_VAR(INIT_SUBMOD_ARGS, Arguments to be passed to init_submodules)
AC_ARG_VAR(GET_SOURCES_ARGS, Arguments to be passed to get_sources)

AM_EXTRA_RECURSIVE_TARGETS([bootstrap])


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
need_cross_compiled_xen=no
AS_IF([test "$stage2_xen" != "no"],
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
     [AS_IF([test ! -f $PREFIX_EARLY/boot/xen.gz],
        [need_cross_compiled_xen=yes])
     ])
  ]
)
AC_SUBST(NEED_CROSS_COMPILED_XEN, $need_cross_compiled_xen)
AM_CONDITIONAL([NEED_CROSS_COMPILED_XEN],
  [test x$need_cross_compiled_xen = xyes])


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
case "$install_type" in
  part|none)
    ;;
  *)
    echo "  Bootstrap fstype.....: $build_fstype"
esac
echo "  Xen stage2...........: $stage2_xen"
echo "  Cross-compile Xen....: $need_cross_compiled_xen"
echo "  Stage1 Xen Only......: $stage1_xen_only"
echo "  Auto stage2..........: $stage2_auto"
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
#        --with-prefix-early=
#
#        or maybe we just go ahead and use the standard --prefix for it?
#
#        Either way, putting it outside our source tree means we should
#        probably plumb up an uninstall target...
#
# FIXME: We probably also need to make sure that all the src/ sourcetrees
#        have been bootstrapped prior to booting into the stage2 builder
#        w/out the bootstrap-early content...  And we probably need to
#        build bootstrap-early prior to even attempting to bootstrap all
#        the src submodules.
#
# FIXME: Using --prefix for PREFIX_EARLY.  However, this means that we now
#        have to worry about root perms for bootstrap-early's install
#        commands...  /usr/local is the default, after all...
#
#        Could take a classic not-my-problem stance here... which is
#        probably the most appropriate thing to do... but then I should
#        probably also split bootstrap and bootstrap-install into seperate
#        targets... chich breaks my current scripts.
#
#        Really can't split boostrap into build/install targets because the
#        install from package a is going to be required to build package
#        b...
#
#        1) Gonna have to use sudo for install targets, which means we
#           should probably check for it.  Maybe add a --with-sudo-install
#           flag?
#
#        or
#
#        2) Check for user writability in PREFIX_EARLY and use the scripts
#           as-is?  Feels pretty odd to require writability in /usr/local...
#
#        or both and let the user decide?

