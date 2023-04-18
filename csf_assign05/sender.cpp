#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection sender;
  sender.connect(server_hostname, server_port);
  if (!sender.is_open()) {
    std::cerr << "Connection cannot be opened!" << std::endl;
    return 1;
  }

  // TODO: send slogin message
  Message slogin_msg;
  slogin_msg.tag = TAG_SLOGIN;
  slogin_msg.data = username;
  sender.send(slogin_msg);

  Message ret_msg;
  sender.receive(ret_msg);
  if (ret_msg.tag != TAG_OK) {
    std::cerr << ret_msg.data << std::endl;
    return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  std::string line;
  std::string room_name;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    std::stringstream sin(line);
    std::string word;
    sin >> word;
    Message msg;
    if (word.compare("/join")) {
      msg.tag = TAG_JOIN;

      std::string::size_type n = 0;
      n = line.find_first_not_of( " \t", n );
      n = line.find_first_of( " \t", n );
      line.erase( 0,  line.find_first_not_of( " \t", n ) );
      room_name = line;
      msg.data = room_name;
      sender.send(msg);
      
      sender.receive(ret_msg);
      if (ret_msg.tag != TAG_OK) {
        std::cerr << ret_msg.data << std::endl;
        return 1;
      }

    } else if (word.compare("/leave")) {
      msg.tag = TAG_LEAVE;
      msg.data = room_name;
      sender.send(msg);
      
      sender.receive(ret_msg);
      if (ret_msg.tag != TAG_OK) {
        std::cerr << ret_msg.data << std::endl;
      }
      
    } else if (word.compare("/quit")) {
      msg.tag = TAG_LEAVE;
      msg.data = room_name;
      sender.send(msg);
      
      sender.receive(ret_msg);
      if (ret_msg.tag == TAG_ERR) {
        std::cerr << ret_msg.data << std::endl;
        return 1;
      } else if (ret_msg.tag == TAG_OK) {
        return 0;
      } else {
        return 2;
      }

    } else if (word[0] == '/') {
      std::cerr << "Invalid command" << std::endl;

    } else {
      msg.tag = TAG_SENDALL;
      msg.data = trim(line);
      sender.send(msg);

      sender.receive(ret_msg);
      if (ret_msg.tag != TAG_OK) {
        std::cerr << ret_msg.data << std::endl;
      }
    }
    line.clear();
  }

  return 0;
}
