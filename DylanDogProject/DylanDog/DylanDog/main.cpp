#include "mainwindow.h"
#include <QApplication>
#include <dbinterface.h>
#include "topicby.h"
#include "illustrator.h"
#include "coverillustrator.h"
#include <iostream>
#include <QDebug>
#include <networkinterface.h>
#include <volume.h>
#include<QDir>

int main(int argc, char *argv[])
{

    DBInterface* myDBInstance = DBInterface::getInstance();
    NetworkInterface* myNetwork = NetworkInterface::getInstance();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    myNetwork->test();
    std::string ttt = QDir::currentPath().toStdString();
    std::cout<<ttt;
    for (unsigned int ii=362; ii<367; ii++)
    {
        volume myVol = volume();
        myVol.setID(ii);//338
        myVol.setDate(addMonths2Date(Date1st, static_cast<int>(ii-1)));
        myNetwork->parseHTMLFile(&myVol);
        myDBInstance->addVolData2DB(&myVol);


        //myDBInstance->addgenericPersonDB2Table(
        //            (myVol.getStoriesVector()->at(0).getScriptByVector()->at(0)));

    }

    //return a.exec();

}
