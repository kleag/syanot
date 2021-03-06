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
#include <kuniqueapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>
#include <iostream>
#include <qdir.h>
#include <QByteArray>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusReply>

#include "syanotadaptor.h"

static const char description[] =
I18N_NOOP("A Syntactic Analysis Annotation Tool for KDE");

static const char version[] = "1.0";

int main(int argc, char **argv)
{
  KAboutData about("syanot", 0, ki18n("Syanot"), version, ki18n(description),
                    KAboutData::License_GPL, ki18n("(C) 2007 Gael de Chalendar"), KLocalizedString(), 0, "kleag@free.fr");
  about.addAuthor( ki18n("Gael de Chalendar"), KLocalizedString(), "kleag@free.fr" );
  KCmdLineArgs::init(argc, argv, &about);

  KCmdLineOptions options;
  options.add("+[URL]", ki18n( "Dot graph to open" ));
  options.add("comparewith file", ki18n("URL of a file containing an analysis to compare the main analysis with"));
  options.add("utterances list", ki18n("List of utterances ids to display"));
  KCmdLineArgs::addCmdLineOptions( options );
  KApplication app;
  
// see if we are starting with session management
  if (app.isSessionRestored())
  {
      RESTORE(Syanot);
  }
  else
  {
      // no session.. just start up normally
      KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

      Syanot *widget = 0;
      if ( args->count() == 0 )
      {
        widget = new Syanot();
        new SyanotAdaptor(widget);
        QDBusConnection::sessionBus().registerObject("/Syanot", widget);
        widget->show();
      }
      else
      {
        QDBusReply<bool> reply = QDBusConnection::sessionBus().interface()->isServiceRegistered( "org.kde.syanot" );
      
        bool instanceExists = reply.value();
  
        for (int i = 0; i < args->count(); i++ )
        {
          if (instanceExists 
              && (KMessageBox::questionYesNo(0, 
                                         i18n("There is already a Syanot window opened. What's your choice ?"),
                                         i18n("Opening in new window confirmation"),
                                             KGuiItem("Open in the existing one"),
                                             KGuiItem("Open in new window"),
                                             "openInNewWindowMode"   ) == KMessageBox::Yes) )
          {
            QByteArray tosenddata;
            QDataStream arg(&tosenddata, QIODevice::WriteOnly);
            QString strarg = args->arg(i);
            KUrl url;
            if (strarg.left(1) == "/")
              url = KUrl(strarg);
            else url = KUrl(QDir::currentPath() + '/' + strarg);
            arg << url;
            QDBusInterface iface("org.kde.syanot", "/Syanot", "", QDBusConnection::sessionBus());
            if (iface.isValid()) 
            {
              if (args->isSet("utterances"))
              {
                QDBusReply<void> reply = iface.call("slotSetUtterances", args->getOption("utterances"));
                if (reply.isValid())
                {
                  kDebug() << "Reply was valid" << endl;
                }
                else
                {
                  kError() << "Call failed: " << reply.error().message() << endl;
                  return 1;
                }
              }
              QDBusReply<void> reply = iface.call("openUrl", url.pathOrUrl());
              if (reply.isValid()) 
              {
                kDebug() << "Reply was valid" << endl;
              }
              else
              {
                kError() << "Call failed: " << reply.error().message() << endl;
                return 1;
              }
              if (args->isSet("comparewith"))
              {
                QString strarg = args->getOption("comparewith");
                KUrl url;
                if (strarg.left(1) == "/")
                  url = KUrl(strarg);
                else url = KUrl(QDir::currentPath() + '/' + strarg);
                reply = iface.call("openSecondaryUrl", url.pathOrUrl());
                if (reply.isValid())
                {
                  kDebug() << "Reply was valid" << endl;
                }
                else
                {
                  kError() << "Call failed: " << reply.error().message() << endl;
                  return 1;
                }
                return 0;
              }
            }
            kError() << "Invalid interface" << endl;
            exit(0);
          }
          else
          {
            widget = new Syanot;
            new SyanotAdaptor(widget);
            QDBusConnection::sessionBus().registerObject("/Syanot", widget);
            widget->show();
            if (args->isSet("utterances"))
            {
              widget->slotSetUtterances(args->getOption("utterances"));
            }
            widget->openUrl( args->url( i ) );
            if (args->isSet("comparewith"))
            {
                QString strarg = args->getOption("comparewith");
              kDebug() << "comparewith" << args->getOption("comparewith");
                KUrl url;
                if (strarg.left(1) == "/")
                  url = KUrl(strarg);
                else url = KUrl(QDir::currentPath() + '/' + strarg);
              kDebug() << "openSecondaryUrl" << url.pathOrUrl();
              widget->openSecondaryUrl(url.pathOrUrl());
            }
          }
        }
      }
      args->clear();
  }
  return app.exec();
}
