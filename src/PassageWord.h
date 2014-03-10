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


#ifndef SYANOT_PASSAGEWORD_H
#define SYANOT_PASSAGEWORD_H

#include <QTextStream>

#include "PassageConstituent.h"

/**
 * This is a token from the easy format. It is made of a token string and an id.
 * 
 * It will be represented by a node in the graphical view
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class PassageWord : public PassageConstituent
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  PassageWord(PassageUtterance* parent);

  /**
    * Default Destructor
    */
  virtual ~PassageWord();

  inline const QList<QString>& tokens() {return m_tokens;}
  inline void setTokens(const QList<QString>& tokens) {m_tokens = tokens;}


  inline int start() const {return m_start;}
  inline void setStart(int f) {m_start = f;emit changed(this);}
  inline int end() const {return m_end;}
  inline void setEnd(int f) {m_end = f;emit changed(this);}
  inline const QString& pos() const {return m_pos;}
  inline void setPos(const QString& f) {m_pos = f;emit changed(this);}
  inline const QString& lemma() const {return m_lemma;}
  inline void setLemma(const QString& f) {m_lemma = f;emit changed(this);}
  inline const QString& form() const {return m_form;}
  inline void setForm(const QString& f) {m_form = f;emit changed(this);}

  inline const QList<QString>& mstag() {return m_mstag;}
  inline void setMstag(const QList<QString>& mstag) {m_mstag = mstag;}

  virtual void saveAsXmlTo(QTextStream& s) const;


private:
  QList<QString> m_tokens;
  int m_start;
  int m_end;
  QString m_pos;
  QString m_lemma;
  QString m_form;
  QList<QString> m_mstag;
};

#endif // SYANOT_EASYFORM_H
