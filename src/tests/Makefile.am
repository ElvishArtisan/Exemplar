## Makefile.am
##
## Makefile.am for Exemplar src/tests/
##
## (C) Copyright 2024 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing -std=c++11 -fPIC -I$(top_srcdir)/src/lib @QT5_CLI_CFLAGS@
LIBS = -L$(top_srcdir)/src/lib
MOC = @QT_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

tests:	all
	./run_tests

bin_PROGRAMS = dump_profile\
               run_tests

dist_dump_profile_SOURCES = dump_profile.cpp dump_profile.h\
                            test_methods.cpp test_methods.h
nodist_dump_profile_SOURCES = moc_dump_profile.cpp
dump_profile_LDADD = -lexemplar @QT5_CLI_LIBS@

dist_run_tests_SOURCES = run_tests.cpp run_tests.h\
                         test_methods.cpp test_methods.h
nodist_run_tests_SOURCES = moc_run_tests.cpp
run_tests_LDADD = -lexemplar @QT5_CLI_LIBS@

CLEANFILES = *~\
             *.idb\
             *ilk\
             *.obj\
             *.pdb\
             moc_*

MAINTAINERCLEANFILES = *~\
                       Makefile.in
