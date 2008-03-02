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


#include "PartMatch.h"

#include "EasyUtterance.h"
#include "EasyRelation.h"
#include "EasyForm.h"
#include "EasyGroup.h"

#include <kparts/partmanager.h>
#include <kdebug.h>

PartMatch::PartMatch(KParts::Part* p, EasyUtterance* u, QObject* parent) :
    QObject(parent),
    m_utteranceId(u->id()),
    m_part(p),
    m_utterance(u)
{
  connectSignals();
  QString previousFormId;
  QMap<QString,QString> graphAttribs;
  graphAttribs["id"] = m_utteranceId;
  graphAttribs["rankdir"] = "LR";
  graphAttribs["ranksep"] = "0.0";
  graphAttribs["fontsize"] = "14";
  setGraphAttributes(graphAttribs);
  
  const QList<EasyConstituent*>&  constituents = m_utterance->constituents();
  foreach (const EasyConstituent* constituent, constituents)
  {
    if (dynamic_cast<const EasyForm*>(constituent) != 0)
    {
      QMap<QString,QString> attribs;
      attribs["id"] = constituent->id();
      attribs["label"] = dynamic_cast<const EasyForm*>(constituent)->form();
      attribs["shape"] = "square";
      addNewNode(attribs);
      if (!previousFormId.isEmpty())
      {
        QMap<QString,QString> eattribs;
        eattribs["style"] = "invis";
        eattribs["id"] = previousFormId+constituent->id();
        addNewEdge(previousFormId,constituent->id(),eattribs);
      }
      previousFormId = constituent->id();
    }
    else if (dynamic_cast<const EasyGroup*>(constituent) != 0)
    {
      const EasyGroup* group = dynamic_cast<const EasyGroup*>(constituent);
      QMap<QString,QString> attribs;
      attribs["id"] = QString("cluster_") + group->id();
      QString color = "black";
      switch (group->type())
      {//GA, GN, GP, GR, NV, PV
        case EasyGroup::GA: color = "violet"; break;
        case EasyGroup::GN: color = "red"; break;
        case EasyGroup::GP: color = "blue"; break;
        case EasyGroup::GR: color = "orange"; break;
        case EasyGroup::NV: color = "green"; break;
        case EasyGroup::PV: color = "grey"; break;
        default:;
      }
      attribs["color"] = color;
      attribs["style"] = "filled";
      addNewSubgraph(attribs);
      foreach(EasyForm* form, group->forms())
      {
        QMap<QString,QString> attribs;
        attribs["id"] = form->id();
        attribs["label"] = form->form();
        attribs["shape"] = "square";
        attribs["style"] = "filled";
        attribs["color"] = "grey";
        attribs["fillcolor"] = "grey";
        attribs["fontsize"] = "14";
        addNewNodeToSubgraph(attribs, QString("cluster_") + group->id());
        if (!previousFormId.isEmpty())
        {
          QMap<QString,QString> eattribs;
          eattribs["style"] = "invis";
          eattribs["id"] = previousFormId+form->id();
          addNewEdge(previousFormId,form->id(),eattribs);
        }
        previousFormId = form->id();
      }
    }
  }
  const QList<EasyRelation*>&  relations = m_utterance->relations();
  foreach (const EasyRelation* relation, relations)
  {
    QMap<QString,QString> attribs;
    attribs["id"] = relation->id();
    attribs["label"] = relation->type();
    if (!relation->value().isEmpty())
    {
      attribs["label"] += QString(' ') += relation->value();
    }
    attribs["weight"] = "0.0";
    attribs["z"] = "5";
    if (relation->type() == "SUJ-V")
    {
      addNewEdge(relation->bounds()["sujet"],relation->bounds()["verbe"],attribs);
    }
    else if (relation->type() == "COD-V")
    {
      addNewEdge(relation->bounds()["cod"],relation->bounds()["verbe"],attribs);
    }
    else if (relation->type() == "MOD-V")
    {
      addNewEdge(relation->bounds()["modifieur"],relation->bounds()["verbe"],attribs);
    }
    else if (relation->type() == "MOD-A")
    {
      addNewEdge(relation->bounds()["modifieur"],relation->bounds()["adjectif"],attribs);
    }
    else if (relation->type() == "MOD-P")
    {
      addNewEdge(relation->bounds()["modifieur"],relation->bounds()["preposition"],attribs);
    }
    else if (relation->type() == "JUXT")
    {
      addNewEdge(relation->bounds()["premier"],relation->bounds()["suivant"],attribs);
    }
    else if (relation->type() == "APPOS")
    {
      addNewEdge(relation->bounds()["premier"],relation->bounds()["appose"],attribs);
    }
    else if (relation->type() == "COORD")
    {
      addNewEdge(relation->bounds()["coordonnant"],relation->bounds()["coord-g"],attribs);
      addNewEdge(relation->bounds()["coordonnant"],relation->bounds()["coord-d"],attribs);
    }
    else if (relation->type() == "MOD-R")
    {
      addNewEdge(relation->bounds()["modifieur"],relation->bounds()["adverbe"],attribs);
    }
    else if (relation->type() == "MOD-N")
    {
      addNewEdge(relation->bounds()["modifieur"],relation->bounds()["nom"],attribs);
    }
    else if (relation->type() == "ATB-SO")
    {
      addNewEdge(relation->bounds()["attribut"],relation->bounds()["verbe"],attribs);
    }
    else if (relation->type() == "CPL-V")
    {
      addNewEdge(relation->bounds()["complement"],relation->bounds()["verbe"],attribs);
    }
    else if (relation->type() == "AUX-V")
    {
      addNewEdge(relation->bounds()["auxiliaire"],relation->bounds()["verbe"],attribs);
    }
  }
  update();
}

PartMatch::~PartMatch()
{
}

void PartMatch::connectSignals()
{
  connect(this,SIGNAL(ssetReadWrite()),m_part,SLOT(setReadWrite()));
  connect(this,SIGNAL(sprepareAddNewEdge(QMap<QString,QString>)),
           m_part,SLOT(prepareAddNewEdge(QMap<QString,QString>)));
  connect(this,SIGNAL(ssetGraphAttributes(QMap<QString,QString>)),
           m_part,SLOT(slotSetGraphAttributes(QMap<QString,QString>)));
  connect(this,SIGNAL(saddNewNode(QMap<QString,QString>)),
           m_part,SLOT(slotAddNewNode(QMap<QString,QString>)));
  connect(this,SIGNAL(saddNewNodeToSubgraph(QMap<QString,QString>,QString)),
           m_part,SLOT(slotAddNewNodeToSubgraph(QMap<QString,QString>,QString)));
  connect(this,SIGNAL(saddNewSubgraph(QMap<QString,QString>)),
           m_part,SLOT(slotAddNewSubgraph(QMap<QString,QString>)));
  connect(this,SIGNAL(saddNewEdge(QString,QString,QMap<QString,QString>)),
           m_part,SLOT(slotAddNewEdge(QString,QString,QMap<QString,QString>)));
  connect(this,SIGNAL(supdate()),m_part,SLOT(slotUpdate()));
  connect(this,SIGNAL(ssaveTo(const QString&)),
           m_part,SLOT(saveTo(const QString&)));
  connect(this,SIGNAL(sremoveNode(const QString&)),
           m_part,SLOT(slotRemoveNode(const QString&)));
  connect(this,SIGNAL(saddAttribute(const QString&)),
           m_part,SLOT(slotAddAttribute(const QString&)));
  connect(this,SIGNAL(sremoveAttribute(const QString&,const QString&)),
           m_part,SLOT(slotRemoveAttribute(const QString&,const QString&)));
  connect(this,SIGNAL(sremoveEdge(const QString&)),
           m_part,SLOT(slotRemoveEdge(const QString&)));
  connect(this,
      SIGNAL(ssetAttribute(const QString&,const QString&,const QString&)),
      m_part,
      SLOT(slotSetAttribute(const QString&,const QString&,const QString&)));
           
  connect(m_part,SIGNAL(newEdgeAdded(QString,QString)),this,SLOT(slotNewEdgeAdded(QString,QString)));
  connect(m_part,SIGNAL(removeEdge(const QString&)),
                    this,SLOT(slotRemoveEdge(const QString&)));
  connect(m_part,SIGNAL( selectionIs(const QList<QString>&) ),
                    this,SLOT( slotSelectionIs(const QList<QString>&) ));
}

void PartMatch::slotNewEdgeAdded(QString src,QString tgt)
{
  kDebug() << src << tgt;
  EasyConstituent* srcc = m_utterance->idsToConstituentsMap()[src];
  EasyConstituent* tgtc = m_utterance->idsToConstituentsMap()[tgt];
  kDebug() << srcc << tgtc;
  EasyRelation* relation = new EasyRelation();
  relation->setId(QString("E1R")+QString::number(m_utterance->relations().size()+1));
  kDebug() << m_utteranceId << QString(QString("E1R")+QString::number(m_utterance->relations().size()+1));
  relation->setType(m_currentRelation);
  if (m_currentRelation=="SUJ-V")
  {
    relation->bounds().insert("sujet",src);
    relation->bounds().insert("verbe",tgt);
  }
  else if (m_currentRelation=="COD-V")
  {
    relation->bounds().insert("cod",src);
    relation->bounds().insert("verbe",tgt);
  }
  else if (m_currentRelation == "MOD-V")
  {
    relation->bounds().insert("modifieur",src);
    relation->bounds().insert("verbe",tgt);
  }
  else if (m_currentRelation == "MOD-A")
  {
    relation->bounds().insert("modifieur",src);
    relation->bounds().insert("adjectif",tgt);
  }
  else if (m_currentRelation == "MOD-P")
  {
    relation->bounds().insert("modifieur",src);
    relation->bounds().insert("preposition",tgt);
  }
  else if (m_currentRelation == "JUXT")
  {
    relation->bounds().insert("premier",src);
    relation->bounds().insert("suivant",tgt);
  }
  else if (m_currentRelation == "APPOS")
  {
    relation->bounds().insert("premier",src);
    relation->bounds().insert("appose",tgt);
  }
  else if (m_currentRelation == "COORD")
  {
    relation->bounds().insert("coordonnant",src);
    relation->bounds().insert("coord-g",tgt);
    /// @TODO handle coordination as needed
//     addNewEdge(relation->bounds()["coordonnant"],relation->bounds()["coord-d"],attribs);
  }
  else if (m_currentRelation == "MOD-R")
  {
    relation->bounds().insert("modifieur",src);
    relation->bounds().insert("adverbe",tgt);
  }
  else if (m_currentRelation == "MOD-N")
  {
    relation->bounds().insert("modifieur",src);
    relation->bounds().insert("nom",tgt);
  }
  else if (m_currentRelation == "ATB-SO")
  {
    relation->bounds().insert("attribut",src);
    relation->bounds().insert("verbe",tgt);
  }
  else if (m_currentRelation == "CPL-V")
  {
    relation->bounds().insert("complement",src);
    relation->bounds().insert("verbe",tgt);
  }
  else if (m_currentRelation == "AUX-V")
  {
    relation->bounds().insert("auxiliaire",src);
    relation->bounds().insert("verbe",tgt);
  }
  
  m_utterance->addRelation(relation);
  update();
}

void PartMatch::slotRemoveEdge(const QString& id)
{
  kDebug() << id;
  for (int i = 0; i < m_utterance->relations().size(); i++)
  {
    EasyRelation* rel = m_utterance->relations().at(i);
    if (rel->id() == id)
    {
      m_utterance->removeRelationAt(i);
      delete rel;
      removeEdge(id);
      update();
      break;
    }
  }
}

void PartMatch::prepareAddNewEdge(QMap<QString,QString> attribs)
{
  kDebug() << attribs;
  bool relabeled = false; // will be true if there is an edge in the selection
                          // in this case, we will relabel it instead of
                          // creating a new edge
  if (!m_selection.isEmpty())
  {
    QList<EasyRelation*>&  relations = m_utterance->relations();
    foreach (EasyRelation* relation, relations)
    {
      if (m_selection.contains(relation->id()))
      {
        relation->setType(attribs["label"]);
        setAttribute(relation->id(),"label",attribs["label"]);
        relabeled = true;
      }
    }
  }
  if (!relabeled)
  {
    m_currentRelation = attribs["label"];
    emit sprepareAddNewEdge(attribs);
  }
  else
  {
    update();
  }
}

void PartMatch::slotSelectionIs(const QList<QString>& selection)
{
  kDebug() << selection;
  m_selection = selection;
}

#include "PartMatch.moc"
