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


#include "EasyXmlReader.h"

#include "EasyConstituent.h"
#include "EasyDocument.h"
#include "EasyUtterance.h"
#include "EasyGroup.h"
#include "EasyForm.h"
#include "EasyRelation.h"


#include <kdebug.h>

#include <QXmlStreamReader>
#include <QIODevice>

EasyXmlReader::EasyXmlReader(EasyDocument* doc) :
  m_document(doc), m_currentGroup(0), m_currentUtterance(0)
{
}

EasyXmlReader::~EasyXmlReader()
{
}

bool EasyXmlReader::read(QIODevice *device)
{
//   kDebug();
  setDevice(device);

  while (!atEnd())
  {
    readNext();

    if (isStartElement())
    {
      if (name() == "DOCUMENT")
      {
        readDocument();
      }
      else
      {
        raiseError(QObject::tr("The file is not in an Easy format."));
      }
    }
  }
  return !error();
}


void EasyXmlReader::readUnknownElement()
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

void EasyXmlReader::readDocument()
{
//   kDebug();
  Q_ASSERT(isStartElement() && name() == "DOCUMENT");

  m_document->setFileName(attributes().value("fichier").toString());
  while (!atEnd())
  {
    readNext();

    if (isEndElement())
        break;

    if (isStartElement())
    {
      if (name() == "E")
        readUtterance();
      else
        readUnknownElement();
    }
  }
}

void EasyXmlReader::readUtterance()
{
//   kDebug() << name().toString();
  Q_ASSERT(isStartElement() && name() == "E");

  m_currentUtterance = new EasyUtterance(m_document);
  m_currentUtterance->setId(attributes().value("id").toString());
  m_document->push_back(m_currentUtterance);
  while (!atEnd())
  {
    readNext();

    if (isEndElement())
        break;

    if (isStartElement())
    {
      if (name() == "constituants")
        readConstituants();
      else if (name() == "relations")
        readRelations();
      else if (name() == "F")
        readForm();
      else if (name() == "Groupe")
      {
        readGroup();
      }
      else
        readUnknownElement();
    }
  }
}

void EasyXmlReader::readConstituants()
{
//   kDebug() << name().toString();
  Q_ASSERT(isStartElement() && name() == "constituants");

  while (!atEnd())
  {
    readNext();

    if (isEndElement())
        break;

    if (isStartElement())
    {
      if (name() == "F")
        readForm();
      else if (name() == "Groupe")
      {
        readGroup();
      }
      else
        readUnknownElement();
    }
  }
}

void EasyXmlReader::readRelations()
{
//   kDebug() << name().toString();
  Q_ASSERT(isStartElement() && name() == "relations");

  while (!atEnd())
  {
    readNext();

    if (isEndElement())
        break;

    if (isStartElement())
    {
      if (name() == "relation")
        readRelation();
      else
        readUnknownElement();
    }
  }
}

void EasyXmlReader::readRelation()
{
  kDebug() << name().toString();
  Q_ASSERT(isStartElement() && name() == "relation");

  EasyRelation* relation = new EasyRelation();
  relation->setType(attributes().value("type").toString());
  relation->setId(attributes().value("id").toString());
  while (!atEnd())
  {
    readNext();

    if (isEndElement() && name() == "relation")
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
        relation->bounds().insert(name().toString(),attributes().value("xlink:href").toString());
      }
    }
  }
  m_currentUtterance->addRelation(relation);
}

void EasyXmlReader::readGroup()
{
//   kDebug() << name().toString();
  Q_ASSERT(isStartElement() && name() == "Groupe");

  m_currentGroup = new EasyGroup(m_currentUtterance);

//   kDebug() << attributes().value("type").toString() << attributes().value("id").toString();
  if (attributes().value("type")=="GA")
  {
    m_currentGroup->setType(EasyGroup::GA);
  }
  else if (attributes().value("type")=="GN")
  {
    m_currentGroup->setType(EasyGroup::GN);
  }
  else if (attributes().value("type")=="GP")
  {
    m_currentGroup->setType(EasyGroup::GP);
  }
  else if (attributes().value("type")=="GR")
  {
    m_currentGroup->setType(EasyGroup::GR);
  }
  else if (attributes().value("type")=="NV")
  {
    m_currentGroup->setType(EasyGroup::NV);
  }
  else if (attributes().value("type")=="PV")
  {
    m_currentGroup->setType(EasyGroup::PV);
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
      if (name() == "F")
      {
        readGroupForm();
      }
      else
      {
        readUnknownElement();
      }
    }
  }
}

void EasyXmlReader::readGroupForm()
{
//   kDebug() << name().toString();
  Q_ASSERT(isStartElement() && name() == "F");

  QString id = attributes().value("id").toString();
  QString form = readElementText();
//   kDebug() << id << form;
//   foreach (QXmlStreamAttribute a, attributes())
//   {
//     kDebug() << a.name().toString() << a.value().toString();
//   }
  EasyForm* f = new EasyForm(m_currentUtterance);
  f->setId(id);
  f->setForm(form);
  m_currentGroup->push_back(f);
//   m_currentUtterance->idsToConstituentsMap().insert(f->id(),f);
}

void EasyXmlReader::readForm()
{
//   kDebug() << name().toString();
  Q_ASSERT(isStartElement() && name() == "F");

  QString id = attributes().value("id").toString();
  QString form = readElementText();
  EasyForm* f = new EasyForm(m_currentUtterance);
  f->setId(id);
  f->setForm(form);
  m_currentUtterance->addConstituent(f);
  m_currentUtterance->idsToConstituentsMap().insert(f->id(),f);
}

