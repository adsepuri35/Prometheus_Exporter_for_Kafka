#include "metrics.h"
#include "messaging.h"
#include <iostream>
#include <zmq.hpp>
#include <thread>

void monitorSocket(zmq::context_t& context, zmq::socket_t& monitored_socket) {
    // Request monitoring for connected / accepted / disconnected events
    int rc = zmq_socket_monitor(monitored_socket.handle(), "inproc://monitor.socket",
                                ZMQ_EVENT_ACCEPTED | ZMQ_EVENT_DISCONNECTED);
    if (rc != 0) {
        std::cerr << "zmq_socket_monitor failed: " << zmq_strerror(zmq_errno()) << '\n';
        return;
    }

    // Create a socket to receive events
    zmq::socket_t monitor(context, ZMQ_PAIR);
    monitor.connect("inproc://monitor.socket");

    while (true) {
        zmq::message_t event_msg;
        zmq::message_t addr_msg;

        // Receive two-part monitor message
        if (!monitor.recv(event_msg, zmq::recv_flags::none)) break;
        if (!monitor.recv(addr_msg, zmq::recv_flags::none)) break;

        // Parse event
        auto* data = static_cast<zmq_event_t*>(event_msg.data());
        switch (data->event) {
            case ZMQ_EVENT_ACCEPTED:
                trackConnectionOpened();
                break;
            case ZMQ_EVENT_DISCONNECTED:
                trackConnectionClosed();
                break;
            default:
                break;
        }
    }
}

int main() {
    std::cout << "Initializing Prometheus metrics..." << '\n';
    initMetrics();

    std::cout << "Starting Prometheus metrics server on port 8080..." << '\n';
    startMetricsServer(8080);

    std::cout << "Creating ZeroMQ context and socket..." << '\n';
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUSH);
    socket.bind("tcp://*:5555");

    // Start monitoring in a separate thread
    std::thread monitor_thread([&context, &socket]() {
        monitorSocket(context, socket);
    });

    std::cout << "Sending messages..." << '\n';
    for (int i = 1; i <= 15; i++) {
        try {
            trackAndSendMessage(socket, "Hello, World!");
            std::cout << "Sent message " << i << '\n';
        } catch (const zmq::error_t& e) {
            trackError();
            std::cerr << "Error sending message: " << e.what() << '\n';
        }
    }

    std::cout << "All messages sent. Press Enter to exit." << '\n';
    std::cin.get();

    // Clean up
    monitor_thread.join();
    std::cout << "Exiting program." << '\n';
    return 0;
}