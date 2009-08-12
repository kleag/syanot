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


#ifndef UTTERANCESPARTSMAPPING_H
#define UTTERANCESPARTSMAPPING_H

#include <QMap>
#include <QString>
#include <kparts/part.h>

class PartMatch;

/**
 * Contains the data concerning the mapping between a set of utterances and the kgraphviewer parts
 * that represent them
 *
 * @short Utterances - Parts Mapping
 * @author Gael de Chalendar <kleag@free.fr>
 */
class UtterancesPartsMapping : public QObject
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  UtterancesPartsMapping(QObject* parent = 0);

  /**
    * Default Destructor
    */
  virtual ~UtterancesPartsMapping();

  inline KParts::Part* partForUtterance(const QString& utt) {return m_utteranceIdPartMap[utt];}
  inline PartMatch* matchForPart(KParts::Part* part) {return m_partPartMatchMap[part];}
  inline PartMatch* matchForUtterance(const QString& utt) {return m_utteranceIdPartMatchMap[utt];}

  void addMapping(PartMatch* match);

  inline KParts::Part* currentPart() {return m_currentPart;}
  inline PartMatch* currentPartMatch() {return m_currentPartMatch;}

  void setCurrentPart(KParts::Part* part);
  void setCurrentPart(const QString& id);

  inline bool contains(const QString& id) const {return m_utteranceIdPartMap.contains(id);}

  inline QList<KParts::Part*> parts() {return m_partPartMatchMap.keys();}

  void clear();
  
private:

  QMap<QString, KParts::Part*> m_utteranceIdPartMap;
  QMap<KParts::Part*, PartMatch*> m_partPartMatchMap;
  QMap<QString, PartMatch*> m_utteranceIdPartMatchMap;
  
  KParts::Part* m_currentPart;
  PartMatch* m_currentPartMatch;
};

#endif // UTTERANCESPARTSMAPPING_H
