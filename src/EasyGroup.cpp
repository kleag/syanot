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


#include "EasyGroup.h"

#include "EasyForm.h"

EasyGroup::EasyGroup() : EasyConstituent(), m_forms()
{
}

EasyGroup::~EasyGroup()
{
}

void EasyGroup::saveAsXmlTo(QTextStream& s) const
{
  s<<"<Groupe type=\""<<m_type<<"\" id=\""<<m_id<<"\">\n";
  foreach(const EasyForm* f, m_forms)
  {
    f->saveAsXmlTo(s);
  }
  s<<"</Groupe>\n";
}

#include "EasyGroup.moc"