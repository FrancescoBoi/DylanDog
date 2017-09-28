#ifndef ILLUSTRATOR_H
#define ILLUSTRATOR_H

#include "persondb.h"

class Illustrator:public PersonDBX<Illustrator>
{
public:
    using PersonDBX::PersonDBX;
    //Illustrator();
    //Illustrator(QString dbName, QString dbSurname);
    Illustrator(QString dbName, QString dbSurname, unsigned int dbid);
    //std::string getTableName();
protected:
    //static const std::string tableName;

};

#endif // ILLUSTRATOR_H
