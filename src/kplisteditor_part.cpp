/***************************************************************************
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "kplisteditor_part.h"

#include "kplisteditor_part.moc"

#include <kaction.h>
#include <kactioncollection.h>
#include <kcomponentdata.h>
#include <kfiledialog.h>
#include <kparts/genericfactory.h>
#include <kstandardaction.h>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QTextEdit>

#include "plisteditorwidget.h"

typedef KParts::GenericFactory<KPlistEditorPart> KPlistEditorPartFactory;
K_EXPORT_COMPONENT_FACTORY( libkplisteditorpart, KPlistEditorPartFactory )

KPlistEditorPart::KPlistEditorPart( QWidget *parentWidget, QObject *parent, const QStringList & /*args*/ )
    : KParts::ReadWritePart(parent)
{
    // we need an instance
    setComponentData( KPlistEditorPartFactory::componentData() );

    // this should be your custom internal widget
    m_widget = new PListEditorWidget ( parentWidget);

    // notify the part that this is our internal widget
    setWidget(m_widget);

    // create our actions
    KStandardAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
    save = KStandardAction::save(this, SLOT(save()), actionCollection());

    // set our XML-UI resource file
    setXMLFile("kplisteditor_part.rc");

    // we are read-write by default
    setReadWrite(true);

    // we are not modified since we haven't done anything yet
    setModified(false);
}

KPlistEditorPart::~KPlistEditorPart()
{
}

void KPlistEditorPart::setReadWrite(bool rw)
{
	qDebug() << "Marking as : " << rw;
	
    // notify your internal widget of the read-write state
    m_widget->setReadOnly(!rw);
    if (rw)
        connect(m_widget, SIGNAL(textChanged()),
                this,     SLOT(setModified()));
    else
    {
        disconnect(m_widget, SIGNAL(textChanged()),
                   this,     SLOT(setModified()));
    }

    ReadWritePart::setReadWrite(rw);
}

void KPlistEditorPart::setModified(bool modified)
{
    // get a handle on our Save action and make sure it is valid
    if (!save)
        return;

    // if so, we either enable or disable it based on the current
    // state
    if (modified)
        save->setEnabled(true);
    else
        save->setEnabled(false);

    // in any event, we want our parent to do it's thing
    ReadWritePart::setModified(modified);
}

KAboutData *KPlistEditorPart::createAboutData()
{
    // the non-i18n name here must be the same as the directory in
    // which the part's rc file is installed ('partrcdir' in the
    // Makefile)
    KAboutData *aboutData = new KAboutData("kplisteditorpart", "kplisteditor", ki18n("KPlistEditorPart"), "%{VERSION}");
    aboutData->addAuthor(ki18n("%{AUTHOR}"), KLocalizedString(), "%{EMAIL}");
    return aboutData;
}

bool KPlistEditorPart::openFile()
{	
    // m_file is always local so we can use QFile on it
    QFile file(localFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
        return false;
	}
	
	QByteArray a = file.readAll();
	
    file.close();

	std::vector<char> array;
	
	for (int x=0; x < a.size(); ++x)
		array.push_back (a[x]);
	
    // now that we have the entire file, display it
    m_widget->loadXML(QString(a).toStdString());
	//m_widget->loadBinary (array);
	
    // just for fun, set the status bar
    //emit setStatusBarText( m_url.prettyUrl() );

    return true;
}

bool KPlistEditorPart::saveFile()
{
    // if we aren't read-write, return immediately
    if (isReadWrite() == false)
        return false;

#if 0
    // m_file is always local, so we use QFile
    QFile file("m_file");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    // use QTextStream to dump the text to the file
    QTextStream stream(&file);
    stream << m_widget->document();

    file.close();
#endif

    return true;
}

void KPlistEditorPart::fileSaveAs()
{
    // this slot is called whenever the File->Save As menu is selected,
    QString file_name = KFileDialog::getSaveFileName();
    if (file_name.isEmpty() == false)
        saveAs(file_name);
}

