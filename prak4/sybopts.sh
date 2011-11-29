#!/bin/sh
###########################################################################
#
# sybopts.sh -- Helper script for UNIX makefile that builds dblibrary
#    applications. This script uses the value of  the SYBPLATFORM 
#    environment variable to determine what platform specific
#    libraries must be linked with the application.
#
# Usage:
#  sybopts.sh <args>
#
#  where <args> can be one or more of the following:
#
#     compile -- Based on SYBPLATFORM, echoes the compiler command
#	and platform-specific compile flags. 
#     dblib -- Based on SYBPLATFORM, echoes the list of required
#	lower-layer Sybase libraries that must be linked with the
#	application.
#     syslibs -- Based on SYBPLATFORM, echoes the list of required
#	system (i.e., non-Sybase) libraries that must be linked with the
#	application.
#     verify -- Check SYBPLATFORM setting, if ok, echoes the library
#	names that will be used. If not ok, returns non-zero. This
#	argument must appear by itself with no other arguments.
#
# Example:
#       COMPILE = `sh sybopts.sh compile` $(INCLUDE) $(DEFS)
#
###########################################################################

SET_SYBPLATFORM_MSG="Unknown machine type. Please set SYBPLATFORM variable"
BAD_ARGUMENT_MSG="Unknown argument"
SYBPLATFORM_MSG="Check the value of your SYBPLATFORM environment variable."

#
# CC: Compiler/Linker name
# CFLAGS: Compiler/linker flags
# DBLIB: Sybase dblibrary. These come in 32 and 64 bit versions.
# SYSLIBS: The list of system libraries that are required in order
#	to build a Sybase Open Client/Server application, in the order
#	in which they must be linked.
#
case "${SYBPLATFORM}" in
	axposf)		CC="cc";
			CFLAGS="-g";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic";
			SYSLIBS="-ldnet_stub" ;;
	hpia)		CC="cc";
			CFLAGS="-g";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic";
			SYSLIBS="-Wl,-E,+s" ;;
	hpia64)		CC="cc";
			CFLAGS="-g -DSYB_LP64 +DD64";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64";
			SYSLIBS="-Wl,-E,+s" ;;
	hpux)		CC="cc";
			CFLAGS="-g";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic";
			SYSLIBS="-Wl,-E,+s" ;;
	hpux64)		CC="cc";
			CFLAGS="-g -DSYB_LP64 +DA2.0W +DS2.0 +Z";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64";
			SYSLIBS="-Wl,-E,+s" ;;
	linux)		CC="cc -m32 ";
			CFLAGS="-g";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic";
			SYSLIBS="-ldl" ;;
	linuxamd64)	CC="cc -m64 ";
			CFLAGS="-g -DSYB_LP64";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64";
			SYSLIBS="-ldl" ;;
	linuxia64)	CC="cc";
			CFLAGS="-g -DSYB_LP64";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64";
			SYSLIBS="-ldl" ;;
	ibmplinux)	CC="xlc -q32";
			CFLAGS="-g ";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic";
			SYSLIBS="-ldl" ;;
	ibmplinux64)	CC="xlc -q64";
			CFLAGS="-g -DSYB_LP64";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64";
			SYSLIBS="-ldl" ;;
	macosx)		CC="cc";
			CFLAGS="-g";
			DBLIB="-lsybdb -lsybtcl -lsybcs -lsybcomn -lsybintl";
			UNILIB="-lsybunic" ;;
	rs6000)		CC="xlc ";
			CFLAGS="-g";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic" ;;
	rs600064)       CC="xlc";
			CFLAGS="-g -q64 -DSYB_LP64";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64" ;;
	sgi)		CC="cc";
			CFLAGS="-g -n32 -mips3";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic" ;;
	sgi64)		CC="cc";
			CFLAGS="-g -64 -mips3 -DSYB_LP64";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64" ;;
	sun_svr4)       CC="cc";
			CFLAGS="-g";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic" ;;
	sun_svr464)     CC="cc";
			CFLAGS="-DSYB_LP64 -xarch=v9 -xchip=ultra -Xa -xO0";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64";
			SYSLIBS="-lnsl" ;;
	sunx86)		CC="cc";
			CFLAGS="-g";
			DBLIB="-lsybdb";
			UNILIB="-lsybunic" ;;
	sunx8664)	CC="cc";
			CFLAGS="-g -DSYB_LP64 -xtarget=opteron -xarch=amd64";
			DBLIB="-lsybdb64";
			UNILIB="-lsybunic64" ;;
	*)		echo " $SET_SYBPLATFORM_MSG"; exit 1 ;;
esac

#
# A single argument of "verify" means that we should print
# the value of SYBPLATFORM, print the choices that will be made
# for dblibs syslibs, etc., then exit with a status of 0. If
# SYBPLATFORM is incorrect, we will not get here.
#
if [ $1 = "verify" ]
  then
	echo "SYBPLATFORM is $SYBPLATFORM"
	echo "Compiling with $CC $CFLAGS"
	echo "Sybase Client library: $DBLIB"
	echo "System libraries: $SYSLIBS"
	exit 0
fi

#
# Build a string to echo what was asked for:
#
ECHOSTRING=""
for a
do
	case "$a" in
		compile)	ECHOSTRING="$ECHOSTRING $CC $CFLAGS" ;;
		syslibs)	ECHOSTRING="$ECHOSTRING $SYSLIBS " ;;
		dblib)          ECHOSTRING="$ECHOSTRING $DBLIB" ;;
		unilib)         ECHOSTRING="$ECHOSTRING $UNILIB" ;;
		*)	        echo "$0: $a: $BAD_ARGUMENT_MSG "; exit 1;;
	esac
done

echo "$ECHOSTRING"
exit 0

