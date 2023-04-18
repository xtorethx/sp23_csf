#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  // TODO: connect to server
  Connection receiver;
  receiver.connect(server_hostname, server_port);
  if (!receiver.is_open()) {
    std::cerr << "Connection cannot be opened!" << std::endl;
    return 1;
  }

  // TODO: send rlogin and join messages (expect a response from the server for each one)
  // send rlogin
  Message rlogin;
  rlogin.tag = TAG_RLOGIN;
  rlogin.data = username;
  receiver.send(rlogin);

  // return message
  Message ret_msg;
  receiver.receive(ret_msg);
  if (ret_msg.tag != TAG_OK) {
    std::cerr << ret_msg.data << std::endl;
    return 1;
  }

  Message join;
  join.tag = TAG_JOIN;
  join.data = room_name;
  receiver.send(join);

  receiver.receive(ret_msg);
  if (ret_msg.tag != TAG_OK) {
    std::cerr << ret_msg.data << std::endl;
    return 1;
  }
 
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  std::string line;
  while(true) {//read in line
    if (line.empty()) {
      continue;
    }

    // handle delivery
    std::string room;
    std::string sender;
    std::string message;
    Message msg;

    // delivery tag:room:sender:message
    // split msg.data into room and sender and msg
    receiver.receive(msg);

    if (msg.tag == "delivery") {
      // compare with room for match
      if (room == room_name) {
        std::stringstream ss(msg.data);
        std::getline(ss, room, ':'); // room
        std::getline(ss, sender, ':'); // sender
        std::getline(ss, sender, ':'); // message
        // display
        std::cout << sender << ":" << message;
      }
    }
    line.clear();
  }

  return 0;
}