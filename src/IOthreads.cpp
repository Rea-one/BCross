#include "IOthreads.h"

ConnectionPool::ConnectionPool(const boost::property_tree::ptree &config)
    : m_config(config), m_max_connections(config.get<size_t>("database.max_connections", 10))
{
    initConnections();
}

ConnectionPool::~ConnectionPool()
{
    while (!m_connections.empty())
    {
        delete m_connections.front();
        m_connections.pop();
    }
}

void ConnectionPool::initConnections()
{
    for (size_t i = 0; i < m_max_connections; ++i)
    {
        try
        {
            std::string conn_str =
                "host=" + m_config.get<std::string>("database.host", "localhost") +
                " port=" + std::to_string(m_config.get<uint16_t>("database.port", 5432)) +
                " dbname=" + m_config.get<std::string>("database.name", "mydb") +
                " user=" + m_config.get<std::string>("database.user", "user") +
                " password=" + m_config.get<std::string>("database.password", "pass");

            pqxx::connection *conn = new pqxx::connection(conn_str);
            m_connections.push(conn);
        }
        catch (const std::exception &e)
        {
            std::cerr << "数据库连接初始化失败: " << e.what() << std::endl;
        }
    }
}

pqxx::connection *ConnectionPool::getConnection()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_connections.empty())
    {
        m_cond.wait(lock); // 等待可用连接
    }
    pqxx::connection *conn = m_connections.front();
    m_connections.pop();
    return conn;
}

void ConnectionPool::releaseConnection(pqxx::connection *conn)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.push(conn);
    }
    m_cond.notify_one(); // 通知可用连接
}

bool ConnectionPOol::sendMessage(const message &msg)
{
    pqxx::connection *conn = getConnection();
    if (!conn)
    {
        return false;
    }
    try
    {
        pqxx::work txn(*conn);
        std::string query = "INSERT INTO messages (content, friend_id, id, timestamp) VALUES (" +
                            pqxx::to_string(msg.content) +", " + std::to_string(msg.friend_id) + ", " +
                            std::to_string(msg.id) + ", " + std::to_string(msg.timestamp.time_since_epoch().count()) + ");";
    }
    catch (const std::exception &e)
    {
        std::cerr << "数据库插入失败: " << e.what() << std::endl;
        return false;
    }
}