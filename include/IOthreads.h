#pragma once
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>

#include <pqxx/pqxx> // PostgreSQL C++ 库
#include <boost/property_tree/ptree.hpp> // 使用现有配置读取

struct message
{
    size_t id;
    size_t friend_id;
    std::string content;
    std::chrono::system_clock::time_point timestamp;
};

class ConnectionPool
{
public:
    ConnectionPool(const boost::property_tree::ptree &config);
    ~ConnectionPool();

    pqxx::connection *getConnection();
    void releaseConnection(pqxx::connection *conn);
    bool sendMessage(const message &msg);

private:
    std::queue<pqxx::connection *> m_connections;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    boost::property_tree::ptree m_config;
    size_t m_max_connections;

    void initConnections();
};