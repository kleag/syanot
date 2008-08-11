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

#include <kparts/partmanager.h>
#include <kdebug.h>
#include <klocale.h>

#include <QMenu>
#include <KAction>
#include <QUuid>

PartMatch::PartMatch(KParts::Part* p, EasyUtterance* u, QObject* parent) :
    QObject(parent),
    m_utteranceId(u->id()),
    m_part(p),
    m_utterance(u),
    m_addingGroup(false)
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
    addRelation(relation);
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
  connect(this,SIGNAL(saddExistingNodeToSubgraph(QMap<QString,QString>,QString)),
           m_part,SLOT(slotAddExistingNodeToSubgraph(QMap<QString,QString>,QString)));
  connect(this,SIGNAL(saddNewSubgraph(QMap<QString,QString>)),
           m_part,SLOT(slotAddNewSubgraph(QMap<QString,QString>)));
  connect(this,SIGNAL(saddNewEdge(QString,QString,QMap<QString,QString>)),
           m_part,SLOT(slotAddNewEdge(QString,QString,QMap<QString,QString>)));
  connect(this,SIGNAL(supdate()),m_part,SLOT(slotUpdate()));
  connect(this,SIGNAL(ssaveTo(const QString&)),
           m_part,SLOT(saveTo(const QString&)));
  connect(this,SIGNAL(sremoveNode(const QString&)),
           m_part,SLOT(slotRemoveNode(const QString&)));
  connect(this,SIGNAL(sremoveElement(const QString&)),
           m_part,SLOT(slotRemoveElement(const QString&)));
  connect(this,SIGNAL(sremoveNodeFromSubgraph(const QString&, const QString&)),
           m_part,SLOT(slotRemoveNodeFromSubgraph(const QString&, const QString&)));
  connect(this,SIGNAL(sremoveSubgraph(const QString&)),
           m_part,SLOT(slotRemoveSubgraph(const QString&)));
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
  connect(this,SIGNAL(ssetHighlighting(bool)),
           m_part,SLOT(slotSetHighlighting(bool)));
  connect(this,SIGNAL(sprepareSelectElements()),
           m_part,SLOT(slotPrepareToSelect()));
  emit ssetHighlighting(true);


  connect(m_part,SIGNAL(newEdgeAdded(QString,QString)),this,SLOT(slotNewEdgeAdded(QString,QString)));
  connect(m_part,SIGNAL(removeEdge(const QString&)),
                    this,SLOT(slotRemoveEdge(const QString&)));
  connect(m_part,SIGNAL(removeElement(const QString&)),
                    this,SLOT(slotRemoveElement(const QString&)));
  connect(m_part,SIGNAL( selectionIs(const QList<QString>, const QPoint&)),
           this,SLOT( slotSelectionIs(const QList<QString>,const QPoint&) ));
  connect(
    m_part,
    SIGNAL( newEdgeFinished(
      const QString&, const QString&, const QMap<QString, QString>&) ),
    this,
    SLOT( slotNewEdgeFinished(
      const QString&, const QString&, const QMap<QString, QString>&) ) );

  connect(
      m_part,SIGNAL( contextMenuEvent(const QString&, const QPoint&) ),
      this, SLOT( slotContextMenuEvent(const QString&, const QPoint&) ) );
}

void PartMatch::addRelation(const EasyRelation* relation)
{
  kDebug() << relation->id();
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
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["sujet"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["verbe"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["sujet"],relation->bounds()["verbe"],attribs);
  }
  else if (relation->type() == "COD-V")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["cod"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["verbe"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["cod"],relation->bounds()["verbe"],attribs);
  }
  else if (relation->type() == "MOD-V")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["modifieur"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["verbe"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["modifieur"],relation->bounds()["verbe"],attribs);
  }
  else if (relation->type() == "MOD-A")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["modifieur"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["adjectif"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["modifieur"],relation->bounds()["adjectif"],attribs);
  }
  else if (relation->type() == "MOD-P")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["modifieur"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["preposition"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["modifieur"],relation->bounds()["preposition"],attribs);
  }
  else if (relation->type() == "JUXT")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["sujet"]])!=0)
    {
      attribs["premier"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["suivant"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["premier"],relation->bounds()["suivant"],attribs);
  }
  else if (relation->type() == "APPOS")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["premier"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["appose"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["premier"],relation->bounds()["appose"],attribs);
  }
  else if (relation->type() == "COORD")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["coordonnant"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["coord-g"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["coordonnant"],relation->bounds()["coord-g"],attribs);
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["coordonnant"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["coord-d"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["coordonnant"],relation->bounds()["coord-d"],attribs);
  }
  else if (relation->type() == "MOD-R")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["modifieur"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["adverbe"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["modifieur"],relation->bounds()["adverbe"],attribs);
  }
  else if (relation->type() == "MOD-N")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["modifieur"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["nom"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["modifieur"],relation->bounds()["nom"],attribs);
  }
  else if (relation->type() == "ATB-SO")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["attribut"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["verbe"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["attribut"],relation->bounds()["verbe"],attribs);
  }
  else if (relation->type() == "CPL-V")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["complement"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["verbe"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["complement"],relation->bounds()["verbe"],attribs);
  }
  else if (relation->type() == "AUX-V")
  {
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["auxiliaire"]])!=0)
    {
      attribs["arrowtail"] = "odot";
    }
    if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[relation->bounds()["verbe"]])!=0)
    {
      attribs["arrowhead"] = "normalodot";
    }
    addNewEdge(relation->bounds()["auxiliaire"],relation->bounds()["verbe"],attribs);
  }
}

void PartMatch::slotNewEdgeFinished(
      const QString& srcId,
      const QString& tgtId,
      const QMap<QString, QString>& attributes)
{
  kDebug() << srcId << tgtId << attributes;

  QMap<QString, QString> attribs = attributes;
  QString src = srcId;
  src.remove("cluster_");
  kDebug() << m_utterance->idsToConstituentsMap()[src] ;
  if (dynamic_cast<EasyGroup*>( m_utterance->idsToConstituentsMap()[src])!=0)
  {
    kDebug() << "arrowtail odot";
    attribs["arrowtail"] = "odot";
  }
  QString tgt = tgtId;
  tgt = tgt.remove("cluster_");
  kDebug() << m_utterance->idsToConstituentsMap()[tgt] ;
  if (dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[tgt])!=0)
  {
    kDebug() << "arrowhead normalodot";
    attribs["arrowhead"] = "normalodot";
  }
  addNewEdge(srcId, tgtId, attribs);
  update();
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
  addRelation(relation);
//   m_utterance->addRelation(relation);
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

void PartMatch::slotRemoveElement(const QString& id)
{
  kDebug() << id;
  if (id.startsWith("cluster_"))
  {
    QString groupId = id;
    groupId.remove("cluster_");

    for (int i = 0; i < m_utterance->relations().size(); i++)
    {
      EasyRelation* rel = m_utterance->relations().at(i);
      foreach (const QString& bound, rel->bounds().values())
      {
        if (bound == groupId)
        {
          kDebug() << "remove " << rel->id();
          m_utterance->removeRelationAt(i);
          removeEdge(rel->id());
          delete rel; rel=0;
          break;
        }
      }
    }

    EasyGroup* group = dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[groupId]);

    foreach (EasyForm* form, group->forms())
    {
      kDebug() << "add " << form->id();
      m_utterance->addConstituent(form);
    }
    kDebug() << "remove " << group->id();
    removeSubgraph(QString("cluster_")+group->id());
    m_utterance->removeConstituent(group);
    delete group;
    update();
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

void PartMatch::slotSelectionIs(const QList<QString> selection, const QPoint& eventPos)
{
  kDebug() << selection;
  m_selection = selection;
//   GA, GN, GP, GR, NV, PV

  if (m_addingGroup)
  {
    KAction* actionGA = new KAction( "GA", this );
    KAction* actionGN = new KAction( "GN", this );
    KAction* actionGP = new KAction( "GP", this );
    KAction* actionGR = new KAction( "GR", this );
    KAction* actionNV = new KAction( "NV", this );
    KAction* actionPV = new KAction( "PV", this );

    QMenu* ctxmenu = new QMenu (  );
    ctxmenu->addAction(actionGA);
    connect(actionGA, SIGNAL(triggered()), this, SLOT(slotAddGA()));
    ctxmenu->addAction(actionGN);
    connect(actionGN, SIGNAL(triggered()), this, SLOT(slotAddGN()));
    ctxmenu->addAction(actionGP);
    connect(actionGP, SIGNAL(triggered()), this, SLOT(slotAddGP()));
    ctxmenu->addAction(actionGR);
    connect(actionGR, SIGNAL(triggered()), this, SLOT(slotAddGR()));
    ctxmenu->addAction(actionNV);
    connect(actionNV, SIGNAL(triggered()), this, SLOT(slotAddNV()));
    ctxmenu->addAction(actionPV);
    connect(actionPV, SIGNAL(triggered()), this, SLOT(slotAddPV()));
    ctxmenu->exec(eventPos);
  }
}

void PartMatch::slotAddGA()
{
  kDebug();
  addGroup(EasyGroup::GA);
}

void PartMatch::slotAddGN()
{
  kDebug();
  addGroup(EasyGroup::GN);
}

void PartMatch::slotAddGP()
{
  kDebug();
  addGroup(EasyGroup::GP);
}

void PartMatch::slotAddGR()
{
  kDebug();
  addGroup(EasyGroup::GR);
}

void PartMatch::slotAddNV()
{
  kDebug();
  addGroup(EasyGroup::NV);
}

void PartMatch::slotAddPV()
{
  kDebug();
  addGroup(EasyGroup::PV);
}

void PartMatch::addGroup(EasyGroup::EasyGroupType type)
{
  kDebug() << type;
  m_addingGroup = false;
  EasyGroup* newGroup = new EasyGroup(m_utterance);
  newGroup->setId(QString("G")+QUuid::createUuid().toString().remove("{").remove("}").remove("-"));
  newGroup->setType(type);
  QList<EasyConstituent*> toRemove;
  foreach (EasyConstituent* constituent, m_utterance->constituents())
  {
    if (dynamic_cast<EasyForm*>(constituent) != 0)
    {
      kDebug() << m_selection << "|" << constituent->id();
      if (m_selection.contains(constituent->id()))
      {
        toRemove.push_back(constituent);
        newGroup->push_back(dynamic_cast<EasyForm*>(constituent));
      }
    }
    else if (dynamic_cast<EasyGroup*>(constituent) != 0)
    {
      QList<EasyForm*> toRemove2;
      foreach (EasyForm* form,  dynamic_cast<EasyGroup*>(constituent)->forms())
      {
        kDebug() << m_selection << "|" << constituent->id() << "|" << form->id();
        if (m_selection.contains(form->id()))
        {
          kDebug() << "set " << form->id() << " to remove";
          toRemove2.push_back(form);
          newGroup->push_back(dynamic_cast<EasyForm*>(form));
        }
      }
      foreach (EasyForm* form, toRemove2)
      {
        dynamic_cast<EasyGroup*>(constituent)->removeForm(form);
        removeNodeFromSubgraph(form->id(), QString("cluster_") + constituent->id());
        if (dynamic_cast<EasyGroup*>(constituent)->isEmpty())
        {
          toRemove.push_back(constituent);
        }
      }
    }
  }
  foreach (EasyConstituent* constituent, toRemove)
  {
    kDebug() << "Removing constituent " << constituent->id();
    m_utterance->removeConstituent(constituent);
  }
  kDebug() << "Adding group " << newGroup->id();
  m_utterance->addConstituent(newGroup);

  QMap<QString,QString> attribs;
  attribs["id"] = QString("cluster_") + newGroup->id();
  QString color = "black";
  switch (newGroup->type())
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
  foreach(EasyForm* form, newGroup->forms())
  {
    kDebug() << form->id();
    QMap<QString,QString> attribs;
    attribs["id"] = form->id();
    attribs["label"] = form->form();
    attribs["shape"] = "square";
    attribs["style"] = "filled";
    attribs["color"] = "grey";
    attribs["fillcolor"] = "grey";
    attribs["fontsize"] = "14";
    addExistingNodeToSubgraph(attribs, QString("cluster_") + newGroup->id());
  }

  update();
}

void PartMatch::slotContextMenuEvent(const QString& id, const QPoint& p)
{
  kDebug() << id << p;
  QMenu* ctxmenu = new QMenu (  );

  if (m_selection.size() > 1)
  {
    KAction* removeElementAction = new KAction(i18n("Remove selected elements"), this);
    ctxmenu->addAction(removeElementAction);
    connect(removeElementAction, SIGNAL(triggered()), this, SLOT(slotRemoveSelectedElements()));
  }
  else
  {
    QString correctedId = id;
    correctedId.remove("cluster_");
    m_selection.clear();
    m_selection.push_back(id);
    if (m_utterance->idsToConstituentsMap().keys().contains(correctedId))
    {
      EasyConstituent* constituent = m_utterance->idsToConstituentsMap()[correctedId];
      kDebug() << "constituent:" << constituent;
      if (dynamic_cast<EasyForm*>(constituent) == 0)
      {
        KAction* removeElementAction = new KAction(i18n("Remove selected element"), this);
        ctxmenu->addAction(removeElementAction);
        connect(removeElementAction, SIGNAL(triggered()), this, SLOT(slotRemoveSelectedElements()));
      }
      if (dynamic_cast<EasyGroup*>(constituent) != 0)
      {
        KAction* actionGA = new KAction( "GA", this );
        KAction* actionGN = new KAction( "GN", this );
        KAction* actionGP = new KAction( "GP", this );
        KAction* actionGR = new KAction( "GR", this );
        KAction* actionNV = new KAction( "NV", this );
        KAction* actionPV = new KAction( "PV", this );

        ctxmenu->addAction(actionGA);
        connect(actionGA, SIGNAL(triggered()), this, SLOT(slotSetGroupTypeGA()));
        ctxmenu->addAction(actionGN);
        connect(actionGN, SIGNAL(triggered()), this, SLOT(slotSetGroupTypeGN()));
        ctxmenu->addAction(actionGP);
        connect(actionGP, SIGNAL(triggered()), this, SLOT(slotSetGroupTypeGP()));
        ctxmenu->addAction(actionGR);
        connect(actionGR, SIGNAL(triggered()), this, SLOT(slotSetGroupTypeGR()));
        ctxmenu->addAction(actionNV);
        connect(actionNV, SIGNAL(triggered()), this, SLOT(slotSetGroupTypeNV()));
        ctxmenu->addAction(actionPV);
        connect(actionPV, SIGNAL(triggered()), this, SLOT(slotSetGroupTypePV()));
      }
    }
    else
    {
      KAction* removeElementAction = new KAction(i18n("Remove selected element"), this);
      ctxmenu->addAction(removeElementAction);
      connect(removeElementAction, SIGNAL(triggered()), this, SLOT(slotRemoveSelectedElements()));
      foreach(EasyRelation* rel, m_utterance->relations())
      {
        kDebug() << rel->id() << correctedId;
        if (rel->id() == correctedId)
        {
          KAction* actionSujV = new KAction( "SUJ-V", this );
          KAction* actionAuxV = new KAction( "AUX-V", this );
          KAction* actionModV = new KAction( "MOD-V", this );
          KAction* actionModN = new KAction( "MOD-N", this );
          KAction* actionModR = new KAction( "MOD-R", this );
          KAction* actionJuxt = new KAction( "JUXT", this );
          KAction* actionCoord = new KAction( "COORD", this );
          KAction* actionCodV = new KAction( "COD-V", this );
          KAction* actionCplV = new KAction( "CPL-V", this );
          KAction* actionAtbSo = new KAction( "ATB-SO", this );
          KAction* actionModA = new KAction( "MOD-A", this );
          KAction* actionModP = new KAction( "MOD-P", this );
          KAction* actionAppos = new KAction( "APPOS", this );

          ctxmenu->addAction(actionSujV);
          connect(actionSujV, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeSujV()));
          ctxmenu->addAction(actionAuxV);
          connect(actionAuxV, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeAuxV()));
          ctxmenu->addAction(actionModV);
          connect(actionModV, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeModV()));
          ctxmenu->addAction(actionModN);
          connect(actionModN, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeModN()));
          ctxmenu->addAction(actionModR);
          connect(actionModR, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeModR()));
          ctxmenu->addAction(actionJuxt);
          connect(actionJuxt, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeJuxt()));
          ctxmenu->addAction(actionCoord);
          connect(actionCoord, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeCoord()));
          ctxmenu->addAction(actionCodV);
          connect(actionCodV, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeCodV()));
          ctxmenu->addAction(actionCplV);
          connect(actionCplV, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeCplV()));
          ctxmenu->addAction(actionAtbSo);
          connect(actionAtbSo, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeAtbSo()));
          ctxmenu->addAction(actionModA);
          connect(actionModA, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeModA()));
          ctxmenu->addAction(actionModP);
          connect(actionModP, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeModP()));
          ctxmenu->addAction(actionAppos);
          connect(actionAppos, SIGNAL(triggered()), this, SLOT(slotSetRelationTypeAppos()));
          break;
        }
      }
    }
  }
  ctxmenu->exec(p);
}

void PartMatch::slotRemoveSelectedElements()
{
  kDebug();
  foreach(const QString& id, m_selection)
  {
    removeElement(id);
  }
}

void PartMatch::slotSetGroupTypeGA()
{
  kDebug() << m_selection;
  setGroupType(m_selection[0], EasyGroup::GA);
}

void PartMatch::slotSetGroupTypeGN()
{
  kDebug();
  setGroupType(m_selection[0], EasyGroup::GN);
}

void PartMatch::slotSetGroupTypeGP()
{
  kDebug();
  setGroupType(m_selection[0], EasyGroup::GP);
}

void PartMatch::slotSetGroupTypeGR()
{
  kDebug();
  setGroupType(m_selection[0], EasyGroup::GR);
}

void PartMatch::slotSetGroupTypeNV()
{
  kDebug();
  setGroupType(m_selection[0], EasyGroup::NV);
}

void PartMatch::slotSetGroupTypePV()
{
  kDebug();
  setGroupType(m_selection[0], EasyGroup::PV);
}

void PartMatch::setGroupType(const QString& id, EasyGroup::EasyGroupType type)
{
  kDebug() << id << type << m_utterance->idsToConstituentsMap().keys();
  QString correctedId = id; correctedId.remove("cluster_");
  EasyGroup* group = dynamic_cast<EasyGroup*>(m_utterance->idsToConstituentsMap()[correctedId]);
  group->setType(type);
  QString color;
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
  setAttribute(id,"color",color);
  update();
}

void PartMatch::slotSetRelationTypeSujV()
{
  kDebug();
  setRelationType(m_selection[0], "SUJ-V");
}

void PartMatch::slotSetRelationTypeAuxV()
{
  kDebug();
  setRelationType(m_selection[0], "AUX-V");
}

void PartMatch::slotSetRelationTypeModV()
{
  kDebug();
  setRelationType(m_selection[0], "MOD-V");
}

void PartMatch::slotSetRelationTypeModN()
{
  kDebug();
  setRelationType(m_selection[0], "MOD-N");
}

void PartMatch::slotSetRelationTypeModR()
{
  kDebug();
  setRelationType(m_selection[0], "MOD-R");
}

void PartMatch::slotSetRelationTypeJuxt()
{
  kDebug();
  setRelationType(m_selection[0], "JUXT");
}

void PartMatch::slotSetRelationTypeCoord()
{
  kDebug();
  setRelationType(m_selection[0], "COORD");
}

void PartMatch::slotSetRelationTypeCodV()
{
  kDebug();
  setRelationType(m_selection[0], "COD-V");
}

void PartMatch::slotSetRelationTypeCplV()
{
  kDebug();
  setRelationType(m_selection[0], "CPL-V");
}

void PartMatch::slotSetRelationTypeAtbSo()
{
  kDebug();
  setRelationType(m_selection[0], "ATB-SO");
}

void PartMatch::slotSetRelationTypeModA()
{
  kDebug();
  setRelationType(m_selection[0], "MOD-A");
}

void PartMatch::slotSetRelationTypeModP()
{
  kDebug();
  setRelationType(m_selection[0], "MOD-P");
}

void PartMatch::slotSetRelationTypeAppos()
{
  kDebug();
  setRelationType(m_selection[0], "APPOS");
}

void PartMatch::setRelationType(const QString& id, const QString& type)
{
  kDebug() << id << type;
  foreach (EasyRelation* relation, m_utterance->relations())
  {
    if (relation->id() == id)
    {
      relation->setType(type);
      QString label = type;
      if (!relation->value().isEmpty())
      {
        label += QString(' ') += relation->value();
      }
      setAttribute(id,"label",label);
      update();
    }
  }
}


#include "PartMatch.moc"
