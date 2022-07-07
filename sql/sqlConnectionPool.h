//
// Created by chen on 2022/7/5.
//

#ifndef UNTITLED1_SQLCONNECTIONPOOL_H
#define UNTITLED1_SQLCONNECTIONPOOL_H
#include <list>
#include <cppconn/connection.h>
#include <mysql_driver.h>
/**
 * 数据库连接池
 * 懒汉 单例模式 c11静态局部变量实现 支持线程安全
 */
class sqlConnectionPool {
public:
	~sqlConnectionPool();
	/**
	 * 初始化数据库连接池
	 * @param url 数据库url
	 * @param user 用户名
	 * @param passwd 密码
	 * @param maxSize 最大连接数量
	 */
	void initConnPool(std::string &url, std::string &user, std::string &passwd, int maxSize);
	/**
	 * 从连接池 返回一个连接
	 * @return 检查返回值 是否为空  若为空 获取连接失败
	 */
	sql::Connection* getConnection();
	/**
	 * 将用完的连接放回连接池
	 * @param conn 需要放回的连接
	 */
	void releaseConnection(sql::Connection *conn);
	/**
	 * 单例模式 获取实例对象的借口
	 * @return 返回实例对象
	 */
	static sqlConnectionPool* getInstance();

private:
	sqlConnectionPool();
	/**
	 * 创建一个连接
	 * @return 成功返回 创建的连接 失败则返回 nullptr
	 */
	sql::Connection* createConnection();
	/**
	 * 初始化数据库连接池中的连接
	 * 注意:此方法 只能调用一次 在第一次获取 连接池实例对象后调用
	 * @param initialSize 初始化多少个连接
	 */
	void initConnection(int initialSize);
	/**
	 * 销毁某个连接
	 * @param conn 要销毁的连接
	 */
	void destroyConnection(sql::Connection *conn);
	/**
	 * 销毁数据库连接池
	 */
	void destroyConnectionPool();

private:
	int curSize;
	int maxSize;
	std::string user;
	std::string passwd;
	std::string url;

	std::list<sql::Connection *> connList;
	pthread_mutex_t mutex;
	pthread_cond_t hasConn;
	sql::mysql::MySQL_Driver* driver;

};


#endif //UNTITLED1_SQLCONNECTIONPOOL_H
