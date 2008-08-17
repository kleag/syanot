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


#ifndef PARTMATCH_H
#define PARTMATCH_H

#include "EasyGroup.h"

#include <QObject>
#include <kparts/mainwindow.h>

class kgraphviewerPart;

class EasyUtterance;
class EasyRelation;

/**
 * @author Gael de Chalendar <kleag@free.fr>
 */
class PartMatch : public QObject
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  PartMatch(KParts::Part* p, EasyUtterance* u, QObject* parent = 0);

  /**
    * Default Destructor
    */
  virtual ~PartMatch();

  inline const QString& utteranceId() const {return m_utteranceId;}
  inline void setUtteranceId(const QString& id) {m_utteranceId = id;}
  
  inline const EasyUtterance* utterance() const {return m_utterance;}
  inline void setUtterance(EasyUtterance* u) {m_utterance = u;}
  
  inline const KParts::Part* part() const {return m_part;}
  void setPart(KParts::Part* p);
  
  inline void setReadWrite() {emit ssetReadWrite();}
  inline void saveTo(const QString& fileName) {emit ssaveTo(fileName);}
  void prepareAddNewEdge(QMap<QString,QString> attribs);
  void prepareAddNewCoord(QMap<QString,QString> attribs);

  inline void prepareSelectElements() { emit sprepareSelectElements(); }
  inline void setGraphAttributes(QMap<QString,QString> attribs) {emit ssetGraphAttributes(attribs);}
  inline void addNewNode(QMap<QString,QString> attribs) {emit saddNewNode(attribs);}
  inline void addNewNodeToSubgraph(QMap<QString,QString> attribs, QString subgraph) {emit saddNewNodeToSubgraph(attribs,subgraph);}
  inline void addExistingNodeToSubgraph(QMap<QString,QString> attribs, QString subgraph) {emit saddExistingNodeToSubgraph(attribs,subgraph);}
  inline void moveExistingNodeToMainGraph(QMap<QString,QString> attribs) {emit smoveExistingNodeToMainGraph(attribs);}
  inline void addNewSubgraph(QMap<QString,QString> attribs) {emit saddNewSubgraph(attribs);}
  inline void addNewEdge(QString src, QString tgt, QMap<QString,QString> attribs) {emit saddNewEdge(src,tgt,attribs);}
  inline void update() {emit supdate();}
  inline void removeNode(const QString& n) {emit sremoveNode(n);}
  inline void removeElement(const QString& e) {emit sremoveElement(e);}
  inline void removeNodeFromSubgraph(const QString& n, const QString& s) {emit sremoveNodeFromSubgraph(n,s);}
  inline void removeSubgraph(const QString& s) {emit sremoveSubgraph(s);}
  inline void addAttribute(const QString& s) {emit saddAttribute(s);}
  inline void removeAttribute(const QString& s1,const QString&s2) {emit sremoveAttribute(s1,s2);}
  /** Asks the part to hide and remove the corresponding edge
   * @param id the edge id
   */
  inline void removeEdge(const QString& s) {emit sremoveEdge(s);}
  inline void setAttribute(const QString& e,const QString& n,const QString& v) {emit ssetAttribute(e,n,v);}
  inline void setCursor(const QCursor& c) {emit ssetCursor(c);}
  inline void unsetCursor() {emit sunsetCursor();}

  inline bool addingGroup() const {return m_addingGroup;}
  inline void setAddingGroup(bool v) {m_addingGroup = v;}

  QString newEdgeAdded(
    const QString& src,
    const QString& tgt);


Q_SIGNALS:
  void ssetReadWrite();
  void ssaveTo(const QString& fileName);
  void sprepareAddNewEdge(QMap<QString,QString> attribs);
  void sprepareSelectElements();
  void ssetGraphAttributes(QMap<QString,QString> attribs);
  void saddNewNode(QMap<QString,QString> attribs);
  void saddNewNodeToSubgraph(QMap<QString,QString> attribs, QString subgraph);
  void saddExistingNodeToSubgraph(QMap<QString,QString> attribs, QString subgraph);
  void smoveExistingNodeToMainGraph(QMap<QString,QString> attribs);
  void saddNewSubgraph(QMap<QString,QString> attribs);
  void saddNewEdge(QString src, QString tgt, QMap<QString,QString> attribs);
  void supdate();
  void sremoveNode(const QString&);
  void sremoveElement(const QString&);
  void sremoveNodeFromSubgraph(const QString&, const QString&);
  void sremoveSubgraph(const QString&);
  void saddAttribute(const QString&);
  void sremoveAttribute(const QString&,const QString&);
  /** Asks the part to hide and remove the corresponding edge
   * @param id the edge id
   */
  void sremoveEdge(const QString&);
  void ssetAttribute(const QString&,const QString&,const QString&);
  void ssetHighlighting(bool);
  void ssetCursor(const QCursor&);
  void sunsetCursor();
  void showSOChooser();
  void hideSOChooser();
  void showAPropagerChooser();
  void hideAPropagerChooser();
  void setSO(const QString&);
  void setAPropager(int);

public Q_SLOTS:
//   void slotAddAttribute(const QString&);
//   void slotRemoveAttribute(const QString&,const QString&);

//   void slotAddNewElementAttribute(const QString&);
//   void slotRemoveNewElementAttribute(const QString&);

  /**
   * Received when the part signals a edge removing action. Will remove the
   * relation in the model and ask back to the part to remove the corresponding
   * edge
   */
  void slotRemoveEdge(const QString& id);

  /**
   * Received when the part signals an element (node or subgraph) removing action. Will remove the token or group from the model and ask back to the part to remove the corresponding element
   */
  void slotRemoveElement(const QString& id);

  void slotSelectionIs(const QList<QString> selection, const QPoint& eventPos);

  void slotNewEdgeFinished(
      const QString&,
      const QString&,
      const QMap<QString, QString>&);

  void slotAddGA();
  void slotAddGN();
  void slotAddGP();
  void slotAddGR();
  void slotAddNV();
  void slotAddPV();

  void slotContextMenuEvent(const QString&, const QPoint&);

  void slotRemoveSelectedElements();

  void slotSetGroupTypeGA();
  void slotSetGroupTypeGN();
  void slotSetGroupTypeGP();
  void slotSetGroupTypeGR();
  void slotSetGroupTypeNV();
  void slotSetGroupTypePV();

  void slotSetRelationTypeSujV();
  void slotSetRelationTypeAuxV();
  void slotSetRelationTypeModV();
  void slotSetRelationTypeModN();
  void slotSetRelationTypeModR();
  void slotSetRelationTypeJuxt();
  void slotSetRelationTypeCoord();
  void slotSetRelationTypeCodV();
  void slotSetRelationTypeCplV();
  void slotSetRelationTypeAtbSo();
  void slotSetRelationTypeModA();
  void slotSetRelationTypeModP();
  void slotSetRelationTypeAppos();

  void slotSoSujet();
  void slotSoObjet();
  void slotSoInd();
  void slotAPropager(int value);
private:
  void connectSignals();
  void addRelation(const EasyRelation* relation);
  void addGroup(EasyGroup::EasyGroupType type);
  void setGroupType(const QString& id, EasyGroup::EasyGroupType type);
  void setRelationType(const QString& id, const QString& type);
  void addNewCoord(const QString& id, const QString& coordonant, const QString& coordg, const QString& coordd);

  QString m_utteranceId;
  
  KParts::Part* m_part;
  
  EasyUtterance* m_utterance;

  QString m_currentRelation;

  QList<QString> m_selection;

  bool m_addingGroup;

  /// contains links from coord elements ids in the kgraphviewer
  /// graph to the coord relation id
  QMap<QString, QString> m_coordMap;

  /// true when the COORD button has been pressed and when the user is
  /// selecting the bounds nodes (coordonant, coord-g and coord-d)
  /// should be set to false if the relation creation is cancelled
  bool m_addingCoord;

  QString m_coordCoordonantFormId;
  QString m_coordCoordgFormId;
  QString m_coordCoorddFormId;
};

#endif // PARTMATCH_H
