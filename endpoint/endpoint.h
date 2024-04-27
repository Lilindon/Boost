#pragma once
#ifndef __END_POINT_H__
#define __END_POINT_H__

#include<boost/asio.hpp>
#include<iostream>


// #pragma once 防止文件被重复包含
extern int client_end_point();// extern 告诉系统这只是一个声明，定义让编译器自己找  不加会使得不同的cpp中包含endpoint.h头文件中，都会需要该函数的重定义  
extern int server_end_point();// 服务器端端点
extern int create_tcp_socket();// 创建TCP socket
extern int create_acceptor_socket();// 创建服务器端 acceptor socket 用来接收新连接
extern int bind_acceptor_socket();// 绑定服务器端 acceptor socket
extern int connect_to_end();// 客户端 连接 服务器端 端点 
extern int dns_connect_to_end();// 客户端 通过 DNS 连接服务器端 端点
extern int accept_new_connection();// 服务器端 acceptor socket 接收新的连接

extern void use_const_buffer();
extern void use_buffer_str();
extern void use_buffer_array();


#endif // !__END_POINT_H__