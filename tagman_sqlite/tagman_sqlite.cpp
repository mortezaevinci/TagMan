// tagman_sqlite.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <stdio.h>
#include "sqlite3.h"
#include <string>

#define iferr if(error){fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));};

int main()
{
	sqlite3 *db;

	int error = sqlite3_open("tagman.db", &db);
iferr;

   error=sqlite3_exec(db,"CREATE TABLE ocrdata (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,name varchar(50),filename varchar(255),ocrstring varchar(10000),ocrclass varchar(10000) ,knowledgeclass varchar(10000), otherarranged varchar(10000), date varchar(15),other varchar(255),UNIQUE (id))",0,0,0);
iferr;

	error=sqlite3_exec(db,"CREATE TABLE ocrdata_test (id INTEGER NOT NULL  PRIMARY KEY AUTOINCREMENT,name varchar(50),filename varchar(255),ocrstring varchar(10000),ocrclass varchar(10000) ,knowledgeclass varchar(10000), otherarranged varchar(10000), date varchar(15),other varchar(255),UNIQUE (id))",0,0,0);
iferr;
   
std::string string1="testingtesting";
std::string query="INSERT INTO ocrdata_test(id,name,filename,ocrstring,ocrclass,knowledgeclass,otherarranged,date,other) VALUES (1,\'test1\',\'"+string1+"\',\'12345456789\',\'\',\'\',\'\',\'\',\'\')";
   error = sqlite3_exec(db,
    query.c_str(),
    0, 0, 0);
    iferr;

   error = sqlite3_exec(db,
    "INSERT INTO ocrdata_test(id,name,filename,ocrstring,ocrclass,knowledgeclass,otherarranged,date,other) VALUES (2,\'test2\',\'\',\'123456\',\'\',\'\',\'\',\'\',\'\')",
    0, 0, 0);
	iferr;

	   error = sqlite3_exec(db,
    "INSERT INTO ocrdata_test(id,name,filename,ocrstring,ocrclass,knowledgeclass,otherarranged,date,other) VALUES (NULL,\'test1\',\'\',\'12345\',\'\',\'\',\'\',\'\',\'\')",
    0, 0, 0);
    iferr;

    error = sqlite3_exec(db,
    "UPDATE ocrdata_test SET ocrstring=\'123\' WHERE id = 1",
    0, 0, 0);
    iferr;


	sqlite3_stmt    *res;

const char      *tail;

error = sqlite3_prepare(db,
        "SELECT id,ocrstring FROM ocrdata_test",
        1000, &res, &tail);

if (error != SQLITE_OK)
    {
    puts("We did not get any data!");
    return 0;
    }


while (sqlite3_step(res) == SQLITE_ROW)
    {
    printf("%d|", sqlite3_column_int(res, 0));
    printf("%s|", sqlite3_column_text(res, 1));
   // printf("%s|", sqlite3_column_text(res, 2));
   // printf("%s|", sqlite3_column_text(res, 3));
   // printf("%s\n", sqlite3_column_text(res, 4));
    }
printf("done!");

//error = sqlite3_exec(db,
//   "DELETE FROM command WHERE target = \'jerr\' AND priority > 1.0",
 //   0, 0, 0);
sqlite3_finalize(res);

   sqlite3_close (db); 

	return 0;
}

