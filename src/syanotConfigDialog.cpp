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


#include "syanotConfigDialog.h"
#include "syanotsettings.h"

#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <ktabwidget.h>
#include <kparts/partmanager.h>
#include <kedittoolbar.h>
#include <kdebug.h>

#include <klibloader.h>
#include <kmessagebox.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <kconfigdialog.h>

//#include <kapp.h>
//#include <dcopclient.h>

#include <iostream>

using namespace Syanot;

SyanotConfigurationDialog::SyanotConfigurationDialog (QWidget *parent, const QString& name, KConfigSkeleton *config,
              KPageDialog::FaceType dialogType, 
              ButtonCodes dialogButtons, 
              ButtonCode defaultButton, bool modal) : 
  KConfigDialog (parent, name, config),//, dialogType, dialogButtons, defaultButton, modal) ,
  m_changed(false)
{
}

SyanotConfigurationDialog::~SyanotConfigurationDialog ()
{
}

void SyanotConfigurationDialog::settingChanged(int)
{
//   std::cerr << "SyanotConfigurationDialog::settingChanged" << std::endl;
  m_changed = true;
//   settingsChangedSlot();
//   updateButtons ();
}

bool SyanotConfigurationDialog::hasChanged()
{
//   std::cerr << "SyanotConfigurationDialog::hasChanged" << std::endl;
  return m_changed;
}

void SyanotConfigurationDialog::updateSettings()
{
//   std::cerr << "SyanotConfigurationDialog::updateSettings" << std::endl;
  m_changed = false;
/*  switch (m_openingWidget->openInExistingWindowMode->selectedId())
  {
    case 0: // no
      KGraphViewerSettings::setOpenInExistingWindowMode("no");
    break;
    case 1: // yes
      KGraphViewerSettings::setOpenInExistingWindowMode("yes");
    break;
    case 2: // ask
      KGraphViewerSettings::setOpenInExistingWindowMode("ask");
    break;
    default: ;
  }
  switch (m_reloadWidget->reloadOnChangeMode->selectedId())
  {
    case 0: // no
      KGraphViewerSettings::setReloadOnChangeMode("no");
    break;
    case 1: // yes
      KGraphViewerSettings::setReloadOnChangeMode("yes");
    break;
    case 2: // ask
      KGraphViewerSettings::setReloadOnChangeMode("ask");
    break;
    default: ;
  }
  switch (m_reopeningWidget->reopenPreviouslyOpenedFilesMode->selectedId())
  {
    case 0: // no
      KGraphViewerSettings::setReopenPreviouslyOpenedFilesMode("no");
    break;
    case 1: // yes
      KGraphViewerSettings::setReopenPreviouslyOpenedFilesMode("yes");
    break;
    case 2: // ask
      KGraphViewerSettings::setReopenPreviouslyOpenedFilesMode("ask");
    break;
    default: ;
  }*/
   //@TODO to port
   //KGraphViewerSettings::writeConfig();
}

void SyanotConfigurationDialog::updateWidgets()
{
//   std::cerr << "SyanotConfigurationDialog::updateWidgets" << std::endl;

  m_changed = false;
//   std::cerr << "  openInExistingWindowMode: " << KGraphViewerSettings::openInExistingWindowMode() << std::endl;
//   if (KGraphViewerSettings::openInExistingWindowMode() == "no")
//     m_openingWidget->openInExistingWindowMode->setButton(0);
//   else if (KGraphViewerSettings::openInExistingWindowMode() == "yes")
//     m_openingWidget->openInExistingWindowMode->setButton(1);
//   else if (KGraphViewerSettings::openInExistingWindowMode() == "ask")
//     m_openingWidget->openInExistingWindowMode->setButton(2);


//   std::cerr << "  reloadOnChangeMode: " << KGraphViewerSettings::reloadOnChangeMode() << std::endl;
//   if (KGraphViewerSettings::reloadOnChangeMode() == "no")
//     m_reloadWidget->reloadOnChangeMode->setButton(0);
//   else if (KGraphViewerSettings::reloadOnChangeMode() == "yes")
//     m_reloadWidget->reloadOnChangeMode->setButton(1);
//   else if (KGraphViewerSettings::reloadOnChangeMode() == "ask")
//     m_reloadWidget->reloadOnChangeMode->setButton(2);

//   std::cerr << "  reopenPreviouslyOpenedFilesMode: " << KGraphViewerSettings::reopenPreviouslyOpenedFilesMode() << std::endl;
/*  if (KGraphViewerSettings::reopenPreviouslyOpenedFilesMode() == "no")
    m_reopeningWidget->reopenPreviouslyOpenedFilesMode->setButton(0);
  else if (KGraphViewerSettings::reopenPreviouslyOpenedFilesMode() == "yes")
    m_reopeningWidget->reopenPreviouslyOpenedFilesMode->setButton(1);
  else if (KGraphViewerSettings::reopenPreviouslyOpenedFilesMode() == "ask")
    m_reopeningWidget->reopenPreviouslyOpenedFilesMode->setButton(2);*/
}

#include "syanotConfigDialog.moc"
