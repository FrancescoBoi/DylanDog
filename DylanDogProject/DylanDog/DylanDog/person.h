#ifndef PERSON_H
#define PERSON_H

#include <QString>

class person
{
public:
    QString name;
    QString surname;
    person();
    person(QString myName, QString mySurname);
    person(std::string myName, std::string mySurname);
    QString getName();
    QString getSurname();
    void setName(QString myName);
    void setSurname(QString mySurname);
};

#endif // PERSON_H
