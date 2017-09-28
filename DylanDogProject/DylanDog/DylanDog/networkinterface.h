#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H
#include "volume.h"
#include <vector>
#include "common.h"
#include<sm_readline.h>
class NetworkInterface
{

public:
    //returns the pointer of this singletone class
    static NetworkInterface* getInstance();
    void test();
    void parseHTMLFile(volume* myvol);
    std::string checkColour(std::string mystr);

protected:
    NetworkInterface();
    NetworkInterface(const NetworkInterface&);
    NetworkInterface& operator= (const NetworkInterface&);
    bool createConnection();
    static NetworkInterface* myInstance;
    static const char* volPath;
    std::vector<std::string> parseTitle(std::string title,
                const std::string* prestring, const std::string* poststring);
    bool parseName(std::string myName, std::vector<std::string> *name, std::vector<std::string> *surname );
    std::string parseSummary(std::string myString);
    unsigned int parseVolId(std::string myString);
    std::vector<std::string> parseString(std::string mystr,
                const std::string* prestring, const std::string* poststring);
    unsigned int parseYear(std::string mystr, const std::string* prestring, const std::string* poststring);
    template<typename abstractPersonArray>
    void BuildVectorOfPeople(std::vector<std::string> mystr, std::vector<abstractPersonArray> *p_arraydb);
    unsigned char calcTotTitles(std::string);

};


template<typename abstractPersonArray>

/* This function checks how many persons are found for a role: for example how many authors and so on
 * */
void NetworkInterface::BuildVectorOfPeople(std::vector<std::string> names, std::vector<abstractPersonArray> *p_arraydb)
{
        std::vector<std::string> tmpName;
        std::vector<std::string> tmpSurname;

        for (unsigned int ii = 0; ii<names.size(); ii++)
        {
            NetworkInterface::parseName(leftRightTrim(names.at(ii)), &tmpName, &tmpSurname);

            abstractPersonArray *tempPerson = new abstractPersonArray(
                        QString::fromStdString(leftRightTrim(tmpName.at(ii))),
                        QString::fromStdString(leftRightTrim(tmpSurname.at(ii))));
            p_arraydb->push_back(*tempPerson);
        }
}

#endif // NETWORKINTERFACE_H
