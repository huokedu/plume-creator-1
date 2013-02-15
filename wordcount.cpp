#include <QObject>
#include <QtGui>
#include <QDebug>

#include "wordcount.h"
//
WordCount::WordCount(QTextDocument *doc, QObject *parent) :
    QObject(parent)
{




    document = doc;

    //    connect(doc,SIGNAL(contentsChanged()), this, SLOT(updateCharCount()));

    connect(document, SIGNAL(contentsChanged()), this, SLOT(updateWordCount()));
    connect(document, SIGNAL(cursorPositionChanged(QTextCursor)), this, SLOT(updateCursorPosition(QTextCursor)));


    connect(document,SIGNAL(blockCountChanged(int)), this, SLOT(updateBlockCount(int)));


    //Character Count:


    //    preCharCount = docText.length();

    //    emit charCountSignal(preCharCount);

    this->calculateCounts();
}

//--------------------------------------------------------------------------------------------------------------------------------

void WordCount::calculateCounts()
{

    docText = document->toPlainText();





    //Word Count:



    QStringList wordsList;

    docText.replace("\n", " ");

    wordsList = docText.split(" ", QString::SkipEmptyParts);

    finalWordCount = wordsList.size();

    //   emit wordCountSignal(finalWordCount);




    QStringList blockWordsList;
    QString blockText;

    QTextCursor textCursor(document);
    textCursor.setPosition(0);
    blockNum = textCursor.blockNumber();

    blockText = document->findBlockByNumber(blockNum).text();
    blockWordsList = blockText.split(" ", QString::SkipEmptyParts);

    preWordCount = finalWordCount - blockWordsList.size();

    preBlockNum = blockNum;

    //    QString debug;
    //     qDebug() << "first finalWordCount : " << debug.setNum(finalWordCount,10);
    //    qDebug() << "pre wordCount :" << debug.setNum(preWordCount, 10);
    //    qDebug() << "textCursor position :" << debug.setNum(textCursor.position(), 10);
    //    qDebug() << "constructor blockNumber : " << debug.setNum(blockNum,10);















    //Paragraph Count:

    preBlockCount = document->blockCount();

    //   emit blockCountSignal(preBlockCount);





    this->updateAll();


}

//--------------------------------------------------------------------------------------------------------------------------------


int WordCount::updateAll()
{
    //     emit charCountSignal(preCharCount);

    emit wordCountSignal(finalWordCount);
    emit blockCountSignal(preBlockCount);

    return finalWordCount;
}

//--------------------------------------------------------------------------------------------------------------------------------

//void WordCount::updateCharCount(/*int position,int charsRemoved,int charsAdded*/)
//{


//    QString docText = document->toPlainText();

//    finalCharCount = docText.length();

//    emit charCountSignal(finalCharCount);
//}


//-------------------------------------------------------------------------------------------------------------------------------

void WordCount::updateWordCount()
{

    int preFinalWordCount = finalWordCount;


    QString blockText;




    if(blockNum == preBlockNum){

        blockText = document->findBlockByNumber(blockNum).text();
        blockText.replace("\n", " ");
        wordsList = blockText.split(" ", QString::SkipEmptyParts);

        finalWordCount = preWordCount + wordsList.size();

        emit wordCountSignal(finalWordCount);

    }
    else{



        blockText = document->findBlockByNumber(blockNum).text();
        blockText.replace("\n", " ");
        wordsList = blockText.split(" ", QString::SkipEmptyParts);

        preWordCount = finalWordCount - wordsList.size();;

        finalWordCount = preWordCount + wordsList.size();

        emit wordCountSignal(finalWordCount);


        preBlockNum = blockNum;

    }

    int postFinalWordCount = finalWordCount;

    emit countDeltaSignal(postFinalWordCount - preFinalWordCount);

}

void WordCount::updateCursorPosition(QTextCursor textCursor)
{

    blockNum = textCursor.blockNumber();


    //    QString debug;
    //     qDebug() << "new cursor position : " << debug.setNum(blockNum,10);



}

void WordCount::updateBlockCount(int blockCount)
{



    //new block

    QString docText = document->toPlainText();
    QStringList wordsList;

    docText.replace("\n", " ");
    wordsList = docText.split(" ", QString::SkipEmptyParts);

    finalWordCount = wordsList.size();


    emit wordCountSignal(finalWordCount);


    preBlockNum =  blockNum + 1;


    finalBlockCount = blockCount;
    emit blockCountSignal(blockCount);


}

