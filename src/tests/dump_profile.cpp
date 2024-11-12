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
#include <unistd.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>
#include <QStringList>

#include <cmdswitch.h>
#include <profile.h>

#include "dump_profile.h"

MainObject::MainObject()
  : QObject()
{
  QString err_msg;
  QStringList err_msgs;
  QStringList paths;
  bool use_section_ids=false;
  QString compare_to;
  
  CmdSwitch *cmd=new CmdSwitch("dump_profile",VERSION,DUMP_PROFILE_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--compare-to") {
      compare_to=cmd->value(i).trimmed();
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--path") {
      paths.push_back(cmd->value(i).trimmed());
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--use-section-ids") {
      use_section_ids=true;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"dump_profile: unrecognized option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }

  Profile *p=new Profile(use_section_ids);
  for(int i=0;i<paths.size();i++) {
    if(p->load(paths.at(i),&err_msgs)<0) {
      fprintf(stderr,"[%d]: %s",i,err_msgs.at(i).toUtf8().constData());
      exit(1);
    }
    else {
      for(int j=0;j<err_msgs.size();j++) {
	fprintf(stderr,"%s\n",err_msgs.at(j).toUtf8().constData());
      }
    }
  }

  if(compare_to.isEmpty()) {
    //
    // Print the dump
    //
    printf("%s",p->dump().toUtf8().constData());
  }
  else {
    //
    // Diff the dump against an exemplar
    //
    FILE *f=fopen("OUTPUT","w");
    if(f==NULL) {
      fprintf(stderr,
	      "dump_profile: failed to create temp file \"OUTPUT\" [%s]\n",
	      strerror(errno));
      exit(1);
    }
    fprintf(f,"%s",p->dump().toUtf8().constData());
    fclose(f);

    QStringList args;
    args.push_back("-u");
    args.push_back(compare_to);
    args.push_back("OUTPUT");
    QProcess *proc=new QProcess(this);
    proc->start("diff",args);
    proc->waitForFinished();
    if(proc->exitStatus()!=QProcess::NormalExit) {
      fprintf(stderr,"dump_profile: diff process crashed\n");
      exit(1);
    }
    if(proc->exitCode()>=2) {
      fprintf(stderr,"dump_profile: diff returned error [%s]\n",
	      proc->readAllStandardError().constData());
      exit(1);
    }
    if(proc->exitCode()!=0) {
      printf("**** DIFF BEGINS ****\n");
    }
    printf("%s",proc->readAllStandardOutput().constData());
    if(proc->exitCode()!=0) {
      printf("**** DIFF ENDS ****\n");
    }
    delete proc;
    unlink("OUTPUT");
  }

  exit(0);
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();
  return a.exec();
}
