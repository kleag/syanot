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
#include "EasyDocument.h"

EasyUtterance::EasyUtterance(EasyDocument* parent):QObject()
{
  connect(this, SIGNAL(changed(EasyUtterance*)),
      parent,SLOT(slotChanged(EasyUtterance*)));
}

EasyUtterance::~EasyUtterance()
{
}

void EasyUtterance::addRelation(EasyRelation* relation)
{
  m_relations.push_back(relation);
  emit changed(this);
}

void EasyUtterance::addConstituent(EasyConstituent* constituent)
{
  m_constituents.push_back(constituent);
  emit changed(this);
}

void EasyUtterance::removeRelationAt(int i)
{
  m_relations.removeAt(i);
  emit changed(this);
}

QTextStream& operator<<(QTextStream& s, const EasyUtterance& u)
{
  s << "<E id=\""<<u.id()<<"\">\n";
  s << "<constituants>\n";
  foreach (const EasyConstituent* c, u.constituents())
  {
    c->saveAsXmlTo(s);
  }
  s << "</constituants>\n";
  s << "<relations>\n";
  foreach (const EasyRelation* r, u.relations())
  {
    s << *r;
  }
  s << "</relations>\n";
  s << "</E>\n";
  return s;
}

#include "EasyUtterance.moc"

