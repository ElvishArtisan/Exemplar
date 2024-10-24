// profile_tests.cpp
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

#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>

#include "profile_tests.h"

MainObject::MainObject()
  : QObject()
{
  int total_pass=0;
  int total_fail=0;
  QStringList lines;
  QString err_msg;
  QStringList err_msgs;
  
  printf("**** Legacy Format ****\n");
  Profile *p=new Profile();
  if(!p->loadFile("../../fixtures/legacy.conf",&err_msg)) {
    fprintf(stderr,"profile_tests: failed to open test data [%s]\n",
	    err_msg.toUtf8().constData());
    exit(1);
  }
  RunLegacyTests(p,&total_pass,&total_fail);
  delete p;
  printf("\n");

  printf("**** Extended Format ****\n");
  p=new Profile(true);
  if(!p->loadFile("../../fixtures/extended.conf",&err_msg)) {
    fprintf(stderr,"profile_tests: failed to open test data [%s]\n",
	    err_msg.toUtf8().constData());
    exit(1);
  }
  RunExtendedTests(p,&total_pass,&total_fail);
  delete p;
  printf("\n");

  printf("**** Unified Directory ****\n");
  p=new Profile();
  p->loadDirectory("../../fixtures","extended_part*.conf",&err_msgs);
  for(int i=0;i<err_msgs.size();i++) {
    fprintf(stderr," Error: %s\n",err_msgs.at(i).toUtf8().constData());
  }
  RunLegacyTests(p,&total_pass,&total_fail);
  printf("\n");

  printf("%d / %d tests passed\n",total_pass,total_pass+total_fail);

  if(total_fail>0) {
    exit(1);
  }
  exit(0);
}


bool MainObject::RunLegacyTests(Profile *p,int *pass_ctr,int *fail_ctr) const
{
  bool ok=false;
  int pass=0;
  int fail=0;
  bool result_ok=false;

  QStringList sections;
  sections.push_back("Tests");
  SingleResult("List Sections",p->sections()==sections,&pass,&fail);

  result_ok=p->stringValue("Tests","StringValue","of their country!",&ok)==
			   "Now is the time";
  Result("String Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->stringValue("Tests","StringMissing","Not this string!",&ok)==
			   "Not this string!";
  Result("String Missing",result_ok,ok==false,&pass,&fail);
  QStringList strings;
  strings.push_back("Now is the time");
  strings.push_back("for all good men");
  strings.push_back("to come to the aid");
  strings.push_back("of their country!");
  result_ok=(p->stringValues("Tests","StringValue")==strings);
  SingleResult("String Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->intValue("Tests","IntegerValue",4321,&ok)==1234;
  Result("Integer Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->intValue("Tests","IntegerMissing",4321,&ok)==4321;
  Result("Integer Missing",result_ok,ok==false,&pass,&fail);
  QList<int> ints;
  ints.push_back(1234);
  ints.push_back(2345);
  ints.push_back(3456);
  ints.push_back(4567);
  result_ok=(p->intValues("Tests","IntegerValue")==ints);
  SingleResult("Integer Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->hexValue("Tests","HexValue",4321,&ok)==1234;
  Result("Hex Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->hexValue("Tests","HexMissing",4321,&ok)==4321;
  Result("Hex Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->hexValues("Tests","HexValue")==ints);
  SingleResult("Hex Multi-Value",result_ok,&pass,&fail);

  result_ok=p->doubleValue("Tests","DoubleValue",2.71,&ok)==3.1415928;
  Result("Double Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->doubleValue("Tests","DoubleMissing",2.71,&ok)==2.71;
  Result("Double Missing",result_ok,ok==false,&pass,&fail);
  QList<double> doubles;
  doubles.push_back(3.1415928);
  doubles.push_back(6.2831856);
  doubles.push_back(9.4247784);
  doubles.push_back(12.5663712);
  result_ok=(p->doubleValues("Tests","DoubleValue")==doubles);
  SingleResult("Double Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->boolValue("Tests","BoolYesValue",false,&ok)==true;
  Result("BoolYes Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolYesMissing",false,&ok)==false;
  Result("BoolYes Missing",result_ok,ok==false,&pass,&fail);
  QList<bool> on_bools;
  on_bools.push_back(true);
  on_bools.push_back(false);
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","BoolYesValue")==on_bools);
  SingleResult("BoolYes Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->boolValue("Tests","BoolNoValue",true,&ok)==false;
  Result("BoolNo Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolNoMissing",true,&ok)==true;
  Result("BoolNo Missing",result_ok,ok==false,&pass,&fail);
  QList<bool> off_bools;
  off_bools.push_back(false);
  off_bools.push_back(true);
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","BoolNoValue")==off_bools);
  SingleResult("BoolNo Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolOnValue",false,&ok)==true;
  Result("BoolOn Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOnMissing",false,&ok)==false;
  Result("BoolOn Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolOnValue")==on_bools);
  SingleResult("BoolOn Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->boolValue("Tests","BoolOffValue",true,&ok)==false;
  Result("BoolOff Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOffMissing",true,&ok)==true;
  Result("BoolOff Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolOffValue")==off_bools);
  SingleResult("BoolOff Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolTrueValue",false,&ok)==true;
  Result("BoolTrue Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolTrueMissing",false,&ok)==false;
  Result("BoolTrue Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolTrueValue")==on_bools);
  SingleResult("BoolTrue Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolFalseValue",true,&ok)==false;
  Result("BoolFalse Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolFalseMissing",true,&ok)==true;
  Result("BoolFalse Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolFalseValue")==off_bools);
  SingleResult("BoolFalse Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolOneValue",false,&ok)==true;
  Result("BoolOne Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOneMissing",false,&ok)==false;
  Result("BoolOne Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolOneValue")==on_bools);
  SingleResult("BoolOne Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolZeroValue",true,&ok)==false;
  Result("BoolZero Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolZeroMissing",true,&ok)==true;
  Result("BoolZero Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolZeroValue")==off_bools);
  SingleResult("BoolZero Multi-Value",result_ok,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMMorningValue",QTime(10,45,0),&ok)==
    QTime(9,30,0);
  Result("TimeHHMMMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->timeValue("Tests","TimeHHMMMorning Missing",QTime(10,45,0),&ok)==
    QTime(10,45,0);
  Result("TimeHHMMMorning Missing",result_ok,ok==false,&pass,&fail);
  QList<QTime> times;
  times.push_back(QTime(9,30,0));
  times.push_back(QTime(10,30,0));
  times.push_back(QTime(11,30,0));
  times.push_back(QTime(12,30,0));
  result_ok=(p->timeValues("Tests","TimeHHMMMorningValue")==times);
  SingleResult("TimeHHMMMorning Multi-Value",result_ok,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMEveningValue",QTime(22,15,0),&ok)==
    QTime(21,30,0);
  Result("TimeHHMMMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->timeValue("Tests","TimeHHMMEvening Missing",QTime(22,15,0),&ok)==
    QTime(22,15,0);
  Result("TimeHHMMEvening Missing",result_ok,ok==false,&pass,&fail);
  times.clear();
  times.push_back(QTime(21,30,0));
  times.push_back(QTime(22,15,0));
  times.push_back(QTime(23,30,0));
  times.push_back(QTime(23,45,0));
  result_ok=(p->timeValues("Tests","TimeHHMMEveningValue")==times);
  SingleResult("TimeHHMMEvening Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->timeValue("Tests","TimeHHMMSSMorningValue",QTime(10,45,45),&ok)==
    QTime(9,30,45);
  Result("TimeHHMMSSMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->
    timeValue("Tests","TimeHHMMSSMorning Missing",QTime(10,45,45),&ok)==
    QTime(10,45,45);
  Result("TimeHHMMSSMorning Missing",result_ok,ok==false,&pass,&fail);
  times.clear();
  times.push_back(QTime(9,30,45));
  times.push_back(QTime(10,30,45));
  times.push_back(QTime(11,30,45));
  times.push_back(QTime(11,45,45));
  result_ok=(p->timeValues("Tests","TimeHHMMSSMorningValue")==times);
  SingleResult("TimeHHMMSSMorning Multi-Value",result_ok,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMSSEveningValue",QTime(22,45,45),&ok)==
    QTime(21,30,45);
  Result("TimeHHMMSSEvening Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->
    timeValue("Tests","TimeHHMMSSEvening Missing",QTime(10,45,45),&ok)==
    QTime(10,45,45);
  Result("TimeHHMMSSEvening Missing",result_ok,ok==false,&pass,&fail);
  times.clear();
  times.push_back(QTime(21,30,45));
  times.push_back(QTime(22,30,45));
  times.push_back(QTime(22,30,45));
  times.push_back(QTime(23,30,45));
  result_ok=(p->timeValues("Tests","TimeHHMMSSEveningValue")==times);
  SingleResult("TimeHHMMSSEvening Multi-Value",result_ok,&pass,&fail);

  result_ok=p->addressValue("Tests","IpAddressValidValue",
		      QHostAddress("4.3.2.1"),&ok)==QHostAddress("1.2.3.4");
  Result("IpAddress Found (QHostAddress Default)",result_ok,ok==true,
	 &pass,&fail);
  result_ok=p->addressValue("Tests","IpAddressValidMissing",
		      QHostAddress("4.3.2.1"),&ok)==QHostAddress("4.3.2.1");
  Result("IpAddress Missing (QHostAddress Default)",result_ok,ok==false,
	 &pass,&fail);
  result_ok=p->addressValue("Tests","IpAddressValidValue",
			    "4.3.2.1",&ok)==QHostAddress("1.2.3.4");
  Result("IpAddress Found (QString Default)",result_ok,ok==true,&pass,&fail);
  result_ok=p->addressValue("Tests","IpAddressValidMissing",
			    "4.3.2.1",&ok)==QHostAddress("4.3.2.1");
  Result("IpAddress Missing (QString Default)",result_ok,ok==false,&pass,&fail);
  QList<QHostAddress> addrs;
  addrs.push_back(QHostAddress("1.2.3.4"));
  addrs.push_back(QHostAddress("5.6.7.8"));
  addrs.push_back(QHostAddress("6.7.8.9"));
  addrs.push_back(QHostAddress("7.8.9.10"));
  result_ok=(p->addressValues("Tests","IpAddressValidValue")==addrs);
  SingleResult("IpAddress Multi-Value",result_ok,&pass,&fail);

  *pass_ctr+=pass;
  *fail_ctr+=fail;

  return fail==0;
}


bool MainObject::RunExtendedTests(Profile *p,int *pass_ctr,int *fail_ctr) const
{
  int pass=0;
  int fail=0;
  bool result_ok=false;

  QStringList sections;
  sections.push_back("Tests");
  SingleResult("List Sections",p->sections()==sections,&pass,&fail);

  QStringList section_ids;
  section_ids.push_back("Default");
  section_ids.push_back("Second");
  SingleResult("List Section IDs",p->sectionIds("Tests")==section_ids,
	       &pass,&fail);

  QStringList strings;
  strings.push_back("Now is the time");
  strings.push_back("for all good men");
  result_ok=(p->stringValues("Tests","Default","StringValue")==strings);
  SingleResult("String Multi-Value [Id=Default]",result_ok,&pass,&fail);
  strings.clear();
  strings.push_back("to come to the aid");
  strings.push_back("of their country!");
  result_ok=(p->stringValues("Tests","Second","StringValue")==strings);
  SingleResult("String Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<int> ints;
  ints.push_back(1234);
  ints.push_back(2345);
  result_ok=(p->intValues("Tests","Default","IntegerValue")==ints);
  SingleResult("Integer Multi-Value [Id=Default]",result_ok,&pass,&fail);
  ints.clear();
  ints.push_back(3456);
  ints.push_back(4567);
  result_ok=(p->intValues("Tests","Second","IntegerValue")==ints);
  SingleResult("Integer Multi-Value [Id=Second]",result_ok,&pass,&fail);

  ints.clear();
  ints.push_back(1234);
  ints.push_back(2345);
  result_ok=(p->hexValues("Tests","Default","HexValue")==ints);
  SingleResult("Hex Multi-Value [Id=Default]",result_ok,&pass,&fail);
  ints.clear();
  ints.push_back(3456);
  ints.push_back(4567);
  result_ok=(p->hexValues("Tests","Second","HexValue")==ints);
  SingleResult("Hex Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<double> doubles;
  doubles.push_back(3.1415928);
  doubles.push_back(6.2831856);
  result_ok=(p->doubleValues("Tests","Default","DoubleValue")==doubles);
  SingleResult("Double Multi-Value [Id=Default]",result_ok,&pass,&fail);
  doubles.clear();
  doubles.push_back(9.4247784);
  doubles.push_back(12.5663712);
  result_ok=(p->doubleValues("Tests","Second","DoubleValue")==doubles);
  SingleResult("Double Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<bool> on_bools;
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolYesValue")==on_bools);
  SingleResult("Bool Yes Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolYesValue")==on_bools);
  SingleResult("Bool Yes Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<bool> off_bools;
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolNoValue")==off_bools);
  SingleResult("Bool No Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolNoValue")==off_bools);
  SingleResult("Bool No Multi-Value [Id=Second]",result_ok,&pass,&fail);

  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolOnValue")==on_bools);
  SingleResult("Bool On Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolOnValue")==on_bools);
  SingleResult("Bool On Multi-Value [Id=Second]",result_ok,&pass,&fail);

  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolOffValue")==off_bools);
  SingleResult("Bool Off Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolOffValue")==off_bools);
  SingleResult("Bool Off Multi-Value [Id=Second]",result_ok,&pass,&fail);

  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolTrueValue")==on_bools);
  SingleResult("Bool True Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolTrueValue")==on_bools);
  SingleResult("Bool True Multi-Value [Id=Second]",result_ok,&pass,&fail);

  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolFalseValue")==off_bools);
  SingleResult("Bool False Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolFalseValue")==off_bools);
  SingleResult("Bool False Multi-Value [Id=Second]",result_ok,&pass,&fail);

  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolOneValue")==on_bools);
  SingleResult("Bool One Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolOneValue")==on_bools);
  SingleResult("Bool One Multi-Value [Id=Second]",result_ok,&pass,&fail);

  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolZeroValue")==off_bools);
  SingleResult("Bool Zero Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolZeroValue")==off_bools);
  SingleResult("Bool Zero Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<QTime> times;
  times.push_back(QTime(9,30,0));
  times.push_back(QTime(10,30,0));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMMorningValue")==times);
  SingleResult("TimeHHMMMorning Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(11,30,0));
  times.push_back(QTime(12,30,0));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMMorningValue")==times);
  SingleResult("TimeHHMMMorning Multi-Value [Id=Second]",result_ok,&pass,&fail);

  times.clear();
  times.push_back(QTime(21,30,0));
  times.push_back(QTime(22,15,0));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMEveningValue")==times);
  SingleResult("TimeHHMMEvening Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(23,30,0));
  times.push_back(QTime(23,45,0));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMEveningValue")==times);
  SingleResult("TimeHHMMEvening Multi-Value [Id=Second]",result_ok,&pass,&fail);

  times.clear();
  times.push_back(QTime(9,30,45));
  times.push_back(QTime(10,30,45));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMSSMorningValue")==times);
  SingleResult("TimeHHMMSSMorning Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(11,30,45));
  times.push_back(QTime(11,45,45));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMSSMorningValue")==times);
  SingleResult("TimeHHMMSSMorning Multi-Value [Id=Second]",result_ok,
	       &pass,&fail);

  times.clear();
  times.push_back(QTime(21,30,45));
  times.push_back(QTime(22,30,45));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMSSEveningValue")==times);
  SingleResult("TimeHHMMSSEvening Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(22,30,45));
  times.push_back(QTime(23,30,45));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMSSEveningValue")==times);
  SingleResult("TimeHHMMSSEvening Multi-Value [Id=Second]",result_ok,
	       &pass,&fail);

  QList<QHostAddress> addrs;
  addrs.push_back(QHostAddress("1.2.3.4"));
  addrs.push_back(QHostAddress("5.6.7.8"));
  result_ok=(p->addressValues("Tests","Default","IpAddressValidValue")==addrs);
  SingleResult("IpAddress Multi-Value [Id=Default]",result_ok,&pass,&fail);
  addrs.clear();
  addrs.push_back(QHostAddress("6.7.8.9"));
  addrs.push_back(QHostAddress("7.8.9.10"));
  result_ok=(p->addressValues("Tests","Second","IpAddressValidValue")==addrs);
  SingleResult("IpAddress Multi-Value [Id=Second]",result_ok,&pass,&fail);
  
  *pass_ctr+=pass;
  *fail_ctr+=fail;

  return fail==0;
}


bool MainObject::RunTestsUnified(Profile *p,int *pass_ctr,int *fail_ctr) const
{
  int pass=0;
  int fail=0;
  
  *pass_ctr+=pass;
  *fail_ctr+=fail;

  return fail==0;
}  


void MainObject::Title(const QString &title) const
{
  printf("  %55s ... ",title.toUtf8().constData());
  fflush(stdout);
}


bool MainObject::SingleResult(const QString &title,bool result_ok,
			      int *pass_ctr,int *fail_ctr) const
{
  Title(title+" Value Result");
  Result(result_ok,pass_ctr,fail_ctr);

  return result_ok;
}


bool MainObject::Result(bool state,int *pass_ctr,int *fail_ctr) const
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


void MainObject::Result(const QString &title,bool result_ok,bool found,
			int *pass_ctr,int *fail_ctr) const
{
  Title(title+" Value Result");
  Result(result_ok,pass_ctr,fail_ctr);

  Title(title+" Found Result");
  Result(found,pass_ctr,fail_ctr);
}


void MainObject::DumpList(const QString &title,const QStringList &list) const
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %s\n",i,list.at(i).toUtf8().constData());
  }
}


void MainObject::DumpList(const QString &title,const QList<int> &list) const
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %d\n",i,list.at(i));
  }
}


void MainObject::DumpList(const QString &title,const QList<double> &list) const
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %10.7lf\n",i,list.at(i));
  }
}


void MainObject::DumpList(const QString &title,const QList<QTime> &list) const
{
  printf("%s\n",title.toUtf8().constData());
  for(int i=0;i<list.size();i++) {
    printf("[%d]: %s\n",i,list.at(i).toString("hh:mm:ss").toUtf8().constData());
  }
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();
  return a.exec();
}

