# Prometheus Exporter for ZeroMQ (In Progress)

C++ exporter to integrate Prometheus metrics with a ZeroMQ application.

## Metrics

The following metrics are exposed:

### 1. Socket Metrics
- zmq_socket_connections: Number of active connections.
- zmq_socket_messages_sent: Total messages sent.
- zmq_socket_messages_received: Total messages received.
- zmq_socket_errors: Number of errors encountered.

### 2. Message Queue Metrics
- zmq_queue_length: Current length of the message queue.
- zmq_queue_max_length: Maximum length of the message queue.
- zmq_queue_overflows: Number of times the queue overflowed.

### 3. Latency Metrics
- zmq_latency_avg: Average latency of message processing.
- zmq_latency_max: Maximum latency of message processing.

### 4. Throughput Metrics
- zmq_throughput_avg: Average messages processed per second.
- zmq_throughput_max: Maximum messages processed per second.