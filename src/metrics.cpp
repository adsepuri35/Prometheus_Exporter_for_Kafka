#include "metrics.h"
#include <prometheus/exposer.h>
#include <prometheus/counter.h>
#include <prometheus/gauge.h>
#include <memory>
#include <string>

// Define the registry in exactly one .cpp file
std::shared_ptr<prometheus::Registry> registry = std::make_shared<prometheus::Registry>();

// static auto& message_counter_family =
//     prometheus::BuildCounter()
//         .Name("messages_sent_total")
//         .Help("Total number of messages sent")
//         .Register(*registry);

static auto& socket_connections_gauge = 
    prometheus::BuildGauge()
        .Name("zmq_socket_connections")
        .Help("# of active connections")
        .Register(*registry)
        .Add({});

static auto& messages_sent_counter = 
    prometheus::BuildCounter()
        .Name("zmq_socket_messages_sent")
        .Help("# of messages sent")
        .Register(*registry)
        .Add({});

static auto& messages_received_counter =
    prometheus::BuildCounter()
        .Name("zmq_socket_messages_received")
        .Help("# of messages received")
        .Register(*registry)
        .Add({});

static auto& errors_counter = 
    prometheus::BuildCounter()
        .Name("zmq_socket_errors")
        .Help("Number of errors encountered")
        .Register(*registry)
        .Add({});

// Keep Exposer alive by defining it globally
std::unique_ptr<prometheus::Exposer> exposer_ptr;

// void incrementMessageCounter() {
//     static auto& message_counter = message_counter_family.Add({});
//     message_counter.Increment();
// }

void trackConnectionOpened() {
    socket_connections_gauge.Increment();
}

void trackConnectionClosed() {
    socket_connections_gauge.Decrement();
}

void trackMessageSent() {
    messages_sent_counter.Increment();
}

void trackMessageReceived() {
    messages_received_counter.Increment();
}

void trackError() {
    errors_counter.Increment();
}

void initMetrics() {
    // Initialization can be done here if needed
}

void startMetricsServer(int port) {
    std::string address = "0.0.0.0:" + std::to_string(port);
    exposer_ptr = std::make_unique<prometheus::Exposer>(address);
    exposer_ptr->RegisterCollectable(registry);
}