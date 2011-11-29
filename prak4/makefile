############################################################################
#
# UNIX Makefile for the DB-Library example programs. To use this
# makefile, you need to set the following environment variables:
#       SYBASE	  	Set to the Sybase installation directory.
#       SYBASE_OCS      Set to the Open Client products directory
#       SYBPLATFORM     Set to the Sybase abbreviation for your platform.
#
# The shell script sybopts.sh is used to create platform-specific command
# line options for compilation and linking with Sybase and system libraries.
# The SYBPLATFORM environment variable must be set in order for this
# script to work correctly. Please see README file for valid settings.
#
#############################################################################
#
# Directories and search paths.
#
#############################################################################

SHELL     = /bin/sh
RELDIR    = $(SYBASE)/$(SYBASE_OCS)

# Sybase include directory
INCDIR    = $(RELDIR)/include
#
# Modify the following line to use debug/nondebug libraries
#
#LIBDIR   = $(RELDIR)/devlib
LIBDIR    = $(RELDIR)/lib

# Specify the include and library search paths for the compiler
INCLUDE   = -I. -I$(INCDIR)
LIBPATH   = -L$(LIBDIR)
#
##############################################################################
#
# Macros for compile/link lines
#
##############################################################################
#
# Specify the compile-time defines.
# Use -DDEBUG only if linking with devlib/*
#
#DEFS = -DDEBUG
#
# Define the compiler command and compile flags.
#
# `sh sybopts.sh compile` echoes a platform-specific command to invoke the C
# compiler. You can replace this invocation with the comand to invoke your
# preferred compiler if you like.
#
COMPILE = `$(SHELL) sybopts.sh compile` $(INCLUDE) $(DEFS)

#
# Define the list of libraries that must be linked with in order to call
# DB-Library
#
# 'sh sybopts.sh' with argument 'dblib' echoes the DB-Library.
#
# `sh sybopts.sh syslibs` echoes a platform-specific list of system libraries
# that are required.
#
DBLIB = `$(SHELL) sybopts.sh dblib`
UNILIB = `$(SHELL) sybopts.sh unilib`
SYSLIBS = `$(SHELL) sybopts.sh syslibs`
##############################################################################
#
# The targets and how to build them.
#
##############################################################################

all:  prakt1 

prakt1: prakt1.c
	@ printf "$(COMPILE) prak1.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) prakt1.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

clean:
	rm -f prakt1 *.o
