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


#ifndef SYANOT_PASSAGEDOCUMENT_H
#define SYANOT_PASSAGEDOCUMENT_H

#include <QList>
#include <QString>
#include <QTextStream>

class PassageUtterance;

/**
 * This is a document from the easy format. It is made of a list of
 * uterances
 *
 * @author Gael de Chalendar <kleag@free.fr>
 */
class PassageDocument : public QObject
{
Q_OBJECT
public:
  /**
    * Default Constructor
    */
  PassageDocument();
  
  /**
    * Default Destructor
    */
  virtual ~PassageDocument();

  inline const QString& fileName() const {return m_fileName;}
  inline void setFileName(const QString& fileName) {m_fileName = fileName; emit changed(this);}

  inline const QList<PassageUtterance*>& utterances() const {return m_utterances;}
  
  PassageUtterance* utteranceIdentifiedBy(const QString& id);

  void push_back(PassageUtterance* utterance);
  
Q_SIGNALS:
  void changed(PassageDocument*);
  
public Q_SLOTS:
  void slotChanged(PassageUtterance*);
  
private:
  QString m_fileName;

  QList<PassageUtterance*> m_utterances;
};

QTextStream& operator<<(QTextStream& s, const PassageDocument& e);

#endif // SYANOT_EASYDOCUMENT_H
