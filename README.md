# DylanDog
This project is not complete. It has been developed with Qt Creator and needs SQLight.
The goal is to create a system to help collectionist managing comic-books collections (in this concept the coic book Dylan Dog has been taken into account).

The system works as follows:
- the collection is made up by one or more volumes (class volume)
- each volume has one or more coverIllustrators (class coverIllustrator), a date in which it was realeased, might be coloured or black and white;
- each volume has one or more stories (class singlestory);
- each story has a title, one or more illustrators (class illustrator), one or more people who wrote the story (class ScriptBy) and a description;
- The cover-illustrators, illustrators, authors are represented by different classes inherited all by the same parent class PersonDBX class defined in the file PersonDB.h.


At the moment the system does the following actions:
- it connects to the wikipedia page of the chosen comic book and download the html page containing all the volumes published until the current date (networkInterface class);
- it parse the html document and esxtracts the various pieces of information;
- it writes the information on the database (dbinterface class).

Take a look on the SQLight DB to see the various tables.

Apart from the various OO programming routines the following paradigms have been used:
- singleton class for the dbInterface and networkInterface;
- Curiously Recurring template paradigm for the class PersonDBX in the Person.h file; this class is parent of the classes Illustrator, CoverIllustrator, ScriptBy. I used it to make the child classes inherit a static (class) attribute (tablename) to be redefined in the child classes.
