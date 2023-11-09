# LoadLinker
LoadLinker serves as a network discovery tool designed to assist load balancers such as Nginx or HAproxy in efficiently directing traffic to the appropriate server.
It swiftly identifies a healthy server within a second and seamlessly incorporates it into the load balancer's configuration.
By employing LoadLinker, you enhance the availability of your service and mitigate the risk of downtime.

### Operation Overview
To get started with LoadLinker you need to understand the client-server communication, with distinct roles assigned to each component. The server assumes the vital task of monitoring server health, while the client facilitates communication with LoadLinker.

The agent on the client initiate a request to the server to obtain its IP address. The server, conducting health checks every second, dynamically decides whether to register the client in the configuration or not.

#### Running the Server
To launch the server, execute the following command in your terminal:

```bash
./LoadLinker <port_to_bind>
```

#### Running the Agent
To activate the agent, use the following command:

```bash
./LoadLinkerAgent <server_ip> <server_port>
```

### Build Instructions
```bash
mkdir build && cd build
cmake -G "Unix Makefiles" .. && cmake --build .
```