// profile.h
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
// EXEMPLAR_VERSION: 1.0.1
//

#ifndef PROFILE_H
#define PROFILE_H

#include <QHostAddress>
#include <QList>
#include <QMultiMap>
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
  bool setSource(const QStringList &values);
  QString stringValue(const QString &section,const QString &tag,
		      const QString &default_value="",bool *found=0);
  QStringList stringValues(const QString &section,const QString &tag);
  int intValue(const QString &section,const QString &tag,
	       int default_value=0,bool *found=0);
  QList<int> intValues(const QString &section,const QString &tag);
  int hexValue(const QString &section,const QString &tag,
	       int default_value=0,bool *found=0);
  QList<int> hexValues(const QString &section,const QString &tag);
  double doubleValue(const QString &section,const QString &tag,
		    double default_value=0.0,bool *found=0);
  QList<double> doubleValues(const QString &section,const QString &tag);
  bool boolValue(const QString &section,const QString &tag,
		 bool default_value=false,bool *found=0);
  QList<bool> boolValues(const QString &section,const QString &tag);
  QTime timeValue(const QString &section,const QString &tag,
		  const QTime &default_value=QTime(),bool *found=0);
  QList<QTime> timeValues(const QString &section,const QString &tag);
  QHostAddress addressValue(const QString &section,const QString &tag,
			    const QHostAddress &default_value=QHostAddress(),
			    bool *found=0);
  QHostAddress addressValue(const QString &section,const QString &tag,
			    const QString &default_value="",bool *found=0);
  QList<QHostAddress> addressValues(const QString &section,const QString &tag);

  void clear();
  QString dump() const;

 private:
  //  void ProcessBlock(const QString &name,
  //		    const QMultiMap<QString,QString> &lines);
  void ProcessBlock(const QString &name,
		    const QMap<QString,QStringList> &lines);
  QStringList InvertList(const QStringList &list) const;
  QString profile_source;
  QMap<QString,QMap<QString,QStringList> > d_blocks;
};


#endif  // PROFILE_H
