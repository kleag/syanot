/* This file is part of Syanot
   Copyright (C) 2007 Gael de Chalendar <kleag@free.fr>

   Syanot is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA
*/


#include "EasyUtterance.h"

#include "EasyConstituent.h"
#include "EasyRelation.h"

EasyUtterance::EasyUtterance() : QObject()
{
}

EasyUtterance::~EasyUtterance()
{
}

QTextStream& operator<<(QTextStream& s, const EasyUtterance& u)
{
  s << "<E id=\""<<u.id()<<"\">\n";
  
  foreach (const EasyConstituent* c, u.constituents())
  {
    c->saveAsXmlTo(s);
  }
  foreach (const EasyRelation* r, u.relations())
  {
    s << *r;
  }
  s << "</E>\n";
}

#include "EasyUtterance.moc"

