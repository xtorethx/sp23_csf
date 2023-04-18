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
  if (ret_msg.tag == TAG_ERR) {
    std::cerr << ret_msg.data << std::endl;
    return 1;
  }

  Message join;
  join.tag = TAG_JOIN;
  join.data = room_name;
  receiver.send(join);

  receiver.receive(ret_msg);
  if (ret_msg.tag == TAG_ERR) {
    std::cerr << ret_msg.data << std::endl;
    return 1;
  }
 
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  std::string line = NULL;
  while(true) {//read in line
    if (line.empty()) {
      continue;
    }

    // handle delivery
    std::stringstream ss(line); // convert line to stringstream
    std::string word;
    std::string sender;
    std::string message;
    Message msg;

    // delivery tag:room:sender:message
    // split msg.data into room and sender and msg
    std::getline(ss, word, ':'); // delivery tag
    if (word == "delivery") {
      msg.tag = word;
      std::getline(ss, word, ':'); // room
      // compare with room for match
      if (word == room_name) {
        msg.data = word;
        std::getline(ss, word, ':'); // sender
        sender = word;
        std::getline(ss, word, ':'); // message
        message = word;
        // display
        std::cout << sender << ": " << message;
      }
    }
  }

  return 0;
}
