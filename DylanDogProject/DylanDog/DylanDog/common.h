#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <iostream>
#include <vector>

static const char* unknown = "Unknown";
static unsigned int year1stVol = 1986;

static const std::vector<std::string> possibleNames =
    {"Anna", "Eleonora", "Francesca","Giulia", "Luisa", "Maria"};


enum en_month {january = 1, february, march, april, may, june,
               july, august, september, october, november, december};


typedef struct personRelationTable
{
    std::string tableName;
    std::string col1;
    std::string col2;
}personRelationTable;

typedef struct date{
    unsigned char day;
    en_month mymonth;
    unsigned int year;

}date;
//const char* projpath = "/Users/BigMac/Dropbox/DylanDog/src/DylanDog/DylanDog";

static const date Date1st = {1, october, year1stVol};

bool isValidYear(unsigned int ui_year);
bool isValidDay(unsigned char uc_day, en_month p_month = (en_month)1 ,unsigned int ui_year = 2000);
unsigned char monthDays(en_month p_month, unsigned int ui_year);
en_month convertString2month(std::string month_str);
void toUpper(std::basic_string<wchar_t>& s);
void toLower(std::basic_string<char>& s);
void toLower(std::basic_string<wchar_t>& s);
void toUpper(std::basic_string<char>& s);
std::string convertMonth2itaString(en_month month_en);
std::string leftRightTrim(const std::string& str);
date addMonths2Date(date myDate, int toBeAdded);
#endif // COMMON_H
