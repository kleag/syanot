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


#include "PassageWord.h"

PassageWord::PassageWord(PassageUtterance* parent) : PassageConstituent(parent)
{
}

PassageWord::~PassageWord()
{
}

void PassageWord::saveAsXmlTo(QTextStream& s) const
{
  s << " <W id=\""<<m_id<<"\"";
  s << " tokens=\"";
  foreach (const QString& tok, m_tokens)
  {
    s << tok << " ";
  }
  s << "\" start=\""<<m_start<<"\" end=\""<<m_end<<"\" pos=\""<<m_pos<<"\" lemma=\""<<m_lemma<<"\"";
  s << " form=\""<<m_form<<"\" ";
  s << " mstag=\"";
  foreach (const QString& tag, m_mstag)
  {
    s << tag << " ";
  }
  s << "/>\n";
}

#include "PassageWord.moc"

