#include "volume.h"
#include <iostream>

volume::volume()
{
    myCoverIllustrator = new std::vector<CoverIllustrator>();
    colour = "false";
    myStories = new std::vector<SingleStory>();
}

void volume::setDate(date p_date)
{
    this->myDate = p_date;
}

void volume::printData()
{
    std::string line;
    unsigned int ii;
    std::cout<<"--------------------------------------------"<<std::endl;
    std::cout<<"Volume ID: " << this->getID()<<std::endl;
    std::cout<< "Date: " << this->getMonthString()<< " "<< this->getYear() << std::endl;
    for (ii=0; ii<myStories->size(); ii++)
    {
        myStories->at(ii).printData();
    }
    line = "Cover illustrations by: ";
    for (unsigned int ii = 0; ii< myCoverIllustrator->size(); ii++)
    {
        line += myCoverIllustrator->at(ii).getName().toStdString() + " ";
        line += myCoverIllustrator->at(ii).getSurname().toStdString() + ", ";
    }
    std::cout<<line.substr(0,line.length()-2)<<std::endl;
    if (!(line = getColourBy()).empty())
    {
        std::cout<<"Colours by: " << line<<std::endl;
    }
}

//Returns the year of the volume
unsigned int volume::getYear()
{
    //unsigned int yy = myDate.year;
    return myDate.year;
}

std::string volume::getMonthString()
{
    //en_month m = myDate.mymonth;
    return convertMonth2itaString(myDate.mymonth);
}

en_month volume::getMonthEnum()
{
    return myDate.mymonth;
}

std::string volume::getColourBy()
{
    return colour;
}

void volume::setColourBy(std::string mycol)
{
    colour = mycol;
}

bool volume::isColour()
{
    return (!getColourBy().empty());
}

std::vector<CoverIllustrator>* volume::getCoverIllustratorsVector()
{
    return myCoverIllustrator;
}

void volume::setCoverIllustratorName(std::vector<std::string> *coverName,
                                     std::vector<std::string> *coverSurname)

{
 //check name and surname have the same length
 for (int ii=0; ii<<static_cast<int>(coverName->size()); ii++)
 {
     CoverIllustrator* tempCoverIllustrator = new CoverIllustrator(
                 QString::fromStdString(coverName->at(static_cast<unsigned long>(ii))),
                 QString::fromStdString(coverSurname->at(static_cast<unsigned long>(ii))));
     myCoverIllustrator->push_back(*tempCoverIllustrator);
 }
}

std::vector<SingleStory>* volume::getStoriesVector()
{
    return myStories;
}
