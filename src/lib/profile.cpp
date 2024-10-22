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
  QMap<QString,QString> block_lines;
  
  if((f=fopen(filename.toUtf8(),"r"))==NULL) {
    if(err_msg!=NULL) {
      *err_msg=strerror(errno);
    }
    return false;
  }

  while(fgets(data,1023,f)!=NULL) {
    QString line=QString::fromUtf8(data).trimmed();
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
      block_lines[tag]=f0.join("=");
    }
  }
  if(!block_name.isEmpty()) {
    ProcessBlock(block_name,block_lines);
  }
  
  return true;
}

/*
bool Profile::setSource(std::vector<QString> *values)
{
  QString section;
  int offset;

  profile_section.resize(0);
  profile_section.push_back(ProfileSection());
  profile_section.back().setName("");
  for(unsigned i=0;i<values->size();i++) {
    if((values->at(i).left(1)!=";")&&(values->at(i).left(1)!="#")) {
      if((values->at(i).left(1)=="[")&&(values->at(i).right(1)=="]")) {
	section=values->at(i).mid(1,values->at(i).length()-2);
	profile_section.push_back(ProfileSection());
	profile_section.back().setName(section);
      }
      else if(((offset=values->at(i).indexOf('='))!=-1)) {
	profile_section.back().
	  addValue(values->at(i).left(offset),
		   values->at(i).right(values->at(i).length()-offset-1).
		   trimmed());
      }
    }
  }

  return true;
}
*/

bool Profile::setSource(const QStringList &values)
{
  QString block_name;
  QMap<QString,QString> block_lines;

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
      block_lines[tag]=f0.join("=");
    }
  }
  if(!block_name.isEmpty()) {
    ProcessBlock(block_name,block_lines);
  }
  
  return true;
}


QString Profile::stringValue(const QString &section,const QString &tag,
			       const QString &default_str,bool *ok)
{
  QMap<QString,QString> block=d_blocks.value(section);
  if(block.size()==0) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_str;
  }
  if(ok!=NULL) {
    *ok=block.contains(tag);
  }
  return block.value(tag,default_str);
}


int Profile::intValue(const QString &section,const QString &tag,
		       int default_value,bool *ok)
{
  QMap<QString,QString> block=d_blocks.value(section);
  if(block.size()==0) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=block.contains(tag);
  }
  if(block.contains(tag)) {
    return block.value(tag).toInt();
  }
  return default_value;
}


int Profile::hexValue(const QString &section,const QString &tag,
		       int default_value,bool *ok)
{
  QMap<QString,QString> block=d_blocks.value(section);
  if(block.size()==0) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=block.contains(tag);
  }
  if(block.contains(tag)) {
    return block.value(tag).toInt(NULL,16);
  }
  return default_value;
}


double Profile::doubleValue(const QString &section,const QString &tag,
			    double default_value,bool *ok)
{
  QMap<QString,QString> block=d_blocks.value(section);
  if(block.size()==0) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=block.contains(tag);
  }
  if(block.contains(tag)) {
    return block.value(tag).toDouble();
  }
  return default_value;
}


bool Profile::boolValue(const QString &section,const QString &tag,
			 bool default_value,bool *ok)
{
  QMap<QString,QString> block=d_blocks.value(section);
  if(block.size()==0) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=block.contains(tag);
  }
  if(block.contains(tag)) {
    return (block.value(tag).toLower()=="yes")||
      (block.value(tag).toLower()=="true")||
      (block.value(tag).toLower()=="on")||
      (block.value(tag).toLower()=="1");
  }
  return default_value;
}


QTime Profile::timeValue(const QString &section,const QString &tag,
			   const QTime &default_value,bool *ok)
{
  QMap<QString,QString> block=d_blocks.value(section);
  if(block.size()==0) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=block.contains(tag);
  }
  if(block.contains(tag)) {
    QStringList fields=block.value(tag).split(":");
    if(fields.size()==2) {
      return QTime(fields[0].toInt(),fields[1].toInt(),0);
    }
    if(fields.size()==3) {
      return QTime(fields[0].toInt(),fields[1].toInt(),fields[2].toInt());
    }
    return QTime();
  }
  return default_value;
}


QHostAddress Profile::addressValue(const QString &section,const QString &tag,
				   const QHostAddress &default_value,bool *ok)
{
  return QHostAddress(stringValue(section,tag,default_value.toString(),ok));
}


QHostAddress Profile::addressValue(const QString &section,const QString &tag,
				     const QString &default_value,bool *ok)
{
  return addressValue(section,tag,QHostAddress(default_value),ok);
}


void Profile::clear()
{
  profile_source="";
  d_blocks.clear();
}


void Profile::ProcessBlock(const QString &name,
			   const QMap<QString,QString> &lines)
{
  d_blocks[name]=lines;
}
