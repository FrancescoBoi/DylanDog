#ifndef PERSONDB_H
#define PERSONDB_H

#include "person.h"
#include"common.h"
class personDB:public person
{
public:
    using person::person;
    //personDB();
    //personDB(QString dbName, QString dbSurname);
    personDB(QString dbName, QString dbSurname, unsigned int dbid);
    personDB(std::string dbName, std::string dbSurname, unsigned int dbid);
    //virtual std::string getTableName() = 0;
    unsigned int getID(void);
    void setID(unsigned int myID);
protected:
    unsigned int id;


};
/* Curiously recurring template paradigm */
template<class Derived>
class PersonDBX: public virtual personDB
{
    using personDB::personDB;
protected:
    static const std::string tableName;
    static const personRelationTable personIDtable;
    //static const std::string tableIDs;
    //static const std::string col1TableID;
    //static const std::string col2TableID;
public:
    static std::string static_getTableName(){ return tableName; }
    std::string getTableName(){return tableName;}
    static std::string static_getIDTableName(){ return personIDtable.tableName; }
    std::string getIDTableName(){return personIDtable.tableName;}

    static std::string static_getCol1TableID(){ return personIDtable.col1; }
    std::string getCol1TableID(){return personIDtable.col1;}

    static std::string static_getCol2TableID(){ return personIDtable.col2; }
    std::string getCol2TableID(){return personIDtable.col2;}



    PersonDBX(QString dbName, QString dbSurname, unsigned int dbid):personDB(dbName, dbSurname)
    {
        id = dbid;
    }
    PersonDBX():personDB(){;}

    PersonDBX(QString dbName, QString dbSurname):personDB(dbName, dbSurname){;}
};
template<class Derived> const std::string PersonDBX<Derived>::tableName = "Null";
template<class Derived> const personRelationTable PersonDBX<Derived>::personIDtable = {"Null", "Null", "Null"};

//template<class Derived> const std::string PersonDBX<Derived>::personIDtable.col1= "Null";
//template<class Derived> const std::string PersonDBX<Derived>::personIDtable.co= "Null";
#endif // PERSONDB_H
