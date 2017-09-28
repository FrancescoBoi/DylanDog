#include "scriptby.h"
#include <iostream>
#include <globals.h>

template<> const std::string PersonDBX<ScriptBy>::tableName = "ScriptBy";
template<> const personRelationTable PersonDBX<ScriptBy>::personIDtable = {"Story_ScriptBy","StoryID", "ScriptByID"};
//template<> const std::string PersonDBX<ScriptBy>::tableIDs = "Story_ScriptBy";
//template<> const std::string PersonDBX<ScriptBy>::col1TableID = "StoryID";
//template<> const std::string PersonDBX<ScriptBy>::col2TableID = "ScriptByID";

/*
ScriptBy::ScriptBy():PersonDBX()
{
    ;
}

ScriptBy::ScriptBy(QString dbName, QString dbSurname):PersonDBX(dbName, dbSurname)
{
    ;
}
*/

ScriptBy::ScriptBy(QString dbName, QString dbSurname, unsigned int dbid):PersonDBX(dbName, dbSurname)
{
    id = dbid;
}


/*
std::string ScriptBy::getTableName(){
    return tableName;
}
*/
