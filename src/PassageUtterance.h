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


#ifndef SYANOT_PASSAGEUTTERANCE_H
#define SYANOT_PASSAGEUTTERANCE_H


#include <QObject>
#include <QMap>
#include <QTextStream>

class PassageConstituent;
class PassageRelation;
class PassageDocument;

/**
 * This is an utterance from the passage format. It is made of a list of
 * constituents and a list of relations
 * 
 * It will be represented by the whole graph in the graphical view
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class PassageUtterance : public QObject
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  PassageUtterance(PassageDocument* parent);

  /**
    * Default Destructor
    */
  virtual ~PassageUtterance();

  inline const QList<PassageConstituent*>& constituents() const {return m_constituents;}
//   inline QList<PassageConstituent*>& constituents() {return m_constituents;}
  inline void setConstituents(const QList<PassageConstituent*>& c) {m_constituents = c; emit changed(this);}

  inline const QString& id() const {return m_id;}
  inline void setId(const QString& id) {m_id = id; emit changed(this);}

  inline const QMap<QString, PassageConstituent*>& idsToConstituentsMap() const {return m_idsToConstituentsMap;}
  inline QMap<QString, PassageConstituent*>& idsToConstituentsMap() {return m_idsToConstituentsMap;}

  inline const QList<PassageRelation*>& relations() const {return m_relations;}
  inline QList<PassageRelation*>& relations() {return m_relations;}

  void addRelation(PassageRelation* relation);
  void addConstituent(PassageConstituent* constituent);

  void removeRelationAt(int i);
  
Q_SIGNALS:
  void changed(PassageUtterance*);
  
private:
  QString m_id;
  QList<PassageConstituent*> m_constituents;
  QMap<QString, PassageConstituent*> m_idsToConstituentsMap;
  QList<PassageRelation*> m_relations;
};

QTextStream& operator<<(QTextStream& s, const PassageUtterance& e);

#endif // SYANOT_PASSAGEFORM_H
