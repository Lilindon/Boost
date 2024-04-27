#include"endpoint.h"
/* 
    using namespace boost;
    using namespace std;
    ��ôд�����ռ����� �ռ����Ⱦ������boost ��std ��������ָ�룬��֪�����ĸ����������ָ��
*/
// �˵㣺socket�����շ��Ĺ���
// socket�������̺���

using namespace boost::asio;

// �ͻ��˶˵�
int client_end_point(){
    // 1.Ŀ��˿� ip:port
    std::string raw_ip_address = "127.4.8.1";
    unsigned short port_num = 3333;
    // 2.asio����ؼ��� error_code
    boost::system::error_code ec;
    // 3.�ַ���ת����ַ
    ip::address ip_address = ip::address::from_string(raw_ip_address,ec);
    // ת��ʧ��
    if(ec.value()!=0){
        std::cout<<"Failed to parse the IP address.Error code = "
                 <<ec.value()<<" .Message is "<<ec.message();
        return ec.value();
    }
    //4.endPoint��� (�Է��˿�IP���Է��˿�port)
    ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// �������˶˵�
int server_end_point(){
    unsigned short port_num = 3333;
    // 1.ʹ��ipv4/ipv6�󶨷��������ص�ַ  ����ʹ��ipv6  ������֮ǰ��ipv4�й�ϵ��
    ip::address ip_address = ip::address_v6::any();
    // 2.����TCPͨ�Ŷ˵�
    ip::tcp::endpoint ep(ip_address,port_num);
    return 0;
}

// ��������ͨ��socket
int create_tcp_socket(){
    // 1.socketͨ��ʱ��������Ҫ������ io_context  io_service(�ϰ汾)
    io_context ioc;
    // 2.����ͨ��Э�� ipv4
    ip::tcp protocol = ip::tcp::v4();
    // 3.����socket
    ip::tcp::socket sock(ioc);
    // 4.��socket ��֤�Ƿ����ɳɹ�
    boost::system::error_code ec;
    //  sock.open(Э�飬������)
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
    io_context ioc;
    // 2.����ͨ��Э�� ipv4
    ip::tcp protocol = ip::tcp::v4();
    // 3.����acceptor_socket ����������
    ip::tcp::acceptor acceptor(ioc);
    // 4.��acceptor_socket ��֤�Ƿ����ɳɹ�
    boost::system::error_code ec;
    acceptor.open(protocol,ec);
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    // �°汾
    /*
        asio::io_context ioc;
        boost::asio::ip::tcp::acceptor a(ioc,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),3333));
        return 0;
    */
    return 0;
}

// �������󶨶˵� ��acceptor socket bind to endpoint. �������Ӵ˶˵�����Ӷ����Ա����յ�
int bind_acceptor_socket(){
    // 1.�����˵�  ep(IP,Port)
    unsigned short port_num = 3333; 
    ip::tcp::endpoint ep(ip::address_v4::any(),port_num);
   // 2.��acceptor_socket (������ios,�˵�Э��ep.protocol())
    io_context ios;
    ip::tcp::acceptor acceptor(ios,ep.protocol());
    // 3.�󶨶˵�  acceptor.bind(ep,ec) ������
    boost::system::error_code ec;
    acceptor.bind(ep,ec); 
    if(ec.value()!=0){
        std::cout << "Failed to parse the IP address.Error code = "
            << ec.value() << " .Message is " << ec.message();
        return ec.value();
    }
    return 0;
}

// �ͻ������Ӷ˵� client::socket to server::endpoint
int connect_to_end(){
    // 1.������IP Port
    std::string raw_ip_address = "192.168.1.124";
    unsigned short port_num = 3333;
    // 2.��ֹ����ʧ�� ʹ���쳣
    try{
        // �����˵� �˵��Ŀ��IP��PORT ep(address,port)
        ip::tcp::endpoint ep(ip::address::from_string(raw_ip_address),port_num);
        // �ͻ������� ����socket
        io_context ios;
        // ����socket  sock(ios,ep.protocol())
        ip::tcp::socket sock(ios,ep.protocol());
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
    io_context ios;
    // ����DNS������ resolver::query 
    ip::tcp::resolver::query resolver_query(host,port_num,ip::tcp::resolver::query::numeric_service);
    // Ϊ������� ������
    ip::tcp::resolver resolver(ios);
    try{
        // ʵ�ֽ���������  ���ص����� it
        ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
        // Ϊ�������socket
        ip::tcp::socket sock(ios);
        // socket���ӽ�����
        connect(sock,it);
    }catch(boost::system::system_error& e){
        std::cout << "Error occured!Error code = " << e.code() << " .Message: " << e.what();
        return e.code().value();
    }
    return 0;
}
