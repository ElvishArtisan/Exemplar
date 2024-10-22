// v1_tests.cpp
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

#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>

#include "v1_tests.h"

MainObject::MainObject()
  : QObject()
{
  int total_pass=0;
  int total_fail=0;
  char data[1024];
  QStringList lines;

  //
  // From File
  //
  QString err_msg;
  Profile *p=new Profile();
  if(!p->setSource("v1_tests.dat",&err_msg)) {
    fprintf(stderr,"v1_tests: failed to open test data [%s]\n",
	    err_msg.toUtf8().constData());
    exit(1);
  }
  RunV1Tests("From File",p,&total_pass,&total_fail);
  delete p;
  printf("\n");
  
  //
  // From QStringList
  //
  FILE *f;
  if((f=fopen("v1_tests.dat","r"))==NULL) {
    fprintf(stderr,"v1_tests: failed to open test data [%s]\n",strerror(errno));
    exit(1);
  }
  while(fgets(data,1023,f)!=NULL) {
    lines.push_back(QString::fromUtf8(data).trimmed());
  }
  fclose(f);
  p=new Profile();
  p->setSource(lines);
  RunV1Tests("From QStringList",p,&total_pass,&total_fail);
  delete p;
  printf("\n");
  
  printf("TOTAL TEST RESULTS\n");
  printf("%d / %d total tests passed\n",total_pass,total_pass+total_fail);
  
  if(total_fail>0) {
    exit(1);
  }
  exit(0);
}


bool MainObject::RunV1Tests(const QString &desc,Profile *p,
			    int *pass_ctr,int *fail_ctr) const
{
  bool ok=false;
  int pass=0;
  int fail=0;
  bool result_ok=false;
  
  printf("**** v1.x Tests (%s)... ****\n",desc.toUtf8().constData());

  result_ok=p->stringValue("Tests","StringValue","Not this string!",&ok)==
			   "This is a string!";
  Result("String Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->stringValue("Tests","StringMissing","Not this string!",&ok)==
			   "Not this string!";
  Result("String Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->intValue("Tests","IntegerValue",4321,&ok)==1234;
  Result("Integer Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->intValue("Tests","IntegerMissing",4321,&ok)==4321;
  Result("Integer Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->hexValue("Tests","HexValue",4321,&ok)==1234;
  Result("Hex Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->hexValue("Tests","HexMissing",4321,&ok)==4321;
  Result("Hex Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->doubleValue("Tests","DoubleValue",2.71,&ok)==3.1415928;
  Result("Double Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->doubleValue("Tests","DoubleMissing",2.71,&ok)==2.71;
  Result("Double Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolYesValue",false,&ok)==true;
  Result("BoolYes Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolYesMissing",false,&ok)==false;
  Result("BoolYes Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolNoValue",true,&ok)==false;
  Result("BoolNo Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolNoMissing",true,&ok)==true;
  Result("BoolNo Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolOnValue",false,&ok)==true;
  Result("BoolOn Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOnMissing",false,&ok)==false;
  Result("BoolOn Missing",result_ok,ok==false,&pass,&fail);
  
  result_ok=p->boolValue("Tests","BoolOffValue",true,&ok)==false;
  Result("BoolOff Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOffMissing",true,&ok)==true;
  Result("BoolOff Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolTrueValue",false,&ok)==true;
  Result("BoolTrue Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolTrueMissing",false,&ok)==false;
  Result("BoolTrue Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolFalseValue",true,&ok)==false;
  Result("BoolFalse Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolFalseMissing",true,&ok)==true;
  Result("BoolFalse Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolOneValue",false,&ok)==true;
  Result("BoolOne Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolOneMissing",false,&ok)==false;
  Result("BoolOne Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->boolValue("Tests","BoolZeroValue",true,&ok)==false;
  Result("BoolZero Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->boolValue("Tests","BoolZeroMissing",true,&ok)==true;
  Result("BoolZero Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMMorningValue",QTime(10,45,0),&ok)==
    QTime(9,30,0);
  Result("TimeHHMMMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->timeValue("Tests","TimeHHMMMorning Missing",QTime(10,45,0),&ok)==
    QTime(10,45,0);
  Result("TimeHHMMMorning Missing",result_ok,ok==false,&pass,&fail);

  result_ok=p->timeValue("Tests","TimeHHMMSSMorningValue",QTime(10,45,45),&ok)==
    QTime(9,30,45);
  Result("TimeHHMMSSMorning Found",result_ok,ok==true,&pass,&fail);
  result_ok=p->
    timeValue("Tests","TimeHHMMSSMorning Missing",QTime(10,45,45),&ok)==
    QTime(10,45,45);
  Result("TimeHHMMSSMorning Missing",result_ok,ok==false,&pass,&fail);

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

  //printf("RES: %10.8f\n",p->floatValue("Tests","FloatValue",2.71,&ok));

  printf("%d / %d tests passed\n",pass,pass+fail);

  *pass_ctr+=pass;
  *fail_ctr+=fail;
  
  return fail==0;
}


void MainObject::Title(const QString &title) const
{
  printf("  %55s ... ",title.toUtf8().constData());
  fflush(stdout);
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


void MainObject::Result(const QString &title,bool result_ok,bool ok_ok,
			int *pass_ctr,int *fail_ctr) const
{
  Title(title+" Value Result");
  Result(result_ok,pass_ctr,fail_ctr);

  Title(title+" OK Result");
  Result(ok_ok,pass_ctr,fail_ctr);
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();
  return a.exec();
}

