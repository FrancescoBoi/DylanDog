#ifndef COMMON_H

#endif
#include <ctime>
#include "common.h"
#include <algorithm>
#include <math.h>

bool isValidYear(unsigned int ui_year)
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    unsigned int this_year = now->tm_year + 1900;
    bool res = ui_year > year1stVol && ui_year<this_year;
    return res;
}

bool isValidDay(unsigned char uc_day, en_month p_month, unsigned int ui_year)
{
    //time_t t = time(0);   // get time now
    //struct tm * now = localtime( & t );
    bool res = uc_day <= monthDays(p_month, ui_year);
    return res;
}

bool isValidMonth(en_month p_month)
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    en_month myMonth = (en_month) now->tm_mon;
    bool res = p_month <= myMonth;
    return res;

}

unsigned char monthDays(en_month p_month, unsigned int ui_year)
{
    unsigned char days = 0;
    if (p_month == 4 || p_month == 6 || p_month == 9 || p_month == 11)
        days = 30;

    else if (p_month == 02)
    {
        bool leapyear = (ui_year % 4 == 0 && ui_year % 100 != 0) || (ui_year % 400 == 0);

        if (leapyear == 0)
            days = 29;
        else
            days = 28;
    }
    else
        days = 31;

    return days;
}

bool isValidDate(unsigned char uc_day, en_month p_month, unsigned int ui_year)
{
    bool res = isValidDay(uc_day, p_month, ui_year) && isValidYear(ui_year) && isValidMonth(p_month);
    return res;
}

en_month convertString2month(std::string month_str)
{
    en_month res = (en_month) 1;
    std::string temp = month_str;
    toLower(temp);
    if(month_str.find("gennaio") != std::string::npos)
    {
        res = (en_month) 1;
    }
    else if (month_str.find("febbraio") != std::string::npos)
    {
        res = (en_month) 2;
    }
    else if (month_str.find("marzo") != std::string::npos)
    {
        res = (en_month) 3;
    }
    else if (month_str.find("aprile") != std::string::npos)
    {
        res = (en_month) 4;
    }
    else if (month_str.find("maggio") != std::string::npos)
    {
        res = (en_month) 5;
    }
    else if (month_str.find("giugno") != std::string::npos)
    {
        res = (en_month) 6;
    }
    else if (month_str.find("luglio")  != std::string::npos)
    {
        res = (en_month) 7;
    }
    else if (month_str.find("agosto") != std::string::npos)
    {
        res = (en_month) 8;
    }
    else if (month_str.find("settembre") != std::string::npos)
    {
        res = (en_month) 9;
    }
    else if (month_str.find("ottobre") != std::string::npos)
    {
        res = (en_month) 10;
    }
    else if (month_str.find("novembre") != std::string::npos)
    {
        res = (en_month) 11;
    }
    else if (month_str.find("dicembre") != std::string::npos)
    {
        res = (en_month) 12;
    }
    return res;
}

std::string convertMonth2itaString(en_month month_en)
{
    std::string res;
    switch(month_en)
    {
        case(january):
        {
            res = "Gennaio";
            break;
        }

        case (february):
        {
            res = "Febbraio";
            break;
        }
        case (march):
        {
            res = "Marzo";
            break;
        }
        case (april):
        {
            res = "Aprile";
            break;
        }
        case (may):
        {
            res = "Maggio";
            break;
        }
        case (june):
        {
            res = "Giugno";
            break;
        }
        case july:
        {
            res = "Luglio";
            break;
        }
        case august:
        {
            res = "Agosto";
            break;
        }
        case (september):
        {
            res = "Settembre";
            break;
        }
        case (october):
        {
            res = "Ottobre";
            break;
        }
        case (november):
        {
            res = "Novembre";
            break;
        }
        case (december):
        {
            res = "Dicembre";
            break;
        }
    }
    return res;
}

void toUpper(std::basic_string<char>& s) {
   for (std::basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = toupper(*p); // toupper is for char
   }
}

void toUpper(std::basic_string<wchar_t>& s) {
   for (std::basic_string<wchar_t>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = towupper(*p); // towupper is for wchar_t
   }
}

void toLower(std::basic_string<char>& s) {
   for (std::basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = tolower(*p);
   }
}

void toLower(std::basic_string<wchar_t>& s) {
   for (std::basic_string<wchar_t>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = towlower(*p);
   }
}

std::string leftRightTrim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

date addMonths2Date(date myDate, int toBeAdded)
{
    date res = myDate;
    en_month temp_month = myDate.mymonth;
    int res_month =  int(myDate.mymonth) + toBeAdded;
    temp_month = en_month(res_month % 12 + (res_month==12)*12);
    res.year += floor( (int(myDate.mymonth) + toBeAdded) / 12);
    if (res.day > monthDays(temp_month, res.year))
    {
        res.day = monthDays(temp_month, res.year);
    }
    res.mymonth = temp_month;
    return res;


}
