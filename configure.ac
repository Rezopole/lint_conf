dnl {
dnl  lint_conf, checks for missing, unusued, etc, declarations in switches and routers configurations, and more ...
dnl  Copyright (C) 2016-2020 Jean-Daniel Pauget <jdpauget@rezopole.net>
dnl  
dnl  This program is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU General Public License
dnl  as published by the Free Software Foundation; either version 2
dnl  of the License, or (at your option) any later version.
dnl  
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU General Public License for more details.
dnl  
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program; if not, write to the Free Software
dnl  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
dnl }


dnl Process this file with autoconf to produce a configure script.
AC_INIT([lint_conf], [0.0.3], [sysadm@rezopole.net])
AC_CONFIG_SRCDIR([./lint_conf.cpp])


dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET


dnl quieten some complains from missing files
AM_INIT_AUTOMAKE([foreign])

dnl the magic of additionnal local m4 ?
AC_CONFIG_MACRO_DIR([m4])

dnl autoheader
AM_CONFIG_HEADER(config.h)
AC_DEFINE_UNQUOTED([LINT_CONF_VERSION], ["$VERSION"], [the release-version of lint_conf])
AC_SUBST(LINT_CONF_VERSION,[$VERSION])

dnl Check for tools

AC_PROG_CC
AC_PROG_CXX
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

dnl Check for compiler environment

AC_C_CONST

dnl JD's standard minimum CFLAG
CFLAGS="$CFLAGS -O2 -Wall -Wcast-qual -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes"
CXXFLAGS="$CXXFLAGS -O2 -Wall -Woverloaded-virtual -Wcast-qual -Wwrite-strings -Wredundant-decls"



dnl check for mac-osX gcc stl variant
case "$target" in
    *-apple-darwin*)
	dnl some older darwin wanted that .... CXXFLAGS="$CXXFLAGS -stdlib=libstdc++"
	CXXFLAGS="$CXXFLAGS-std=libc++"
	AC_MSG_RESULT([stdlib variant : -stdlib=libstdc++])
	;;
    *)
	:
	;;
esac


dnl  dnl concatenation of include dirs
dnl  CFLAGS="$INCPATH $CFLAGS"
dnl  CXXFLAGS="$INCPATH $CXXFLAGS"

dnl Finally create all the generated files
AC_OUTPUT([
	Makefile
	lint_conf.1
])


