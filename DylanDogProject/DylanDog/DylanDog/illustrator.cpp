#include "illustrator.h"
#include "globals.h"

template<> const std::string PersonDBX<Illustrator>::tableName = "Illustrator";
template<> const personRelationTable PersonDBX<Illustrator>::personIDtable = {"Story_Illustrator", "StoryID", "IllustratorID"};
//template<> const std::string PersonDBX<Illustrator>::tableIDs = "Story_Illustrator";
//template<> const std::string PersonDBX<Illustrator>::col1TableID = "StoryID";
//template<> const std::string PersonDBX<Illustrator>::col2TableID = "IllustratorID";

/*
Illustrator::Illustrator():PersonDBX()
{
    ;
}

Illustrator::Illustrator(QString dbName, QString dbSurname):PersonDBX(dbName, dbSurname)
{
    ;
}
*/

Illustrator::Illustrator(QString dbName, QString dbSurname, unsigned int dbid):PersonDBX(dbName, dbSurname)
{
    id = dbid;
}
/*
std::string Illustrator::getTableName(){
    return tableName;
}*/
