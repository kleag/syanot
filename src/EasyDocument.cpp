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


#include "EasyDocument.h"
#include "EasyUtterance.h"

EasyDocument::EasyDocument() : QList<EasyUtterance*>()
{
}

EasyDocument::~EasyDocument()
{
}

EasyUtterance* EasyDocument::utteranceIdentifiedBy(const QString& id)
{
  EasyUtterance* utterance = 0;
  foreach (EasyUtterance* u, *this)
  {
    if (u->id() == id)
    {
      utterance = u;
      break;
    }
  }
  return utterance;
}

QTextStream& operator<<(QTextStream& s, const EasyDocument& d)
{
  s << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
  "<!DOCTYPE DOCUMENT SYSTEM \"Guide.ph2.dtd\">\n"
  "<DOCUMENT fichier=\"" << d.fileName() << "\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n";
  foreach (const EasyUtterance* u, d)
  {
    s << *u;
  }
  s << "</DOCUMENT>";
}
