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


#ifndef SYANOT_EASYRELATION_H
#define SYANOT_EASYRELATION_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QTextStream>

/**
 * This is a relation from the easy format.
 * 
 * It will be represented by an edge in the graphical view
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class EasyRelation : public QObject
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  EasyRelation() {}

  /**
    * Default Destructor
    */
  virtual ~EasyRelation() {}

  inline const QMap<QString,QString>& bounds() const {return m_bounds;}
  inline QMap<QString,QString>& bounds() {return m_bounds;}

  inline const QString& type() const {return m_type;}
  inline void setType(const QString& t) {m_type = t;}

  inline bool toPropagate() const {return m_toPropagate;}
  inline void setToPropagate(bool b) {m_toPropagate = b;}

  inline const QString& id() const {return m_id;}
  inline void setId(const QString& id) {m_id = id;}

  inline const QString& value() const {return m_value;}
  inline void setValue(const QString& v) {m_value = v;}

  private:
  QMap<QString,QString> m_bounds;
  QString m_type;
  bool m_toPropagate;
  QString m_id;
  QString m_value; // used only for ATT-SO relations to distinguish S and O attributes (values are respectively sujet and objet)
};

QTextStream& operator<<(QTextStream& s, const EasyRelation& r);

#endif // SYANOT_EASYRELATION_H
