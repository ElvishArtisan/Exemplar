// profile.cpp
//
// A container class for profile lines.
//
// (C) Copyright 2013-2024 Fred Gleason <fredg@paravelsystems.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of version 2.1 of the GNU Lesser General Public
//    License as published by the Free Software Foundation;
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, 
//    Boston, MA  02111-1307  USA
//
// EXEMPLAR_VERSION: 1.0.1
//

#include <stdio.h>

#include <QFile>
#include <QStringList>
#include <QTextStream>

#include "profile.h"

Profile::Profile()
{
}


QStringList Profile::sectionNames() const
{
  return d_blocks.keys();
}


QString Profile::source() const
{
  return profile_source;
}


bool Profile::setSource(const QString &filename,QString *err_msg)
{
  FILE *f=NULL;
  char data[1024];
  QString block_name;
  QMultiMap<QString,QString> block_lines;
  
  if((f=fopen(filename.toUtf8(),"r"))==NULL) {
    if(err_msg!=NULL) {
      *err_msg=strerror(errno);
    }
    return false;
  }

  QStringList values;
  while(fgets(data,1023,f)!=NULL) {
    values.push_back(QString::fromUtf8(data).trimmed());
  }
  setSource(values);
  return true;
}


bool Profile::setSource(const QStringList &values)
{
  QString block_name;
  QMap<QString,QStringList> block_lines;

  for(int i=0;i<values.size();i++) {
    QString line=values.at(i);
    if((line.left(1)=="[")&&(line.right(1)=="]")) {  // Block Starts
      if(!block_name.isEmpty()) {
	ProcessBlock(block_name,block_lines);
      }
      block_name=line.mid(1,line.size()-2);
      block_lines.clear();
    }
    if((!line.isEmpty())&&(line.left(1)!=";")&&(line.left(1)!="#")) {
      QStringList f0=line.split("=",Qt::KeepEmptyParts);
      QString tag=f0.at(0);
      f0.removeFirst();
      QStringList f1=block_lines.value(tag,QStringList());
      f1.push_back(f0.join("="));
      block_lines[tag]=f1;
    }
  }
  if(!block_name.isEmpty()) {
    ProcessBlock(block_name,block_lines);
  }
  
  return true;
}


QString Profile::stringValue(const QString &section,const QString &tag,
			     const QString &default_str,bool *found)
{
  QStringList values=stringValues(section,tag);
  if(found!=NULL) {
    *found=values.size()>0;
  }
  if(values.size()==0) {
    return default_str;
  }
  return values.first();
}


QStringList Profile::stringValues(const QString &section,const QString &tag)
{
  QMap<QString,QStringList> block=d_blocks.value(section);
  if(block.size()==0) {
    return QStringList();
  }
  return block.value(tag);
}


int Profile::intValue(const QString &section,const QString &tag,
		      int default_value,bool *found)
{
  QList<int> values=intValues(section,tag);
  if(found!=NULL) {
    *found=values.size()>0;
  }
  if(values.size()==0) {
    return default_value;
  }
  return values.first();
}


QList<int> Profile::intValues(const QString &section,const QString &tag)
{
  QMap<QString,QStringList> block=d_blocks.value(section);
  if(block.size()==0) {
    return QList<int>();
  }
  QList<int> ret;
  QStringList values=block.value(tag);
  for(int i=0;i<values.size();i++) {
    ret.push_back(values.at(i).toInt());
  }
  return ret;
}


int Profile::hexValue(const QString &section,const QString &tag,
		       int default_value,bool *found)
{
  QList<int> values=hexValues(section,tag);
  if(found!=NULL) {
    *found=values.size()>0;
  }
  if(values.size()==0) {
    return default_value;
  }
  return values.first();
}


QList<int> Profile::hexValues(const QString &section,const QString &tag)
{
  QMap<QString,QStringList> block=d_blocks.value(section);
  if(block.size()==0) {
    return QList<int>();
  }
  QList<int> ret;
  QStringList values=block.value(tag);
  for(int i=0;i<values.size();i++) {
    ret.push_back(values.at(i).toInt(NULL,16));
  }
  return ret;
}


double Profile::doubleValue(const QString &section,const QString &tag,
			    double default_value,bool *found)
{
  QList<double> values=doubleValues(section,tag);
  if(found!=NULL) {
    *found=values.size()>0;
  }
  if(values.size()==0) {
    return default_value;
  }
  return values.first();
}


QList<double> Profile::doubleValues(const QString &section,const QString &tag)
{
  QMap<QString,QStringList> block=d_blocks.value(section);
  if(block.size()==0) {
    return QList<double>();
  }
  QList<double> ret;
  QStringList values=block.value(tag);
  for(int i=0;i<values.size();i++) {
    ret.push_back(values.at(i).toDouble());
  }
  return ret;
}


bool Profile::boolValue(const QString &section,const QString &tag,
			 bool default_value,bool *found)
{
  QList<bool> values=boolValues(section,tag);
  if(found!=NULL) {
    *found=values.size()>0;
  }
  if(values.size()==0) {
    return default_value;
  }
  return values.first();
}


QList<bool> Profile::boolValues(const QString &section,const QString &tag)
{
  QMap<QString,QStringList> block=d_blocks.value(section);
  if(block.size()==0) {
    return QList<bool>();
  }
  QList<bool> ret;
  QStringList values=block.value(tag);
  for(int i=0;i<values.size();i++) {
    ret.push_back((values.at(i).toLower()=="yes")||
		  (values.at(i).toLower()=="true")||
		  (values.at(i).toLower()=="on")||
		  (values.at(i).toLower()=="1"));
  }
  return ret;
}


QTime Profile::timeValue(const QString &section,const QString &tag,
			   const QTime &default_value,bool *found)
{
  QList<QTime> values=timeValues(section,tag);
  if(found!=NULL) {
    *found=values.size()>0;
  }
  if(values.size()==0) {
    return default_value;
  }
  return values.first();
}


QList<QTime> Profile::timeValues(const QString &section,const QString &tag)
{
  QMap<QString,QStringList> block=d_blocks.value(section);
  if(block.size()==0) {
    return QList<QTime>();
  }
  QList<QTime> ret;
  QStringList values=block.value(tag);
  for(int i=0;i<values.size();i++) {
    QStringList fields=values.at(i).split(":");
    if(fields.size()==2) {
      ret.push_back(QTime(fields.at(0).toInt(),fields.at(1).toInt(),0));
    }
    else {
      if(fields.size()==3) {
	ret.push_back(QTime(fields.at(0).toInt(),fields.at(1).toInt(),
			    fields.at(2).toInt()));
      }
      else {
	ret.push_back(QTime());
      }
    }
  }
  return ret;
}


QHostAddress Profile::addressValue(const QString &section,const QString &tag,
				  const QHostAddress &default_value,bool *found)
{
  return QHostAddress(stringValue(section,tag,default_value.toString(),found));
}


QHostAddress Profile::addressValue(const QString &section,const QString &tag,
				     const QString &default_value,bool *found)
{
  return addressValue(section,tag,QHostAddress(default_value),found);
}


QList<QHostAddress> Profile::addressValues(const QString &section,
					   const QString &tag)
{
  QMap<QString,QStringList> block=d_blocks.value(section);
  if(block.size()==0) {
    return QList<QHostAddress>();
  }
  QList<QHostAddress> ret;
  QStringList values=block.value(tag);
  for(int i=0;i<values.size();i++) {
    ret.push_back(QHostAddress(values.at(i)));
  }
  return ret;
}


void Profile::clear()
{
  profile_source="";
  d_blocks.clear();
}


QString Profile::dump() const
{
  QString ret;
  
  for(QMap<QString,QMap<QString,QStringList> >::const_iterator it0=
	d_blocks.begin();it0!=d_blocks.end();it0++) {
    ret+=QString::asprintf("[%s]\n",it0.key().toUtf8().constData());
    for(QMap<QString,QStringList>::const_iterator it1=it0.value().begin();
	it1!=it0.value().end();it1++) {
      for(int i=0;i<it1.value().size();i++) {
	ret+=QString::asprintf("%s=%s\n",it1.key().toUtf8().constData(),
			       it1.value().at(i).toUtf8().constData());
      }
    }
    ret+="\n";
  }

  return ret;
}


void Profile::ProcessBlock(const QString &name,
			   const QMap<QString,QStringList> &lines)
{
  QMap<QString,QStringList> block=
    d_blocks.value(name,QMap<QString,QStringList>());

  for(QMap<QString,QStringList>::const_iterator it=lines.begin();
      it!=lines.end();it++) {
    block[it.key()].append(it.value());
  }
  d_blocks[name]=block;
}


QStringList Profile::InvertList(const QStringList &list) const
{
  QStringList ret;

  for(int i=0;i<list.size();i++) {
    ret.push_back(list.at(list.size()-i-1));
  }
  
  return ret;
}
