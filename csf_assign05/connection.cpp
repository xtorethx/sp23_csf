#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  m_fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (m_fd >= 0) {
    Close(m_fd);
  }
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  if (m_fd >= 0) {
    Close(m_fd);
    m_fd = -1;
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  std::string message;
  message = msg.tag + ":" + msg.data +"\n";

  int bytes_sent = rio_writen(m_fd, message.c_str(), message.length());
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  m_last_result = (bytes_sent == sizeof(msg)) ? SUCCESS : EOF_OR_ERROR;
  
  return m_last_result == SUCCESS;
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  char message[Message::MAX_LEN];
  int bytes_received = rio_readlineb(&m_fdbuf, message, Message::MAX_LEN);//use rio_readlineb
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  if (bytes_received == 0) {
    m_last_result = EOF_OR_ERROR;
  } else if (bytes_received == sizeof(msg)) {
    m_last_result = SUCCESS;
  } else {
    m_last_result = INVALID_MSG;
  }

  //convert message from char array to string
  std::string sarray(message);
  std::string tag;
  std::string data;

  size_t del = sarray.find(':');
  tag = sarray.substr(0, del);
  data = sarray.substr(del+1);
  msg.tag = tag;
  msg.data = data;

  return m_last_result == SUCCESS;
}
