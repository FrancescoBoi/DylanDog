#include "persondb.h"

//const std::string personDB::tableName = "No name";
/*
personDB::personDB():person()
{

}

personDB::personDB(QString dbName, QString dbSurname):person(dbName, dbSurname)
{
    ;
}
*/

personDB::personDB(QString dbName, QString dbSurname, unsigned int dbid):person(dbName, dbSurname){

    id = dbid;
}

personDB::personDB(std::string dbName, std::string dbSurname,
                   unsigned int dbid):person(dbName, dbSurname){

    id = dbid;
}


//std::string personDB::getTableName(void){
 //   return "PersonDBTableNameNotExisting";
//}

unsigned int personDB::getID(void){
    return id;
}

void personDB::setID(unsigned int myID)
{
    id = myID;
}
