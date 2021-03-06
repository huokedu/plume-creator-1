/***************************************************************************
 *   Copyright (C) 2013 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                        *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef CHECKABLETREE_H
#define CHECKABLETREE_H

#include <QTreeView>
#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"


class CheckableTree : public QTreeView
{



    Q_OBJECT
public:
    explicit CheckableTree(QWidget *parent = 0);
    // don't forget to use setHub(...) and setMainTreeAbstractModel(...) before postConstructor();
    void postConstructor();


    void clickCheckBox(QModelIndex index);

protected:

//    void mousePressEvent(QMouseEvent *event);


signals:
    void clickCheckBoxSignal(QModelIndex index);
    void textAndNoteSignal(int number, QString action);
    void currentOpenedSheetSignal(int sheetIdNumber);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}
    void applySettingsFromData();

private slots:
    void itemCollapsedSlot(QModelIndex index);
    void itemExpandedSlot(QModelIndex index);
    void itemClicked(QModelIndex index);

private:
    Hub *hub;
    MainTreeAbstractModel  *absTreeModel;

    QList<QModelIndex> allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent);
QModelIndexList m_selectedIndexes;



// clicks :
bool oneClickCheckpoint, twoClicksCheckpoint, threeClicksCheckpoint;
QModelIndex oldIndex;
};



#endif // CHECKABLETREE_H
