#include "coverillustrator.h"
#include "globals.h"

template<> const std::string PersonDBX<CoverIllustrator>::tableName = "CoverIllustrator";
template<> const personRelationTable PersonDBX<CoverIllustrator>::personIDtable = {"Null", "Null", "Null"};


CoverIllustrator::CoverIllustrator():PersonDBX()
{
    ;
}

CoverIllustrator::CoverIllustrator(QString dbName, QString dbSurname):PersonDBX(dbName, dbSurname)
{
    ;
}


CoverIllustrator::CoverIllustrator(QString dbName, QString dbSurname, unsigned int dbid):PersonDBX(dbName, dbSurname)
{
    id = dbid;
}

/*
std::string CoverIllustrator::getTableName(){
    return tableName;
}
*/
