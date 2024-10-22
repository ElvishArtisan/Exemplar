// syprofile.h
//
// Class for reading INI configuration files.
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

#ifndef SYPROFILE_H
#define SYPROFILE_H

#include <QHostAddress>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QTime>

class Profile
{
 public:
  Profile();
  QStringList sectionNames() const;
  QString source() const;
  bool setSource(const QString &filename,QString *err_msg=NULL);
  //  bool setSource(std::vector<QString> *values);
  bool setSource(const QStringList &values);
  QString stringValue(const QString &section,const QString &tag,
		      const QString &default_value="",bool *ok=0);
  int intValue(const QString &section,const QString &tag,
	       int default_value=0,bool *ok=0);
  int hexValue(const QString &section,const QString &tag,
	       int default_value=0,bool *ok=0);
  double doubleValue(const QString &section,const QString &tag,
		    double default_value=0.0,bool *ok=0);
  bool boolValue(const QString &section,const QString &tag,
		 bool default_value=false,bool *ok=0);
  QTime timeValue(const QString &section,const QString &tag,
		  const QTime &default_value=QTime(),bool *ok=0);
  QHostAddress addressValue(const QString &section,const QString &tag,
			    const QHostAddress &default_value=QHostAddress(),
			    bool *ok=0);
  QHostAddress addressValue(const QString &section,const QString &tag,
			    const QString &default_value="",bool *ok=0);
  void clear();

 private:
  void ProcessBlock(const QString &name,const QMap<QString,QString> &lines);
  QString profile_source;
  QMap<QString,QMap<QString,QString> > d_blocks;
};


#endif  // SYPROFILE_H
