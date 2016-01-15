#ifndef SDLAB_H
#define SDLAB_H
 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errors.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <float.h>
 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>
 
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
 
#include <vector>
#include <complex>
#include <string>

#include "cmdline.h"
 
using namespace std;
typedef std::complex<double> sdlab_complex;
 
#endif

#define POINT_1SEC 2097152

int main(int argc, char **argv)
{
  cmdline::parser p;
  p.add<int>("freq", 'f', "frequency of sine wave.",
             false, 20);
  p.add<string>("dst", 'd', "destination ip address.", false, "127.0.0.1");
  p.add("help", 'h', "print help");

  p.parse(argc, argv);
  if(p.parse(argc, argv) == false || p.exist("help") == true){
    printf("%s\n", p.error_full().c_str());
    printf("%s", p.usage().c_str());
    return 0;
  }

  int freq = p.get<int>("freq");
  string str_ip = p.get<string>("dst");

  int i;
  double noise;
  double p_signal = 1.0;
  int id = 0;
  int flag = 0;

  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(0x4000);
  addr.sin_addr.s_addr = inet_addr(str_ip.c_str());

  srand(0);

  while(1){
    id = 0;
    for(i = 0; i < POINT_1SEC; i++){
      double s;
      double s01k;

      short val;
      unsigned char buf[2048];

      s = p_signal
        * cos(2 * M_PI * ((double) freq * (((double)i) / 2.0)) /
              (double) POINT_1SEC);
      s = s * 32767.0;

      val = (short) (s);

      *((short *) (buf + 4 + (i % 256) * 4)) = htons(val);
      *((short *) (buf + 4 + (i % 256) * 4 + 2)) = htons(val / 2);

      if((i + 1) % 256 == 0){
        int ret;
        *((int *) buf) = htonl(id);
        id += 512;
        ret = sendto(sock, buf, 1028, 0,
                     (struct sockaddr *)&addr, sizeof(addr));
      }
    }

    printf("sleep\n");
    sleep(1);
  }
}
