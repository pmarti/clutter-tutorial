# Configure.in
#
#  This file tests for various compiler features needed to configure
#  the gtkmm package.  Original skeleton was provided by Stephan Kulow.
#  All tests were written by Tero Pulkkinen, Mirko Streckenbach, and
#  Karl Nelson.
#
#
#  Copyright 2001 Free Software Foundation
#  Copyright 1999 gtkmm Development Team
#  Copyright 1998 Stephan Kulow

AC_INIT(config.h.in)
#AC_PREREQ(2.59)

#########################################################################
#  Version and initialization
#########################################################################
CLUTTER_DOC_MAJOR_VERSION=0
CLUTTER_DOC_MINOR_VERSION=2
CLUTTER_DOC_MICRO_VERSION=0

CLUTTER_DOC_VERSION=$CLUTTER_DOC_MAJOR_VERSION.$CLUTTER_DOC_MINOR_VERSION.$CLUTTER_DOC_MICRO_VERSION
CLUTTER_DOC_RELEASE=$CLUTTER_DOC_MAJOR_VERSION.$CLUTTER_DOC_MINOR_VERSION
AC_DEFINE_UNQUOTED(CLUTTER_DOC_MAJOR_VERSION, $CLUTTER_DOC_MAJOR_VERSION)
AC_DEFINE_UNQUOTED(CLUTTER_DOC_MINOR_VERSION, $CLUTTER_DOC_MINOR_VERSION)
AC_DEFINE_UNQUOTED(CLUTTER_DOC_MICRO_VERSION, $CLUTTER_DOC_MICRO_VERSION)
AC_DEFINE_UNQUOTED(CLUTTER_DOC_VERSION, "$CLUTTER_DOC_VERSION")
AC_SUBST(CLUTTER_DOC_VERSION)
AC_SUBST(CLUTTER_DOC_RELEASE)

AC_CONFIG_AUX_DIR(scripts)

dnl For automake.
VERSION=$CLUTTER_DOC_VERSION
PACKAGE=clutter-tutorial

dnl Initialize automake stuff
AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

dnl Specify a configuration file (no autoheader)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE
AL_ACLOCAL_INCLUDE(scripts)


#########################################################################
#  Configure arguments
#########################################################################

#########################################################################
#  Environment Checks
#########################################################################
AC_PROG_CPP
AC_PROG_MAKE_SET
AC_CANONICAL_HOST
AM_DISABLE_STATIC
AM_PROG_LIBTOOL

#########################################################################
#  Dependancy checks
#########################################################################
PKG_CHECK_MODULES(CLUTTER_DOC, clutter-1.0 clutter-gtk-0.10)
AC_SUBST(CLUTTER_DOC_CFLAGS)
AC_SUBST(CLUTTER_DOC_LIBS)

## See scripts/dk-warn.m4 for usage details
DK_ARG_ENABLE_WARNINGS([CLUTTER_DOC_WARNING_FLAGS],
                       [-Wall -w1],
                       [-Wall -Wextra -w1])

# Dummy conditional just to make automake-1.4 happy.
# We need an always-false condition in docs/Makefile.am.
AM_CONDITIONAL(CLUTTER_DOC_FALSE,[false])

# HACK:  Assign a dummy in order to prevent execution of autoheader by the
# maintainer-mode rules.  That would fail since we aren't using autoheader.
AUTOHEADER=':'

AC_OUTPUT([
  Makefile
  scripts/Makefile

  docs/Makefile
    docs/tutorial/Makefile
    docs/tutorial/icons/Makefile
    docs/tutorial/figures/Makefile

  examples/Makefile
])
