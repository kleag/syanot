/* This file is part of Syanot
   Copyright (C) 2005-2007 Gael de Chalendar <kleag@free.fr>

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


#ifndef SYANOTCONFIGDIALOG_H
#define SYANOTCONFIGDIALOG_H

#include <kconfigdialog.h>

/**
 * This is the Syanot configuration dialog
 *
 * @short Configuration dialog
 * @author Gael de Chalendar <kleag@free.fr>
 */
class SyanotConfigurationDialog : public KConfigDialog
{
  Q_OBJECT
public:
   SyanotConfigurationDialog (QWidget *parent, const QString& name, KConfigSkeleton *config,
                     KPageDialog::FaceType dialogType,//=IconList, 
                  ButtonCodes dialogButtons=Default|Ok|Apply|Cancel|Help, 
                  ButtonCode defaultButton=Ok, bool modal=false);

  virtual ~SyanotConfigurationDialog ();

protected slots:
  virtual void updateSettings();
  virtual void updateWidgets();
  void settingChanged(int);

protected:
  virtual bool hasChanged();

  bool m_changed;
};

#endif // SYANOTCONFIGDIALOG_H
