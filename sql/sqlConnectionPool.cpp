//
// Created by chen on 2022/7/5.
//

#include "sqlConnectionPool.h"

void sqlConnectionPool::initConnPool(std::string &url, std::string &user, std::string &passwd, int maxSize) {
	this->maxSize = maxSize;
	this->curSize = 0;
	this->url = url;
	this->user = user;
	this->passwd = passwd;
	try{
		this->driver = sql::mysql::get_mysql_driver_instance();
	}
	catch (sql::SQLException &e) {
		perror("驱动连接出错;");
	}
	catch (std::runtime_error &e) {
		perror("运行时出错;");
	}
	this->initConnection(maxSize / 2);
	pthread_mutex_init(&this->mutex, nullptr);
	pthread_cond_init(&this->hasConn, nullptr);
}

sql::Connection *sqlConnectionPool::getConnection() {
	sql::Connection* conn;
	pthread_mutex_lock(&mutex);
	//如果连接池中没有连接 并且已达最大连接数 那么等待连接空闲
	while(connList.size() <= 0 && curSize >= maxSize){
		pthread_cond_wait(&hasConn, &mutex);
	}
	//如果连接池中 有空闲连接 优先从连接池中获取
	if(connList.size() > 0){
		conn = connList.front();
		connList.pop_front();
		if(conn->isClosed()){
			//如果连接已经关闭 释放资源 重新创建连接
			delete conn;
			conn = createConnection();
		}
		if(conn == nullptr) curSize--;
		pthread_mutex_unlock(&mutex);
		return conn;
	}

	//连接池中没有空闲连接 但是当前连接数小于最大连接数 那么创建一个连接返回
	if(curSize < maxSize){
		conn = createConnection();
		if(conn){
			++curSize;
			pthread_mutex_unlock(&mutex);
			return conn;
		}
		else{
			pthread_mutex_unlock(&mutex);
			return nullptr;
		}
	}

	pthread_mutex_unlock(&mutex);
	return nullptr;
}

void sqlConnectionPool::releaseConnection(sql::Connection *conn) {
	if(conn){
		pthread_mutex_lock(&mutex);
		connList.push_back(conn);
		pthread_cond_signal(&hasConn);
		pthread_mutex_unlock(&mutex);
	}
}

sqlConnectionPool *sqlConnectionPool::getInstance(){
	static sqlConnectionPool pool;
	return &pool;
}

sqlConnectionPool::~sqlConnectionPool() {
	destroyConnectionPool();
}

sqlConnectionPool::sqlConnectionPool() {

}

sql::Connection *sqlConnectionPool::createConnection() {
	sql::Connection *conn;
	try{
		//通过驱动器 创建连接
		conn = this->driver->connect(url, user, passwd);
		return conn;
	}
	catch(sql::SQLException &e) {
		perror(e.what());
	}
	catch (std::runtime_error &e) {
		perror(e.what());
	}
	return nullptr;
}

void sqlConnectionPool::initConnection(int initialSize) {
	sql::Connection* conn;
	pthread_mutex_lock(&mutex);
	for(int i = 0; i < initialSize; i++){
		conn = createConnection();
		if(conn){
			connList.push_back(conn);
			++curSize;
		}
		else perror("初始化连接池连接失败;");
	}
	pthread_mutex_unlock(&mutex);
}

void sqlConnectionPool::destroyConnection(sql::Connection *conn) {
	if(conn){
		try{
			conn->close();
		}
		catch (sql::SQLException &e){
			perror(e.what());
		}
		catch (std::exception &e) {
			perror(e.what());
		}
		delete conn;
	}
}

void sqlConnectionPool::destroyConnectionPool() {
	pthread_mutex_lock(&mutex);
	for(auto it: this->connList){
		this->destroyConnection(it);
	}
	curSize = 0;
	connList.clear();
	pthread_mutex_unlock(&mutex);
}






