# Build commands
```
g++ -o udp_multicast_sender udp_multicast_sender.cpp
g++ -o udp_multicast_receiver udp_multicast_receiver.cpp
```

# Monitoring
```
$ sudo tcpdump -i <interface> udp and multicast and host <multicast-group> and port <port>
$ sudo tcpdump -i <interface> udp and multicast and host <multicast-group> and port <port> -X

$ sudo tcpdump -i eno1 udp and multicast and host 239.0.0.1 and port 12345 -X
```


