#ifndef SCRIPT_H
#define SCRIPT_H

#include "persondb.h"

class ScriptBy : public PersonDBX<ScriptBy>
{
public:
    using PersonDBX::PersonDBX;
    //ScriptBy();
    //ScriptBy(QString dbName, QString dbSurname);
    ScriptBy(QString dbName, QString dbSurname, unsigned int dbid);
    //std::string getTableName();
protected:
    //static const std::string tableName;
};

#endif // SCRIPT_H
