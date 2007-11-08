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

#include <QtDBus/QtDBus>
#include <QDockWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>

#include <iostream>

using namespace Syanot;

Syanot::Syanot() :
    KParts::MainWindow(),
    m_widget(0),
    m_rfa(0),
    m_currentPart(0),
    m_document(0)
{
  kDebug();
  m_factory = KLibLoader::self()->factory("kgraphviewerpart");
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
}

Syanot::~Syanot()
{
  kDebug() ;
}

void Syanot::openUrl(const KUrl& url)
{
  kDebug() << url;
  if (m_document != 0)
  {
    delete m_document;
  }
  m_document = new EasyDocument();

  QString tmpFileName;
  KIO::NetAccess::download(url,tmpFileName,this);
  QFile file(url.path());
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
    statusBar()->showMessage(i18n("File loaded"), 2000);

    foreach (EasyUtterance* utterance, *m_document)
    {
      m_utterancesWidget->addItem(utterance->id());
    }
    m_utterancesWidget->setCurrentRow(0);
    activateUtterance(m_utterancesWidget->currentItem()->text());
  }
}

void Syanot::fileOpen()
{
  kDebug();
  // this slot is called whenever the File->Open menu is selected,
  // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
  // button is clicked
  QString fileName = KFileDialog::getOpenFileName(KUrl(QString()), QString("*"), 0, QString::null);
  
  if (m_rfa != 0)
  {
    m_rfa->addUrl(fileName);
  }
  openUrl(fileName);
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

  actionCollection()->addAction( KStandardAction::Quit, "file_quit", this, SLOT( quit() ) );

  m_statusbarAction = KStandardAction::showStatusbar(this, SLOT(optionsShowStatusbar()), this);

  actionCollection()->addAction( KStandardAction::KeyBindings, "options_configure_keybinding", this, SLOT( optionsConfigureKeys() ) );
//   KStandardAction::keyBindings(this, SLOT(optionsConfigureKeys()), this);
  actionCollection()->addAction( KStandardAction::ConfigureToolbars, "options_configure_toolbars", this, SLOT( optionsConfigureToolbars() ) );
  actionCollection()->addAction( KStandardAction::Preferences, "options_configure", this, SLOT( optionsConfigure() ) );
}

bool Syanot::queryExit()
{
  kDebug() ;
  m_rfa->saveEntries(KGlobal::config()->group("syanot"));

  // 
  //@TODO to port
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
  openUrl(url);
}

void Syanot::close()
{
}

void Syanot::fileSave()
{
  kDebug();
  if (m_widget != 0)
  {
    QFile f(m_openedFile);
    if (f.open(QIODevice::WriteOnly))
    {
      QTextStream s(&f);
      s << *m_document;
    }
  }
}

void Syanot::fileSaveAs()
{
  if (m_widget != 0)
  {
    QString fileName = KFileDialog::getSaveFileName(KUrl(),
                "*.easy.xml", m_widget,
                i18n("Save current graph to..."));
    m_openedFile = fileName;
    fileSave();
  }
}

void Syanot::slotSetActiveGraph( KParts::Part* part)
{
  kDebug();
  m_currentPart = part;
  m_currentPartMatch = m_partPartMatchMap[part];
  if (m_currentPart == 0)
  {
    return;
  }

  m_currentPartMatch-> setReadWrite();
}

void Syanot::slotAddAttribute(const QString& attribName)
{
  m_currentPartMatch-> addAttribute(attribName);
}

void Syanot::slotRemoveAttribute(const QString& nodeName, const QString& attribName)
{
  kDebug();
  m_currentPartMatch-> removeAttribute(nodeName,attribName);
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
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "SUJ-V";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotCodVButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "COD-V";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotModVButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-V";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotModAButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-A";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotModPButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-P";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotJuxtButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "JUXT";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotApposButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "APPOS";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotCoordButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "COORD";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotModRButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-R";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotModNButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "MOD-N";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotAtbSOButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "ATB-SO";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotCplVButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "CPL-V";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotAuxVButtonClicked()
{
  QMap<QString,QString> attribs;
  attribs["color"] = "black";
  attribs["label"] = "AUX-V";
  attribs["weight"] = "0.0";
  m_currentPartMatch-> prepareAddNewEdge(attribs);
}

void Syanot::slotUtteranceClicked(QListWidgetItem* item)
{
  kDebug() << item->text();
  activateUtterance(item->text());
}

void Syanot::activateUtterance(const QString& id)
{
  kDebug() << id;
  if (m_widget != 0)
  {
    m_widget->hide();
    vboxLayout->removeWidget(m_widget);
  }
  else
  {
    vboxLayout->removeItem(spacerItem1);
  }

  if (!m_utteranceIdPartMap.contains(id))
  {
    createPartFor(id);
  }
  m_currentPart = m_utteranceIdPartMap[id];
  m_currentPartMatch = m_utteranceIdPartMatchMap[id];
  m_manager->setActivePart(m_currentPart);

  m_widget = m_currentPart->widget();
  m_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  m_widget->sizePolicy().setHorizontalStretch(1);
  m_widget->sizePolicy().setVerticalStretch(1);

  vboxLayout->addWidget(m_widget);
  m_widget->show();
}

void Syanot::createPartFor(const QString& id)
{
  kDebug() << id;
  
  EasyUtterance* utterance = m_document->utteranceIdentifiedBy(id);
  if (utterance == 0) return;

  if (m_utteranceIdPartMap.contains(id))
  {
    return;
  }
  
  KParts::ReadOnlyPart* part = static_cast<KParts::ReadOnlyPart*>(
  m_factory->create<KParts::ReadOnlyPart>(centralWidget(), this));
  kDebug() << "part created" << part;
  if (part)
  {
    PartMatch* partMatch = new PartMatch(part, utterance);
    m_utteranceIdPartMap[id] = part;
    m_utteranceIdPartMatchMap[id] = partMatch;
    m_partPartMatchMap[part] = partMatch;
    
    createGUI(part);
    m_manager->addPart( part, true );
    connect(this,SIGNAL(hide(KParts::Part*)),part,SLOT(slotHide(KParts::Part*)));
  }
}

#include "syanot.moc"