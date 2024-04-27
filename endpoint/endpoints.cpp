#include"endpoint.h"
/* 
    using namespace boost;
    using namespace std;
    这么写命名空间会造成 空间的污染，例如boost 与std 均有智能指针，不知道是哪个里面的智能指针
*/
// 端点：socket用来收发的工具
// socket：网络编程核心

using namespace boost::asio;

// 客户端端点
int client_end_point(){
    // 1.目标端口 ip:port
    std::string raw_ip_address = "127.4.8.1";
    unsigned short port_num = 3333;
    // 2.asio错误关键字 error_code
    boost::system::error_code ec;
    // 3.字符串转换地址
    ip::address ip_address = ip::address::from_string(raw_ip_address,ec);
    // 转换失败
    if(ec.value()!=0){
        std::cout<<"Failed to parse the IP address.Error code = "
                 <<ec.value()<<" .Message is "<<ec.message();
        return ec.value();
    }
    //4.endPoint组成 (对方端口IP，对方端口port)
    ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// 服务器端端点
int server_end_point(){
    unsigned short port_num = 3333;
    // 1.使用ipv4/ipv6绑定服务器本地地址  这里使用ipv6  这里与之前的ipv4有关系吗？
    ip::address ip_address = ip::address_v6::any();
    // 2.生成TCP通信端点
    ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// 创建进程通信socket
int create_tcp_socket(){
    // 1.socket通信时，必须需要上下文 io_context  io_service(老版本)
    io_context ioc;
    // 2.创建通信协议 ipv4
    ip::tcp protocol = ip::tcp::v4();
    // 3.生成socket
    ip::tcp::socket sock(ioc);
    // 4.打开socket 验证是否生成成功
    boost::system::error_code ec;
    //  sock.open(协议，错误码)
    sock.open(protocol,ec);
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = " 
        << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    return 0;
}

// 创建服务器端 acceptor socket 用于接收新连接
int create_acceptor_socket(){
    // 1.创建acceptor_socket 的上下文context
    io_context ioc;
    // 2.创建通信协议 ipv4
    ip::tcp protocol = ip::tcp::v4();
    // 3.生成acceptor_socket 并绑定上下文
    ip::tcp::acceptor acceptor(ioc);
    // 4.打开acceptor_socket 验证是否生成成功
    boost::system::error_code ec;
    acceptor.open(protocol,ec);
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    // 新版本
    /*
        asio::io_context ioc;
        boost::asio::ip::tcp::acceptor a(ioc,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),3333));
        return 0;
    */
    return 0;
}

// 服务器绑定端点 将acceptor socket bind to endpoint. 所有连接此端点的连接都可以被接收到
int bind_acceptor_socket(){
    // 1.创建端点  ep(IP,Port)
    unsigned short port_num = 3333; 
    ip::tcp::endpoint ep(ip::address_v4::any(),port_num);
   // 2.绑定acceptor_socket (上下文ios,端点协议ep.protocol())
    io_context ios;
    ip::tcp::acceptor acceptor(ios,ep.protocol());
    // 3.绑定端点  acceptor.bind(ep,ec) 错误码
    boost::system::error_code ec;
    acceptor.bind(ep,ec); 
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    return 0;
}

// 客户端连接端点 client::socket to server::endpoint
int connect_to_end(){
    // 1.服务器IP Port
    std::string raw_ip_address = "192.168.1.124";
    unsigned short port_num = 3333;
    // 2.防止连接失败 使用异常
    try{
        // 创建端点 端点绑定目标IP和PORT ep(address,port)
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address),port_num);
        // 客户端连接 创建socket
        io_context ios;
        // 创建socket  sock(ios,ep.protocol())
        ip::tcp::socket sock(ios,ep.protocol());
        // 客户端连接服务器  sock.connect(ep)
        sock.connect(ep);
    }catch(boost::system::system_error &e){
        std::cout<<"Error occured!Error code = "<<e.code()<<" .Message: "<<e.what();
        return e.code().value();
    }
    return 0;
}

// DNS 域名解析 客户端连接服务器
int dns_connect_to_end(){
    // 这里 域名与port均为字符串
    std::string host = "dns.address";
    std::string port_num = "3333";
   
    // 创建服务 
    io_context ios;
    // 创建DNS解析器 resolver::query 
    ip::tcp::resolver::query resolver_query(host,port_num,ip::tcp::resolver::query::numeric_service);
    // 为服务添加 解析器
    ip::tcp::resolver resolver(ios);
    try{
        // 实现解析器功能  返回迭代器 it
        ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
        // 为服务添加socket
        ip::tcp::socket sock(ios);
        // socket连接解析器
        connect(sock,it);
    }catch(boost::system::system_error& e){
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();
        return e.code().value();
    }
    return 0;
}
