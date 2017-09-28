#include "topicby.h"
#include <iostream>
#include <globals.h>

template<> const std::string PersonDBX<TopicBy>::tableName = "TopicBy";
template<> const personRelationTable PersonDBX<TopicBy>::personIDtable = {"Story_TopicBy", "StoryID", "TopicByID"};
//template<> const std::string PersonDBX<TopicBy>::tableIDs = "Story_TopicBy";
//template<> const std::string PersonDBX<TopicBy>::col1TableID = "StoryID";
//template<> const std::string PersonDBX<TopicBy>::col2TableID = "TopicByID";

/*
TopicBy::TopicBy():PersonDBX()
{
    ;
}

TopicBy::TopicBy(QString dbName, QString dbSurname):PersonDBX(dbName, dbSurname)
{
    ;
}
*/


TopicBy::TopicBy(QString dbName, QString dbSurname, unsigned int dbid):PersonDBX(dbName, dbSurname)
{
    id = dbid;
}

/*std::string TopicBy::getTableName(){
    return tableName;
}*/

