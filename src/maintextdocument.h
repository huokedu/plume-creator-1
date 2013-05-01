/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   terreville@gmail.com                                                 *
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
#ifndef MAINTEXTDOCUMENT_H
#define MAINTEXTDOCUMENT_H


#include <QObject>
#include <QTextDocument>
#include <QDebug>

#include "wordcountengine.h"
#include "texthighlighter.h"

class MainTextDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit MainTextDocument(QObject *parent = 0);


    int idNumber() const;
    void setIdNumber(int number);
    QString docType() const;
    void setDocType(QString type);
    int cursorPos() const;

    int wordCount(){return wordCountEngine->wordCount();}
    TextHighlighter* textHighlighter();

signals:
    void wordCountChanged(QString type, int id, int count);

public slots:
    void setCursorPos(int pos);

private slots:
    void wordCountChangedSlot(int count);

private:
    WordCountEngine *wordCountEngine;
    TextHighlighter *highlighter;
    int itemId, m_cursorPos;
    QString m_docType;

};

#endif // MAINTEXTDOCUMENT_H
