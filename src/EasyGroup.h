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


#ifndef SYANOT_EASYGROUP_H
#define SYANOT_EASYGROUP_H

#include "EasyConstituent.h"

#include <QList>

class EasyForm;

/**
 * This is a group of tokens from the easy format. It is a list of tokens and
 * has a type.
 * 
 * It will be represented by a subgraph in the graphical view
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class EasyGroup : public EasyConstituent
{
  Q_OBJECT
public:
  enum EasyGroupType
  {
    Undefined, GA, GN, GP, GR, NV, PV
  };

  /**
    * Default Constructor
    */
  EasyGroup(EasyUtterance* parent);

  /**
    * Default Destructor
    */
  virtual ~EasyGroup();

  inline EasyGroupType type() const {return m_type;}
  void setType(const EasyGroupType t);

  inline const QList<EasyForm*>& forms()const {return m_forms;}
//   inline QList<EasyForm*>& forms() {return m_forms;}
  inline void setForms(QList<EasyForm*>& forms) {m_forms = forms;emit changed(this);}

  virtual void saveAsXmlTo(QTextStream& s) const;

  void push_back(EasyForm* form);

  void removeForm(EasyForm* form) {m_forms.removeAll(form);}

  inline bool isEmpty() const {return m_forms.isEmpty();}
  
private:
  virtual void dummy() {};

private:
  EasyGroupType m_type;
  QList<EasyForm*> m_forms;
};

#endif // SYANOT_EASYGROUP_H
