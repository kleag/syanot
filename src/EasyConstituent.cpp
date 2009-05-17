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


#include "EasyConstituent.h"
#include "EasyUtterance.h"

EasyConstituent::EasyConstituent(EasyUtterance* parent) : QObject(parent)
{
//   connect(this, SIGNAL(changed(EasyConstituent*)),
//     parent, SLOT(slotChanged(EasyConstituent*)));
}

EasyConstituent::~EasyConstituent()
{
}

void EasyConstituent::setId(const QString& id)
{
  m_id = id;
  emit(changed(this));
}

QString EasyConstituent::text() const
{
  return "";
}

#include "EasyConstituent.moc"
