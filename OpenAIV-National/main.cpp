/************************************************************************

    main.cpp

    OpenAIV - Domesday Retrieval application
    Copyright (C) 2020 Simon Inns

    This file is part of OpenAIV.

    This application is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#include <QApplication>
#include <QDebug>
#include <QtGlobal>
#include <QCommandLineParser>

#include "mainwindow.h"
#include "logging.h"

int main(int argc, char *argv[])
{
    // Install the local debug message handler
    setDebug(true);
    qInstallMessageHandler(debugOutputHandler);

    QApplication a(argc, argv);

    // Set application name and version
    QCoreApplication::setApplicationName("OpenAIV");
    QCoreApplication::setApplicationVersion("1.0");
    QCoreApplication::setOrganizationDomain("domesday86.com");

    // Set up the command line parser
    QCommandLineParser parser;
    parser.setApplicationDescription(
                "OpenAIV - Domesday Retrieval application\n"
                "\n"
                "(c)2020 Simon Inns\n"
                "GPLv3 Open-Source - github: https://github.com/simoninns");
    parser.addHelpOption();
    parser.addVersionOption();

    // Option to show debug (-d / --debug)
    QCommandLineOption showDebugOption(QStringList() << "d" << "debug",
                                       QCoreApplication::translate("main", "Show debug"));
    parser.addOption(showDebugOption);

    // Process the command line options and arguments given by the user
    parser.process(a);

    // Get the options from the parser
    bool isDebugOn = parser.isSet(showDebugOption);

    // Process the command line options
    if (isDebugOn) setDebug(true); else setDebug(false);

    // Start the GUI application
    MainWindow w;
    w.show();

    return a.exec();
}
