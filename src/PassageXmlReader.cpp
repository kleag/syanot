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


#include "PassageXmlReader.h"

#include "PassageConstituent.h"
#include "PassageDocument.h"
#include "PassageUtterance.h"
#include "PassageGroup.h"
#include "PassageToken.h"
#include "PassageWord.h"
#include "PassageRelation.h"


#include <kdebug.h>

#include <QXmlStreamReader>
#include <QIODevice>

PassageXmlReader::PassageXmlReader(PassageDocument* doc) :
  m_document(doc), m_currentGroup(0), m_currentUtterance(0)
{
}

PassageXmlReader::~PassageXmlReader()
{
}

bool PassageXmlReader::read(QIODevice *device)
{
//   kDebug();
  setDevice(device);

  while (!atEnd())
  {
    readNext();

    if (isStartElement())
    {
      if (name() == "Document")
      {
        readDocument();
      }
      else
      {
        raiseError(QObject::tr("The file is not in an Passage format."));
      }
    }
  }
  return !error();
}


void PassageXmlReader::readUnknownElement()
{
//   kDebug();
  Q_ASSERT(isStartElement());

  while (!atEnd())
  {
    readNext();

    if (isEndElement())
    {
      break;
    }

    if (isStartElement())
    {
      readUnknownElement();
    }
  }
}

void PassageXmlReader::readDocument()
{
//   kDebug();
  Q_ASSERT(isStartElement() && name() == "Document");

  m_document->setFileName(attributes().value("file").toString());
  while (!atEnd())
  {
    readNext();

    if (isEndElement())
        break;

    if (isStartElement())
    {
      if (name() == "Sentence")
        readUtterance();
      else
        readUnknownElement();
    }
  }
}

void PassageXmlReader::readUtterance()
{
//   kDebug();
  Q_ASSERT(isStartElement() && name() == "Sentence");

  m_currentUtterance = new PassageUtterance(m_document);
  m_currentUtterance->setId(attributes().value("id").toString());
  m_document->push_back(m_currentUtterance);
  while (!atEnd())
  {
    readNext();

    if (isEndElement())
        break;

    if (isStartElement())
    {
      if (name() == "T")
      {
        readToken();
      }
      else if (name() == "G")
      {
        readGroup();
      }
      else if (name() == "W")
      {
        readWord();
      }
      else if (name() == "R")
        readRelation();
      else if (name() == "M")
        readMessage();
      else
        readUnknownElement();
    }
  }
}

void PassageXmlReader::readToken()
{
//   kDebug();
  Q_ASSERT(isStartElement() && name() == "T");

  QString id = attributes().value("id").toString();
  QString form = readElementText();
  PassageWord* f = new PassageWord(m_currentUtterance);
  f->setId(id);
  f->setForm(form);
  m_currentUtterance->addConstituent(f);
  m_currentUtterance->idsToConstituentsMap().insert(f->id(),f);
}

void PassageXmlReader::readGroup()
{
  Q_ASSERT(isStartElement() && name() == "G");

  m_currentGroup = new PassageGroup(m_currentUtterance);

//   kDebug() << attributes().value("type").toString() << attributes().value("id").toString();
  if (attributes().value("type")=="GA")
  {
    m_currentGroup->setType(PassageGroup::GA);
  }
  else if (attributes().value("type")=="GN")
  {
    m_currentGroup->setType(PassageGroup::GN);
  }
  else if (attributes().value("type")=="GP")
  {
    m_currentGroup->setType(PassageGroup::GP);
  }
  else if (attributes().value("type")=="GR")
  {
    m_currentGroup->setType(PassageGroup::GR);
  }
  else if (attributes().value("type")=="NV")
  {
    m_currentGroup->setType(PassageGroup::NV);
  }
  else if (attributes().value("type")=="PV")
  {
    m_currentGroup->setType(PassageGroup::PV);
  }
  m_currentGroup->setId(attributes().value("id").toString());
  m_currentUtterance->addConstituent(m_currentGroup);
  m_currentUtterance->idsToConstituentsMap().insert(m_currentGroup->id(),m_currentGroup);
  while (!atEnd())
  {
    readNext();

    if (isEndElement())
        break;

    if (isStartElement())
    {
      if (name() == "W")
      {
        readGroupWord();
      }
      else
      {
        readUnknownElement();
      }
    }
  }
}

void PassageXmlReader::readGroupWord()
{
  Q_ASSERT(isStartElement() && name() == "W");

  QString id = attributes().value("id").toString();
  QString form = readElementText();
//   kDebug() << id << form;
//   foreach (QXmlStreamAttribute a, attributes())
//   {
//     kDebug() << a.name().toString() << a.value().toString();
//   }
  PassageWord* f = new PassageWord(m_currentUtterance);
  f->setId(id);
  f->setForm(form);
  m_currentGroup->push_back(f);
  m_currentUtterance->idsToConstituentsMap().insert(f->id(),f);
}

void PassageXmlReader::readWord()
{
//   kDebug();
  Q_ASSERT(isStartElement() && name() == "W");

  QString id = attributes().value("id").toString();
  QString form = readElementText();
  PassageWord* f = new PassageWord(m_currentUtterance);
  f->setId(id);
  f->setForm(form);
  m_currentUtterance->addConstituent(f);
  m_currentUtterance->idsToConstituentsMap().insert(f->id(),f);
}

void PassageXmlReader::readRelation()
{
//   kDebug();
  Q_ASSERT(isStartElement() && name() == "R");

  PassageRelation* relation = new PassageRelation();
  relation->setId(attributes().value("id").toString());
  relation->setType(attributes().value("type").toString());
  while (!atEnd())
  {
    readNext();

    if (isEndElement() && name() == "R")
        break;

    if (isStartElement())
    {
      if (name() == "a-propager")
      {
        if (attributes().value("booleen") == "vrai")
        {
          relation->setToPropagate(true);
        }
        else if (attributes().value("booleen") == "faux")
        {
          relation->setToPropagate(false);
        }
      }
      else if (name() == "s-o")
      {
        relation->setValue(attributes().value("valeur").toString());
      }
      else 
      {
        relation->bounds().insert(name().toString(),attributes().value(name().toString()).toString());
      }
    }
  }
  m_currentUtterance->addRelation(relation);
}

void PassageXmlReader::readMessage()
{
//   kDebug();
  Q_ASSERT(isStartElement() && name() == "M");

/*  QString id = attributes().value("id").toString();
  QString form = readElementText();
  PassageWord* f = new PassageWord(m_currentUtterance);
  f->setId(id);
  f->setForm(form);
  m_currentUtterance->addConstituent(f);
  m_currentUtterance->idsToConstituentsMap().insert(f->id(),f);*/
}

