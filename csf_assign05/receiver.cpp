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

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message rlogin;
  rlogin.tag = TAG_RLOGIN;
  rlogin.data = username;
  conn.send(rlogin);

  Message join;
  join.tag = TAG_JOIN;
  join.data = room_name;
  conn.send(join);
  

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  std::string line = NULL;
  while(std::getline(std::cin, line)) {
    std::stringstream ss;

    //convert to stringstream
    //check if first word matches command
    //remove first word
    //continue action with rest of method (deliver)
    //
  }

  return 0;
}
