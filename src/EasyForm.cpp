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


#include "EasyForm.h"

EasyForm::EasyForm(EasyUtterance* parent) : EasyConstituent(parent)
{
}

EasyForm::~EasyForm()
{
}

void EasyForm::saveAsXmlTo(QTextStream& s) const
{
  QString form = m_form;
  s << " <F id=\""<<m_id<<"\">"<<form.replace("&","&amp;").replace("<","&lt;").replace(">","&gt;")<<"</F>\n";
}

#include "EasyForm.moc"

