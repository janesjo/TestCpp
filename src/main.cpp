//
//  main.cpp
//  TestCpp
//
//  Created by Jonathan Janesjo on 2026-04-07.
//
//

#include <cerrno>
#include <cstring>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "network.h"


void product(std::promise<int>&& intPromise, int a, int b){
  intPromise.set_value(a*b);
}

struct Div{

  void operator() (std::promise<int>&& intPromise, int a, int b) const {
    intPromise.set_value(a/b);
  }

};

void messageSender(unsigned short port) {
  SocketHandle sender = openUdpSocket();
  const std::string message = "Hello from openUdpSocket";

  if (sendUdpMessage(sender, "127.0.0.1", port, message)) {
    std::cout << "Sent UDP message: " << message << std::endl;
  } else {
    std::cerr << "Failed to send UDP message: " << std::strerror(errno) << std::endl;
  }

  closeSocket(sender);
}

void messageFunction(SocketHandle receiver, std::promise<std::string>&& messagePromise) {
  std::string receivedMessage;
  std::cout << "Waiting to receive UDP hello message..." << std::endl;
  if (receiveUdpMessage(receiver, receivedMessage)) {
    std::cout << "Received UDP message: " << receivedMessage << std::endl;
  } else {
    std::cerr << "Failed to receive UDP message: " << std::strerror(errno) << std::endl;
  }

  messagePromise.set_value(receivedMessage);
}

int main(){

  int a= 20;
  int b= 10;

  
  

  std::cout << std::endl;

  // define the promises
  std::promise<int> prodPromise;
  std::promise<int> divPromise;
  std::promise<std::string> messagePromise;
  SocketHandle receiver = openUdpSocket();
  const unsigned short receiverPort = getSocketPort(receiver);

  // get the futures
  std::future<int> prodResult= prodPromise.get_future();
  std::future<int> divResult= divPromise.get_future();
  std::future<std::string> messageResult= messagePromise.get_future();

  // calculate the result in a separat thread
  std::thread prodThread(product,std::move(prodPromise),a,b);
  Div div;
  std::thread divThread(div,std::move(divPromise),a,b);
  std::thread messageThread(messageFunction,receiver,std::move(messagePromise));
  std::thread messageSenderThread(messageSender,receiverPort);

  // get the result
  std::cout << ":-) 20*10= " << prodResult.get() << std::endl;
  std::cout << ":-) 20/10= " << divResult.get() << std::endl;
  std::cout << "Message: " << messageResult.get() << std::endl;

  prodThread.join();
  divThread.join();
  messageThread.join();
  messageSenderThread.join();
  closeSocket(receiver);

  std::cout << std::endl;


}
