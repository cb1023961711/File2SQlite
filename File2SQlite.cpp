// ConsoleApplication16.cpp : 定义控制台应用程序的入口点。
//

// test1.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i < argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (i == 3){
			float* f = new float[Length];
			memmove(f, argv[i], Length*4);
			printf("%d\n", sizeof(float));
			printf("this is Hello");
			for (int i = 0; i < Length; i++){
				//printf("%f\n", f[i]);
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
	rc = sqlite3_open("test2.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

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

void InsertElement(int begin,int end){
	sqlite3 *db;
	int rc;
	char *sql;
	char *zErrMsg = 0;
	
	/* Open database */
	rc = sqlite3_open("test1.db", &db);

	for (int j = begin; j < end; j++){
		
		if (rc){
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			exit(0);
		}
		else{
			fprintf(stderr, "Opened database successfully\n");
		}
		printf("%d\n", j);
		float* f = new float[Length];
		for (int i = 0; i < Length; i++){
			f[i] = i / 1000.0f;
		}
		sqlite3_stmt * stat;
		sqlite3_prepare(db, "INSERT INTO ChannelMap(ID,Position,MatrixH,Fading) VALUES(?,?,?, 20000.00 )", -1, &stat, 0);
		sqlite3_bind_int(stat, 1, j);

		std::string str = std::to_string(j);
		sqlite3_bind_text(stat, 2, str.c_str(), strlen(str.data()), NULL);


		//std::string str = std::to_string(j);
		//const char *tmp = str.c_str();
		////str1= str.data();
		//sqlite3_bind_text(stat, 2, tmp,-1, SQLITE_STATIC);

		//sqlite3_bind_int(stat, 3, j);
		//sqlite3_bind_text(stat, 2, m1.c_str(), strlen(m1.data()), NULL);
		//sqlite3_bind_text(stat, 3, m2.c_str(), strlen(m2.data()), NULL);

		sqlite3_bind_blob(stat, 3, f, Length * 4, NULL); // pdata为数据缓冲区，length_of_data_in_bytes为数据大小，以字节为单位
		int result = sqlite3_step(stat);
		sqlite3_finalize(stat); //把刚才分配的内容析构掉
		
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
		sqlite3_bind_int(stat, 1, count++);

		//std::string str = std::to_string(filename);
		sqlite3_bind_text(stat, 2, filename, strlen(filename), NULL);

		sqlite3_bind_blob(stat, 3, f, Length * 4, NULL); // pdata为数据缓冲区，length_of_data_in_bytes为数据大小，以字节为单位
		sqlite3_bind_double(stat, 4, MatrixH[355][0]);
		int result = sqlite3_step(stat);
		sqlite3_finalize(stat); //把刚才分配的内容析构掉

	sqlite3_close(db);

}


void listFiles(const char * dir)
{
	char dirNew[200];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // 检查是否成功
		return;

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			std::cout << findData.name << "\t<dir>\n";

			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name);

			listFiles(dirNew);
		}
		else{
			std::cout << findData.name << "\t" << "\n";
			char filename[1315];// = findData.name;
			char filename1[1315];
			sprintf(filename, "D:\\download\\Base-11ad-20191024\\data3\\%s",findData.name);
			sprintf(filename1, "%s", findData.name);
			std::fstream _file;
			_file.open(filename, std::ios::in);
			if (!_file){
				_ASSERT(0);
			}
			else{
				_file.close();
				float aa[356][2] = { 0 };
				FILE *file_out;
				file_out = fopen(filename, "r");
				//NS_ASSERT_MSG(file_out , strerror(errno));
				for (int i = 0; i < 356; i++)
				{
					int valu = fscanf(file_out, "%f%f\n", &(aa[i][0]), &(aa[i][1]));
					valu = valu + 1;
					//std::cout<<aa[i][0]<<" "<<aa[i][1]<<std::endl;
				}
				fclose(file_out);
				InsertElement(filename1,aa);
			}
		}
			
			//std::cout << findData.name << "\t" << findData.size << " bytes.\n";
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄
}


int main(int argc, char* argv[])
{
	char dir[200] = "D:\\download\\Base-11ad-20191024\\data3";
	std::cout << "Enter a directory: ";
	//std::cin.getline(dir, 200);
	CreateTable();

	listFiles(dir);






	
	std::queue<std::thread> n;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	
	/*std::thread t1(&InsertElement,0,400000);
	std::thread t2(&InsertElement, 400001, 800000);
	std::thread t3(&InsertElement, 800001, 1200000);
	std::thread t4(&InsertElement, 1200001, 1600000);
	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();*/
	//InsertElement(0,40000);



	

	//rc = sqlite3_open("test1.db", &db);
	//if (rc){
	//	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	//	exit(0);
	//}
	//else{
	//	fprintf(stdout, "Opened database successfully\n");
	//}
	//sql = "CREATE INDEX idx_ChannelMap ON ChannelMap(SnederX,SnederY,SnederZ,ReceiverX,ReceiverY,ReceiverZ);";
	//rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	//if (rc != SQLITE_OK){
	//	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	//	sqlite3_free(zErrMsg);
	//}
	//else{
	//	fprintf(stdout, "Table created successfully\n");
	//}
	//sqlite3_close(db);

	///* Open database */
	//rc = sqlite3_open("test.db", &db);
	//if (rc){
	//	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	//	exit(0);
	//}
	//else{
	//	fprintf(stderr, "Opened database successfully\n");
	//}

	///* Create SQL statement */
	//sql = "SELECT * from ChannelMap";

	//const char* data = "Callback function called";

	///* Execute SQL statement */
	//rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	//if (rc != SQLITE_OK){
	//	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	//	sqlite3_free(zErrMsg);
	//}
	//else{
	//	fprintf(stdout, "Operation done successfully\n");
	//}
	//sqlite3_close(db);

	while (1);
	return 0;
}



