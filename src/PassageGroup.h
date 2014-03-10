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


#ifndef SYANOT_PASSAGEGROUP_H
#define SYANOT_PASSAGEGROUP_H

#include "PassageConstituent.h"

#include <QList>

class PassageWord;

/**
 * This is a group of tokens from the easy format. It is a list of tokens and
 * has a type.
 * 
 * It will be represented by a subgraph in the graphical view
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class PassageGroup : public PassageConstituent
{
  Q_OBJECT
public:
  enum PassageGroupType
  {
    Undefined, GA, GN, GP, GR, NV, PV
  };

  /**
    * Default Constructor
    */
  PassageGroup(PassageUtterance* parent);

  /**
    * Default Destructor
    */
  virtual ~PassageGroup();

  inline PassageGroupType type() const {return m_type;}
  inline void setType(const PassageGroupType t) {m_type = t;emit changed(this);}

  inline const QList<PassageWord*>& forms()const {return m_forms;}
//   inline QList<PassageForm*>& forms() {return m_forms;}
  inline void setForms(QList<PassageWord*>& forms) {m_forms = forms;emit changed(this);}

  virtual void saveAsXmlTo(QTextStream& s) const;

  void push_back(PassageWord* form);
  
private:
  virtual void dummy() {};

private:
  PassageGroupType m_type;
  QList<PassageWord*> m_forms;
};

#endif // SYANOT_EASYGROUP_H
