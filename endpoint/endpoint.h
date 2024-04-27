#pragma once
#ifndef __END_POINT_H__
#define __END_POINT_H__

#include<boost/asio.hpp>
#include<iostream>


// #pragma once ��ֹ�ļ����ظ�����
extern int client_end_point();// extern ����ϵͳ��ֻ��һ�������������ñ������Լ���  ���ӻ�ʹ�ò�ͬ��cpp�а���endpoint.hͷ�ļ��У�������Ҫ�ú������ض���  
extern int server_end_point();// �������˶˵�
extern int create_tcp_socket();// ����TCP socket
extern int create_acceptor_socket();// ������������ acceptor socket ��������������
extern int bind_acceptor_socket();// �󶨷������� acceptor socket
extern int connect_to_end();// �ͻ��� ���� �������� �˵� 
extern int dns_connect_to_end();// �ͻ��� ͨ�� DNS ���ӷ������� �˵�
extern int accept_new_connection();// �������� acceptor socket �����µ�����

extern void use_const_buffer();
extern void use_buffer_str();
extern void use_buffer_array();


#endif // !__END_POINT_H__