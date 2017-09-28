#ifndef VOLUME_H
#define VOLUME_H
#include <vector>
#include <common.h>
#include<singlestory.h>

class volume : public genericDBItem
{
public:
    volume();
    void printData();

    unsigned int getYear();
    //char getMonth();
    std::string getMonthString();
    en_month getMonthEnum();
    void setDate(date p_date);
    std::string getColourBy();
    void setColourBy(std::string mycol);
    bool isColour();
    std::vector<CoverIllustrator>* getCoverIllustratorsVector();
    void setCoverIllustratorName(std::vector<std::string> *coverName, std::vector<std::string> *coverSurname);
    std::vector<SingleStory>* getStoriesVector();
private:
    std::vector<SingleStory>* myStories;
    std::vector<CoverIllustrator>*myCoverIllustrator;
    std::string colour;
    date myDate;

};

#endif // VOLUME_H
