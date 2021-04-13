/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     V4fr3e <V4fr3e@deepin.io>
*
* Maintainer: V4fr3e <liujinli@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "environments.h"
#include "vnoteapplication.h"
#include "views/vnotemainwindow.h"
#include "globaldef.h"
#include "common/performancemonitor.h"

#include <signal.h>

#include <DApplication>
#include <DApplicationSettings>
#include <DGuiApplicationHelper>
#include <DMainWindow>
#include <DLog>
#include <DWidgetUtil>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

void signalHander(int signo)
{
    switch (signo) {
    case SIGINT:
        break;
    case SIGTERM: {
        VNoteApplication *app = dynamic_cast<VNoteApplication *>(qApp);
        app->handleQuitAction();
        qInfo() << "get SIGTERM";
    } break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    PerformanceMonitor::initializeAppStart();

    VNoteApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationName(DEEPIN_VOICE_NOTE);
    app.setApplicationVersion(VERSION);
    app.setProductIcon(QIcon::fromTheme(DEEPIN_VOICE_NOTE));
    app.setProductName(DApplication::translate("AppMain", "Voice Notes"));
    app.setApplicationDisplayName(DApplication::translate("AppMain", "Voice Notes"));
    app.setApplicationDescription(DApplication::translate("AppMain",
                                                          "Voice Notes is a lightweight memo tool to make text notes and voice recordings."));

    qputenv("DTK_USE_SEMAPHORE_SINGLEINSTANCE", "1");

    DGuiApplicationHelper::instance()->setSingleInstanceInterval(-1);
    if (!DGuiApplicationHelper::instance()->setSingleInstance(
            app.applicationName(),
            DGuiApplicationHelper::UserScope)) {
        return 0;
    }

    signal(SIGTERM, signalHander);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    app.activateWindow();

    return app.exec();
}
