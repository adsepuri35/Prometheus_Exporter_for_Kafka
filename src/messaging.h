#ifndef MESSAGING_H
#define MESSAGING_H

#include <zmq.hpp>
#include <string>

// Function declaration
void trackAndSendMessage(zmq::socket_t& socket, const std::string& message);

#endif // MESSAGING_H