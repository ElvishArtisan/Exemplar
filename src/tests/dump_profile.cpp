// dump_profile.cpp
//
// Print a unified dump of a Profile configuration.
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
#include <QFileInfo>
#include <QStringList>

#include <cmdswitch.h>
#include <profile.h>

#include "dump_profile.h"

MainObject::MainObject()
  : QObject()
{
  QString err_msg;
  QStringList err_msgs;
  QStringList pathnames;
  QList<bool> is_directories;
  int err_count=0;
  
  CmdSwitch *cmd=new CmdSwitch("dump_profile",VERSION,DUMP_PROFILE_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--filename") {
      pathnames.push_back(cmd->value(i).trimmed());
      is_directories.push_back(false);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--dirname") {
      pathnames.push_back(cmd->value(i).trimmed());
      is_directories.push_back(true);
      cmd->setProcessed(i,true);
    }
  }
  if(pathnames.size()==0) {
    fprintf(stderr,"dump_profile: USAGE: %s\n",DUMP_PROFILE_USAGE);
    exit(1);
  }

  Profile *p=new Profile();
  for(int i=0;i<pathnames.size();i++) {
    if(is_directories.at(i)) {
      // Directory
      QStringList f0=pathnames.at(i).split("/",Qt::SkipEmptyParts);
      if(!f0.last().isEmpty()) {
	QString glob_template=f0.last();
	f0.removeLast();
	QString dirpath=f0.join("/");
	p->loadDirectory(dirpath,glob_template,&err_msgs);
	for(int i=0;i<err_msgs.size();i++) {
	  fprintf(stderr,"dump_profile: %s\n",
		  err_msgs.at(i).toUtf8().constData());
	}
	err_count+=err_msgs.size();
      }
    }
    else {
      // File
      if(!p->loadFile(pathnames.at(i),&err_msg)) {
	fprintf(stderr,"dump_profile: unable to load file \"%s\": %s\n",
		pathnames.at(i).toUtf8().constData(),
		err_msg.toUtf8().constData());
	err_count++;
      }
    }
  }

  printf("%s",p->dump().toUtf8().constData());

  exit(err_count);
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();
  return a.exec();
}
