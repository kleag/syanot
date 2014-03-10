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


#ifndef SYANOT_PASSAGEXMLREADER_H
#define SYANOT_PASSAGEXMLREADER_H

#include <QXmlStreamReader>

class PassageDocument;
class PassageUtterance;
class PassageGroup;
class QIODevice;

/**
 * This is class allowing to load the content of an easy input file into memory.
 * 
 * @author Gael de Chalendar <kleag@free.fr>
 */
class PassageXmlReader : public QXmlStreamReader
{
public:
  PassageXmlReader(PassageDocument* doc);
  virtual ~PassageXmlReader();

  bool read(QIODevice *device);

private:
  void readUnknownElement();
  void readGroup();
  void readToken();
  void readDocument();
  void readUtterance();
  void readRelations();
  void readRelation();
  void readWord();
  void readGroupWord();
  void readMessage();

  PassageDocument* m_document;
  PassageGroup* m_currentGroup;
  PassageUtterance* m_currentUtterance;
};

#endif // SYANOT_EASYXMLREADER_H
