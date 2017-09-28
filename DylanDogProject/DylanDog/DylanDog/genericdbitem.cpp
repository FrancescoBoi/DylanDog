#include "genericdbitem.h"

genericDBItem::genericDBItem()
{
    ;
}

void genericDBItem::setID(unsigned int inID)
{
    ID = inID;
}

unsigned int genericDBItem::getID(){
    return ID;
}
