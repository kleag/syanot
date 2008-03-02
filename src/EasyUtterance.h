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


#ifndef SYANOT_EASYUTTERANCE_H
#define SYANOT_EASYUTTERANCE_H


#include <QObject>
#include <QMap>
#include <QTextStream>

class EasyConstituent;
class EasyRelation;
class EasyDocument;

/**
 * This is an utterance from the easy format. It is made of a list of
 * constituents and a list of relations
 * 
 * It will be represented by the whole graph in the graphical view
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class EasyUtterance : public QObject
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  EasyUtterance(EasyDocument* parent);

  /**
    * Default Destructor
    */
  virtual ~EasyUtterance();

  inline const QList<EasyConstituent*>& constituents() const {return m_constituents;}
//   inline QList<EasyConstituent*>& constituents() {return m_constituents;}
  inline void setConstituents(const QList<EasyConstituent*>& c) {m_constituents = c; emit changed(this);}

  inline const QString& id() const {return m_id;}
  inline void setId(const QString& id) {m_id = id; emit changed(this);}

  inline const QMap<QString, EasyConstituent*>& idsToConstituentsMap() const {return m_idsToConstituentsMap;}
  inline QMap<QString, EasyConstituent*>& idsToConstituentsMap() {return m_idsToConstituentsMap;}

  inline const QList<EasyRelation*>& relations() const {return m_relations;}
  inline QList<EasyRelation*>& relations() {return m_relations;}

  void addRelation(EasyRelation* relation);
  void addConstituent(EasyConstituent* constituent);

  void removeRelationAt(int i);
  
Q_SIGNALS:
  void changed(EasyUtterance*);
  
private:
  QString m_id;
  QList<EasyConstituent*> m_constituents;
  QMap<QString, EasyConstituent*> m_idsToConstituentsMap;
  QList<EasyRelation*> m_relations;
};

QTextStream& operator<<(QTextStream& s, const EasyUtterance& e);

#endif // SYANOT_EASYFORM_H
