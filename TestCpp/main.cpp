//
//  main.cpp
//  TestCpp
//
//  Created by Jonathan Janesjo on 2026-04-07.
//

#include <future>
#include <iostream>
#include <thread>
#include <utility>

// Iterative Fibonacci: returns the n-th Fibonacci number (0-indexed)
// fib(0) = 0, fib(1) = 1, fib(2) = 1, ...
long long fib(int n) {
  if (n < 0) {
    throw std::invalid_argument("n must be non-negative");
  }
  if (n == 0) return 0;
  if (n == 1) return 1;
  long long a = 0;
  long long b = 1;
  for (int i = 2; i <= n; ++i) {
    long long next = a + b;
    a = b;
    b = next;
  }
  return b;
}

void product(std::promise<int>&& intPromise, int a, int b){
  intPromise.set_value(a*b);
}

struct Div{

  void operator() (std::promise<int>&& intPromise, int a, int b) const {
    intPromise.set_value(a/b);
  }

};

int main(){

  int a= 20;
  int b= 10;

  std::cout << std::endl;

  // demo Fibonacci
  int n = 10;
  std::cout << "fib(" << n << ") = " << fib(n) << std::endl;

  // define the promises
  std::promise<int> prodPromise;
  std::promise<int> divPromise;

  // get the futures
  std::future<int> prodResult= prodPromise.get_future();
  std::future<int> divResult= divPromise.get_future();

  // calculate the result in a separat thread
  std::thread prodThread(product,std::move(prodPromise),a,b);
  Div div;
  std::thread divThread(div,std::move(divPromise),a,b);

  // get the result
  std::cout << "20*10= " << prodResult.get() << std::endl;
  std::cout << "20/10= " << divResult.get() << std::endl;

  prodThread.join();
  
  divThread.join();

  std::cout << std::endl;

}

