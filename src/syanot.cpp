/* This file is part of Syanot.
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


#include "syanot.h"
#include "syanotConfigDialog.h"
#include "syanotsettings.h"
// #include "syanotmainwindow.h"
#include "EasyDocument.h"
#include "EasyConstituent.h"
#include "EasyRelation.h"
#include "EasyForm.h"
#include "EasyGroup.h"
#include "EasyDocument.h"
#include "EasyUtterance.h"
#include "EasyXmlReader.h"
#include "PartMatch.h"

#include <KTemporaryFile>
#include <kshortcutsdialog.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <ktabwidget.h>
#include <kparts/partmanager.h>
#include <kedittoolbar.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kstandardaction.h>
#include <ktoggleaction.h>
#include <kmessagebox.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <kconfigdialog.h>
#include <kiconloader.h>
#include <krecentfilesaction.h>
#include <ktoolbar.h>
#include <KActionCollection>
#include <kio/netaccess.h>
#include <KListWidget>
#include <KMenu>
#include <kpluginfactory.h>
#include <kpluginloader.h>

#include <QtDBus/QtDBus>
#include <QDockWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QFrame>
#include <QTextStream>
#include <QCheckBox>

#include <iostream>

// using namespace Syanot;

Syanot::Syanot(QWidget *parent, Qt::WindowFlags flags) :
    KParts::MainWindow(parent,flags),
    m_primaryGraph(0),
    m_secondaryGraph(0),
    m_rfa(0),
    m_primaryMapping(this),
    m_secondaryMapping(this),
    m_document(0),
    m_secondaryDocument(0),
    m_documentModified(false)
{
  kDebug();
  m_factory = KPluginLoader("kgraphviewerpart").factory();
  if (m_factory == 0)
  {
    // if we couldn't find our Part, we exit since the Shell by
    // itself can't do anything useful
    KMessageBox::error(this, i18n("Could not find our part."));
    kapp->quit();
    // we return here, cause kapp->quit() only means "exit the
    // next time we enter the event loop...
    return;
  }
  QFrame* f = new QFrame();
  setupUi(f);
  // set the shell's ui resource file
  setXMLFile("syanotui.rc");

  connect(this->sujvbutton,SIGNAL(clicked()),this,SLOT(slotSujVButtonClicked()));
  connect(this->codvbutton,SIGNAL(clicked()),this,SLOT(slotCodVButtonClicked()));
  connect(this->modvbutton,SIGNAL(clicked()),this,SLOT(slotModVButtonClicked()));
  connect(this->modabutton,SIGNAL(clicked()),this,SLOT(slotModAButtonClicked()));
  connect(this->modpbutton,SIGNAL(clicked()),this,SLOT(slotModPButtonClicked()));
  connect(this->juxtbutton,SIGNAL(clicked()),this,SLOT(slotJuxtButtonClicked()));
  connect(this->apposbutton,SIGNAL(clicked()),this,SLOT(slotApposButtonClicked()));
  connect(this->coordbutton,SIGNAL(clicked()),this,SLOT(slotCoordButtonClicked()));
  connect(this->modrbutton,SIGNAL(clicked()),this,SLOT(slotModRButtonClicked()));
  connect(this->modnbutton,SIGNAL(clicked()),this,SLOT(slotModNButtonClicked()));
  connect(this->atbsobutton,SIGNAL(clicked()),this,SLOT(slotAtbSOButtonClicked()));
  connect(this->cplvbutton,SIGNAL(clicked()),this,SLOT(slotCplVButtonClicked()));
  connect(this->auxvbutton,SIGNAL(clicked()),this,SLOT(slotAuxVButtonClicked()));
  connect(this->groupbutton,SIGNAL(clicked()),this,SLOT(slotGroupButtonClicked()));

//     QDockWidget* topLeftDockWidget = new QDockWidget(this);
//   connect(m_treeWidget, SIGNAL(removeNode(const QString&)),
//           this, SLOT(slotRemoveNode(const QString&)));
//   connect(m_treeWidget, SIGNAL(addAttribute(const QString&)),
//           this, SLOT(slotAddAttribute(const QString&)));
//   connect(m_treeWidget, SIGNAL(removeAttribute(const QString&,const QString&)),
//           this, SLOT(slotRemoveAttribute(const QString&,const QString&)));

//   m_treeWidget->setItemDelegate(new VariantDelegate(m_treeWidget));
//   topLeftDockWidget->setWidget(m_treeWidget);
//   addDockWidget ( Qt::LeftDockWidgetArea, topLeftDockWidget );

  if (QDBusConnection::sessionBus().registerService( "org.kde.syanot" ))
  {
    kDebug() << "Service Registered successfuly";
    QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
  }
  else
  {
    kDebug() << "Failed to register service...";
  }

  // then, setup our actions
  setupActions();

  // and a status bar
  statusBar()->show();
  createStandardStatusBarAction();

  // this routine will find and load our Part.  it finds the Part by
  // name which is a bad idea usually.. but it's alright in this
  // case since our Part is made for this Shell

 // Create a KParts part manager, to handle part activation/deactivation
  m_manager = new KParts::PartManager( this );
  
  // When the manager says the active part changes, the window updates (recreates) the GUI
  connect( m_manager, SIGNAL( activePartChanged( KParts::Part * ) ),
           this, SLOT( createGUI( KParts::Part * ) ) );
    
  // Creates the GUI with a null part to make appear the main app menus and tools
  createGUI(0);
  setupGUI();

  // apply the saved mainwindow settings, if any, and ask the mainwindow
  // to automatically save settings if changed: window size, toolbar
  // position, icon size, etc.
  setAutoSaveSettings();

  connect( m_manager, SIGNAL( activePartChanged( KParts::Part * ) ),
           this, SLOT( slotSetActiveGraph( KParts::Part * ) ) );


  setCentralWidget(f);

  QDockWidget* topLeftDockWidget = new QDockWidget(this);
  m_utterancesWidget = new KListWidget(topLeftDockWidget);

  topLeftDockWidget->setWidget(m_utterancesWidget);
  addDockWidget ( Qt::LeftDockWidgetArea, topLeftDockWidget );

  kDebug() << "connecting utterance activated";
  connect(m_utterancesWidget, SIGNAL(itemClicked(QListWidgetItem*)),
           this, SLOT(slotUtteranceClicked(QListWidgetItem*)));

  createSOChooser();
  createAPropagerChooser();

}

Syanot::~Syanot()
{
  kDebug() ;
}

void Syanot::openUrl(const KUrl& url)
{
  kDebug() << url;
  if (slotClose() == false)
  {
    return ;
  }
  m_document = new EasyDocument();
  QString tmpFileName;
  KIO::NetAccess::download(url,tmpFileName,this);
  QFile file(tmpFileName);
  if (!file.open(QFile::ReadOnly | QFile::Text))
  {
      KMessageBox::error(this,
                          i18n("Cannot read file %1:\n%2.",
                              tmpFileName,file.errorString()),
                          i18n("Syanot loading utterances"));
      return;
  }
  kDebug() << "tmp file opened:" << tmpFileName;

  EasyXmlReader reader(m_document);
  if (!reader.read(&file))
  {
      KMessageBox::error(this,
                          i18n("Parse error in file %1 at line %2, column %3:\n%4",
                              tmpFileName, reader.lineNumber(),
                              reader.columnNumber(), reader.errorString()),
                          i18n("Syanot loading utterances"));
  }
  else
  {
    m_utterancesWidget->clear();
    m_openedFile = url;
    
    statusBar()->showMessage(i18n("File loaded"), 2000);

    foreach (EasyUtterance* utterance, m_document->utterances())
    {
      m_utterancesWidget->addItem(utterance->id());
    }
    m_utterancesWidget->setCurrentRow(0);
    if (m_utterancesWidget->currentItem()!=0)
    {
      activateUtterance(m_utterancesWidget->currentItem()->text());
    }
  }
  m_documentModified = false;
  connect (m_document,SIGNAL(changed(EasyDocument*)),
            this,SLOT(slotDocumentModified()));

  this->setWindowTitle(i18n("Syanot - %1", url.prettyUrl()));
}

void Syanot::fileOpen()
{
  kDebug();
  // this slot is called whenever the File->Open menu is selected,
  // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
  // button is clicked
  KUrl url = KFileDialog::getOpenUrl(KUrl(), QString("*"), 0, QString::null);
  if (url.isEmpty()) return;

  if (m_rfa != 0)
  {
    m_rfa->addUrl(url);
  }
  openUrl(url);
}


void Syanot::setupActions()
{
  // create our actions

  actionCollection()->addAction( KStandardAction::New, "file_new", this, SLOT( fileNew() ) );
  actionCollection()->addAction( KStandardAction::Open, "file_open", this, SLOT( fileOpen() ) );
  m_rfa = (KRecentFilesAction*) actionCollection()->addAction(KStandardAction::OpenRecent, "file_open_recent", this, SLOT( slotURLSelected(const KUrl&) ) );
  m_rfa->loadEntries(KGlobal::config()->group("syanot"));
  actionCollection()->addAction( KStandardAction::Save, "file_save", this, SLOT( fileSave() ) );
  actionCollection()->addAction( KStandardAction::SaveAs, "file_save_as", this, SLOT( fileSaveAs() ) );

  actionCollection()->addAction( KStandardAction::Quit, "file_quit", this, SLOT( slotQuit() ) );

  m_statusbarAction = KStandardAction::showStatusbar(this, SLOT(optionsShowStatusbar()), this);

  actionCollection()->addAction( KStandardAction::KeyBindings, "options_configure_keybinding", this, SLOT( optionsConfigureKeys() ) );
//   KStandardAction::keyBindings(this, SLOT(optionsConfigureKeys()), this);
  actionCollection()->addAction( KStandardAction::ConfigureToolbars, "options_configure_toolbars", this, SLOT( optionsConfigureToolbars() ) );
  actionCollection()->addAction( KStandardAction::Preferences, "options_configure", this, SLOT( optionsConfigure() ) );

  QAction* edit_clear_all_rels = actionCollection()->addAction( "edit_clear_all_rels" );
  edit_clear_all_rels->setText(i18n("Clear Relations"));
//   edit_clear_all_rels->setIcon(QPixmap(KGlobal::dirs()->findResource("data","kgraphviewerpart/pics/kgraphviewer-newnode.png")));
  connect( edit_clear_all_rels, SIGNAL(triggered(bool)), this, SLOT( slotEditClearAllRels() ) );
  
  QAction* file_open_compare = actionCollection()->addAction( "file_open_compare" );
  file_open_compare->setText(i18n("Open for comparison..."));
  connect( file_open_compare, SIGNAL(triggered(bool)), this, SLOT(slotOpenForComparison(bool)));
  QAction* file_close_compare = actionCollection()->addAction( "file_close_compare" );
  file_close_compare->setText(i18n("Close current comparison"));
  connect( file_close_compare, SIGNAL(triggered(bool)), this, SLOT(slotCloseComparison(bool)));
}

void Syanot::slotEditClearAllRels()
{
  kDebug();
  
}

bool Syanot::queryClose()
{
  kDebug() << m_documentModified;
  if (m_documentModified &&
        (KMessageBox::questionYesNo(0,
            i18n("Current document has unsaved changes. Do you really want to close it ?"),
            i18n("Closing a modified document"),
            KGuiItem("Close anyway"),
            KGuiItem("Don't close"),
            "closeAnyway"   ) == KMessageBox::No))
  {
    return false;
  }

  m_rfa->saveEntries(KGlobal::config()->group("syanot"));

  return true;
}

void Syanot::fileNew()
{
  // this slot is called whenever the File->New menu is selected,
  // the New shortcut is pressed (usually CTRL+N) or the New toolbar
  // button is clicked

  (new Syanot)->show();
}


void Syanot::optionsShowToolbar()
{
  // this is all very cut and paste code for showing/hiding the
  // toolbar
  if (m_toolbarAction->isChecked())
      toolBar()->show();
  else
      toolBar()->hide();
}

void Syanot::optionsShowStatusbar()
{
  // this is all very cut and paste code for showing/hiding the
  // statusbar
  if (m_statusbarAction->isChecked())
      statusBar()->show();
  else
      statusBar()->hide();
}

void Syanot::optionsConfigureKeys()
{
  KShortcutsDialog::configure(actionCollection());
}

void Syanot::optionsConfigureToolbars()
{
    saveMainWindowSettings(KGlobal::config()->group("syanot") );

  // use the standard toolbar editor
  KEditToolBar dlg(factory());
  connect(&dlg, SIGNAL(newToolbarConfig()),
          this, SLOT(applyNewToolbarConfig()));
  dlg.exec();
}

void Syanot::optionsConfigure()
{
  //An instance of your dialog could be already created and could be cached, 
  //in which case you want to display the cached dialog instead of creating 
  //another one 
  if ( SyanotConfigurationDialog::showDialog( "settings" ) )
    return; 
 
  //KConfigDialog didn't find an instance of this dialog, so lets create it : 
  KPageDialog::FaceType ft = KPageDialog::Auto;
  SyanotConfigurationDialog* dialog = new SyanotConfigurationDialog( this, "settings",
                                             SyanotSettings::self(),ft );
  
  dialog->show();
}

void Syanot::applyNewToolbarConfig()
{
  applyMainWindowSettings(KGlobal::config()->group("syanot"));
}



void Syanot::slotURLSelected(const KUrl& url)
{
  kDebug();
  openUrl(url);
}

void Syanot::close()
{
  kDebug();
}

void Syanot::fileSave()
{
  kDebug();
  if (m_primaryGraph != 0)
  {
    KTemporaryFile* temp = new KTemporaryFile();
    temp->setAutoRemove(false);
    temp->open();
    QString tempFileName = temp->fileName();
    delete temp;
    QFile tmpFile(tempFileName);
//     if (!temp.open())
    if (!tmpFile.open(QIODevice::WriteOnly))
    {
      KMessageBox::error(this, i18n("Cannot open a temporary file."));
      return;
    }
    kDebug() << "using temp file" << tmpFile.fileName();
    QTextStream s(&tmpFile);
    s << *m_document;
    tmpFile.close();
    kDebug() << "saving to" << m_openedFile.url();
    if (!KIO::NetAccess::upload(tmpFile.fileName(), m_openedFile, 0))
    {
      KMessageBox::error(this, i18n("Failed to save to %1",m_openedFile.url()));
    }
    tmpFile.remove();
    m_documentModified = false;
  }
}

void Syanot::fileSaveAs()
{
  if (m_primaryGraph != 0)
  {
    m_openedFile = KFileDialog::getOpenUrl(KUrl(),
                "*.easy.xml", m_primaryGraph,
                i18n("Save current graph to..."));
    fileSave();
  }
}

void Syanot::slotSetActiveGraph( KParts::Part* part)
{
  kDebug();
  if (m_primaryMapping.currentPartMatch() != 0)
  {
    disconnect(m_primaryMapping.currentPartMatch(),SIGNAL(showSOChooser()),this,SLOT(slotShowSOChooser()));
    disconnect(m_primaryMapping.currentPartMatch(),SIGNAL(hideSOChooser()),this,SLOT(slotHideSOChooser()));
    disconnect(m_primaryMapping.currentPartMatch(),SIGNAL(showAPropagerChooser()),this,SLOT(slotShowAPropagerChooser()));
    disconnect(m_primaryMapping.currentPartMatch(),SIGNAL(hideAPropagerChooser()),this,SLOT(slotHideAPropagerChooser()));
    disconnect(this,SIGNAL(soSujet()),m_primaryMapping.currentPartMatch(),SLOT(slotSoSujet()));
    disconnect(this,SIGNAL(soObjet()),m_primaryMapping.currentPartMatch(),SLOT(slotSoObjet()));
    disconnect(this,SIGNAL(soInd()),m_primaryMapping.currentPartMatch(),SLOT(slotSoInd()));
    disconnect(this,SIGNAL(aPropager(int)),m_primaryMapping.currentPartMatch(),SLOT(slotAPropager(int)));
    disconnect(m_primaryMapping.currentPartMatch(),SIGNAL(setSO(const QString&)),this,SLOT(slotSetSO(const QString&)));
    disconnect(m_primaryMapping.currentPartMatch(),SIGNAL(setAPropager(int)),this,SLOT(slotSetAPropager(int)));
  }
  m_primaryMapping.setCurrentPart(part);
  if (m_primaryMapping.currentPart() == 0)
  {
    return;
  }
  connect(m_primaryMapping.currentPartMatch(),SIGNAL(showSOChooser()),this,SLOT(slotShowSOChooser()));
  connect(m_primaryMapping.currentPartMatch(),SIGNAL(hideSOChooser()),this,SLOT(slotHideSOChooser()));
  connect(m_primaryMapping.currentPartMatch(),SIGNAL(showAPropagerChooser()),this,SLOT(slotShowAPropagerChooser()));
  connect(m_primaryMapping.currentPartMatch(),SIGNAL(hideAPropagerChooser()),this,SLOT(slotHideAPropagerChooser()));
  connect(this,SIGNAL(soSujet()),m_primaryMapping.currentPartMatch(),SLOT(slotSoSujet()));
  connect(this,SIGNAL(soObjet()),m_primaryMapping.currentPartMatch(),SLOT(slotSoObjet()));
  connect(this,SIGNAL(soInd()),m_primaryMapping.currentPartMatch(),SLOT(slotSoInd()));
  connect(this,SIGNAL(aPropager(int)),m_primaryMapping.currentPartMatch(),SLOT(slotAPropager(int)));
  connect(m_primaryMapping.currentPartMatch(),SIGNAL(setSO(const QString&)),this,SLOT(slotSetSO(const QString&)));
  connect(m_primaryMapping.currentPartMatch(),SIGNAL(setAPropager(int)),this,SLOT(slotSetAPropager(int)));

  m_primaryMapping.currentPartMatch()-> setReadWrite();
}

void Syanot::slotAddAttribute(const QString& attribName)
{
  m_primaryMapping.currentPartMatch()-> addAttribute(attribName);
}

void Syanot::slotRemoveAttribute(const QString& nodeName, const QString& attribName)
{
  kDebug();
  m_primaryMapping.currentPartMatch()-> removeAttribute(nodeName,attribName);
}

void Syanot::slotAddNewElementAttribute(const QString& attrib)
{
  kDebug();
  m_newElementAttributes[attrib] = QString();
}

void Syanot::slotRemoveNewElementAttribute(const QString& attrib)
{
  kDebug();
  m_newElementAttributes.remove(attrib);
}

void Syanot::slotSujVButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "SUJ-V";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotCodVButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "COD-V";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotModVButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-V";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotModAButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-A";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotModPButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-P";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotJuxtButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "JUXT";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotApposButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "APPOS";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotCoordButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "COORD";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewCoord(attribs);
}

void Syanot::slotModRButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-R";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotModNButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-N";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotAtbSOButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "ATB-SO";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotCplVButtonClicked()
{
  m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "CPL-V";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotAuxVButtonClicked()
{
  if (m_primaryMapping.currentPartMatch() !=0 ) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "AUX-V";
  attribs["weight"] = "0.0";
  attribs["z"] = "5";
  if (m_primaryMapping.currentPartMatch() != 0)
    m_primaryMapping.currentPartMatch()-> prepareAddNewEdge(attribs);
}

void Syanot::slotGroupButtonClicked()
{
  kDebug();
  if (m_primaryMapping.currentPartMatch() != 0)
  {
    m_primaryMapping.currentPartMatch()-> prepareSelectElements();
    m_primaryMapping.currentPartMatch()->setAddingGroup(true);
  }
}

void Syanot::slotUtteranceClicked(QListWidgetItem* item)
{
  kDebug() << item->text();
  activateUtterance(item->text());
}

void Syanot::activateUtterance(const QString& id)
{
  kDebug() << id;
  if (m_primaryGraph != 0)
  {
    m_primaryGraph->hide();
    vboxLayout->removeWidget(m_primaryGraph);
  }
  else
  {
    vboxLayout->removeItem(spacerItem1);
  }
  if (m_secondaryGraph != 0)
  {
    m_secondaryGraph->hide();
    vboxLayout->removeWidget(m_secondaryGraph);
  }

  createPartFor(id);

  m_primaryMapping.setCurrentPart(id);
  m_secondaryMapping.setCurrentPart(id);
  m_manager->setActivePart(m_primaryMapping.currentPart());

  m_primaryGraph = m_primaryMapping.currentPart()->widget();
  m_primaryGraph->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  m_primaryGraph->sizePolicy().setHorizontalStretch(1);
  m_primaryGraph->sizePolicy().setVerticalStretch(1);

  vboxLayout->addWidget(m_primaryGraph);
  m_primaryGraph->show();

  if (m_secondaryMapping.contains(id))
  {
    m_secondaryGraph = m_secondaryMapping.currentPart()->widget();
    m_secondaryGraph->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_secondaryGraph->sizePolicy().setHorizontalStretch(1);
    m_secondaryGraph->sizePolicy().setVerticalStretch(1);

    vboxLayout->addWidget(m_secondaryGraph);
    m_secondaryGraph->show();
  }

  enonce->setText(m_primaryMapping.currentPartMatch()->text());
}

void Syanot::createPartFor(const QString& id)
{
  kDebug() << id;
  
  EasyUtterance* utterance = m_document->utteranceIdentifiedBy(id);
  if (utterance == 0) return;

  if (!m_primaryMapping.contains(id))
  {
    kDebug() << "creating part";
    KParts::ReadOnlyPart* part = static_cast<KParts::ReadOnlyPart*>(
    m_factory->create<KParts::ReadOnlyPart>(centralWidget(), this));
    kDebug() << "part created" << part;
    if (part)
    {
      PartMatch* partMatch = new PartMatch(part, utterance);
      m_primaryMapping.addMapping(partMatch);

      createGUI(part);
      part->unplugActionList("view_actionlist");
      m_manager->addPart( part, true );
      connect(this,SIGNAL(hide(KParts::Part*)),part,SLOT(slotHide(KParts::Part*)));
      connect(part,SIGNAL(close()), this,SLOT(slotClose()));
    }
  }

  if (m_secondaryDocument == 0) return;
  EasyUtterance* secondaryUtterance = m_secondaryDocument->utteranceIdentifiedBy(id);
  if (secondaryUtterance == 0) return;

  if (!m_secondaryMapping.contains(id))
  {
    kDebug() << "creating secondary part";
    KParts::ReadOnlyPart* part = static_cast<KParts::ReadOnlyPart*>(
    m_factory->create<KParts::ReadOnlyPart>(centralWidget(), this));
    kDebug() << "secondary part created" << part;
    if (part)
    {
      PartMatch* partMatch = new PartMatch(part, secondaryUtterance);
      m_secondaryMapping.addMapping(partMatch);

      createGUI(part);
      part->unplugActionList("view_actionlist");
      m_manager->addPart( part, true );
      connect(this,SIGNAL(hide(KParts::Part*)),part,SLOT(slotHide(KParts::Part*)));
      connect(part,SIGNAL(close()), this,SLOT(slotClose()));
    }
  }
}

bool Syanot::slotClose()
{
  kDebug() << m_documentModified;

  bool close;
  
  if (m_primaryMapping.currentPartMatch()) m_primaryMapping.currentPartMatch()->setAddingGroup(false);
  if (m_documentModified)
  {
    close = (KMessageBox::questionYesNo(0,
        i18n("Current document has unsaved changes. Do you really want to close it ?"),
        i18n("Closing a modified document"),
        KGuiItem("Close anyway"),
        KGuiItem("Don't close"),
        "closeAnyway"   ) == KMessageBox::Yes);
  }
  else
  {
   close = true;
  }
  kDebug() << "close=" << close << endl;
  if (close)
  {
    if (m_primaryGraph != 0)
    {
      m_primaryGraph->hide();
      vboxLayout->removeWidget(m_primaryGraph);
      m_primaryGraph = 0;
    }
    m_utterancesWidget->clear();
    m_primaryMapping.clear();
    m_newElementAttributes.clear();
    m_openedFile = KUrl();
    
    if (m_document != 0)
    {
      delete m_document;
      m_document = 0;
      m_documentModified = false;
    }
  }
  return close;
}

void Syanot::slotDocumentModified()
{
  kDebug();
  m_documentModified = true;
}

void Syanot::slotQuit()
{
  kDebug();
  if (queryClose())
    KApplication::kApplication()->quit();
}

void Syanot::createSOChooser()
{
  m_soChooserDockWidget = new QDockWidget(this);
  m_soButton = new KPushButton("s-o", m_soChooserDockWidget);

  KMenu* soMenu = new KMenu(m_soChooserDockWidget);
  KAction* sujetAction = new KAction(i18n("sujet"), this);
  soMenu->addAction(sujetAction);
  connect(sujetAction,SIGNAL(triggered()),this,SIGNAL(soSujet()));
  KAction* objetAction = new KAction(i18n("objet"), this);
  soMenu->addAction(objetAction);
  connect(objetAction,SIGNAL(triggered()),this,SIGNAL(soObjet()));
  KAction* indAction = new KAction(i18n("ind"), this);
  soMenu->addAction(indAction);
  connect(indAction,SIGNAL(triggered()),this,SIGNAL(soInd()));

  m_soButton->setMenu(soMenu);
  
  m_soChooserDockWidget->setWidget(m_soButton);
  addDockWidget ( Qt::LeftDockWidgetArea, m_soChooserDockWidget );
  m_soChooserDockWidget->hide();
}

void Syanot::createAPropagerChooser()
{
  m_aPropageDockWidget = new QDockWidget(this);

  m_aPropagerCheckbox = new QCheckBox(i18n("to propagate"), this);
  connect(m_aPropagerCheckbox,SIGNAL(stateChanged(int)),this,SIGNAL(aPropager(int)));
  m_aPropageDockWidget->setWidget(m_aPropagerCheckbox);
  addDockWidget ( Qt::LeftDockWidgetArea, m_aPropageDockWidget );
  m_aPropageDockWidget->hide();
}

void Syanot::slotShowSOChooser()
{
  kDebug() << m_soChooserDockWidget;
  m_soChooserDockWidget->show();
//   addDockWidget ( Qt::LeftDockWidgetArea, m_soChooserDockWidget );
}

void Syanot::slotHideSOChooser()
{
  kDebug();
  m_soChooserDockWidget->hide();
//   removeDockWidget ( m_soChooserDockWidget );
}

void Syanot::slotShowAPropagerChooser()
{
  kDebug() << m_aPropageDockWidget;
  m_aPropageDockWidget->show();
//   addDockWidget ( Qt::LeftDockWidgetArea, m_aPropageDockWidget );
}

void Syanot::slotHideAPropagerChooser()
{
  kDebug();
  m_aPropageDockWidget->hide();
//   removeDockWidget ( m_aPropageDockWidget );
}

void Syanot::slotSetSO(const QString& value)
{
  kDebug();
  m_soButton->setText(value);
}

void Syanot::slotSetAPropager(int value)
{
  kDebug();
  m_aPropagerCheckbox-> setCheckState ( value?Qt::Checked:Qt::Unchecked );
}

void Syanot::slotOpenForComparison(bool)
{
  kDebug();
  KUrl url = KFileDialog::getOpenUrl(KUrl(), QString("*"), 0, QString::null);
  if (url.isEmpty()) return;
  
  openSecondaryUrl(url);
}

void Syanot::openSecondaryUrl(const KUrl& url)
{
  kDebug() << url;
  m_secondaryDocument = new EasyDocument();
  QString tmpFileName;
  KIO::NetAccess::download(url,tmpFileName,this);
  QFile file(tmpFileName);
  if (!file.open(QFile::ReadOnly | QFile::Text))
  {
      KMessageBox::error(this,
                          i18n("Cannot read file %1:\n%2.",
                              tmpFileName,file.errorString()),
                          i18n("Syanot loading secondary utterances"));
      return;
  }
  kDebug() << "tmp file opened:" << tmpFileName;

  EasyXmlReader reader(m_secondaryDocument);
  if (!reader.read(&file))
  {
      KMessageBox::error(this,
                          i18n("Parse error in file %1 at line %2, column %3:\n%4",
                              tmpFileName, reader.lineNumber(),
                              reader.columnNumber(), reader.errorString()),
                          i18n("Syanot loading secondary utterances"));
  }
  else
  {
    statusBar()->showMessage(i18n("File loaded"), 2000);
    activateUtterance(m_primaryMapping.currentPartMatch()->utteranceId());
  }
}

void Syanot::slotCloseComparison(bool)
{
  if (m_secondaryGraph != 0)
  {
    m_secondaryGraph->hide();
    vboxLayout->removeWidget(m_secondaryGraph);
    m_secondaryGraph = 0;
  }
  m_secondaryMapping.clear();
  delete m_secondaryDocument;
  m_secondaryDocument = 0;
}

#include "syanot.moc"
