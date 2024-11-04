// test_methods.h
//
// Miscellaneous methods for reporting the results of unit tests.
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

#ifndef TEST_METHODS_H
#define TEST_METHODS_H

#include <QString>
#include <QTime>

void PrintTitle(const QString &title);
bool PrintSingleResult(const QString &title,bool result_ok,
		  int *pass_ctr,int *fail_ctr);
bool PrintResultState(bool state,int *pass_ctr,int *fail_ctr);
void PrintDualResult(const QString &title,bool result_ok,bool found,
		     int *pass_ctr,int *fail_ctr);
void DumpList(const QString &title,const QStringList &list);
void DumpList(const QString &title,const QList<int> &list);
void DumpList(const QString &title,const QList<double> &list);
void DumpList(const QString &title,const QList<QTime> &list);


#endif  // TEST_METHODS_H
