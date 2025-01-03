#pragma once
#include <prometheus/registry.h>
#include <memory>

// External registry
extern std::shared_ptr<prometheus::Registry> registry;

// Function declarations
void trackConnectionOpened();
void trackConnectionClosed();
void trackMessageSent();
void trackMessageReceived();
void trackError();
void initMetrics();
void startMetricsServer(int port);