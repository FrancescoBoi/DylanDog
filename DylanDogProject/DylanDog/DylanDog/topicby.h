#ifndef TOPIC_H
#define TOPIC_H

#include "persondb.h"

class TopicBy : public PersonDBX<TopicBy>
{
public:
    using PersonDBX::PersonDBX;
    //TopicBy();
    //TopicBy(QString dbName, QString dbSurname);
    TopicBy(QString dbName, QString dbSurname, unsigned int dbid);
    //std::string getTableName();
protected:
    //static const std::string tableName;
};

#endif // TOPIC_H
