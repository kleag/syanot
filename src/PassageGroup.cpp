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


#include "PassageGroup.h"

#include "PassageWord.h"

#include <kdebug.h>

PassageGroup::PassageGroup(PassageUtterance* parent) : PassageConstituent(parent), m_forms()
{
}

PassageGroup::~PassageGroup()
{
}

void PassageGroup::saveAsXmlTo(QTextStream& s) const
{
  s<<"<Groupe type=\"";
  switch (m_type)
  {
    case GA:
    {
      s << "GA";
      break;
    }
    case GN:
    {
      s << "GN";
      break;
    }
    case GP:
    {
      s << "GP";
      break;
    }
    case GR:
    {
      s << "GR";
      break;
    }
    case NV:
    {
      s << "NV";
      break;
    }
    case PV:
    {
      s << "PV";
      break;
    }
    default:
      kError() << "Unknown group type" << m_type;
      s << "";
  }
  s<<"\" id=\""<<m_id<<"\">\n";
  foreach(const PassageWord* f, m_forms)
  {
    f->saveAsXmlTo(s);
  }
  s<<"</Groupe>\n";
}

void PassageGroup::push_back(PassageWord* form)
{
  m_forms.push_back(form);
  emit (changed(this));
}


#include "PassageGroup.moc"
