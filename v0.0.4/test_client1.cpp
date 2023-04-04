#include "client.h"

int main(){
  client c(8888,"127.0.0.1");
  c.run();
}