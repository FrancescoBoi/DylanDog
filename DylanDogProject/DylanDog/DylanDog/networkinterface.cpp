#include "networkinterface.h"

#include "dbinterface.h"
#include <iostream>
#include <sqlite3.h>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <common.h>
#include <QVariant>
#include <QtDebug>
#include <curl/curl.h>
#include <stdio.h>
#include <exception>
#include <regex>
#include <regex.h>
#include <fstream>
#include <QDebug>
#include <QString>
#include <sstream>
#include <QFile>
#include <singlestory.h>

NetworkInterface* NetworkInterface::myInstance = 0;
const char* NetworkInterface::volPath = "../DylanDogFile.html";

enum sm_readline_status {idle, readTitleYear, readMonth, readTitleOnly, readTopicBy,
                          readScriptBy, readIllustrator, readCoverillustrator, read_end};

NetworkInterface::NetworkInterface()
{
    createConnection();
    }

bool NetworkInterface::createConnection()
{
    return false;
}


NetworkInterface* NetworkInterface::getInstance()
{

    if (!myInstance)
    {
        myInstance = new NetworkInterface();
    }
    return myInstance;
}


void NetworkInterface::test ()
{
  //declarations
  CURL *curl;
  CURLcode res; //non cancellare
  FILE* fp;
  //end of declarations

  curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt (curl, CURLOPT_URL, "https://it.wikipedia.org/wiki/Albi_di_Dylan_Dog");
    try
    {
        fp = fopen(volPath, "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform (curl); //non commentare MAI
        curl_easy_cleanup (curl);
        fclose(fp);
    }
    catch(std::exception& e)
    {
        std::cout<<"Problem with DylanDogF"
                   "ile.html file: file not found";
    }

  }

}

void NetworkInterface::parseHTMLFile(volume* myvol){
    //string identifying the html element
    static const std::string preYear = " (";
    static const std::string postYear = ")";
    static const std::string preMonth = "<td>";
    static const std::string postMonth = "</td>";
    static const std::string titleData = "<h1 itemprop=\"headline name\" class=\"titolo_articolo titolo\">";
    static const std::string preName = ">";
    static const std::string postName = "</a";
    static const std::string preIllustratorData = "Disegni.*: <.*>";
    static const std::string preCoverIllData = "Copertina.*: <.*>";
    static const std::string preTitle = "\">";
    static const std::string postTitle = "</a>";
    static const std::string preTitle2 = "i>";
    static const std::string postTitle2 = "</i";
    std::vector<std::string> completeNames;
    std::string myLine;
    QFile myfile(volPath);
    static unsigned int currIterator = 1;
    std::string month;
    //date myDate;
    SingleStory tmpStory;
    sm_readline_status myStatus = idle;
    unsigned char totTitles = 0;

    if (myfile.open(QFile::ReadOnly))
    {
        QTextStream myStream(&myfile);
        myStream.setCodec("UTF-8");
        while (!myStream.atEnd() && myStatus != read_end)
        {
            myLine = myStream.readLine().toStdString();
            if (myLine.find(">" + std::to_string(myvol->getID()) + "</td>")!= std::string::npos)
            {
                totTitles = calcTotTitles(myLine);
                myStatus = readTitleYear;
                currIterator = 1;
                continue;
            }
            if (currIterator <= totTitles  && myLine.find("</tr>")==std::string::npos)
            {
                switch (myStatus)
                {
                case idle:
                    break;
                case read_end:
                    break;
                    case SM_readline::readTitleYear:
                    {
                        // we expect the title to be the first element
                        tmpStory.setTitle(NetworkInterface::parseTitle(myLine, &preTitle, &postTitle).at(0));
                        //myDate.year = parseYear(myLine, &preYear , &postYear);
                        myStatus = readMonth;
                        break;
                    }
                case SM_readline::readTitleOnly:
                {
                    //there are 2 lines before reading the title:
                    // </tr> which is filtered by the if before switch
                    // <tr> which is not filtered
                    if (myLine.find("<tr>") != std::string::npos)
                    {
                        continue;
                    }
                    tmpStory.setTitle(NetworkInterface::parseTitle(myLine,
                            &preTitle2, &postTitle2).at(0));
                    myStatus = readTopicBy;
                    break;
                }
                case SM_readline::readMonth:
                    {
                        //expect the month to be the only element
                        //myDate.mymonth = convertString2month(NetworkInterface::parseString(myLine,
                        //                        &preMonth, &postMonth).at(0));
                        //myvol->setDate(myDate);
                        myStatus = readTopicBy;
                        break;
                    }
                case SM_readline::readTopicBy:
                    {
                    completeNames = NetworkInterface::parseString(myLine, &preName, &postName);
                    NetworkInterface::BuildVectorOfPeople(completeNames, tmpStory.getTopicByVector());
                    myStatus = readScriptBy;
                    break;
                    }
                case readScriptBy:
                    {
                    completeNames = NetworkInterface::parseString(myLine, &preName, &postName);
                    NetworkInterface::BuildVectorOfPeople(completeNames, tmpStory.getScriptByVector());
                    myStatus = readIllustrator;
                    break;
                }
                case SM_readline::readIllustrator:
                {
                    completeNames = NetworkInterface::parseString(myLine, &preName, &postName);
                    NetworkInterface::BuildVectorOfPeople(completeNames,tmpStory.getIllustratorsVector());
                    if (currIterator == 1)
                    {
                        myvol->setColourBy(checkColour(myLine));
                        myStatus = readCoverillustrator;
                    }
                    else
                    {
                        myvol->getStoriesVector()->push_back(tmpStory);
                        tmpStory = SingleStory();
                        if (currIterator < totTitles)
                        {
                            myStatus = readTitleOnly;
                        }
                        else
                        {
                            myStatus = read_end;
                            currIterator++;
                        }
                    }
                    break;
                }
                case SM_readline::readCoverillustrator:
                {
                    //completeNames = NetworkInterface::parseString(myLine, &preName, &postName);
                    //Assume for the cover only one illustrator is employed

                    std::vector<CoverIllustrator> tmpVect;
                    NetworkInterface::BuildVectorOfPeople(completeNames, &tmpVect);
                    if (tmpVect.size()>1)
                    {
                        for (unsigned int ii =0; ii<tmpVect.size(); ii++)
                        {
                            //This was implemented because in 362 vol there was not
                            //only the name but other stuff too.
                            //search in the DB for this surname
                            // if not already existing do not consider it
                            if (DBInterface::getInstance()->getGenericPerson(&(tmpVect.at(ii))).size()>0)
                            {
                                myvol->getCoverIllustratorsVector()->push_back(tmpVect.at(ii));
                                continue;
                            }
                            if (DBInterface::getInstance()->getGenericPersonBySurnameOnly(&(tmpVect.at(ii))).size()>0)
                            {
                                myvol->getCoverIllustratorsVector()->push_back(
                                            DBInterface::getInstance()->getGenericPersonBySurnameOnly(&(tmpVect.at(ii))).at(0));
                                continue;
                            }

                        }
                    }
                    else
                    {
                        myvol->getCoverIllustratorsVector()->push_back(tmpVect.at(0));
                    }
                    myvol->getStoriesVector()->push_back(tmpStory);
                    tmpStory = SingleStory();
                    if (currIterator == totTitles)
                    {
                        currIterator++;
                        myStatus = read_end;
                    }
                    else
                    {
                        myStatus = readTitleOnly;
                        //incremento
                        currIterator++;
                    }
                    break;
                }
                default:
                    break;

                }
            }
            //currIterator = myStateMachine.calcStatus(totTitles, currIterator);
        }
        myStatus = idle;
        myvol->printData();
        myfile.close();
    }
}

std::vector<std::string> NetworkInterface::parseTitle(std::string mystr,
        const std::string* prestring, const std::string* poststring)
{
    std::regex base_regex (*prestring + "(.*)" + *poststring);
    std::regex base_regex2(*prestring + "(.*)" + *"</td>");
    std::regex base_regex3 ("<td>(.*)");
    std::regex second_base(*prestring + "(.*)" );
    std::smatch base_match;
    std::smatch base_match2;
    std::string matched;
    std::regex base_title(static_cast<const std::string>("title=\"") +
                          static_cast<const std::string>("(.*?)") +
                          static_cast<const std::string>("\">"));//("title=\"" + "(.*)" + "\">");

    std::smatch title_match;
    std::string title_string;
    std::vector<std::string> names;
    std::vector<std::string> names_copyBuffer;
    std::istringstream ss(mystr);
    std::string token;
    std::vector<std::string>::iterator it;
    std::string myname;
    std::size_t pos;

    unsigned int mysize = static_cast<unsigned int> (names.size());
    //now we clean each name found
    it = names.begin();
    names.insert(it, mystr);
    mysize = static_cast<unsigned int>(names.size());
    for (unsigned int ii = 0; ii<mysize; ii++)
    {
        myname = names.back();
        matched = myname;
        //replace the &amp; with &
        names.pop_back();
        if (std::regex_search(myname, base_match, base_regex))
        {
            //The first sub_match is the whole string; the next
            //sub_match is the first parenthesized expression.
            while (base_match.size() > 1)
            {
                matched = base_match[base_match.size()-1].str();
                std::regex_search(matched, base_match, second_base);
             }
        }
        // if nothing is found we try with other delimiters
        else
        {
            if (std::regex_search(myname, base_match, base_regex2))
            {
                //The first sub_match is the whole string; the next
                //sub_match is the first parenthesized expression.
                while (base_match.size() > 1)
                {
                    matched = base_match[base_match.size()-1].str();
                    std::regex_search(matched, base_match, second_base);
                }
            }
            if (std::regex_search(matched, base_match2, base_regex3))
            {
                while (base_match2.size() > 1)
                {
                    matched = base_match2[base_match2.size()-1].str();
                    std::regex_search(matched, base_match2, second_base);
                }
            }
        }
        if (std::regex_search(myname, title_match, base_title))
        {
            //The first sub_match is the whole string; the next
            //sub_match is the first parenthesized expression.
            title_string = title_match[title_match.size()-1].str();
            if  (title_string.find(matched) != std::string::npos &&
                title_string.find("Albi di Dylan Dog") == std::string::npos)
            {
               if ((pos = title_string.find("("))!= std::string::npos)
               {
                   matched = title_string.substr(0, pos);
               }
               else
               {
                   matched = title_string;
               }
           }
       }
        while ((pos = matched.find("&amp;")) != std::string::npos)
        {
            matched.replace(pos + 1, 4, "");
        }
        it = names.begin();//try catch??
        names.insert(it, matched);
    }
    return names;
}


bool NetworkInterface::parseName(std::string myName, std::vector<std::string> *name, std::vector<std::string>* surname )
{
    bool result = false;
    std::size_t pos;
    std::string temp_name = unknown, res=unknown;
    if ((pos = myName.find(" ")) != std::string::npos)
    {
        temp_name = leftRightTrim( myName.substr( 0, pos) );
        res = leftRightTrim( myName.substr(pos, myName.length()) );
        while (((pos = res.find(" ")) != std::string::npos))
        {
            if (std::find(possibleNames.begin(), possibleNames.end(),
                          leftRightTrim(res.substr(0, pos))) != possibleNames.end())
            {
                temp_name += " " + leftRightTrim(res.substr(0, pos));
                res = leftRightTrim(res.substr(pos, res.length()));
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        res = myName;
    }
    name->push_back( leftRightTrim(temp_name));
    surname->push_back( leftRightTrim(res));
    result = true;
    return result;
}

std::string NetworkInterface::parseSummary(std::string myString)
{
    static const std::string preSummary = "<br /><br />";
    std::regex base_regex(preSummary + "(.*)");
    std::smatch match;
    std::smatch base_match;
    std::string matched;

    if (std::regex_search(myString, base_match, base_regex))
    {
        //The first sub_match is the whole string; the next
        //sub_match is the first parenthesized expression.
        if (base_match.size() == 2) {
           matched = base_match[1].str();
        }
    }
    return matched;
}

//Parse the string to extract the year
unsigned int NetworkInterface::parseYear(std::string mystr,
           const std::string* prestring, const std::string* poststring)
{
    std::regex base_regex (*prestring + "[^0-9]*([0-9]+).*" + *poststring);
    std::smatch base_match;
    std::string matched;
    unsigned int result;
    if(std::regex_search(mystr, base_match, base_regex))
    {
        //The first sub_match is the whole string; the next
        //sub_match is the first parenthesized expression.
        while (base_match.size() > 1)
        {
            matched = base_match[base_match.size()-1].str();
            std::regex_search(matched, base_match, base_regex);
        }
    }
    result = stoi(matched);
    return result;
}


//The titles that contain a ',' will be separated: the
std::vector<std::string> NetworkInterface::parseString(std::string mystr,
                    const std::string* prestring, const std::string* poststring)
{
    std::regex base_regex (*prestring + "(.*)" + *poststring);
    std::regex base_regex2( *prestring + "(.*)" + *"</td>");
    std::regex base_regex_small("(.*)<");//to delete volumes for small
    std::regex base_regex3 (*"<td>" + "(.*)");
    std::regex second_base(*prestring + "(.*)" );
    std::smatch match;
    std::smatch base_match;
    std::smatch base_match2;
    std::string matched;
    std::regex base_title(static_cast<const std::string>("title=\"") +
                          static_cast<const std::string>("(.*?)") +
                          static_cast<const std::string>("\">"));//("title=\"" + "(.*)" + "\">");

    std::smatch title_match;
    std::string title_string;
    std::vector<std::string> names;
    std::vector<std::string> names_copyBuffer;

    std::istringstream ss(mystr);
    std::string token, second_token = "";
    std::vector<std::string>::iterator it;
    std::string myname;
    std::size_t pos;
    std::string temp;
    unsigned int mysize;
    while(std::getline(ss, token, ','))
    {
        it = names.begin();
        names.insert(it, token);
    }
    //now we separate the names linked by &
    mysize = names.size();
    for (unsigned int ii = 0; ii< mysize; ii++)
    {
        temp = names.back();
        names.pop_back();
        pos = temp.find("&amp;");
        if (pos == std::string::npos)
        {
            names_copyBuffer.push_back(temp);
        }
        else
        {
            names_copyBuffer.push_back(temp.substr(0, pos));
            names_copyBuffer.push_back(">" + temp.substr(pos + 6, temp.length()));
         }
    }
    names = names_copyBuffer;
    mysize = names.size();
    names_copyBuffer.clear();
    for (unsigned int ii = 0; ii< mysize; ii++)
    {
        temp = names.back();
        names.pop_back();
        pos = temp.find(" e ");
        if (pos == std::string::npos)
        {
            names_copyBuffer.push_back(temp);
        }
        else
        {
            names_copyBuffer.push_back(temp.substr(0, pos));
            names_copyBuffer.push_back(">" + temp.substr(pos + 3, temp.length()));
         }
    }
    names = names_copyBuffer;
    //now we clean each name found
    mysize = names.size();
    for (unsigned int ii = 0; ii<mysize; ii++)
    {
        myname = names.back();
        matched = myname;

        //std::string col = checkColour(myname);
        names.pop_back();
        if (std::regex_search(myname, base_match, base_regex))
        {
            //The first sub_match is the whole string; the next
            //sub_match is the first parenthesized expression.
            while (base_match.size() > 1)
            {
                matched = base_match[base_match.size()-1].str();
                std::regex_search(matched, base_match, second_base);
            }
        }
        // if nothing is found we try with other delimiters
        else
        {
            if (std::regex_search(myname, base_match, base_regex2))
            {
                //The first sub_match is the whole string; the next
                //sub_match is the first parenthesized expression.
                while (base_match.size() > 1)
                {
                    matched = base_match[base_match.size()-1].str();
                    std::regex_search(matched, base_match, base_regex_small);
                }
                std::regex_search(matched, base_match, second_base);
                while (base_match.size() > 1)
                {
                    matched = base_match[base_match.size()-1].str();
                    std::regex_search(matched, base_match, second_base);
                }
            }

            if ((pos = matched.find("<td>"))!=std::string::npos)
            {
                matched = matched.substr(pos + 4, matched.length()-1);
            }
            if ((pos = matched.find("</td>"))!=std::string::npos)
            {
                matched = matched.substr(0, pos);
            }
            /* NOT WORKING: DO NOT KNOW WHY*/
            if (std::regex_search(matched, base_match2, base_regex3))
            {
                while (base_match2.size() > 1)
                {
                    matched = base_match2[base_match2.size()-1].str();
                    std::regex_search(matched, base_match2, second_base);
                }
            }
        }
        //sometimes we can find the title in the hyperlink
        if (std::regex_search(myname, title_match, base_title))
        {
            //The first sub_match is the whole string; the next
            //sub_match is the first parenthesized expression.
            //THIS CAN BE ELIMANATED: SHOULD BE ONLY IN TITLE
            title_string = title_match[title_match.size()-1].str();
            if  (title_string.find(matched) != std::string::npos &&
                 title_string.find("Albi di Dylan Dog") == std::string::npos)
            {
                if ((pos = title_string.find("("))!= std::string::npos)
                {
                    matched = title_string.substr(0, pos);
                }
                else
                {
                    matched = title_string;
                }
            }
        }
        if (!matched.empty())
        {
            it = names.begin();//try catch??
            names.insert(it, matched);
        }
    }
    if (names.empty())
    {
        it = names.begin();//try catch??
        names.insert(it, matched);
    }
    return names;
}

std::string NetworkInterface::checkColour(std::string mystr)
{
    std::size_t pos;
    std::string str2search = "colori a cura";
    std::string colourBy = "";
    std::string token;
    std::vector<std::string>::iterator it;
    std::vector<std::string> temp;
    if ((pos = mystr.find(str2search))!= std::string::npos)
    {
        colourBy = mystr.substr(pos + str2search.length(), mystr.size()-1);
        std::istringstream ss(colourBy);
        while(std::getline(ss, token, ' '))
        {
            it = temp.end();
            if ((pos = token.find(")")) != std::string::npos)
            {
                token = token.substr(0, pos);
            }
            if (!token.empty())
            {
                temp.insert(it, token);
            }
        }
        colourBy = "";
        for (it=temp.begin()+1;it<temp.end(); it++)
        {
            colourBy += temp.at(std::distance(temp.begin(), it)) +" ";
        }
    }
    colourBy = leftRightTrim(colourBy);
    return colourBy;
}


unsigned char NetworkInterface::calcTotTitles(std::string mystr)
{
    std::regex base_regex ("rowspan=\"([2-9])\">");
    std::smatch matched;
    std::string match = "";
    unsigned char res = 1;
    regex_search(mystr, matched, base_regex);
    if (matched.length()>0)
    {
        match = matched[(matched.size()-1)].str();
        try
        {
            res = std::stoi(match);
        }
        catch(std::exception& e)
        {
            res = 1;
        }
    }
    return res;
}
