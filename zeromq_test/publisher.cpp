#include <zmq.hpp>
#include <string>
#include <iostream>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <prometheus/counter.h>
#include <prometheus/gauge.h>
#include <prometheus/summary.h>
#include <chrono>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n) Sleep(n)
#endif

int main() {
    zmq::context_t context(2);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    prometheus::Exposer exposer{"127.0.0.1:8080"};
    auto registry = std::make_shared<prometheus::Registry>();

    // Gauges and counters
    auto& socket_connections_family = prometheus::BuildGauge()
        .Name("zmq_socket_connections")
        .Help("Number of active connections.")
        .Register(*registry);
    auto& socket_connections_metric = socket_connections_family.Add({});

    auto& messages_sent_family = prometheus::BuildCounter()
        .Name("zmq_socket_messages_sent")
        .Help("Total messages sent.")
        .Register(*registry);
    auto& messages_sent_metric = messages_sent_family.Add({});

    auto& messages_received_family = prometheus::BuildCounter()
        .Name("zmq_socket_messages_received")
        .Help("Total messages received.")
        .Register(*registry);
    auto& messages_received_metric = messages_received_family.Add({});

    // Build the summary families
    auto& latency_summary_family = prometheus::BuildSummary()
        .Name("zmq_latency")
        .Help("Latency of message processing.")
        .Register(*registry);

    // Pass quantiles in Add(...)
    auto& latency_summary_metric = latency_summary_family.Add(
        {},
        prometheus::Summary::Quantiles{ {0.5, 0.05}, {0.9, 0.01}, {0.99, 0.001} },
        std::chrono::milliseconds(60000), // max age
        5                                 // number of buckets
    );

    // Build the throughput summary family
    auto& throughput_summary_family = prometheus::BuildSummary()
        .Name("zmq_throughput")
        .Help("Throughput of message processing.")
        .Register(*registry);

    auto& throughput_summary_metric = throughput_summary_family.Add(
        {},
        prometheus::Summary::Quantiles{ {0.5, 0.05}, {0.9, 0.01}, {0.99, 0.001} },
        std::chrono::milliseconds(60000),
        5
    );

    exposer.RegisterCollectable(registry);

    while (true) {
        zmq::message_t request;
        auto start = std::chrono::high_resolution_clock::now();
        auto recv_result = socket.recv(request, zmq::recv_flags::none);
        auto end = std::chrono::high_resolution_clock::now();
        if (recv_result) {
            double latency = std::chrono::duration<double>(end - start).count();
            latency_summary_metric.Observe(latency);
            messages_received_metric.Increment();
            std::cout << "Received Hello" << std::endl;
        }

        sleep(1);

        zmq::message_t reply(5);
        memcpy(reply.data(), "World", 5);
        socket.send(reply, zmq::send_flags::none);
        messages_sent_metric.Increment();

        socket_connections_metric.Set(1);
        // queue_length_metric.Set(...);
        // queue_overflows_metric.Increment();
        throughput_summary_metric.Observe(1);
    }

    return 0;
}