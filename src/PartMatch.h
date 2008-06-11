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
  inline void setGraphAttributes(QMap<QString,QString> attribs) {emit ssetGraphAttributes(attribs);}
  inline void addNewNode(QMap<QString,QString> attribs) {emit saddNewNode(attribs);}
  inline void addNewNodeToSubgraph(QMap<QString,QString> attribs, QString subgraph) {emit saddNewNodeToSubgraph(attribs,subgraph);}
  inline void addNewSubgraph(QMap<QString,QString> attribs) {emit saddNewSubgraph(attribs);}
  inline void addNewEdge(QString src, QString tgt, QMap<QString,QString> attribs) {emit saddNewEdge(src,tgt,attribs);}
  inline void update() {emit supdate();}
  inline void removeNode(const QString& s) {emit sremoveNode(s);}
  inline void removeSubgraph(const QString& s) {emit sremoveSubgraph(s);}
  inline void addAttribute(const QString& s) {emit saddAttribute(s);}
  inline void removeAttribute(const QString& s1,const QString&s2) {emit sremoveAttribute(s1,s2);}
  /** Asks the part to hide and remove the corresponding edge
   * @param id the edge id
   */
  inline void removeEdge(const QString& s) {emit sremoveEdge(s);}
  inline void setAttribute(const QString& e,const QString& n,const QString& v) {emit ssetAttribute(e,n,v);}
  
Q_SIGNALS:
  void ssetReadWrite();
  void ssaveTo(const QString& fileName);
  void sprepareAddNewEdge(QMap<QString,QString> attribs);
  void ssetGraphAttributes(QMap<QString,QString> attribs);
  void saddNewNode(QMap<QString,QString> attribs);
  void saddNewNodeToSubgraph(QMap<QString,QString> attribs, QString subgraph);
  void saddNewSubgraph(QMap<QString,QString> attribs);
  void saddNewEdge(QString src, QString tgt, QMap<QString,QString> attribs);
  void supdate();
  void sremoveNode(const QString&);
  void sremoveSubgraph(const QString&);
  void saddAttribute(const QString&);
  void sremoveAttribute(const QString&,const QString&);
  /** Asks the part to hide and remove the corresponding edge
   * @param id the edge id
   */
  void sremoveEdge(const QString&);
  void ssetAttribute(const QString&,const QString&,const QString&);
  void ssetHighlighting(bool);

public Q_SLOTS:
//   void slotAddAttribute(const QString&);
//   void slotRemoveAttribute(const QString&,const QString&);

//   void slotAddNewElementAttribute(const QString&);
//   void slotRemoveNewElementAttribute(const QString&);

  void slotNewEdgeAdded(QString,QString);
  
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

  void slotSelectionIs(const QList<QString>&);

  void slotNewEdgeFinished(
      const QString&,
      const QString&,
      const QMap<QString, QString>&);

private:
  void connectSignals();
  void addRelation(const EasyRelation* relation);

  QString m_utteranceId;
  
  KParts::Part* m_part;
  
  EasyUtterance* m_utterance;

  QString m_currentRelation;

  QList<QString> m_selection;
};

#endif // PARTMATCH_H
