#pragma once
#include "boost/asio.hpp"
#include <iostream>
using namespace std;
using namespace boost;
#include <queue>
//最大报文接收大小
const int RECVSIZE = 1024;
class  MsgNode {
public:
	MsgNode(const char* msg, int total_len) : _total_len(total_len), _cur_len(0) {
		_msg = new char[total_len];
		memcpy(_msg, msg, total_len);
	}

	MsgNode(int total_len) :_total_len(total_len), _cur_len(0) {
		_msg = new char[total_len];
	}

	~MsgNode() {
		delete[]_msg;
	}
	//消息首地址
	char* _msg;
	//总长度
	int _total_len;
	//当前长度
	int _cur_len;
};

class Session {
public:
	Session(std::shared_ptr<asio::ip::tcp::socket> socket);
	void Connect(const asio::ip::tcp::endpoint& ep);
	void WriteCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void WriteToSocket(const std::string& buf);
	void WriteAllToSocket(const std::string& buf);
	void WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void ReadFromSocket();
	void ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void ReadAllFromSocket(const std::string& buf);
	void ReadAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void WriteCallBackErr(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<MsgNode>);
	//有隐患的发送函数，不提倡
	void WriteToSocketErr(const std::string& buf);
private:
	std::queue<std::shared_ptr<MsgNode>> _send_queue;
	std::shared_ptr<asio::ip::tcp::socket> _socket;
	std::shared_ptr<MsgNode> _recv_node;
	std::shared_ptr<MsgNode> _send_node;
	bool _send_pending;
	bool _recv_pending;
};
