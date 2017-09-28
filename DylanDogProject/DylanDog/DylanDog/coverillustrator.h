#ifndef COVERILLUSTRATOR_H
#define COVERILLUSTRATOR_H
//#include<QString>
#include<persondb.h>

class CoverIllustrator : public PersonDBX<CoverIllustrator>, public virtual personDB
{
    using personDB::personDB;
public:
    CoverIllustrator();
    CoverIllustrator(QString dbName, QString dbSurname);
    CoverIllustrator(QString dbName, QString dbSurname, unsigned int dbid);
    //std::string getTableName();
private:
    //static const std::string tableName;
};

#endif // COVERILLUSTRATOR_H
