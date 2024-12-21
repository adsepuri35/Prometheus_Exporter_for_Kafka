#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include <prometheus/counter.h>
#include <thread>
#include <chrono>

int main() {
    // Create an exposer to expose metrics
    prometheus::Exposer exposer{"127.0.0.1:8080"};

    // Create a registry to hold metrics
    auto registry = std::make_shared<prometheus::Registry>();

    // Create a counter
    auto& counter_family = prometheus::BuildCounter()
        .Name("test_counter")
        .Help("Just a test")
        .Register(*registry);

    auto& counter = counter_family.Add({{"label", "value"}});

    // Expose the registry via HTTP
    exposer.RegisterCollectable(registry);

    // Increment counter every second
    while (true) {
        counter.Increment();
        std::cout << "Counter value: " << counter.Value() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}