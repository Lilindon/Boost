#include"endpoint.h"
#include<boost/asio.hpp>
#include<iostream>
/* 
    using namespace boost;
    using namespace std;
    这么写命名空间会造成 空间的污染，例如boost 与std 均有智能指针
*/
// 端点：socket用来收发的工具
// socket：网络编程核心


// 客户端端点
int client_end_point(){
    // 1.对方端口 ip:port
    std::string raw_ip_address = "127.4.8.1";
    unsigned short port_num = 3333;
    // 2.asio错误关键字 error_code
    boost::system::error_code ec;
    // 字符串转换地址
    boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(raw_ip_address,ec);
    // 转换失败
    if(ec.value()!=0){
        std::cout<<"Failed to parse the IP address.Error code = "
                 <<ec.value()<<" .Message is "<<ec.message();
        return ec.value();
    }
    //3.端点组成 (对方端口IP，对方端口port)
    boost::asio::ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// 服务器端端点
int server_end_point(){
    unsigned short port_num = 3333;
    // 1.使用ipv4/ipv6绑定服务器本地地址
    boost::asio::ip::address ip_address = boost::asio::ip::address_v6::any();
    // 2.生成TCP通信端点
    boost::asio::ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// 创建socket
int create_tcp_socket(){
    // 1.socket通信时，必须需要上下文 io_context  io_service(老版本)
    boost::asio::io_context ios;
    // 2.创建通信协议 ipv4
    boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
    // 3.生成socket
    boost::asio::ip::tcp::socket sock(ios);
    // 4.打开socket 验证是否生成成功
    boost::system::error_code ec;
    // sock.open(协议，错误码)
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
    boost::asio::io_context ios;
    // 2.创建通信协议 ipv4
    boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
    // 3.生成acceptor_socket 并绑定上下文
    boost::asio::ip::tcp::acceptor acceptor(ios);
    // 4.打开acceptor_socket 验证是否生成成功
    boost::system::error_code ec;
    acceptor.open(protocol,ec);
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    return 0;
}

// 服务器绑定端点 将acceptor socket bind to endpoint. 所有连接此端点的连接都可以被接收到
int bind_acceptor_socket(){

    unsigned short port_num = 3333; 
    // 1.创建端点  ep(address,port)
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(),port_num);
    // 2.绑定acceptor_socket (上下文ios,端点协议ep.protocol())
    boost::asio::io_context ios;
    boost::asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
    boost::system::error_code ec;
    // 3.绑定端点  bind(ep,ec) 错误码
    acceptor.bind(ep,ec); 
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }

    return 0;
}

// 客户端连接端点  connect endpoint
int connect_to_end(){
    // 1.服务器地址
    std::string raw_ip_address = "192.168.1.124";
    unsigned short port_num = 3333;
    // 2.防止连接失败 使用异常
    try{
        // 创建端点 ep(address,port)
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address),port_num);
        // 客户端连接 创建socket
        boost::asio::io_context ios;
        // 创建socket  sock(ios,ep.protocol())
        boost::asio::ip::tcp::socket sock(ios,ep.protocol());
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
    boost::asio::io_context ios;
    // 创建DNS解析器
    boost::asio::ip::tcp::resolver::query resolver_query(host,port_num,boost::asio::ip::tcp::resolver::query::numeric_service);
    // 为服务添加 解析器
    boost::asio::ip::tcp::resolver resolver(ios);

    try{
        // 实现解析器功能  返回迭代器 it
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
        // 为服务添加socket
        boost::asio::ip::tcp::socket sock(ios);
        // socket连接解析器
        boost::asio::connect(sock,it);
    }catch(boost::system::system_error& e){
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }
}

// 服务器accept 接收新的连接
int accept_new_connection(){
    // 设置服务器监听连接数量 缓冲数》30
    const int BACKLOG_SIZE = 30;
    unsigned short port_num = 3333;

    // 创建端点 ep(address,port)
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);
    // 创建服务
    boost::asio::io_context ios;
    try{
        // 创建acceptor  服务+处理的协议
        boost::asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
        // 服务器acceptor 绑定 端口
        acceptor.bind(ep);
        // 服务器acceptor 监听
        acceptor.listen(BACKLOG_SIZE);
        // 创建与客户端通信的socket  服务器acceptor接收新socket 
        boost::asio::ip::tcp::socket sock(ios);
        acceptor.accept(sock);

    }catch(boost::system::system_error& e){
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }
}  


void use_const_buffer(){
    std::string buf = "hello world";
    boost::asio::const_buffer asio_buf(buf.c_str(), buf.length());
    std::vector<boost::asio::const_buffer> buffer_sequence;
    buffer_sequence.push_back(asio_buf);
   /* boost::asio.send(buffer_sequence);*/
}

void use_buffer_str() {
    boost::asio::const_buffers_1 output_buf = boost::asio::buffer("hello world");
}

void use_buffer_array(){
    const size_t BUF_SIZE_BYTES = 20;
    std::unique_ptr<char []>buf(new char[BUF_SIZE_BYTES]);
    auto input_buf = boost::asio::buffer(static_cast<void*>(buf.get()),BUF_SIZE_BYTES);

}

void write_to_socket(boost::asio::ip::tcp::socket& sock){
    std::string buf = "Hello World";
    std::size_t total_bytes_written = 0; // std::siez_t 可理解为无符号整型  表示数据长度
    // 循环发送
    // write_some 返回每次写入的字节数

    /*
        用户态发送缓冲区
        TCP发送缓冲区  
        由于TCP发送缓冲区可能比用户态发送缓冲区小，因此会存在上次发送的数据未发完，需要与下次发送数据结合一起发送 需要下面的代码
    */

    while(total_bytes_written!=buf.length()){
        // 每次传入的字节   buffer(buf首地址,buf长度)  每次传入的都不一样 因此需要进行偏移
        total_bytes_written += sock.write_some(boost::asio::buffer(buf.c_str()+total_bytes_written,
        buf.length()-total_bytes_written)); 
    }
}

int send_data_by_write_some() {
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_nums = 3333;

    try {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port_nums);
        boost::asio::io_service ios;
        boost::asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);// 客户端连接到对端
        write_to_socket(sock);
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }

    return 0;
}


// 同步发送(每次完全发送用户态发送缓冲区的数据)
int send_data_by_send(){
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_nums = 3333;

    try{
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address),port_nums);
        boost::asio::io_service ios;
        boost::asio::ip::tcp::socket sock(ios,ep.protocol());
        sock.connect(ep);// 客户端连接到对端
        //write_to_socket(sock);// 发送数据  存在用户态发送缓冲区与TCP发送缓冲区的问题
        std::string buf = "hello world";
        int send_length = sock.send(boost::asio::buffer(buf.c_str(),buf.length())); // 使用sock.send 强制sock发送buf全部的数据  将用户态发送缓冲区内容拷贝到TCP发送缓冲区 TCP若没发完 send会让TCP阻塞 直到TCP发完
        // send_length 1:  <0 系统错误  2：=0 对端关闭 3：>0  buf.length()
        if(send_length<=0){
            return 0; // 发送失败
        }
    }catch(boost::system::system_error& e){
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }

    return 0;
}

//read_some
std::string read_from_socket(boost::asio::ip::tcp::socket& sock){
    const unsigned char MESSAGE_SIZE = 9;
    char buf[MESSAGE_SIZE];
    std::size_t total_read_bytes = 0;//记录已读字节数
    while(total_read_bytes!=MESSAGE_SIZE){
        total_read_bytes+=sock.read_some(boost::asio::buffer(buf+total_read_bytes,MESSAGE_SIZE-total_read_bytes));
    }
       
    return std::string(buf,total_read_bytes);
}

 
int read_data_by_read_some(){
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try{
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address),port_num);
        boost::asio::io_service ios;
        boost::asio::ip::tcp::socket sock(ios,ep.protocol());
        sock.connect(ep);
        read_from_socket(sock);
    }catch(boost::system::system_error& e){
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }
    return 0;
}

// receive
int read_data_by_receive() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port_num);
        boost::asio::io_service ios;
        boost::asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        const unsigned char BUFF_SIZE = 7;
        char buffer_receive[BUFF_SIZE];
        int receive_length = sock.receive(boost::asio::buffer(buffer_receive,BUFF_SIZE));// receive 1: <0 系统级错误 2: =0 对端关闭 3: = BUFF_SIZE 一次性读完 读不完阻塞
        if(receive_length<=0){
            std::cout<<"receive failed"<<std::endl;
            return 0; // 失败
        }
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }
    return 0;
}

// read

int read_data_by_read() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port_num);
        boost::asio::io_service ios;
        boost::asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        const unsigned char BUFF_SIZE = 7;
        char buffer_receive[BUFF_SIZE];
        int receive_length = boost::asio::read(sock,boost::asio::buffer(buffer_receive, BUFF_SIZE));// receive 1: <0 系统级错误 2: =0 对端关闭 3: = BUFF_SIZE 一次性读完 读不完阻塞
        if (receive_length <= 0) {
            std::cout << "receive failed" << std::endl;
            return 0; // 失败
        }
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }
    return 0;
}