// sendmail.cpp
//
// Send an e-mail message using sendmail(1)
//
//   (C) Copyright 2020-2025 Fred Gleason <fredg@paravelsystems.com>
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
// EXEMPLAR_VERSION: 2.0.2
//

#include <QObject>

#include "sendmail.h"

#include <QProcess>

bool __SendMail_IsAscii(const QString &str)
{
  for(int i=0;i<str.length();i++) {
    QChar ch=str.at(i);
    if((ch.cell()>127)||(ch.row()>0)) {
      return false;
    }
  }

  return true;
}


QByteArray __SendMail_EncodeBody(QString *charset,QString *encoding,
				 const QString &str)
{
  QByteArray raw;
  int index=0;
  QByteArray ret;

  if(__SendMail_IsAscii(str)) {
    //
    // All 7 Bit Characters
    //
    *charset="";
    *encoding="";

    //
    // Ensure no naked CR or LF characters (RFC5322 Section 2.3)
    //
    ret=str.toUtf8();
    index=0;
    while((index=ret.indexOf("/r",index))>=0) {
      if(ret.mid(index+1,1)!="/n") {
	ret.insert(index+1,"/n");
	index++;
      }
    }
    index=0;
    while((index=ret.indexOf("\n",index))>=0) {
      if((index==0)||(ret.mid(index-1,1)!="\r")) {
	ret.insert(index,"\r");
	index++;
      }
      index++;
    }
    return ret;
  }

  //
  // 8+ Bit Characters
  //
  *charset=";charset=utf8";
  *encoding="Content-Transfer-Encoding: base64\r\n";
  raw=str.toUtf8();
  for(int i=0;i<raw.length();i+=48) {
    ret+=raw.mid(i,48).toBase64()+"\r\n";
  }
  return ret;
}


QByteArray __SendMail_EncodeHeaderValue(const QString &str)
{
  if(__SendMail_IsAscii(str)) {
    return str.toUtf8();
  }
  return QByteArray("=?utf-8?B?")+str.toUtf8().toBase64()+"?=";
}


bool __SendMail_EmailAddressIsValid(const QString &mail_addr)
{
  QStringList f0=mail_addr.split("@",Qt::KeepEmptyParts);

  if(f0.size()!=2) {
    return false;
  }
  for(int i=0;i<f0.size();i++) {
    if(f0.at(i).trimmed().isEmpty()) {
      return false;
    }
    if(f0.at(i).trimmed().left(1).trimmed()==".") {
      return false;
    }
    if(f0.at(i).trimmed().right(1).trimmed()==".") {
      return false;
    }
  }
  QStringList f1=f0.last().split(".");
  if(f1.size()<2) {
    return false;
  }
  return true;
}


QByteArray __SendMail_EncodeAddress(const QString &str,bool *ok)
{
  //
  // See RFC5322 Section 3.4 for these formats
  //
  int start=0;
  int end=0;
  QString addr;
  QString name;

  addr=str;
  if(str.contains("<")&&str.contains(">")) {
     start=str.indexOf("<");
     end=str.indexOf(">");
     if(start<end) {
       addr=str.mid(start+1,end-start-1).trimmed();
       name=str.left(start).trimmed();
     }
  }
  if(str.contains("(")&&str.contains(")")) {
     start=str.indexOf("(");
     end=str.indexOf(")");
     if(start<end) {
       name=str.mid(start+1,end-start-1).trimmed();
       addr=str;
       addr=addr.remove("("+name+")").trimmed();
     }
  }
  if(!__SendMail_EmailAddressIsValid(addr)) {
    *ok=false;
    return QByteArray();
  }
  *ok=true;

  //
  // Output in "display-name <local@domain>" format
  //
  // FIXME: Add support for IDNA (see RFC5891)
  //
  if(name.isEmpty()) {
    return addr.toUtf8();
  }
  return __SendMail_EncodeHeaderValue(name)+" <"+addr.toUtf8()+">";
}




//
// This implements a basic email sending capability using the system's
// sendmail(1) interface.
//
bool SendMail(QString *err_msg,const QString &subject,const QString &body,
	      const QString &from_addr,const QStringList &to_addrs,
	      const QStringList &cc_addrs,const QStringList &bcc_addrs,
	      bool dry_run)
{
  QStringList args;
  QProcess *proc=NULL;
  QString msg="";
  QByteArray from_addr_enc;
  QList<QByteArray> to_addrs_enc;
  QList<QByteArray> cc_addrs_enc;
  QList<QByteArray> bcc_addrs_enc;
  bool ok=false;

  *err_msg="";

  //
  // Validate Addresses
  //
  if(from_addr.isEmpty()) {
    *err_msg+=QObject::tr("You must supply a \"from\" address")+"\n";
  }
  else {
    from_addr_enc=__SendMail_EncodeAddress(from_addr,&ok);
    if(!ok) {
      *err_msg+=QObject::tr("address")+" \""+from_addr+"\" "+
	QObject::tr("is invalid")+"\n";
    }
  }
  for(int i=0;i<to_addrs.size();i++) {
    to_addrs_enc.push_back(__SendMail_EncodeAddress(to_addrs.at(i),&ok));
    if(!ok) {
      *err_msg+=QObject::tr("address")+" \""+to_addrs.at(i)+"\" "+
	QObject::tr("is invalid")+"\n";
    }
  }
  for(int i=0;i<cc_addrs.size();i++) {
    cc_addrs_enc.push_back(__SendMail_EncodeAddress(cc_addrs.at(i),&ok));
    if(!ok) {
      *err_msg+=QObject::tr("address")+" \""+cc_addrs.at(i)+"\" "+
	QObject::tr("is invalid")+"\n";
    }
  }
  for(int i=0;i<bcc_addrs.size();i++) {
    bcc_addrs_enc.push_back(__SendMail_EncodeAddress(bcc_addrs.at(i),&ok));
    if(!ok) {
      *err_msg+=QObject::tr("address")+" \""+bcc_addrs.at(i)+"\" "+
	QObject::tr("is invalid")+"\n";
    }
  }

  if(!err_msg->isEmpty()) {
    return false;
  }

  //
  // Compose Message
  //
  QString charset;
  QString encoding;
  QByteArray raw=__SendMail_EncodeBody(&charset,&encoding,body);

  msg+="From: "+from_addr_enc+"\r\n";

  msg+="Content-Type: text/plain"+charset+"\r\n";
  msg+=encoding;

  if(to_addrs_enc.size()>0) {
    msg+="To: ";
    for(int i=0;i<to_addrs_enc.size();i++) {
      msg+=to_addrs_enc.at(i)+", ";
    }
    msg=msg.left(msg.length()-2);
    msg+="\r\n";
  }
  if(cc_addrs_enc.size()>0) {
    msg+="Cc: ";
    for(int i=0;i<cc_addrs_enc.size();i++) {
      msg+=cc_addrs_enc.at(i)+", ";
    }
    msg=msg.left(msg.length()-2);
    msg+="\r\n";
  }
  if(bcc_addrs_enc.size()>0) {
    msg+="Bcc: ";
    for(int i=0;i<bcc_addrs_enc.size();i++) {
      msg+=bcc_addrs_enc.at(i)+", ";
    }
    msg=msg.left(msg.length()-2);
    msg+="\r\n";
  }
  msg+="Subject: "+__SendMail_EncodeHeaderValue(subject)+"\r\n";

  msg+="\r\n";
  msg+=raw;

  if(dry_run) {
    printf("*** MESSAGE STARTS ***\n");
    printf("%s",msg.toUtf8().constData());
    printf("*** MESSAGE ENDS ***\n");
    return true;
  }

  //
  // Send message
  //
  args.clear();
  args.push_back("-bm");
  args.push_back("-t");
  proc=new QProcess();
  proc->start("sendmail",args);
  if(!proc->waitForStarted()) {
    *err_msg=QObject::tr("unable to start sendmail")+"\n";
    delete proc;
    return false;
  }
  proc->write(msg.toUtf8());
  proc->closeWriteChannel();
  proc->waitForFinished();
  if(proc->exitStatus()!=QProcess::NormalExit) {
    *err_msg=QObject::tr("sendmail crashed")+"\r\n";
    delete proc;
    return false;
  }
  if(proc->exitCode()!=0) {
    *err_msg=QObject::tr("sendmail returned non-zero exit code")+
      QString::asprintf(": %d [",proc->exitCode())+
      QString::fromUtf8(proc->readAllStandardError())+"]\n";
    delete proc;
    return false;
  }
  delete proc;

  *err_msg=QObject::tr("ok");

  return true;
}


bool SendMail(QString *err_msg,const QString &subject,const QString &body,
	      const QString &from_addr,const QString &to_addrs,
	      const QString &cc_addrs,const QString &bcc_addrs,bool dry_run)
{
  return SendMail(err_msg,subject,body,from_addr,
		  to_addrs.split(",",Qt::SkipEmptyParts),
		  cc_addrs.split(",",Qt::SkipEmptyParts),
		  bcc_addrs.split(",",Qt::SkipEmptyParts),
		  dry_run);
}
