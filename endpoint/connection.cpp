#include "connect.h"

// ������accept �����µ�����
int accept_new_connection() {
    // ���÷����������������� ��������30
    const int BACKLOG_SIZE = 30;
    unsigned short port_num = 3333;

    // �����˵� ep(address,port)
    ip::tcp::endpoint ep(ip::address_v4::any(), port_num);
    // �������� io_context
    boost::asio::io_context ios;
    try {
        // ����acceptor  ����+�����Э��
        boost::asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
        // ������acceptor �� �˿�
        acceptor.bind(ep);
        // ������acceptor ����
        acceptor.listen(BACKLOG_SIZE);
        // ������ͻ���ͨ�ŵ�socket  ������acceptor������socket 
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


// mutable_bufferд �� const_buffer ��
void use_const_buffer(const std::string& buf) {
    const_buffer asio_buf(buf.c_str(), buf.length());// string�׵�ַ�볤��
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



// ͬ����д
// д socket 
void write_to_socket(ip::tcp::socket& sock) {
    std::string buf = "Hello World";
    std::size_t total_bytes_written = 0; // std::siez_t �����Ϊ�޷�������  ��ʾ���ݳ���
    // ѭ������
    // write_some ����ÿ��д����ֽ���
    /*
        �û�̬���ͻ�����
        TCP���ͻ�����
        ����TCP���ͻ��������ܱ��û�̬���ͻ�����С����˻�����ϴη��͵�����δ���꣬��Ҫ���´η������ݽ��һ���� ��Ҫ����Ĵ���
    */
    while (total_bytes_written != buf.length()) {
        // ÿ�δ�����ֽ�   buffer(buf�׵�ַ,buf����)  ÿ�δ���Ķ���һ�� �����Ҫ����ƫ��
        total_bytes_written += sock.write_some(buffer(buf.c_str() + total_bytes_written, buf.length() - total_bytes_written));
    }
}
// ͬ����������
int send_data_by_write_some() {
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_nums = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_nums);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);// �ͻ������ӵ��Զ�
        write_to_socket(sock);// ���д����
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();
        return e.code().value();
    }
    return 0;
}

// ͬ������(ÿ����ȫ�����û�̬���ͻ�����������) // socl.send ����ֱ��TCP������������ȫ�����û�̬����������
int send_data_by_send() {
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_nums = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_nums);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);// �ͻ������ӵ��Զ�
        //write_to_socket(sock);// ��������  �����û�̬���ͻ�������TCP���ͻ������Ĵ�С��һ�¶���ҪTCP�ظ���η�������
        std::string buf = "hello world";
        int send_length = sock.send(buffer(buf.c_str(), buf.length())); // ʹ��sock.send ǿ��sock����bufȫ��������  ���û�̬���ͻ��������ݿ�����TCP���ͻ����� TCP��û���� send����TCP���� ֱ��TCP����
        // send_length 1:  <0 ϵͳ����  2��=0 �Զ˹ر� 3��>0  buf.length()
        if (send_length <= 0) {
            return 0; // ����ʧ��
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


//  �����ݣ����ý��ջ�����������
std::string read_from_socket(ip::tcp::socket& sock) {
    const unsigned char MESSAGE_SIZE = 9;
    char buf[MESSAGE_SIZE];
    std::size_t total_read_bytes = 0;//��¼�Ѷ��ֽ���
    while (total_read_bytes != MESSAGE_SIZE) {
        total_read_bytes += sock.read_some(buffer(buf + total_read_bytes, MESSAGE_SIZE - total_read_bytes));
    }
    return std::string(buf, total_read_bytes);
}

//  ����socket ��
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

// ͬ����ȡָ���ֽ����Ľӿ� sock.receive(buf,buf_size);
int read_data_by_receive() {
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 3333;
    try {
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address), port_num);
        io_service ios;
        ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        const unsigned char BUFF_SIZE = 7;
        char buffer_receive[BUFF_SIZE]; // ���ջ�����������С
        int receive_length = sock.receive(buffer(buffer_receive, BUFF_SIZE));// receive 1: <0 ϵͳ������ 2: =0 �Զ˹ر� 3: = BUFF_SIZE һ���Զ��� ����������
        if (receive_length <= 0) {
            std::cout << "receive failed" << std::endl;
            return 0; // ʧ��
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


// ͬ��һ���Զ�ȡȫ������ read(socket,buffer(buf,buf_size));
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
        int receive_length = read(sock, buffer(buffer_receive, BUFF_SIZE));// receive 1: <0 ϵͳ������ 2: =0 �Զ˹ر� 3: = BUFF_SIZE һ���Զ��� ����������
        if (receive_length <= 0) {
            std::cout << "receive failed" << std::endl;
            return 0; // ʧ��
        }
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }
    return 0;
}
