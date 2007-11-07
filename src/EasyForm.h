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


#ifndef SYANOT_EASYFORM_H
#define SYANOT_EASYFORM_H

#include <QTextStream>

#include "EasyConstituent.h"

/**
 * This is a token from the easy format. It is made of a token string and an id.
 * 
 * It will be represented by a node in the graphical view
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class EasyForm : public EasyConstituent
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  EasyForm();

  /**
    * Default Destructor
    */
  virtual ~EasyForm();

  inline const QString& form() const {return m_form;}
  inline void setForm(const QString& f) {m_form = f;}

  virtual void saveAsXmlTo(QTextStream& s) const;


private:
  QString m_form;
};

#endif // SYANOT_EASYFORM_H
