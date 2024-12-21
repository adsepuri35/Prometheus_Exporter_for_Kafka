#include <prometheus/counter.h>
#include <prometheus/registry.h>
#include <iostream>

int main() {
    auto registry = std::make_shared<prometheus::Registry>();
    auto& counter_family = prometheus::BuildCounter()
        .Name("test_counter")
        .Help("Just a test")
        .Register(*registry);

    auto& counter = counter_family.Add({{"label", "value"}});
    counter.Increment();

    std::cout << "Counter value: " << counter.Value() << std::endl;
    return 0;
}