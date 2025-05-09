## Makefile.am
##
## Makefile.am for Exemplar src/lib
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
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" @QT5_CLI_CFLAGS@ -Wno-strict-aliasing -std=c++11 -fPIC
MOC = @QT_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

lib_LTLIBRARIES = libexemplar.la
dist_libexemplar_la_SOURCES = cmdswitch.cpp cmdswitch.h\
                              profile.cpp profile.h\
                              sendmail.cpp sendmail.h

libexemplar_la_LDFLAGS = -release $(VERSION)

CLEANFILES = *~\
             moc_*\
             *.lib\
             *.obj\
             *.qm

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*
