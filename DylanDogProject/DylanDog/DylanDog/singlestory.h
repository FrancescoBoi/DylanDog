#ifndef SINGLESTORY_H
#define SINGLESTORY_H
#include <iostream>
#include "topicby.h"
#include "scriptby.h"
#include "illustrator.h"
#include "coverillustrator.h"
#include "genericdbitem.h"

class SingleStory : public genericDBItem
{
public:
    SingleStory();
    void setTitle(std::string myTitle);
    void setNameGenericEl(std::string authName, std::string authSurname, personDB* genericEl);
    void setIllustratorName(std::vector<std::string> *authName, std::vector<std::string> *authSurname);
    void setScript(std::vector<std::string> *coverName, std::vector<std::string> *coverSurname);
    void setDescription(std::string Description);
    //GETTER
    QString getTopicName();
    QString getTopicSurname();
    std::string getTitle();
    std::vector<ScriptBy>* getScriptByVector();
    std::vector<TopicBy>* getTopicByVector();
    std::vector<Illustrator>* getIllustratorsVector();
    void printData();
    void clear();
private:
    std::vector<TopicBy> *myTopicBy;
    std::vector<ScriptBy> *myScriptBy;
    std::vector<Illustrator>* myIllustrator;
    std::string title;
    std::string description;
    static const std::string tableName;
};

#endif // SINGLESTORY_H
