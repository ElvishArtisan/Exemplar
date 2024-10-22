// v1_tests.h
//
// v1.x tests for the Profile configurator class.
//
//   (C) Copyright 2024 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef V1_TESTS_H
#define V1_TESTS_H

#include <QObject>

#include <profile.h>

#define V1_TESTS_USAGE "\n\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject();

 private:
  bool RunV1Tests(const QString &desc,Profile *p,
		  int *pass_ctr,int *fail_ctr) const;
  void Title(const QString &title) const;
  bool Result(bool state,int *pass_ctr,int *fail_ctr) const;
  void Result(const QString &title,bool result_ok,bool ok_ok,
	      int *pass_ctr,int *fail_ctr) const;
};


#endif  // V1_TESTS_H
