// run_tests.cpp
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
#include <QDir>

#include "run_tests.h"
#include "test_methods.h"

//
// So we can test internal e-mail methods
//
bool __SendMail_IsAscii(const QString &str);
QByteArray __SendMail_EncodeBody(QString *charset,QString *encoding,
				 const QString &str);
QByteArray __SendMail_EncodeHeaderValue(const QString &str);
bool __SendMail_EmailAddressIsValid(const QString &mail_addr);
QByteArray __SendMail_EncodeAddress(const QString &str,bool *ok);

MainObject::MainObject()
  : QObject()
{
  int total_pass=0;
  int total_fail=0;
  QStringList lines;
  QString err_msg;
  QStringList err_msgs;
  int num;
  
  printf("**** Legacy Format ****\n");
  Profile *p=new Profile();
  if(!p->loadFile("../../fixtures/legacy.conf",&err_msg)) {
    fprintf(stderr,"run_tests: failed to open test data [%s]\n",
	    err_msg.toUtf8().constData());
    exit(1);
  }
  RunLegacyTests(p,&total_pass,&total_fail);
  delete p;
  printf("\n");

  printf("**** Extended Format ****\n");
  p=new Profile(true);
  if(!p->loadFile("../../fixtures/extended.conf",&err_msg)) {
    fprintf(stderr,"run_tests: failed to open test data [%s]\n",
	    err_msg.toUtf8().constData());
    exit(1);
  }
  RunExtendedTests(p,&total_pass,&total_fail);
  delete p;
  printf("\n");

  printf("**** Extended Multipart Format ****\n");
  p=new Profile();
  num=p->load("../../fixtures*/extended_part*.conf",&err_msgs);
  PrintTitle("Directory Load Failed Test");
  PrintResultState(num==-1,&total_pass,&total_fail);
  delete p;

  p=new Profile();
  QString abspath=
    QDir::cleanPath(QDir::currentPath()+"/../../fixtures/extended_part*.conf");
  num=p->load(abspath,&err_msgs);
  PrintTitle("Directory Load Absolute Path Test");
  PrintResultState(num==4,&total_pass,&total_fail);
  delete p;

  p=new Profile();
  num=p->load("../../fixtures/extended_part*.conf",&err_msgs);
  PrintTitle("Directory Load Passed Test");
  PrintResultState(num==4,&total_pass,&total_fail);

  RunLegacyTests(p,&total_pass,&total_fail);
  printf("\n");
  delete p;

  printf("**** Sendmail Interface ****\n");
  RunSendmailTests(&total_pass,&total_fail);
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
  PrintSingleResult("List Sections",p->sections()==sections,&pass,&fail);

  result_ok=p->stringValue("Tests","StringValue","of their country!",&ok)==
			   "Now is the time";
  PrintDualResult("String Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->stringValue("Tests","StringMissing","Not this string!",&ok)==
			   "Not this string!";
  PrintDualResult("String Missing",result_ok,ok==false,&pass,&fail);
  QStringList strings;
  strings.push_back("Now is the time");
  strings.push_back("for all good men");
  strings.push_back("to come to the aid");
  strings.push_back("of their country!");
  result_ok=(p->stringValues("Tests","StringValue")==strings);
  PrintSingleResult("String Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->intValue("Tests","IntegerValue",4321,&ok)==1234;
  PrintDualResult("Integer Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->intValue("Tests","IntegerMissing",4321,&ok)==4321;
  PrintDualResult("Integer Missing",result_ok,ok==false,&pass,&fail);
  QList<int> ints;
  ints.push_back(1234);
  ints.push_back(2345);
  ints.push_back(3456);
  ints.push_back(4567);
  result_ok=(p->intValues("Tests","IntegerValue")==ints);
  PrintSingleResult("Integer Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->hexValue("Tests","HexValue",4321,&ok)==1234;
  PrintDualResult("Hex Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->hexValue("Tests","HexMissing",4321,&ok)==4321;
  PrintDualResult("Hex Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->hexValues("Tests","HexValue")==ints);
  PrintSingleResult("Hex Multi-Value",result_ok,&pass,&fail);

  result_ok=p->doubleValue("Tests","DoubleValue",2.71,&ok)==3.1415928;
  PrintDualResult("Double Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->doubleValue("Tests","DoubleMissing",2.71,&ok)==2.71;
  PrintDualResult("Double Missing",result_ok,ok==false,&pass,&fail);
  QList<double> doubles;
  doubles.push_back(3.1415928);
  doubles.push_back(6.2831856);
  doubles.push_back(9.4247784);
  doubles.push_back(12.5663712);
  result_ok=(p->doubleValues("Tests","DoubleValue")==doubles);
  PrintSingleResult("Double Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->boolValue("Tests","BoolYesValue",false,&ok)==true;
  PrintDualResult("BoolYes Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolYesMissing",false,&ok)==false;
  PrintDualResult("BoolYes Missing",result_ok,ok==false,&pass,&fail);
  QList<bool> on_bools;
  on_bools.push_back(true);
  on_bools.push_back(false);
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","BoolYesValue")==on_bools);
  PrintSingleResult("BoolYes Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->boolValue("Tests","BoolNoValue",true,&ok)==false;
  PrintDualResult("BoolNo Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolNoMissing",true,&ok)==true;
  PrintDualResult("BoolNo Missing",result_ok,ok==false,&pass,&fail);
  QList<bool> off_bools;
  off_bools.push_back(false);
  off_bools.push_back(true);
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","BoolNoValue")==off_bools);
  PrintSingleResult("BoolNo Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolOnValue",false,&ok)==true;
  PrintDualResult("BoolOn Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOnMissing",false,&ok)==false;
  PrintDualResult("BoolOn Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolOnValue")==on_bools);
  PrintSingleResult("BoolOn Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->boolValue("Tests","BoolOffValue",true,&ok)==false;
  PrintDualResult("BoolOff Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOffMissing",true,&ok)==true;
  PrintDualResult("BoolOff Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolOffValue")==off_bools);
  PrintSingleResult("BoolOff Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolTrueValue",false,&ok)==true;
  PrintDualResult("BoolTrue Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolTrueMissing",false,&ok)==false;
  PrintDualResult("BoolTrue Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolTrueValue")==on_bools);
  PrintSingleResult("BoolTrue Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolFalseValue",true,&ok)==false;
  PrintDualResult("BoolFalse Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolFalseMissing",true,&ok)==true;
  PrintDualResult("BoolFalse Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolFalseValue")==off_bools);
  PrintSingleResult("BoolFalse Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolOneValue",false,&ok)==true;
  PrintDualResult("BoolOne Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOneMissing",false,&ok)==false;
  PrintDualResult("BoolOne Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolOneValue")==on_bools);
  PrintSingleResult("BoolOne Multi-Value",result_ok,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolZeroValue",true,&ok)==false;
  PrintDualResult("BoolZero Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolZeroMissing",true,&ok)==true;
  PrintDualResult("BoolZero Missing",result_ok,ok==false,&pass,&fail);
  result_ok=(p->boolValues("Tests","BoolZeroValue")==off_bools);
  PrintSingleResult("BoolZero Multi-Value",result_ok,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMMorningValue",QTime(10,45,0),&ok)==
    QTime(9,30,0);
  PrintDualResult("TimeHHMMMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->timeValue("Tests","TimeHHMMMorning Missing",QTime(10,45,0),&ok)==
    QTime(10,45,0);
  PrintDualResult("TimeHHMMMorning Missing",result_ok,ok==false,&pass,&fail);
  QList<QTime> times;
  times.push_back(QTime(9,30,0));
  times.push_back(QTime(10,30,0));
  times.push_back(QTime(11,30,0));
  times.push_back(QTime(12,30,0));
  result_ok=(p->timeValues("Tests","TimeHHMMMorningValue")==times);
  PrintSingleResult("TimeHHMMMorning Multi-Value",result_ok,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMEveningValue",QTime(22,15,0),&ok)==
    QTime(21,30,0);
  PrintDualResult("TimeHHMMMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->timeValue("Tests","TimeHHMMEvening Missing",QTime(22,15,0),&ok)==
    QTime(22,15,0);
  PrintDualResult("TimeHHMMEvening Missing",result_ok,ok==false,&pass,&fail);
  times.clear();
  times.push_back(QTime(21,30,0));
  times.push_back(QTime(22,15,0));
  times.push_back(QTime(23,30,0));
  times.push_back(QTime(23,45,0));
  result_ok=(p->timeValues("Tests","TimeHHMMEveningValue")==times);
  PrintSingleResult("TimeHHMMEvening Multi-Value",result_ok,&pass,&fail);
  
  result_ok=p->timeValue("Tests","TimeHHMMSSMorningValue",QTime(10,45,45),&ok)==
    QTime(9,30,45);
  PrintDualResult("TimeHHMMSSMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->
    timeValue("Tests","TimeHHMMSSMorning Missing",QTime(10,45,45),&ok)==
    QTime(10,45,45);
  PrintDualResult("TimeHHMMSSMorning Missing",result_ok,ok==false,&pass,&fail);
  times.clear();
  times.push_back(QTime(9,30,45));
  times.push_back(QTime(10,30,45));
  times.push_back(QTime(11,30,45));
  times.push_back(QTime(11,45,45));
  result_ok=(p->timeValues("Tests","TimeHHMMSSMorningValue")==times);
  PrintSingleResult("TimeHHMMSSMorning Multi-Value",result_ok,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMSSEveningValue",QTime(22,45,45),&ok)==
    QTime(21,30,45);
  PrintDualResult("TimeHHMMSSEvening Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->
    timeValue("Tests","TimeHHMMSSEvening Missing",QTime(10,45,45),&ok)==
    QTime(10,45,45);
  PrintDualResult("TimeHHMMSSEvening Missing",result_ok,ok==false,&pass,&fail);
  times.clear();
  times.push_back(QTime(21,30,45));
  times.push_back(QTime(22,30,45));
  times.push_back(QTime(22,30,45));
  times.push_back(QTime(23,30,45));
  result_ok=(p->timeValues("Tests","TimeHHMMSSEveningValue")==times);
  PrintSingleResult("TimeHHMMSSEvening Multi-Value",result_ok,&pass,&fail);

  result_ok=p->addressValue("Tests","IpAddressValidValue",
		      QHostAddress("4.3.2.1"),&ok)==QHostAddress("1.2.3.4");
  PrintDualResult("IpAddress Found (QHostAddress Default)",result_ok,ok==true,
	 &pass,&fail);
  result_ok=p->addressValue("Tests","IpAddressValidMissing",
		      QHostAddress("4.3.2.1"),&ok)==QHostAddress("4.3.2.1");
  PrintDualResult("IpAddress Missing (QHostAddress Default)",result_ok,ok==false,
	 &pass,&fail);
  result_ok=p->addressValue("Tests","IpAddressValidValue",
			    "4.3.2.1",&ok)==QHostAddress("1.2.3.4");
  PrintDualResult("IpAddress Found (QString Default)",result_ok,ok==true,&pass,&fail);
  result_ok=p->addressValue("Tests","IpAddressValidMissing",
			    "4.3.2.1",&ok)==QHostAddress("4.3.2.1");
  PrintDualResult("IpAddress Missing (QString Default)",result_ok,ok==false,&pass,&fail);
  QList<QHostAddress> addrs;
  addrs.push_back(QHostAddress("1.2.3.4"));
  addrs.push_back(QHostAddress("5.6.7.8"));
  addrs.push_back(QHostAddress("6.7.8.9"));
  addrs.push_back(QHostAddress("7.8.9.10"));
  result_ok=(p->addressValues("Tests","IpAddressValidValue")==addrs);
  PrintSingleResult("IpAddress Multi-Value",result_ok,&pass,&fail);

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
  PrintSingleResult("List Sections",p->sections()==sections,&pass,&fail);

  QStringList section_ids;
  section_ids.push_back("Default");
  section_ids.push_back("Second");
  PrintSingleResult("List Section IDs",p->sectionIds("Tests")==section_ids,
	       &pass,&fail);

  QStringList strings;
  strings.push_back("Now is the time");
  strings.push_back("for all good men");
  result_ok=(p->stringValues("Tests","Default","StringValue")==strings);
  PrintSingleResult("String Multi-Value [Id=Default]",result_ok,&pass,&fail);
  strings.clear();
  strings.push_back("to come to the aid");
  strings.push_back("of their country!");
  result_ok=(p->stringValues("Tests","Second","StringValue")==strings);
  PrintSingleResult("String Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<int> ints;
  ints.push_back(1234);
  ints.push_back(2345);
  result_ok=(p->intValues("Tests","Default","IntegerValue")==ints);
  PrintSingleResult("Integer Multi-Value [Id=Default]",result_ok,&pass,&fail);
  ints.clear();
  ints.push_back(3456);
  ints.push_back(4567);
  result_ok=(p->intValues("Tests","Second","IntegerValue")==ints);
  PrintSingleResult("Integer Multi-Value [Id=Second]",result_ok,&pass,&fail);

  ints.clear();
  ints.push_back(1234);
  ints.push_back(2345);
  result_ok=(p->hexValues("Tests","Default","HexValue")==ints);
  PrintSingleResult("Hex Multi-Value [Id=Default]",result_ok,&pass,&fail);
  ints.clear();
  ints.push_back(3456);
  ints.push_back(4567);
  result_ok=(p->hexValues("Tests","Second","HexValue")==ints);
  PrintSingleResult("Hex Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<double> doubles;
  doubles.push_back(3.1415928);
  doubles.push_back(6.2831856);
  result_ok=(p->doubleValues("Tests","Default","DoubleValue")==doubles);
  PrintSingleResult("Double Multi-Value [Id=Default]",result_ok,&pass,&fail);
  doubles.clear();
  doubles.push_back(9.4247784);
  doubles.push_back(12.5663712);
  result_ok=(p->doubleValues("Tests","Second","DoubleValue")==doubles);
  PrintSingleResult("Double Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<bool> on_bools;
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolYesValue")==on_bools);
  PrintSingleResult("Bool Yes Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolYesValue")==on_bools);
  PrintSingleResult("Bool Yes Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<bool> off_bools;
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolNoValue")==off_bools);
  PrintSingleResult("Bool No Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolNoValue")==off_bools);
  PrintSingleResult("Bool No Multi-Value [Id=Second]",result_ok,&pass,&fail);

  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolOnValue")==on_bools);
  PrintSingleResult("Bool On Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolOnValue")==on_bools);
  PrintSingleResult("Bool On Multi-Value [Id=Second]",result_ok,&pass,&fail);

  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolOffValue")==off_bools);
  PrintSingleResult("Bool Off Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolOffValue")==off_bools);
  PrintSingleResult("Bool Off Multi-Value [Id=Second]",result_ok,&pass,&fail);

  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolTrueValue")==on_bools);
  PrintSingleResult("Bool True Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolTrueValue")==on_bools);
  PrintSingleResult("Bool True Multi-Value [Id=Second]",result_ok,&pass,&fail);

  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolFalseValue")==off_bools);
  PrintSingleResult("Bool False Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolFalseValue")==off_bools);
  PrintSingleResult("Bool False Multi-Value [Id=Second]",result_ok,&pass,&fail);

  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Default","BoolOneValue")==on_bools);
  PrintSingleResult("Bool One Multi-Value [Id=Default]",result_ok,&pass,&fail);
  on_bools.clear();
  on_bools.push_back(true);
  on_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Second","BoolOneValue")==on_bools);
  PrintSingleResult("Bool One Multi-Value [Id=Second]",result_ok,&pass,&fail);

  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(true);
  result_ok=(p->boolValues("Tests","Default","BoolZeroValue")==off_bools);
  PrintSingleResult("Bool Zero Multi-Value [Id=Default]",result_ok,&pass,&fail);
  off_bools.clear();
  off_bools.push_back(false);
  off_bools.push_back(false);
  result_ok=(p->boolValues("Tests","Second","BoolZeroValue")==off_bools);
  PrintSingleResult("Bool Zero Multi-Value [Id=Second]",result_ok,&pass,&fail);

  QList<QTime> times;
  times.push_back(QTime(9,30,0));
  times.push_back(QTime(10,30,0));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMMorningValue")==times);
  PrintSingleResult("TimeHHMMMorning Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(11,30,0));
  times.push_back(QTime(12,30,0));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMMorningValue")==times);
  PrintSingleResult("TimeHHMMMorning Multi-Value [Id=Second]",result_ok,&pass,&fail);

  times.clear();
  times.push_back(QTime(21,30,0));
  times.push_back(QTime(22,15,0));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMEveningValue")==times);
  PrintSingleResult("TimeHHMMEvening Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(23,30,0));
  times.push_back(QTime(23,45,0));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMEveningValue")==times);
  PrintSingleResult("TimeHHMMEvening Multi-Value [Id=Second]",result_ok,&pass,&fail);

  times.clear();
  times.push_back(QTime(9,30,45));
  times.push_back(QTime(10,30,45));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMSSMorningValue")==times);
  PrintSingleResult("TimeHHMMSSMorning Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(11,30,45));
  times.push_back(QTime(11,45,45));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMSSMorningValue")==times);
  PrintSingleResult("TimeHHMMSSMorning Multi-Value [Id=Second]",result_ok,
	       &pass,&fail);

  times.clear();
  times.push_back(QTime(21,30,45));
  times.push_back(QTime(22,30,45));
  result_ok=(p->timeValues("Tests","Default","TimeHHMMSSEveningValue")==times);
  PrintSingleResult("TimeHHMMSSEvening Multi-Value [Id=Default]",result_ok,
	       &pass,&fail);
  times.clear();
  times.push_back(QTime(22,30,45));
  times.push_back(QTime(23,30,45));
  result_ok=(p->timeValues("Tests","Second","TimeHHMMSSEveningValue")==times);
  PrintSingleResult("TimeHHMMSSEvening Multi-Value [Id=Second]",result_ok,
	       &pass,&fail);

  QList<QHostAddress> addrs;
  addrs.push_back(QHostAddress("1.2.3.4"));
  addrs.push_back(QHostAddress("5.6.7.8"));
  result_ok=(p->addressValues("Tests","Default","IpAddressValidValue")==addrs);
  PrintSingleResult("IpAddress Multi-Value [Id=Default]",result_ok,&pass,&fail);
  addrs.clear();
  addrs.push_back(QHostAddress("6.7.8.9"));
  addrs.push_back(QHostAddress("7.8.9.10"));
  result_ok=(p->addressValues("Tests","Second","IpAddressValidValue")==addrs);
  PrintSingleResult("IpAddress Multi-Value [Id=Second]",result_ok,&pass,&fail);
  
  *pass_ctr+=pass;
  *fail_ctr+=fail;

  return fail==0;
}


bool MainObject::RunSendmailTests(int *pass_ctr,int *fail_ctr) const
{
  bool result=false;
  int pass=0;
  int fail=0;
  bool ok=false;

  result=__SendMail_IsAscii(QString::fromUtf8("Hello World!"));
  PrintSingleResult("ASCII Detection Test",result,&pass,&fail);

  result=!__SendMail_IsAscii(QString::fromUtf8("Hello 文字化け!"));
  PrintSingleResult("Unicode Detection Test",result,&pass,&fail);

  result=__SendMail_EncodeHeaderValue("This is a subject!")==
    QString::fromUtf8("This is a subject!");
  PrintSingleResult("ASCII Header Value Test",result,&pass,&fail);

  result=__SendMail_EncodeHeaderValue("This is 文字化け!")==
    "=?utf-8?B?VGhpcyBpcyDmloflrZfljJbjgZEh?=";
  PrintSingleResult("Unicode Header Value Test",result,&pass,&fail);

  result=__SendMail_EmailAddressIsValid("someone@somewhere.com");
  PrintSingleResult("Address Validity Test",result,&pass,&fail);

  result=!__SendMail_EmailAddressIsValid("someone@example@com");
  PrintSingleResult("Address Invalidity Test 1",result,&pass,&fail);
  result=!__SendMail_EmailAddressIsValid("someone.example.com");
  PrintSingleResult("Address Invalidity Test 2",result,&pass,&fail);
  result=!__SendMail_EmailAddressIsValid("@someone.example.com");
  PrintSingleResult("Address Invalidity Test 3",result,&pass,&fail);
  result=!__SendMail_EmailAddressIsValid("someone@.example.com");
  PrintSingleResult("Address Invalidity Test 4",result,&pass,&fail);
  result=!__SendMail_EmailAddressIsValid("someone.example.com@");
  PrintSingleResult("Address Invalidity Test 5",result,&pass,&fail);

  result=__SendMail_EncodeAddress("Jane Q. Doe <someone@example.com>",&ok)==
    QString::fromUtf8("Jane Q. Doe <someone@example.com>");
  PrintSingleResult("ASCII Address Encoding Test",result,&pass,&fail);

  result=__SendMail_EncodeAddress("文字化け <someone@example.com>",&ok)==
    "=?utf-8?B?5paH5a2X5YyW44GR?= <someone@example.com>";
  PrintSingleResult("Unicode Address Encoding Test",result,&pass,&fail);

  QString body_in="Now is the time for all good men to come to the aid of their country.\n\nThe quick brown fox jumps over the lazy dog.";
  QString body_out="Now is the time for all good men to come to the aid of their country.\r\n\r\nThe quick brown fox jumps over the lazy dog.";
  QString charset;
  QString encoding;
  result=__SendMail_EncodeBody(&charset,&encoding,body_in)==body_out;
  PrintSingleResult("ASCII Body Encoding Test",result,&pass,&fail);

  body_in=QString::fromUtf8("Now is the time for all good men to come to the aid of their 文字化け.\n\nThe quick brown fox jumps over the lazy dog.");
  body_out="Tm93IGlzIHRoZSB0aW1lIGZvciBhbGwgZ29vZCBtZW4gdG8gY29tZSB0byB0aGUg\r\nYWlkIG9mIHRoZWlyIOaWh+Wtl+WMluOBkS4KClRoZSBxdWljayBicm93biBmb3gg\r\nanVtcHMgb3ZlciB0aGUgbGF6eSBkb2cu\r\n";
  result=__SendMail_EncodeBody(&charset,&encoding,body_in)==body_out;
  PrintSingleResult("Unicode Body Encoding Test",result,&pass,&fail);

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

/*
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
*/

int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();
  return a.exec();
}

