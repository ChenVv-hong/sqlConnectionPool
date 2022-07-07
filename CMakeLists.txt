cmake_minimum_required(VERSION 3.21)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -lmysqlcppconn -lmysqlclient")

add_executable(untitled1 main.cpp sql/sqlConnectionPool.cpp sql/sqlConnectionPool.h)

#link_directories(/usr/local/lib)
#target_link_libraries(untitled1 libmysqlcppconn-static.a)
#add_library(connector_c++1.1/lib/libmysqlcppconn-static.a)
#INCLUDE_DIRECTORIES(connector_c++1.1/include)
#link_directories(connector_c++1.1/lib)
#FIND_LIBRARY(MYSQL_CPPCONN mysqlcppconn connector_c++1.1/lib)
#target_link_libraries(untitled1 ${MYSQL_CPPCONN})
#link_libraries(connector_c++1.1/lib/libmysqlcppconn-static.a)


#INCLUDE_DIRECTORIES(connector_c++/include/jdbc)
#FIND_LIBRARY(MYSQL_CPPCONN mysqlcppconn HINTS connector_c++/lib64)
#target_link_libraries(untitled1 ${MYSQL_CPPCONN})
#link_libraries(connector_c++/lib64/libmysqlcppconn-static.a)


#FIND_LIBRARY(SQL_CLIENT mysqlclient)
#target_link_libraries(untitled1 ${SQL_CLIENT})




