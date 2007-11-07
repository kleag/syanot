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


#ifndef SYANOT_EASYDOCUMENT_H
#define SYANOT_EASYDOCUMENT_H

#include <QList>
#include <QString>
#include <QTextStream>

class EasyUtterance;

/**
 * This is a document from the easy format. It is made of a list of
 * uterances
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class EasyDocument : public QList<EasyUtterance*>
{
public:
  /**
    * Default Constructor
    */
  EasyDocument();

  /**
    * Default Destructor
    */
  virtual ~EasyDocument();

  inline const QString& fileName() const {return m_fileName;}
  inline void setFileName(const QString& fileName) {m_fileName = fileName;}

  EasyUtterance* utteranceIdentifiedBy(const QString& id);
  
private:
  QString m_fileName;
};

QTextStream& operator<<(QTextStream& s, const EasyDocument& e);

#endif // SYANOT_EASYDOCUMENT_H
