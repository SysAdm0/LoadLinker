# LoadLinker
LoadLinker serves as a network discovery tool designed to assist load balancers such as Nginx or HAproxy in efficiently directing traffic to the appropriate server.
It swiftly identifies a healthy server within a second and seamlessly incorporates it into the load balancer's configuration.
By employing LoadLinker, you enhance the availability of your service and mitigate the risk of downtime.

### Operation Overview
To get started with LoadLinker you need to understand the client-server communication, with distinct roles assigned to each component. The server assumes the vital task of monitoring server health, while the client facilitates communication with LoadLinker.

The agent on the client initiate a request to the server to obtain its IP address. The server, conducting health checks every second, dynamically decides whether to register the client in the configuration or not.

### Build Instructions
```bash
mkdir build && cd build
cmake -G "Unix Makefiles" .. && cmake --build .
```

### Testing Procedure
To run the test, execute the following command in your terminal:
```bash
cd test && ./run_test.sh
```
If you want to check the test results, you can display upstream file like this:
```bash
docker exec -it loadbalancer /bin/watch -n1 "cat /etc/loadlinker/upstream.conf"
```
To stop the test and remove the containers, run the command on the test directory:
```bash
docker-compose -f docker-compose.yml down
```