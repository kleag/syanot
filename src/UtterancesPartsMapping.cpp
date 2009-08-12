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



#include "UtterancesPartsMapping.h"
#include "PartMatch.h"

UtterancesPartsMapping::UtterancesPartsMapping(QObject* parent) : QObject(parent),
    m_utteranceIdPartMap(), m_partPartMatchMap(), m_utteranceIdPartMatchMap(),
    m_currentPart(0), m_currentPartMatch(0)
{
}

UtterancesPartsMapping::~UtterancesPartsMapping()
{
}

void UtterancesPartsMapping::addMapping(PartMatch* match)
{
  m_utteranceIdPartMap.insert(match->utteranceId(),match->part());
  m_partPartMatchMap.insert(match->part(), match);
  m_utteranceIdPartMatchMap.insert(match->utteranceId(),match);
}
  
void UtterancesPartsMapping::setCurrentPart(KParts::Part* part)
{
  m_currentPart = part;
  m_currentPartMatch = m_partPartMatchMap[part];
}

void UtterancesPartsMapping::setCurrentPart(const QString& id)
{
  m_currentPart = m_utteranceIdPartMap[id];
  m_currentPartMatch = m_utteranceIdPartMatchMap[id];
}

void UtterancesPartsMapping::clear()
{
  foreach (KParts::Part* part, m_partPartMatchMap.keys())
  {
    delete m_partPartMatchMap[part];
    delete part;
  }
  m_partPartMatchMap.clear();
  m_utteranceIdPartMap.clear();
  m_utteranceIdPartMatchMap.clear();
  
  m_currentPart = 0;
  m_currentPartMatch = 0;
}
