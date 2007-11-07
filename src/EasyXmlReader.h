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


#ifndef SYANOT_EASYXMLREADER_H
#define SYANOT_EASYXMLREADER_H

#include <QXmlStreamReader>

class EasyDocument;
class EasyUtterance;
class EasyGroup;
class QIODevice;

/**
 * This is class allowing to load the content of an easy input file into memory.
 * 
 * @author Gael de Chalendar <kleag@free.fr>
 */
class EasyXmlReader : public QXmlStreamReader
{
public:
  EasyXmlReader(EasyDocument* doc);
  virtual ~EasyXmlReader();

  bool read(QIODevice *device);

private:
  void readUnknownElement();
  void readGroup();
  void readForm();
  void readDocument();
  void readConstituants();
  void readUtterance();
  void readRelations();
  void readRelation();
  void readGroupForm();

  EasyDocument* m_document;
  EasyGroup* m_currentGroup;
  EasyUtterance* m_currentUtterance;
};

#endif // SYANOT_EASYXMLREADER_H
