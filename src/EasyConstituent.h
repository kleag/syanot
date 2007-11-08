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


#ifndef SYANOT_EASYCONSTITUENT_H
#define SYANOT_EASYCONSTITUENT_H

#include <QObject>
#include <QString>
#include <QTextStream>

/**
 * This is the base class for forms and groups, an utterance being a list of
 * constituents
 * 
 * @author Gael de Chalendar <kleag@free.fr>
 */
class EasyConstituent : public QObject
{
public:
  /**
    * Default Constructor
    */
  EasyConstituent();

  virtual ~EasyConstituent();

  inline const QString& id() const {return m_id;}
  inline void setId(const QString& id) {m_id = id;}

  /**
  * This class is an interface. It cannot be
  * instanciated.
  */
  virtual void saveAsXmlTo(QTextStream& s) const = 0;

protected:
  QString m_id;
};

#endif // SYANOT_EASYCONSTITUENT_H