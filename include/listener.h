#pragma once

#include <iostream>

#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/property_tree/ptree.hpp>

#include "config_reader.h"
#include "IOthreads.h"

class Listener
{
public:
    Listener(const std::string &config_path);
    void run();

private:
    void init_server();                                                        // 服务器初始化
    void start_accept();                                                       // 启动接受连接
    void handle_accept(boost::system::error_code ec);                          // 连接回调
    void handle_read(boost::system::error_code ec, size_t bytes_transferred);  // 读取回调
    void handle_write(boost::system::error_code ec, size_t bytes_transferred); // 写入回调

    boost::asio::io_context m_io_context;      // 事件循环
    boost::asio::ip::tcp::acceptor m_acceptor; // 接收器
    boost::asio::ip::tcp::socket m_socket;     // 当前连接套接字
    boost::property_tree::ptree m_config;      // 配置数据
};