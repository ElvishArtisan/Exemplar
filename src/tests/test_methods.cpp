// test_methods.cpp
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

#include <stdio.h>
#include <stdlib.h>

#include "test_methods.h"

void PrintTitle(const QString &title)
{
  printf("  %55s ... ",title.toUtf8().constData());
  fflush(stdout);
}


bool PrintSingleResult(const QString &title,bool result_ok,
		  int *pass_ctr,int *fail_ctr)
{
  PrintTitle(title+" Value Result");
  PrintResultState(result_ok,pass_ctr,fail_ctr);

  return result_ok;
}


bool PrintResultState(bool state,int *pass_ctr,int *fail_ctr)
{
  if(state) {
    printf("PASS\n");
    (*pass_ctr)++;
  }
  else {
    printf("FAIL\n");
    (*fail_ctr)++;
  }

  return state;
}


void PrintDualResult(const QString &title,bool result_ok,bool found,
		     int *pass_ctr,int *fail_ctr)
{
  PrintTitle(title+" Value Result");
  PrintResultState(result_ok,pass_ctr,fail_ctr);

  PrintTitle(title+" Found Result");
  PrintResultState(found,pass_ctr,fail_ctr);
}


void DumpList(const QString &title,const QStringList &list)
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %s\n",i,list.at(i).toUtf8().constData());
  }
}


void DumpList(const QString &title,const QList<int> &list)
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %d\n",i,list.at(i));
  }
}


void DumpList(const QString &title,const QList<double> &list)
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %10.7lf\n",i,list.at(i));
  }
}


void DumpList(const QString &title,const QList<QTime> &list)
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %s\n",i,list.at(i).toString("hh:mm:ss").toUtf8().constData());
  }
}
