# UDP-Pinger
Two simple programs: one for sending UDP pings to a server, and one that will run the server responding to pings

server.cpp will create a socket bound to the specific IPv4/port and respond to any pings it receives. <br>

client.cpp will send a ping to the specific IPv4/port, receive the reponse ping, and output the round trip time. <br>

Server usage: ./server IPv4 Port <br>
Example: ./server 127.0.0.1 8888 <br>

Client usage: ./client IPv4 Port <br>
Example: ./client 127.0.0.1 8888 <br>

Simple compilation commands: <br>
g++ server.cpp -o server <br>
g++ client.cpp -o client <br>
