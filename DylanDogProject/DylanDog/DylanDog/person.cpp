#include "person.h"
#include "common.h"

person::person()
{
    name = QString::fromStdString(unknown);
    surname = QString::fromStdString(unknown);
}


person::person(QString myName, QString mySurname)
{
    name = myName;
    surname = mySurname;
}

person::person(std::string myName, std::string mySurname)
{
    name = QString::fromStdString(myName);
    surname = QString::fromStdString(mySurname);
}



QString person::getName(){
    return name;

}

QString person::getSurname()
{
    return surname;
}


void person::setName(QString myName){
    this->name = myName;
}

void person::setSurname(QString mySurname){
    surname = mySurname;
}
