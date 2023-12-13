# SOCKETCHATAPP.C
## A TPC SERVER AND CLIENT SERVER FOR A CHAT APP WRITTEN WITH C.



#### You can talk to someone uses unix socket(Linux,MacOS,etc.) anywhere in the world.
Clients and Server uses TCP Sockets to communicate. It's just so easy to run it.
I'd recommend you AWS EC2 Instance for this job but of course you can use whatever server you want which has unix socket.

### Setup Steps:

#### 1- You need to clone my repo in your server:
```
$ git clone https://github.com/Egel3n/SOCKETCHATAPP.C.git
```
#### 2- You need an C compiler like gcc or clang:
```
$ gcc Server.c -o server 
```
#### 3-After compiling and getting the output file, you need to run server:
```
$ ./server <PORT>
example: ./server 8080
```
Your server must be up and running right now.

#### 4- Now you need your clients. Clone this repo to your client pc and :
```
$ gcc Client.c -o client
```
#### 5- AND Final Step:
```
$ ./client <IpAddresssofYourServer> <PORT> <Username>
example: ./client 0.0.0.0 8080 Egelen
```

##### Now connect your firends up to 10 clients and start chatting.
