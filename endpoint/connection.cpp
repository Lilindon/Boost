#include "connect.h"

// 服务器accept 接收新的连接
int accept_new_connection() {
    // 设置服务器监听连接数量 缓冲数》30
    const int BACKLOG_SIZE = 30;
    unsigned short port_num = 3333;

    // 创建端点 ep(address,port)
    ip::tcp::endpoint ep(ip::address_v4::any(), port_num);
    // 创建服务 io_context
    boost::asio::io_context ios;
    try {
        // 创建acceptor  服务+处理的协议
        boost::asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
        // 服务器acceptor 绑定 端口
        acceptor.bind(ep);
        // 服务器acceptor 监听
        acceptor.listen(BACKLOG_SIZE);
        // 创建与客户端通信的socket  服务器acceptor接收新socket 
        ip::tcp::socket sock(ios);
        acceptor.accept(sock);
    }
    catch (boost::system::system_error& e) {
        cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();
        return e.code().value();
    }
    return 0;
}

void use_buffer_str() {
    const_buffers_1 output_buf = buffer("hello world");
}

void use_buffer_array() {
    const std::size_t BUF_SIZE_BYTES = 20;
    std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);
    auto input_buf = buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
}


// mutable_buffer写 和 const_buffer 读
void use_const_buffer(const std::string& buf) {
    const_buffer asio_buf(buf.c_str(), buf.length());// string首地址与长度
    std::vector<const_buffer> buffer_sequence;
    buffer_sequence.push_back(asio_buf);
    /* boost::asio.send(buffer_sequence);*/
}

void use_stream_buffer() {
    boost::asio::streambuf buf;

    ostream output(&buf);

    // Writing the message to the stream-based buffer.
    output << "Message1\nMessage2";

    // Now we want to read all data from a streambuf
    // until '\n' delimiter.
    // Instantiate an input stream which uses our 
    // stream buffer.
    istream input(&buf);

    // We'll read data into this string.
    std::string message1;
    std::getline(input, message1);
    // Now message1 string contains 'Message1'.
}



// 同步读写
// 写 socket 
void write_to_socket(ip::tcp::socket& sock) {
    std::string buf = "Hello World";
    std::size_t total_bytes_written = 0; // std::siez_t 可理解为无符号整型  表示数据长度
    // 循环发送
    // write_some 返回每次写入的字节数
    /*
        用户态发送缓冲区
        TCP发送缓冲区
        由于TCP发送缓冲区可能比用户态发送缓冲区小，因此会存在上次发送的数据未发完，需要与下次发送数据结合一起发送 需要下面的代码
    */
    while (total_bytes_written != buf.length()) {
        // 每次传入的字节   buffer(buf首地址,buf长度)  每次传入的都不一样 因此需要进行偏移
        total_bytes_written += sock.write_some(buffer(buf.c_str() + total_bytes_written, buf.length() - total_bytes_written));
    }
}
// 同步发送数据
int send_data_by_write_some() {
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_nums = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_nums);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);// 客户端连接到对端
        write_to_socket(sock);// 多次写数据
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();
        return e.code().value();
    }
    return 0;
}

// 同步发送(每次完全发送用户态发送缓冲区的数据) // socl.send 阻塞直至TCP缓冲区发送完全部的用户态缓冲区数据
int send_data_by_send() {
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_nums = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_nums);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);// 客户端连接到对端
        //write_to_socket(sock);// 发送数据  存在用户态发送缓冲区与TCP发送缓冲区的大小不一致而需要TCP重复多次发送问题
        std::string buf = "hello world";
        int send_length = sock.send(buffer(buf.c_str(), buf.length())); // 使用sock.send 强制sock发送buf全部的数据  将用户态发送缓冲区内容拷贝到TCP发送缓冲区 TCP若没发完 send会让TCP阻塞 直到TCP发完
        // send_length 1:  <0 系统错误  2：=0 对端关闭 3：>0  buf.length()
        if (send_length <= 0) {
            return 0; // 发送失败
        }
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();
        return e.code().value();
    }
    return 0;
}

int send_data_by_wirte() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        ip::tcp::endpoint
            ep(ip::address::from_string(raw_ip_address),
                port_num);
        io_service ios;
        // Step 1. Allocating and opening the socket.
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        std::string buf = "Hello World!";
        int send_length = write(sock, buffer(buf.c_str(), buf.length()));
        if (send_length <= 0) {
            std::cout << "send failed" << std::endl;
            return 0;
        }
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
        return e.code().value();
    }
    return 0;
}


//  读数据，利用接收缓冲区，读。
std::string read_from_socket(ip::tcp::socket& sock) {
    const unsigned char MESSAGE_SIZE = 9;
    char buf[MESSAGE_SIZE];
    std::size_t total_read_bytes = 0;//记录已读字节数
    while (total_read_bytes != MESSAGE_SIZE) {
        total_read_bytes += sock.read_some(buffer(buf + total_read_bytes, MESSAGE_SIZE - total_read_bytes));
    }
    return std::string(buf, total_read_bytes);
}

//  利用socket 读
int read_data_by_read_some() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_num);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        read_from_socket(sock);
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();
        return e.code().value();
    }
    return 0;
}

// 同步读取指定字节数的接口 sock.receive(buf,buf_size);
int read_data_by_receive() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_num);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        const unsigned char BUFF_SIZE = 7;
        char buffer_receive[BUFF_SIZE]; // 接收缓冲区，及大小
        int receive_length = sock.receive(buffer(buffer_receive, BUFF_SIZE));// receive 1: <0 系统级错误 2: =0 对端关闭 3: = BUFF_SIZE 一次性读完 读不完阻塞
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

std::string  read_data_by_until(ip::tcp::socket& sock) {
    boost::asio::streambuf buf;
    // Synchronously read data from the socket until
    // '\n' symbol is encountered.  
    boost::asio::read_until(sock, buf, '\n');
    std::string message;
    // Because buffer 'buf' may contain some other data
    // after '\n' symbol, we have to parse the buffer and
    // extract only symbols before the delimiter. 
    std::istream input_stream(&buf);
    std::getline(input_stream, message);
    return message;
}


// 同步一次性读取全部数据 read(socket,buffer(buf,buf_size));
int read_data_by_read() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_num);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        const unsigned char BUFF_SIZE = 7;
        char buffer_receive[BUFF_SIZE];
        int receive_length = read(sock, buffer(buffer_receive, BUFF_SIZE));// receive 1: <0 系统级错误 2: =0 对端关闭 3: = BUFF_SIZE 一次性读完 读不完阻塞
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
