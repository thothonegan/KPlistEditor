/*
    KPListEditor - KDE editor of Apple Property Lists
    Copyright (C) 2011  Kenneth Perry <thothonegan@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PLISTEDITORWIDGET_H
#define PLISTEDITORWIDGET_H

#include <QtGui/QWidget>

#include "ui_plisteditorwidget.h"

#include "plist/plist++.h"

class PListEditorWidget : public QWidget
{
	public:
		PListEditorWidget(QWidget* parent = NULL);
		virtual ~PListEditorWidget();
		
		void setReadOnly (bool state);
		
		void loadXML (const std::string& xml);
		void loadBinary (const std::vector<char>& bin);
		
		void reloadGui (void);
		
		QTreeWidgetItem* generateArrayItem (PList::Node* node);
		QTreeWidgetItem* generateBooleanItem (PList::Node* node);
		QTreeWidgetItem* generateDataItem (PList::Node* node);
		QTreeWidgetItem* generateDateItem (PList::Node* node);
		QTreeWidgetItem* generateDictionaryItem (PList::Node* node);
		QTreeWidgetItem* generateIntegerItem (PList::Node* node);
		QTreeWidgetItem* generateRealItem (PList::Node* node);
		QTreeWidgetItem* generateStringItem (PList::Node* node);
		
	private:
		Ui::PListEditorWidget m_ui;
		
		PList::Structure* m_currentPlist;
		
		bool m_readOnly;
};

#endif // PLISTEDITORWIDGET_H
