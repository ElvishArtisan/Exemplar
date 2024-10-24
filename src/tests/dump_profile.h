// dump_profile.h
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

#ifndef DUMP_PROFILE_H
#define DUMP_PROFILE_H

#include <QObject>

#define DUMP_PROFILE_USAGE "--filename=<path-name> --dirname=<path-name> [...]\n\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject();
};


#endif  // DUMP_PROFILE_H
