#include"endpoint.h"
#include<boost/asio.hpp>
#include<iostream>
/* 
    using namespace boost;
    using namespace std;
    ��ôд�����ռ����� �ռ����Ⱦ������boost ��std ��������ָ��
*/
// �˵㣺socket�����շ��Ĺ���
// socket�������̺���


// �ͻ��˶˵�
int client_end_point(){
    // 1.�Է��˿� ip:port
    std::string raw_ip_address = "127.4.8.1";
    unsigned short port_num = 3333;
    // 2.asio����ؼ��� error_code
    boost::system::error_code ec;
    // �ַ���ת����ַ
    boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(raw_ip_address,ec);
    // ת��ʧ��
    if(ec.value()!=0){
        std::cout<<"Failed to parse the IP address.Error code = "
                 <<ec.value()<<" .Message is "<<ec.message();
        return ec.value();
    }
    //3.�˵���� (�Է��˿�IP���Է��˿�port)
    boost::asio::ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// �������˶˵�
int server_end_point(){
    unsigned short port_num = 3333;
    // 1.ʹ��ipv4/ipv6�󶨷��������ص�ַ
    boost::asio::ip::address ip_address = boost::asio::ip::address_v6::any();
    // 2.����TCPͨ�Ŷ˵�
    boost::asio::ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// ����socket
int create_tcp_socket(){
    // 1.socketͨ��ʱ��������Ҫ������ io_context  io_service(�ϰ汾)
    boost::asio::io_context ios;
    // 2.����ͨ��Э�� ipv4
    boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
    // 3.����socket
    boost::asio::ip::tcp::socket sock(ios);
    // 4.��socket ��֤�Ƿ����ɳɹ�
    boost::system::error_code ec;
    // sock.open(Э�飬������)
    sock.open(protocol,ec);
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = " 
        << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    return 0;
}

// ������������ acceptor socket ���ڽ���������
int create_acceptor_socket(){
    // 1.����acceptor_socket ��������context
    boost::asio::io_context ios;
    // 2.����ͨ��Э�� ipv4
    boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
    // 3.����acceptor_socket ����������
    boost::asio::ip::tcp::acceptor acceptor(ios);
    // 4.��acceptor_socket ��֤�Ƿ����ɳɹ�
    boost::system::error_code ec;
    acceptor.open(protocol,ec);
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    return 0;
}

// �������󶨶˵� ��acceptor socket bind to endpoint. �������Ӵ˶˵�����Ӷ����Ա����յ�
int bind_acceptor_socket(){

    unsigned short port_num = 3333; 
    // 1.�����˵�  ep(address,port)
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(),port_num);
    // 2.��acceptor_socket (������ios,�˵�Э��ep.protocol())
    boost::asio::io_context ios;
    boost::asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
    boost::system::error_code ec;
    // 3.�󶨶˵�  bind(ep,ec) ������
    acceptor.bind(ep,ec); 
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }

    return 0;
}

// �ͻ������Ӷ˵�  connect endpoint
int connect_to_end(){
    // 1.��������ַ
    std::string raw_ip_address = "192.168.1.124";
    unsigned short port_num = 3333;
    // 2.��ֹ����ʧ�� ʹ���쳣
    try{
        // �����˵� ep(address,port)
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address),port_num);
        // �ͻ������� ����socket
        boost::asio::io_context ios;
        // ����socket  sock(ios,ep.protocol())
        boost::asio::ip::tcp::socket sock(ios,ep.protocol());
        // �ͻ������ӷ�����  sock.connect(ep)
        sock.connect(ep);

   }catch(boost::system::system_error &e){
        std::cout<<"Error occured!Error code = "<<e.code()<<" .Message: "<<e.what();
        
        return e.code().value();
    }

    return 0;
}

// DNS �������� �ͻ������ӷ�����
int dns_connect_to_end(){
    // ���� ������port��Ϊ�ַ���
    std::string host = "dns.address";
    std::string port_num = "3333";
   
    // �������� 
    boost::asio::io_context ios;
    // ����DNS������
    boost::asio::ip::tcp::resolver::query resolver_query(host,port_num,boost::asio::ip::tcp::resolver::query::numeric_service);
    // Ϊ������� ������
    boost::asio::ip::tcp::resolver resolver(ios);

    try{
        // ʵ�ֽ���������  ���ص����� it
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
        // Ϊ�������socket
        boost::asio::ip::tcp::socket sock(ios);
        // socket���ӽ�����
        boost::asio::connect(sock,it);
    }catch(boost::system::system_error& e){
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }
}

// ������accept �����µ�����
int accept_new_connection(){
    // ���÷����������������� ��������30
    const int BACKLOG_SIZE = 30;
    unsigned short port_num = 3333;

    // �����˵� ep(address,port)
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);
    // ��������
    boost::asio::io_context ios;
    try{
        // ����acceptor  ����+�����Э��
        boost::asio::ip::tcp::acceptor acceptor(ios,ep.protocol());
        // ������acceptor �� �˿�
        acceptor.bind(ep);
        // ������acceptor ����
        acceptor.listen(BACKLOG_SIZE);
        // ������ͻ���ͨ�ŵ�socket  ������acceptor������socket 
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
    std::size_t total_bytes_written = 0; // std::siez_t �����Ϊ�޷�������  ��ʾ���ݳ���
    // ѭ������
    // write_some ����ÿ��д����ֽ���

    /*
        �û�̬���ͻ�����
        TCP���ͻ�����  
        ����TCP���ͻ��������ܱ��û�̬���ͻ�����С����˻�����ϴη��͵�����δ���꣬��Ҫ���´η������ݽ��һ���� ��Ҫ����Ĵ���
    */

    while(total_bytes_written!=buf.length()){
        // ÿ�δ�����ֽ�   buffer(buf�׵�ַ,buf����)  ÿ�δ���Ķ���һ�� �����Ҫ����ƫ��
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
        sock.connect(ep);// �ͻ������ӵ��Զ�
        write_to_socket(sock);
    }
    catch (boost::system::system_error& e) {
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();

        return e.code().value();
    }

    return 0;
}


// ͬ������(ÿ����ȫ�����û�̬���ͻ�����������)
int send_data_by_send(){
    std::string raw_ip_address = "192.168.3.11";
    unsigned short port_nums = 3333;

    try{
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address),port_nums);
        boost::asio::io_service ios;
        boost::asio::ip::tcp::socket sock(ios,ep.protocol());
        sock.connect(ep);// �ͻ������ӵ��Զ�
        //write_to_socket(sock);// ��������  �����û�̬���ͻ�������TCP���ͻ�����������
        std::string buf = "hello world";
        int send_length = sock.send(boost::asio::buffer(buf.c_str(),buf.length())); // ʹ��sock.send ǿ��sock����bufȫ��������  ���û�̬���ͻ��������ݿ�����TCP���ͻ����� TCP��û���� send����TCP���� ֱ��TCP����
        // send_length 1:  <0 ϵͳ����  2��=0 �Զ˹ر� 3��>0  buf.length()
        if(send_length<=0){
            return 0; // ����ʧ��
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
    std::size_t total_read_bytes = 0;//��¼�Ѷ��ֽ���
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
        int receive_length = sock.receive(boost::asio::buffer(buffer_receive,BUFF_SIZE));// receive 1: <0 ϵͳ������ 2: =0 �Զ˹ر� 3: = BUFF_SIZE һ���Զ��� ����������
        if(receive_length<=0){
            std::cout<<"receive failed"<<std::endl;
            return 0; // ʧ��
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
        int receive_length = boost::asio::read(sock,boost::asio::buffer(buffer_receive, BUFF_SIZE));// receive 1: <0 ϵͳ������ 2: =0 �Զ˹ر� 3: = BUFF_SIZE һ���Զ��� ����������
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