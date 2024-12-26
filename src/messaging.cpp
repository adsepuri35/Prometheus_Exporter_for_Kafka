#include "messaging.h"
#include "metrics.h"
#include <zmq.hpp>
#include <iostream>

void trackAndSendMessage(zmq::socket_t& socket, const std::string& message) {
    try {
        std::cout << "Sending message..." << std::endl;
        zmq::message_t zmq_message(message.size());
        memcpy(zmq_message.data(), message.data(), message.size());
        
        // Send message (blocking)
        socket.send(zmq_message, zmq::send_flags::none);
        std::cout << "Message sent." << std::endl;

        // Increment counter
        incrementMessageCounter();
    }
    catch (const zmq::error_t& e) {
        std::cerr << "ZeroMQ error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred in trackAndSendMessage." << std::endl;
    }
}