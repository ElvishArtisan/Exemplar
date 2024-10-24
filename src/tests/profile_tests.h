// profile_tests.h
//
// Tests for the Profile configurator class.
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

#ifndef PROFILE_TESTS_H
#define PROFILE_TESTS_H

#include <QObject>

#include <profile.h>

#define PROFILE_TESTS_USAGE "\n\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject();

 private:
  bool RunTestsNoSectionIds(Profile *p,int *pass_ctr,int *fail_ctr) const;
  bool RunTestsSectionIds(Profile *p,int *pass_ctr,int *fail_ctr) const;
  bool RunTestsUnified(Profile *p,int *pass_ctr,int *fail_ctr) const;
  void Title(const QString &title) const;
  bool SingleResult(const QString &title,bool result_ok,
		    int *pass_ctr,int *fail_ctr) const;
  bool Result(bool state,int *pass_ctr,int *fail_ctr) const;
  void Result(const QString &title,bool result_ok,bool found,
	      int *pass_ctr,int *fail_ctr) const;
  void DumpList(const QString &title,const QStringList &list) const;
  void DumpList(const QString &title,const QList<int> &list) const;
  void DumpList(const QString &title,const QList<double> &list) const;
  void DumpList(const QString &title,const QList<QTime> &list) const;
};


#endif  // PROFILE_TESTS_H
