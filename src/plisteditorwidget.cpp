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


#include "plisteditorwidget.h"

#include <qdebug.h>

#define NameColumn 0
#define TypeColumn 1
#define ValueColumn 2

PListEditorWidget::PListEditorWidget(QWidget* parent)
: QWidget (parent), m_currentPlist (NULL)
{
	m_ui.setupUi (this);
}

PListEditorWidget::~PListEditorWidget()
{

}

void PListEditorWidget::setReadOnly(bool state)
{
	m_readOnly = state;
}

void PListEditorWidget::reloadGui(void )
{
	QTreeWidget* tree = m_ui.plistTree;
	tree->clear();

	QTreeWidgetItem* rootNode = NULL;
	
	if (m_currentPlist)
	{
		// Add information
		if (m_currentPlist->GetType() == PLIST_ARRAY)
		{
			rootNode = generateArrayItem (m_currentPlist);
			rootNode->setText (NameColumn, "Root");
		}
		else if (m_currentPlist->GetType() == PLIST_DICT)
		{
			rootNode = generateDictionaryItem (m_currentPlist);
			rootNode->setText (NameColumn, "Root");
		}
	}
	
	if (rootNode)
		tree->addTopLevelItem(rootNode);
	
	tree->expandAll();
	
	for (int x=0; x < tree->columnCount(); ++x)
		tree->resizeColumnToContents(x);
}

void PListEditorWidget::loadXML(const std::string& xml)
{
	m_currentPlist = PList::Structure::FromXml(xml);

	qDebug() << "Loaded " << QString::fromStdString(xml);
	
	reloadGui();
}

void PListEditorWidget::loadBinary (const std::vector<char>& bin)
{
	m_currentPlist = PList::Structure::FromBin (bin);
	
	reloadGui();
}

QTreeWidgetItem* PListEditorWidget::generateArrayItem(PList::Node* node)
{
	PList::Array* array = (PList::Array*) node;
	
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "Array");
	item->setText (ValueColumn, "(" + QString::number(array->GetSize()) + " items)");
	item->setForeground(ValueColumn, Qt::gray);
	
	for (unsigned int x=0; x < array->GetSize(); ++x)
	{
		QString name = "Item " + QString::number(x);
		PList::Node* n = (*array)[x];
		
		QTreeWidgetItem* child = NULL;
		switch (n->GetType())
		{
			case PLIST_ARRAY:
				child = generateArrayItem(n);
				break;
				
			case PLIST_BOOLEAN:
				child = generateBooleanItem(n);
				break;
				
			case PLIST_DATA:
				child = generateDataItem(n);
				break;
				
			case PLIST_DATE:
				child = generateDateItem(n);
				break;
				
			case PLIST_DICT:
				child = generateDictionaryItem(n);
				break;
				
			case PLIST_UINT:
				child = generateIntegerItem(n);
				break;
				
			case PLIST_REAL:
				child = generateRealItem(n);
				break;
				
			case PLIST_STRING:
				child = generateStringItem(n);
				break;
				
			default:
				qDebug() << "Unknown type";
		}
		
		if (child)
		{
			child->setText (NameColumn, name);
			item->addChild (child);
		}
	}
	return item;
}

QTreeWidgetItem* PListEditorWidget::generateBooleanItem(PList::Node* node )
{
	PList::Boolean* boolean = (PList::Boolean*) node;
	
	// Simple
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "Boolean");
	item->setText (ValueColumn, (boolean->GetValue() ? "true" : "false"));
	
	return item;
}

QTreeWidgetItem* PListEditorWidget::generateDataItem(PList::Node* node )
{
	PList::Data* data = (PList::Data*) node;
	
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "Data");
	item->setText (ValueColumn, "( " + QString::number(data->GetValue().size()) + " bytes)");
	item->setForeground(ValueColumn, Qt::gray);
	
	return item;
}

QTreeWidgetItem* PListEditorWidget::generateDateItem(PList::Node* node )
{
	PList::Date* date = (PList::Date*) node;
	timeval t = date->GetValue();
	
	uint64_t secondsSince1970 = t.tv_sec;
	
	QDateTime dateTime = QDateTime::fromTime_t(secondsSince1970);
	
	// Simple
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "Date");
	item->setText (ValueColumn, dateTime.toString());
	
	return item;
}

QTreeWidgetItem* PListEditorWidget::generateDictionaryItem(PList::Node* node )
{
	PList::Dictionary* dict = (PList::Dictionary*) node;
	
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "Dictionary");
	item->setText (ValueColumn, "(" + QString::number(dict->GetSize()) + " items)");
	item->setForeground(ValueColumn, Qt::gray);
	
	for (PList::Dictionary::iterator i = dict->Begin(); i != dict->End(); ++i)
	{		
		PList::Node* n = i->second;
		
		QString name = QString::fromStdString(i->first);
		
		QTreeWidgetItem* child = NULL;
		switch (n->GetType())
		{
			case PLIST_ARRAY:
				child = generateArrayItem(n);
				break;
				
			case PLIST_BOOLEAN:
				child = generateBooleanItem(n);
				break;
				
			case PLIST_DATA:
				child = generateDataItem(n);
				break;
				
			case PLIST_DATE:
				child = generateDateItem(n);
				break;
				
			case PLIST_DICT:
				child = generateDictionaryItem(n);
				break;
				
			case PLIST_UINT:
				child = generateIntegerItem(n);
				break;
				
			case PLIST_REAL:
				child = generateRealItem(n);
				break;
				
			case PLIST_STRING:
				child = generateStringItem(n);
				break;
				
			default:
				qDebug() << "Unknown type";
		}
		
		if (child)
		{
			child->setText (NameColumn, name);
			item->addChild (child);
		}
	}
	
	return item;
}

QTreeWidgetItem* PListEditorWidget::generateIntegerItem(PList::Node* node )
{
	PList::Integer* integer = (PList::Integer*) node;
	
	// Simple
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "Integer");
	item->setText (ValueColumn, QString::number(integer->GetValue()));
	
	return item;
}

QTreeWidgetItem* PListEditorWidget::generateRealItem(PList::Node* node )
{
	PList::Real* real = (PList::Real*) node;
	
	// Simple
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "Real");
	item->setText (ValueColumn, QString::number(real->GetValue()));
	
	return item;
}

QTreeWidgetItem* PListEditorWidget::generateStringItem(PList::Node* node )
{
	PList::String* str = (PList::String*) node;
	
	// Simple
	QTreeWidgetItem* item = new QTreeWidgetItem();
	
	item->setText (NameColumn, "<unknown>");
	item->setText (TypeColumn, "String");
	item->setText (ValueColumn, str->GetValue().c_str());
	
	return item;
}


