#include "listener.h"

Listener::Listener(const std::string &config_file) : m_config(config_file) {}


Listener::run()
{
    init_server();
    while (true)
    {
        m_acceptor.accept(m_socket);
        start_accept();
        ConnectionPool::get_connection()->close();
    }
}


// 初始化服务器参数
void Listener::init_server()
{
    // 读取配置参数（如端口）
    uint16_t port = m_config.get<uint16_t>("net.postgre.port", 8080); // 默认8080
    auto ip = m_config.get<std::string>("net.postgre.ip", "127.0.0.1");
    // 配置acceptor
    m_acceptor.open(boost::asio::ip::tcp::v4());
    m_acceptor.bind({ip, port});
    m_acceptor.listen();

    // 启动接受连接
    start_accept();
}

// 启动异步接受连接
void Listener::start_accept()
{
    m_acceptor.async_accept(m_socket, [this](boost::system::error_code ec)
                            { handle_accept(ec); });
}

// 连接回调处理
void Listener::handle_accept(boost::system::error_code ec)
{
    if (!ec)
    {
        // 成功建立连接，开始读取数据
        boost::asio::async_read_until(
            m_socket,
            boost::asio::dynamic_buffer(m_read_buffer),
            "\n", // 根据协议定义终止符
            [this](boost::system::error_code ec, size_t bytes_transferred)
            {
                handle_read(ec, bytes_transferred);
            });
    }
    else
    {
        // 处理错误（如端口被占用）
        std::cerr << "Accept 失败： " << ec.message() << std::endl;
    }
    // 继续监听新连接
    start_accept();
}

// 读取数据回调
void Listener::handle_read(boost::system::error_code ec, size_t bytes_transferred)
{
    if (!ec)
    {
        std::string request = m_read_buffer.substr(0, bytes_transferred);
        m_read_buffer.erase(0, bytes_transferred);

        // 处理请求并发送响应
        handle_request(request);
    }
    else
    {
        // 处理读取错误
        std::cerr << "Read failed: " << ec.message() << std::endl;
    }
}

// 请求处理函数（需根据业务逻辑实现）
void Listener::handle_request(const std::string &request)
{
    std::string response = "HTTP/1.1 200 OK\n\nHello World!"; // 示例响应
    boost::asio::async_write(
        m_socket,
        boost::asio::buffer(response),
        [this](boost::system::error_code ec, size_t bytes_transferred)
        {
            handle_write(ec, bytes_transferred);
        });
}

// 写入完成回调
void Listener::handle_write(boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec)
    {
        std::cerr << "Write 失败： " << ec.message() << std::endl;
    }
    // 可选择关闭连接或继续监听
    m_socket.close();
}