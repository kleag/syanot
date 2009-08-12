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


#ifndef _SYANOT_H_
#define _SYANOT_H_

#include "UtterancesPartsMapping.h"

#include <kapplication.h>
#include <kparts/mainwindow.h>
#include <ktabwidget.h>
#include <kaction.h>
#include <krecentfilesaction.h>

class QListWidgetItem;
class QDockWidget;
class QCheckBox;

class KToggleAction;
class KListWidget;
class KPushButton;
class KPluginFactory;

class kgraphviewerPart;

class EasyDocument;
class EasyUtterance;
class PartMatch;

#include "ui_syanotmainwindow.h"

/**
 * This is the application "Shell".  It has a menubar, toolbar, and
 * statusbar but relies on the "Part" to do all the real work.
 *
 * @short Application Shell
 * @author Gael de Chalendar <kleag@free.fr>
 */
class Syanot : public KParts::MainWindow, public Ui::SyanotMainWindow
{
  Q_OBJECT
public:
  /**
    * Default Constructor
    */
  Syanot(QWidget *parent=0, Qt::WindowFlags f=KDE_DEFAULT_WINDOWFLAGS);

  /**
    * Default Destructor
    */
  virtual ~Syanot();

  /**
    * Use this method to load whatever file/URL you have
    */
  void openUrl(const KUrl& url);

protected:

  bool queryClose(); 

signals:
  void hide(KParts::Part* part);
  void soSujet();
  void soObjet();
  void soInd();
  void aPropager(int);

public slots:
  /**
    * Use this method to load whatever file/URL you have
    */
  void openUrl(const QString& url) {openUrl(KUrl(url));}

  /**
   * Received when the part signals a close action.   */
  bool slotClose();

  void slotQuit();


  void slotSetActiveGraph( KParts::Part* part);

  void slotAddAttribute(const QString&);
  void slotRemoveAttribute(const QString&,const QString&);

  void slotAddNewElementAttribute(const QString&);
  void slotRemoveNewElementAttribute(const QString&);

  void slotSujVButtonClicked();
  void slotCodVButtonClicked();
  void slotModVButtonClicked();
  void slotModAButtonClicked();
  void slotModPButtonClicked();
  void slotJuxtButtonClicked();
  void slotApposButtonClicked();
  void slotCoordButtonClicked();
  void slotModRButtonClicked();
  void slotModNButtonClicked();
  void slotAtbSOButtonClicked();
  void slotCplVButtonClicked();
  void slotAuxVButtonClicked();
  void slotGroupButtonClicked();
  
  void slotDocumentModified();
/*public slots:
  void reloadOnChangeMode_pressed(int value);
  void openInExistingWindowMode_pressed(int value);
  void reopenPreviouslyOpenedFilesMode_pressed(int value);*/

  void slotShowSOChooser();
  void slotHideSOChooser();
  void slotShowAPropagerChooser();
  void slotHideAPropagerChooser();

  void slotSetSO(const QString& value);
  void slotSetAPropager(int value);
  
private slots:
  void fileNew();
  void fileOpen();
  void fileSave();
  void fileSaveAs();
  void slotEditClearAllRels();
  void close();
  void slotURLSelected(const KUrl&);
  void optionsShowToolbar();
  void optionsShowStatusbar();
  void optionsConfigureKeys();
  void optionsConfigureToolbars();
  void optionsConfigure();
    
  void applyNewToolbarConfig();
  void slotUtteranceClicked(QListWidgetItem*);
  
private:
  void setupAccel();
  void setupActions();
  void activateUtterance(const QString& id);
    
private:
  void createPartFor(const QString& id);
  void createSOChooser();
  void createAPropagerChooser();

  QWidget* m_widget;
  KRecentFilesAction* m_rfa;
  KParts::PartManager* m_manager;
  
  KToggleAction *m_toolbarAction;
  KToggleAction *m_statusbarAction;

  UtterancesPartsMapping m_primaryMapping;
  UtterancesPartsMapping m_secondaryMapping;
  
  KUrl m_openedFile;
  
  QMap<QString, QString> m_newElementAttributes;

  EasyDocument* m_document;
  bool m_documentModified;

  QString m_currentRelation;

  KListWidget* m_utterancesWidget;

  KPluginFactory* m_factory;

  QDockWidget* m_aPropageDockWidget;
  QDockWidget* m_soChooserDockWidget;

  KPushButton* m_soButton;
  QCheckBox* m_aPropagerCheckbox;
};

#endif // _SYANOT_H_
