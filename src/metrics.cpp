#include "metrics.h"
#include <prometheus/exposer.h>
#include <prometheus/counter.h>
#include <memory>
#include <string>

// Define the registry in exactly one .cpp file
std::shared_ptr<prometheus::Registry> registry = std::make_shared<prometheus::Registry>();
static auto& message_counter_family =
    prometheus::BuildCounter()
        .Name("messages_sent_total")
        .Help("Total number of messages sent")
        .Register(*registry);

// Keep Exposer alive by defining it globally
std::unique_ptr<prometheus::Exposer> exposer_ptr;

void incrementMessageCounter() {
    static auto& message_counter = message_counter_family.Add({});
    message_counter.Increment();
}

void initMetrics() {
    // Initialization can be done here if needed
}

void startMetricsServer(int port) {
    std::string address = "0.0.0.0:" + std::to_string(port);
    exposer_ptr = std::make_unique<prometheus::Exposer>(address);
    exposer_ptr->RegisterCollectable(registry);
}