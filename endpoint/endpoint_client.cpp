#include "endpoint_client.h"
#include <iostream>
#include <boost/asio.hpp>
#include <string.h>
#include <cstring> // strlen
#include <cstddef>

using namespace std;
using namespace boost::asio::ip;
const int MAX_LENGTH = 1024;

void client(){
    // endpoint and socket, socket.connect(ep,error);
    boost::asio::io_context ioc;
    tcp::endpoint remote_ep(address::from_string("127.0.0.1"), 10086);
    tcp::socket sock(ioc);
    boost::system::error_code error = boost::asio::error::host_not_found;
    sock.connect(remote_ep, error);
    if (error) {
        cout << "connect failed,code is " << error.value() << " error msg is " << error.message();
    }
    // input message
    cout << " Enter message: ";
    char request[MAX_LENGTH];
    memset(request, 0x00, MAX_LENGTH); // ��ʼ��
    cin.getline(request, MAX_LENGTH);
    size_t request_length = strlen(request); // ��ʼɨ�裬ֱ��������һ���ַ���������'\\0'Ϊֹ��Ȼ�󷵻ؼ�����ֵ(���Ȳ�����'\\0')
    //strlen()����ʵ�ʴ�С  sizeof()�����ַ��ķ����С
    boost::asio::write(sock, boost::asio::buffer(request, request_length));

    char reply[MAX_LENGTH];
    size_t reply_length = strlen(reply);
    boost::asio::buffer(reply, reply_length);
    cout << "reply is ";
    cout.write(reply, reply_length);

}

int main(){
    try{
        client();
    }catch(std::exception &e){
        cerr<<" Exception: "<<e.what()<<endl;// cerr ��׼��������������Ϣ�������� ֱ����ʾ �����
    }
    return 0;
}