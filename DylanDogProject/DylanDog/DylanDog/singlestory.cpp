#include "singlestory.h"
#include <vector>

const std::string SingleStory::tableName = "Story";
SingleStory::SingleStory()
{
    myTopicBy = new std::vector<TopicBy>();
    myScriptBy = new std::vector<ScriptBy>();
    myIllustrator = new std::vector<Illustrator>();
}


void SingleStory::clear()
{
    myTopicBy->clear();
    myScriptBy->clear();
    myIllustrator->clear();
    title = "";
    description = "";
}


void SingleStory::setTitle(std::string myTitle){
    title = myTitle;
}

std::string SingleStory::getTitle(){
    return title;
}


void SingleStory::setNameGenericEl(std::string authName, std::string authSurname, personDB* genericEl)
{
    if (genericEl != NULL)
    {
        genericEl->setName(QString::fromStdString(authName));
        genericEl->setSurname( QString::fromStdString(authSurname));
    }
}

void SingleStory::setIllustratorName(std::vector<std::string> *authName, std::vector<std::string> *authSurname)
{
 //add a test to check name and surname have the same length
 //Illustrator tempIllustrator;
 for (int ii=0; ii<<static_cast<int>(authName->size()); ii++)
 {
     Illustrator *tempIllustrator = new Illustrator(QString::fromStdString(authName->at(ii)),
                                                    QString::fromStdString(authSurname->at(ii)));
     //myIllustrator->push_back(new Illustrator(QString::fromStdString(authName.at(ii)), QString::fromStdString(authSurname.at(ii))));
     myIllustrator->push_back(*tempIllustrator);
 }
}

void SingleStory::setScript(std::vector<std::string> *scriptName, std::vector<std::string> *scriptSurname)
{
 //check name and surname have the same length
 for (int ii=0; ii<static_cast<int>(scriptName->size()); ii++)
 {
     ScriptBy *tempScript = new ScriptBy(QString::fromStdString(scriptName->at(ii)),
                                     QString::fromStdString(scriptSurname->at(ii)));
     myScriptBy->push_back(*tempScript);
 }
}

void SingleStory::setDescription(std::string Description)
{
    description = Description;
}

std::vector<ScriptBy>* SingleStory::getScriptByVector()
{
    return myScriptBy;
}

std::vector<TopicBy>* SingleStory::getTopicByVector()
{
    return myTopicBy;
}

std::vector<Illustrator>* SingleStory::getIllustratorsVector()
{
    return myIllustrator;
}

void SingleStory::printData()
{
    std::cout<<"Title: "<<title<<std::endl;
    //unsigned int temp = myTopicBy->size();
    std::string line = "Topic by: ";
    for (unsigned int ii = 0; ii< myTopicBy->size(); ii++)
    {
        line += myTopicBy->at(ii).getName().toStdString() + " ";
        line += myTopicBy->at(ii).getSurname().toStdString() + ", ";
    }
    std::cout<<line.substr(0,line.length()-2)<<std::endl;
    line = "Script by: ";
    for (unsigned int ii = 0; ii< myScriptBy->size(); ii++)
    {
        line += myScriptBy->at(ii).getName().toStdString() + " ";
        line += myScriptBy->at(ii).getSurname().toStdString() + ", ";
    }
    std::cout<<line.substr(0,line.length()-2)<<std::endl;
    line = "Illustrations by: ";
    for (unsigned int ii = 0; ii< myIllustrator->size(); ii++)
    {
        line += myIllustrator->at(ii).getName().toStdString() + " ";
        line += myIllustrator->at(ii).getSurname().toStdString() + ", ";
    }
    std::cout<<line.substr(0,line.length()-2)<<std::endl;

}

QString SingleStory::getTopicName()
{
    return myTopicBy->at(0).getName();
}

QString SingleStory::getTopicSurname()
{
    return myTopicBy->at(0).getSurname();
}





