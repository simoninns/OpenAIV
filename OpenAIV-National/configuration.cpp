/************************************************************************

    configuration.cpp

    OpenAIV-National - Domesday Retrieval application
    Copyright (C) 2020-2022 Simon Inns

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

#include "configuration.h"

// This define should be incremented if the settings file format changes
static const qint32 SETTINGSVERSION = 1;

Configuration::Configuration(QObject *parent) : QObject(parent)
{
    // Open the application's configuration file
    QString configurationPath;
    QString configurationFileName;

    configurationPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) ;
    configurationFileName = "openAIV.ini" ;
    configuration = new QSettings(configurationPath + "/"+ configurationFileName, QSettings::IniFormat);

    // Read the configuration
    readConfiguration();

    // Are the configuration settings valid?
    if (settings.version != SETTINGSVERSION) {
        qDebug() << "Configuration::Configuration(): Configuration invalid or wrong version (" <<
                    settings.version << "!= " << SETTINGSVERSION <<").  Setting to default values";

        // Set default configuration
        setDefault();
    }
}

Configuration::~Configuration()
{
    delete configuration;
}

void Configuration::writeConfiguration(void)
{
    // Write the valid configuration flag
    configuration->setValue("version", settings.version);

    // Directories
    configuration->beginGroup("directories");
    configuration->setValue("dataDirectory", settings.directories.dataDirectory);
    configuration->setValue("videoDirectory", settings.directories.videoDirectory);
    configuration->endGroup();

    // Windows
    configuration->beginGroup("windows");
    configuration->setValue("mainWindowGeometry", settings.windows.mainWindowGeometry);
    configuration->setValue("essayDialogGeometry", settings.windows.essayDialogGeometry);
    configuration->setValue("pictureSetDialogGeometry", settings.windows.pictureSetDialogGeometry);
    configuration->setValue("dataSetDialogGeometry", settings.windows.dataSetDialogGeometry);
    configuration->setValue("walkDialogGeometry", settings.windows.walkDialogGeometry);
    configuration->endGroup();

    // Sync the settings with disk
    qDebug() << "Configuration::writeConfiguration(): Writing configuration to disk";
    configuration->sync();
}

void Configuration::readConfiguration(void)
{
    qDebug() << "Configuration::readConfiguration(): Reading configuration from" << configuration->fileName();

    // Read the valid configuration flag
    settings.version = configuration->value("version").toInt();

    // Directories
    configuration->beginGroup("directories");
    settings.directories.dataDirectory = configuration->value("dataDirectory").toString();
    settings.directories.videoDirectory = configuration->value("videoDirectory").toString();
    configuration->endGroup();

    // Windows
    configuration->beginGroup("windows");
    settings.windows.mainWindowGeometry = configuration->value("mainWindowGeometry").toByteArray();
    settings.windows.essayDialogGeometry = configuration->value("essayDialogGeometry").toByteArray();
    settings.windows.pictureSetDialogGeometry = configuration->value("pictureSetDialogGeometry").toByteArray();
    settings.windows.dataSetDialogGeometry = configuration->value("dataSetDialogGeometry").toByteArray();
    settings.windows.walkDialogGeometry = configuration->value("walkDialogGeometry").toByteArray();
    configuration->endGroup();
}

void Configuration::setDefault(void)
{
    // Set up the default values
    settings.version = SETTINGSVERSION;

    // Directories
    settings.directories.dataDirectory = QDir::homePath();
    settings.directories.videoDirectory = QDir::homePath();

    // Windows
    settings.windows.mainWindowGeometry = QByteArray();
    settings.windows.essayDialogGeometry = QByteArray();
    settings.windows.pictureSetDialogGeometry = QByteArray();
    settings.windows.dataSetDialogGeometry = QByteArray();
    settings.windows.walkDialogGeometry = QByteArray();

    // Write the configuration
    writeConfiguration();
}

// Functions to get and set configuration values ----------------------------------------------------------------------

// Directories
void Configuration::setDataDirectory(QString dataDirectory)
{
    settings.directories.dataDirectory = dataDirectory;
}

QString Configuration::getDataDirectory(void)
{
    return settings.directories.dataDirectory;
}

void Configuration::setVideoDirectory(QString videoDirectory)
{
    settings.directories.videoDirectory = videoDirectory;
}

QString Configuration::getVideoDirectory(void)
{
    return settings.directories.videoDirectory;
}

// Windows
void Configuration::setMainWindowGeometry(QByteArray mainWindowGeometry)
{
    settings.windows.mainWindowGeometry = mainWindowGeometry;
}

QByteArray Configuration::getMainWindowGeometry(void)
{
    return settings.windows.mainWindowGeometry;
}

void Configuration::setEssayDialogGeometry(QByteArray essayDialogGeometry)
{
    settings.windows.essayDialogGeometry = essayDialogGeometry;
}

QByteArray Configuration::getEssayDialogGeometry(void)
{
    return settings.windows.essayDialogGeometry;
}

void Configuration::setPictureSetDialogGeometry(QByteArray pictureSetDialogGeometry)
{
    settings.windows.pictureSetDialogGeometry = pictureSetDialogGeometry;
}

QByteArray Configuration::getPictureSetDialogGeometry(void)
{
    return settings.windows.pictureSetDialogGeometry;
}

void Configuration::setDataSetDialogGeometry(QByteArray dataSetDialogGeometry)
{
    settings.windows.dataSetDialogGeometry = dataSetDialogGeometry;
}

QByteArray Configuration::getDataSetDialogGeometry(void)
{
    return settings.windows.dataSetDialogGeometry;
}

void Configuration::setWalkDialogGeometry(QByteArray walkDialogGeometry)
{
    settings.windows.walkDialogGeometry = walkDialogGeometry;
}

QByteArray Configuration::getWalkDialogGeometry(void)
{
    return settings.windows.walkDialogGeometry;
}
