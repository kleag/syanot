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


#include "EasyRelation.h"

QTextStream& operator<<(QTextStream& s, const EasyRelation& r)
{
  s<<"<relation xlink:type=\"extended\" type=\""<<r.type()<<"\" id=\""<<r.id()<<"\">\n";
  foreach (const QString& key, r.bounds().keys())
  {
    QString href = r.bounds()[key];
    href.remove("cluster_");
    s<<"<"<<key<<" xlink:type=\"locator\" xlink:href=\""<<href<<"\"/>\n";
  }
  if (r.type() == "MOD-N")
  {
    s<<"<a-propager booleen=\""<<(r.toPropagate()?"vrai":"faux")<<"\"/\n";
  }
  else if (r.type() == "ATB-SO")
  {
    s << "<s-o valeur=\""<<r.value()<<"\"/>\n";
  }
  s<<"</relation>\n";
}

#include "EasyRelation.moc"
