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


#ifndef SYANOTMAINWINDOW_H
#define SYANOTMAINWINDOW_H

#include "ui_syanotmainwindow.h"

class SyanotMainWindowUI : public QFrame, public Ui::SyanotMainWindow
{
    Q_OBJECT
    public:
        SyanotMainWindowUI( QWidget *parent=0 );
};

class SyanotMainWindow : public QWidget
{
    Q_OBJECT
    public:
        SyanotMainWindow( QWidget *parent=0 );
        ~SyanotMainWindow();

        QWidget* graphWidget() {return ui->graphWidget;}
        void setGraphWidget(QWidget* w) {ui->graphWidget = w;}
    private slots:
//         void slotButtonClicked();

    private:
        SyanotMainWindowUI *ui;
};

#endif

