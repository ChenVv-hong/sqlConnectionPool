//#include <bits/stdc++.h>
//#include <algorithm>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "sql/sqlConnectionPool.h"
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include <mysql_driver.h>

using namespace std;
using namespace sql;

int main(){
	sql::mysql::MySQL_Driver *driver = sql::mysql::get_driver_instance();
	sql::Connection *conn = driver->connect("tcp://localhost:3306", "root", "ChenJiaHong123!!");
	sql::PreparedStatement* statement = conn->prepareStatement("use test");
	statement->execute();
	delete statement;

	statement = conn->prepareStatement("select * from book");
	sql::ResultSet *set = statement->executeQuery();
	while(set->next()){
		cout << "isbn\t";
		std::cout << set->getString("isbn");
		cout << "bname\t\t";
		cout << set->getString("bname");
		cout << "author\t";
		cout << set->getString("author");
		cout << "publish\t\t";
		cout << set->getString("publish");
		cout << "price\t";
		cout << set->getString("price");
		cout << "bcount\t";
		cout << set->getString("bcount");
	}
	delete statement;
	delete set;
	delete conn;
//	sqlConnectionPool* connPool = sqlConnectionPool::getInstance();
//	string url = "tcp://localhost:3306";
//	string user = "root";
//	string passwd = "ChenJiaHong123!!";
//	connPool->initConnPool(url, user, passwd, 10);
//	sql::Connection *conn = connPool->getConnection();
//
//	sql::PreparedStatement* statement = conn->prepareStatement("use librarysystem");
//	statement->execute();
//	delete statement;
//
//	statement = conn->prepareStatement("select * from book");
//	sql::ResultSet *set = statement->executeQuery();
//	while(set->next()){
//		cout << "isbn\t";
//		std::cout << set->getString("isbn");
//		cout << "bname\t\t";
//		cout << set->getString("bname");
//		cout << "author\t";
//		cout << set->getString("author");
//		cout << "publish\t\t";
//		cout << set->getString("publish");
//		cout << "price\t";
//		cout << set->getString("price");
//		cout << "bcount\t";
//		cout << set->getString("bcount");
//	}
//	delete statement;
//	delete set;
//	connPool->releaseConnection(conn);
	return 0;
}