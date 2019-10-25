
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string>
#include<iostream>
#include <sstream>
#include<cstring>
#include<pthread.h>
#include<thread>
#include<queue>
#include<thread>
#include <cstring>        // for strcpy(), strcat()
#include <io.h>
#include<fstream>
#pragma warning(disable:4996)



const int Length = 710;
static int count = 0;
std::string SQlitedb="test2.db";
std::string FilePath="";

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	//test whether the data is right or not 
	for (i = 0; i < argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (i == 3){
			float* f = new float[Length];
			memmove(f, argv[i], Length*4);
			for (int i = 0; i < Length; i++){
				printf("%f\n", f[i]);
			}
		}
	}
	printf("\n");
	return 0;
}
void CreateTable(){
	sqlite3 *db;
	int rc;
	char *sql;
	char *zErrMsg = 0;

	/* Open database */
	rc = sqlite3_open(SQlitedb, &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}
	//create the table and the elements are ID,Position,MatrixH,Fading
	sql = "CREATE TABLE ChannelMap("  \
		"ID  INT PRIMARY KEY  NOT NULL," \
		"Position TEXT NOT NULL,"\
		"MatrixH blob NOT NULL,"\
		"Fading DOUBLE NOT NULL);";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);

}

void InsertElement(char filename[1315], float MatrixH[356][2]){
	sqlite3 *db;
	int rc;
	char *sql;
	char *zErrMsg = 0;

	/* Open database */
	rc = sqlite3_open("test2.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stderr, "Opened database successfully\n");
	}
	float* f = new float[Length];
	for (int i = 0; i < Length; i++){
		f[i] = MatrixH[i/2][i%2];
	}
	sqlite3_stmt * stat;
	sqlite3_prepare(db, "INSERT INTO ChannelMap(ID,Position,MatrixH,Fading) VALUES(?,?,?,?)", -1, &stat, 0);
	//以下的1,2,3,4与上一句的问号位置相对应
	sqlite3_bind_int(stat, 1, count++);
	sqlite3_bind_text(stat, 2, filename, strlen(filename), NULL);
	sqlite3_bind_blob(stat, 3, f, Length * 4, NULL); // store MatrixH
	sqlite3_bind_double(stat, 4, MatrixH[355][0]);
	int result = sqlite3_step(stat);
	sqlite3_finalize(stat); //rsset stat
	sqlite3_close(db);

}

void GetFileInformation(char path[1315],float& MatrixHAndFading[356][2]){
	std::fstream file;
	file.open(path, std::ios::in);
	if (!file){
		_ASSERT(0);
	}
	else{
		file.close();
		FILE *file_out;
		file_out = fopen(path, "r");
		//NS_ASSERT_MSG(file_out , strerror(errno));
		for (int i = 0; i < 356; i++){
			int valu = fscanf(file_out, "%f%f\n", &(MatrixHAndFading[i][0]), &(MatrixHAndFading[i][1]));
			valu = valu + 1;
			//std::cout<<MatrixHAndFading[i][0]<<" "<<MatrixHAndFading[i][1]<<std::endl;
		}
		fclose(file_out);
	}
}

void listFiles(const char * dir){
	char dirNew[200];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // 检查是否成功
		return;
	do{
		if (findData.attrib & _A_SUBDIR){
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			std::cout << findData.name << "\t<dir>\n";

			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索（在子目录中进行搜索）
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name);
			listFiles(dirNew);
		}
		else{
			std::cout << findData.name << "\t" << "\n";

			char pathname[1315];// = findData.name;
			char filename[1315];
			sprintf(pathname, "D:\\download\\Base-11ad-20191024\\data3\\%s",findData.name);
			sprintf(filename, "%s", findData.name);
			float MatrixHAndFading[356][2] = { 0 };
			GetFileInformation(pathname,MatrixHAndFading);
			InsertElement(filename,MatrixHAndFading);
		}
	} while (_findnext(handle, &findData) == 0);
	_findclose(handle);    // 关闭搜索句柄
}

int main(int argc, char* argv[])
{
	char dir[200] = "D:\\download\\Base-11ad-20191024\\data3";
	//std::cout << "Enter a directory: ";
	//std::cin.getline(dir, 200);
	CreateTable();

	listFiles(dir);
	
	std::queue<std::thread> n;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	
	while (1);
	return 0;
}