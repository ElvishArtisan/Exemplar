// run_tests.h
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

#ifndef RUN_TESTS_H
#define RUN_TESTS_H

#include <QObject>

#include <profile.h>

#define RUN_TESTS_USAGE "\n\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject();

 private:
  bool RunLegacyTests(Profile *p,int *pass_ctr,int *fail_ctr) const;
  bool RunExtendedTests(Profile *p,int *pass_ctr,int *fail_ctr) const;
  bool RunSendmailTests(int *pass_ctr,int *fail_ctr) const;
  bool RunDumpTests(int *pass_ctr,int *fail_ctr);
  bool DumpTest(const QString &fixture,const QString &exemplar,
		bool use_sect_ids,const QString &name,
		int *pass_ctr,int *fail_ctr);
};


#endif  // RUN_TESTS_H
