#include "dbinterface.h"
#include <iostream>
#include <sqlite3.h>
#include <common.h>
#include <QVariant>
#include <QFile>
#include <globals.h>
#include<QDir>

DBInterface* DBInterface::myInstance = 0;
const char* DBInterface::dbPath = "../../../../DB/DylanDog.db";
DBInterface::DBInterface()
{
    createConnection();
}

bool DBInterface::createConnection()
{
    QFile dbFile(dbPath);
    bool dbFileExist = dbFile.exists();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString::fromStdString(dbPath));

    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return false;
    }
    else{
        if (!dbFileExist)
        {
            CreateAlltables();
        }
        db.close();
    }
    return true;
}


DBInterface* DBInterface::getInstance()
{

    if (!myInstance)
    {
        myInstance = new DBInterface();
    }
    return myInstance;
}

bool DBInterface::addElement2CollectionTable(volume myVol)
{
    db.open();
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return false;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO " + QString::fromStdString(collectionTableName) +
                  " (VolID, Colour, Date, CoverByID, Pages)"
                  "VALUES (:VolID, :Colour, :Date, :CoverByID, :Pages)");
    query.bindValue(":VolID", myVol.getID());
    if (myVol.getColourBy().empty())
    {
        query.bindValue(":Colour", "Black and White");
    }
    else
    {
        query.bindValue(":Date", myVol.getColourBy().c_str());
    }


    //YYYY-MM-DD HH:MM:SS.SSS
    QString dateString = QString::fromStdString(std::to_string(myVol.getYear()) +"/"+
          myVol.getMonthString() + "/01" + " h:00:00:00.000");
    query.bindValue(":Date", dateString);
    for (unsigned int ii=0; ii<myVol.getCoverIllustratorsVector()->size(); ii++){


        query.bindValue(":CoverByID", myVol.getCoverIllustratorsVector()->at(ii).getID());
        if( !query.exec() ){
            qDebug() << query.lastError();
            return false;
        }
        else
        {
            qDebug("Element added into database" );
        }
        db.close();
        }
    return true;
}


bool DBInterface::CreateAlltables ()
{
    bool res = false;
    db.open();
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
        db.lastError().text());
        return false;
    }

    QSqlQuery query;

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + TopicBy::static_getTableName()
               + " (ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
               "Name TEXT NOT NULL DEFAULT 'Unknown', "
               "Surname TEXT NOT NULL DEFAULT 'Unknown', "
               "UNIQUE (Name, Surname))" )))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " +
                                          ScriptBy::static_getTableName()
               + " (ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
               "Name TEXT NOT NULL DEFAULT 'Unknown', "
               "Surname TEXT NOT NULL DEFAULT 'Unknown', "
               "UNIQUE (Name, Surname))" )))
    {
        qDebug() << query.lastError();
        return false;
    }

    if (!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " +
                                          Illustrator::static_getTableName() +
               " (ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
               "Name TEXT NOT NULL DEFAULT 'Unknown', "
               "Surname TEXT NOT NULL DEFAULT 'Unknown', "
               "UNIQUE (Name, Surname))" )))
    {
        qDebug() << query.lastError();
        return false;
    }

    if (!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS "
                                           + CoverIllustrator::static_getTableName() +
               " (ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
               "Name TEXT NOT NULL DEFAULT 'Unknown', Surname TEXT NOT NULL DEFAULT 'Unknown', "
               "UNIQUE (Name, Surname) ) ")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + storyTableName +
              "(StoryID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
               " Title TEXT UNIQUE NOT NULL, "
               "Description TEXT)")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + TopicBy::static_getIDTableName() +
               "(StoryID INTEGER NOT NULL, TopicByID INTEGER NOT NULL, "
               "FOREIGN KEY (StoryID) REFERENCES Story(StoryID), "
               "FOREIGN KEY (TopicByID) REFERENCES TopicBy(ID)"
               "PRIMARY KEY (StoryID, TopicByID), "
               "UNIQUE (StoryID, TopicByID))")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + ScriptBy::static_getIDTableName() +
               " (StoryID INTEGER NOT NULL, ScriptByID INTEGER NOT NULL,"
               "FOREIGN KEY (StoryID) REFERENCES Story(StoryID), "
               "FOREIGN KEY (ScriptByID) REFERENCES ScriptBy(ID)"
               "PRIMARY KEY (StoryID, ScriptByID),"
               "UNIQUE (StoryID, ScriptByID))")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + Illustrator::static_getIDTableName() +
               " (StoryID INTEGER NOT NULL, IllustratorID INTEGER NOT NULL,"
               "FOREIGN KEY (StoryID) REFERENCES Story(StoryID), "
               "FOREIGN KEY (IllustratorID) REFERENCES Illustrator(ID)"
               "PRIMARY KEY (StoryID, IllustratorID),"
               "UNIQUE(StoryID, IllustratorID))")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + collectionTableName +
               " (VolID INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT NOT NULL,"
               "Colour TEXT NOT NULL DEFAULT 'NO', Date TEXT NOT NULL,"
               "CoverByID INTEGER NOT NULL,"
               "Pages INTEGER DEFAULT 98, "
               "FOREIGN KEY (CoverByID) REFERENCES CoverIllustrator(ID))")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + collectionStoriesTableName +
               " (VolID INTEGER NOT NULL, StoryID INTEGER NOT NULL, "
               "FOREIGN KEY (VolID) REFERENCES Collection(VolID),"
               "FOREIGN KEY (StoryID) REFERENCES Story(StoryID)"
               "PRIMARY KEY (VolID, StoryID), "
               "UNIQUE(VolID, StoryID))")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + directorTableName +
               " (ID INTEGER UNIQUE NOT NULL PRIMARY KEY AUTOINCREMENT, "
               "Name TEXT NOT NULL DEFAULT 'Unknown', 'Surname' TEXT NOT NULL DEFAULT 'Unknown' ,"
               "UNIQUE(Name, Surname) )")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + storyReferencesTableName +
               "( StoryID INTEGER NOT NULL, Reference INTEGER NOT NULL, " //
               "FOREIGN KEY (StoryID) REFERENCES Story(StoryID), "
               "FOREIGN KEY (Reference) REFERENCES Story(StoryID), "
               "UNIQUE(StoryID, Reference), "
               "PRIMARY KEY (StoryID, Reference) )")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + volDependenciesTableName +
               " (VolID INTEGER NOT NULL, "
               "Dependency	INTEGER NOT NULL, "
               " FOREIGN KEY (VolID) REFERENCES Collection(VolID), "
               " FOREIGN KEY (Dependency) REFERENCES Collection(VolID), "
               "PRIMARY KEY (VolID, Dependency),"
               "UNIQUE(VolID, Dependency))")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + grouchoJokesTableName +
               " (ID INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT,"
               "Joke TEXT NOT NULL,"
               "VolID INTEGER NOT NULL, "
               "Page INTEGER NOT NULL, "
               "FOREIGN KEY (VolID) REFERENCES Collection(VolID) "
               ")")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + charactersTableName +
               "(ID INTEGER UNIQUE PRIMARY KEY NOT NULL AUTOINCREMENT,"
               "Name TEXT NOT NULL,"
               "Surname TEXT)")))
    {
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS " + volCharacterTableName +
               "(CharacterID INTEGER NOT NULL,"
               "VolID INTEGER NOT NULL,"
               "FOREIGN KEY (CharacterID) REFERENCES Characters(ID),"
               "FOREIGN KEY (VolID) REFERENCES Collection(VolID),"
               "PRIMARY KEY (CharacterID, VolID),"
               "UNIQUE (CharacterID, VolID))")))
    {
        qDebug() << query.lastError();
        return false;
    }
    res = true;
    return res;

}

bool DBInterface::addVolData2DB(volume* myVol)
{
    myInstance->addgenericVector2Table(*(myVol->getCoverIllustratorsVector()));
    myInstance->addElement2CollectionTable(*myVol);

    for (unsigned char ii=0; ii<myVol->getStoriesVector()->size(); ii++)
    {
        myInstance->addgenericVector2Table
                (*(myVol->getStoriesVector()->at(ii).getIllustratorsVector()));
        myInstance->addgenericVector2Table
                (*(myVol->getStoriesVector()->at(ii).getScriptByVector()));
        myInstance->addgenericVector2Table
                (*(myVol->getStoriesVector()->at(ii).getTopicByVector()));
    }

    for (unsigned char ii=0; ii<myVol->getStoriesVector()->size(); ii++)
    {
        addStory2StoryTable(myVol->getStoriesVector()->at(ii));
    }
    myInstance->addVolStory2DB(*myVol);
    return true;
}

bool DBInterface::addStory2StoryTable(SingleStory &myStory)
{
    QSqlQuery query;
    db.open();
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return false;
    }

    query.prepare("INSERT INTO " + QString::fromStdString(storyTableName) +
                  " (Title, Description)"
                  "VALUES (:Title, :Description)");

    query.bindValue(":Title", myStory.getTitle().c_str());
    query.bindValue(":Description", "None");
    if( !query.exec() ){
        qDebug() << query.lastError();
        return false;
    }
    else
    {
        qDebug("Element added into database" );
    }
    myStory.setID(query.lastInsertId().toUInt());
    myInstance->addgenericVector2RelationTable(*(myStory.getIllustratorsVector()), myStory.getID());
    myInstance->addgenericVector2RelationTable(*(myStory.getScriptByVector()), myStory.getID());
    myInstance->addgenericVector2RelationTable(*(myStory.getTopicByVector()), myStory.getID());

    db.close();
    return true;

}

bool DBInterface::addVolStory2DB(volume myVol)
{
    db.open();
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return false;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO " + QString::fromStdString(collectionStoriesTableName) +
                  " (VolID, StoryID)"
                  "VALUES (:VolID, :StoryID)");

    query.bindValue(":VolID", myVol.getID());
    for (unsigned char ii=0; ii< myVol.getStoriesVector()->size(); ii++)
    {
        query.bindValue(":StoryID", myVol.getStoriesVector()->at(ii).getID());
        if( !query.exec() ){
            qDebug() << query.lastError();
            db.close();
            return false;
        }
        else
        {
            qDebug("Element added into database" );
        }
    }
    db.close();
    return true;

}
