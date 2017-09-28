#ifndef DBINTERFACE_H
#define DBINTERFACE_H
#include <QString>
#include <sqlite3.h>
#include <common.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlError>
#include <QtDebug>
#include <persondb.h>
#include <volume.h>

class DBInterface
{
    sqlite3* DBhandler;
    static const char* dbPath;
public:
    //returns the pointer of this singletone class
    static DBInterface* getInstance();
    bool addElement2CollectionTable(volume myVol);
    bool addStory2StoryTable(SingleStory &myStory);
    bool addVolStory2DB(volume myVol);
    template<typename abstractPerson>
    bool addgenericPersonDB2Table(abstractPerson& myPerson);

    template<typename abstractPerson>
    std::vector<personDB> getGenericPerson(abstractPerson *myPerson);

    template<typename abstractPerson>
    std::vector<abstractPerson> getGenericPersonBySurnameOnly(abstractPerson *myPerson);

    template<typename abstractPerson>
    bool addgenericVector2Table(std::vector<abstractPerson> &vectorPerson);

    template<typename abstractPerson>
    bool addgenericPersonDB2RelationTable(abstractPerson &myPerson, unsigned int elementID);

    template<typename abstractPerson>
    bool addgenericVector2RelationTable(std::vector<abstractPerson> &vectorPerson, unsigned int elementID);

    bool CreateAlltables ();
    bool addVolData2DB(volume* myVol);


protected:
    DBInterface();
    DBInterface(const DBInterface&);
    DBInterface& operator= (const DBInterface&);
    bool createConnection();
    static DBInterface* myInstance;
    QSqlDatabase db;

};

template<typename abstractPerson>
bool DBInterface::addgenericPersonDB2Table(abstractPerson &myPerson)
{
    std::string tableName = myPerson.getTableName();
    qDebug() << QString::fromStdString("Writing into " + tableName);
    db.open();
    bool res = false;
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return false;
    }
    QSqlQuery query;
    QString mystr = ("INSERT INTO " + QString::fromStdString(tableName) + " (Name, Surname)"
                "VALUES (:Name, :Surname)");
    query.prepare("INSERT INTO " + QString::fromStdString(tableName) + " (Name, Surname)"
                "VALUES (:Name, :Surname)");
    query.bindValue(":Name", myPerson.getName());
    query.bindValue(":Surname", myPerson.getSurname());
    if( !query.exec() ){
        qDebug() << query.lastError();
    }
    else
    {
        qDebug("Element added into database" );
    }
    myPerson.setID(myInstance->getGenericPerson(&myPerson).at(0).getID());
    db.close();
    res = true;
    return res;
}

template<typename abstractPerson>
bool DBInterface::addgenericVector2Table(std::vector<abstractPerson> &vectorPerson)
{
    for (unsigned int ii=0; ii<vectorPerson.size(); ii++)
    {
        myInstance->addgenericPersonDB2Table(vectorPerson.at(ii));
    }
    return false;
}


//the function takes a pointer of DBPerson containg
// in which table to be search and overwrite the pointer.
//Returns true if at least one person was
template<typename abstractPerson>
std::vector<personDB> DBInterface::getGenericPerson(abstractPerson *myPerson)
{
    std::vector<personDB> res;
    db.open();
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return res;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM "
                  "" + QString::fromStdString(myPerson->getTableName() )
                  + " WHERE Surname = :Surname AND Name = :Name");
    query.bindValue(":Name", myPerson->getName());
    query.bindValue(":Surname", myPerson->getSurname());
    query.exec();
    int fieldName = query.record().indexOf("Name");
    int fieldSurname = query.record().indexOf("Surname");


    int fieldID = query.record().indexOf("ID");
    while (query.next())
    {
        std::string temp_name = query.value(fieldName).toString().toStdString();
        std::string temp_surname = query.value(fieldSurname).toString().toStdString();
        unsigned int temp_ID = query.value(fieldID).toUInt();
        res.push_back(personDB(temp_name, temp_surname, temp_ID));

    }
    return res;
}



template<typename abstractPerson>
std::vector<abstractPerson> DBInterface::getGenericPersonBySurnameOnly(abstractPerson *myPerson)
{
    std::vector<abstractPerson> res;
    db.open();
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return res;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM "
                  "" + QString::fromStdString(myPerson->getTableName() )
                  + " WHERE Surname = :Surname");
    query.bindValue(":Surname", myPerson->getSurname());
    query.exec();
    int fieldName = query.record().indexOf("Name");
    int fieldSurname = query.record().indexOf("Surname");


    int fieldID = query.record().indexOf("ID");
    while (query.next())
    {
        std::string temp_name = query.value(fieldName).toString().toStdString();
        std::string temp_surname = query.value(fieldSurname).toString().toStdString();
        unsigned int temp_ID = query.value(fieldID).toUInt();
        res.push_back(abstractPerson(temp_name, temp_surname, temp_ID));

    }
    return res;
}


template<typename abstractPerson>
bool DBInterface::addgenericPersonDB2RelationTable(abstractPerson &myPerson, unsigned int elementID)
{
    std::string tableName = myPerson.getIDTableName();
    qDebug() << QString::fromStdString("Writing into " + tableName);
    db.open();
    bool res = false;
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
        return false;
    }
    QSqlQuery query;
    QString mystr = ("INSERT INTO " + QString::fromStdString(tableName) + " (Name, Surname)"
                "VALUES (:Name, :Surname)");
    query.prepare(QString::fromStdString("INSERT INTO " + tableName + " ("+ myPerson.getCol1TableID()
                  + ", " + myPerson.getCol2TableID()+ ")"
                "VALUES (:Val1, :Val2)"));
    query.bindValue(":Val1", elementID);
    query.bindValue(":Val2", myPerson.getID());
    if( !query.exec() ){
        qDebug() << query.lastError();
    }
    else
    {
        qDebug("Element added into database" );
    }
    myPerson.setID(myInstance->getGenericPerson(&myPerson).at(0).getID());
    db.close();
    res = true;
    return res;
}

template<typename abstractPerson>
bool DBInterface::addgenericVector2RelationTable(std::vector<abstractPerson> &vectorPerson, unsigned int elementID)
{
    bool res = true;
    for (unsigned int ii=0; ii<vectorPerson.size(); ii++)
    {
         res = res & (myInstance->addgenericPersonDB2RelationTable(vectorPerson.at(ii), elementID));
    }
    return res;
}
#endif // DBINTERFACE_H
