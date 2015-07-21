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
 
using namespace std;
typedef std::complex<double> sdlab_complex;
 
#endif

#define POINT_1SEC 2097152

int main()
{
  int i;
  double noise;
  double p_signal = 1.0;
  int f01k = 64;//Hz
  int f02k = 128;//Hz
  int f04k = 256;//Hz
  int f08k = 512;//Hz
  int f16k = 1024;//Hz
  int f32k = 16384;//Hz
  int f64k = 32768;//Hz

  int id = 0;
  int flag = 0;

  int sock;
  struct sockaddr_in addr0, addr1;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr0.sin_family = AF_INET;
  addr0.sin_port = htons(0x4000);
  addr0.sin_addr.s_addr = inet_addr("127.0.0.1");

  addr1.sin_family = AF_INET;
  addr1.sin_port = htons(0x4001);
  addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  srand(0);

  while(1){
    id = 0;
    for(i = 0; i < POINT_1SEC; i++){
      double s;
      double s01k, s02k, s04k, s08k, s16k, s32k, s64k;
      unsigned short val;
      unsigned char buf[2048];

      noise = rand();
      noise = noise / (double)RAND_MAX;
      noise = noise * 18.0 - 9.0;

      s01k = p_signal
        * cos(2 * M_PI * ((double) f01k * i) /  (double) POINT_1SEC);
      s02k = p_signal
        * cos(2 * M_PI * ((double) f02k * i) /  (double) POINT_1SEC);
      s04k = p_signal
        * cos(2 * M_PI * ((double) f04k * i) /  (double) POINT_1SEC);
      s08k = p_signal
        * cos(2 * M_PI * ((double) f08k * i) /  (double) POINT_1SEC);
      s16k = p_signal
        * cos(2 * M_PI * ((double) f16k * i) /  (double) POINT_1SEC);
      s32k = p_signal
        * cos(2 * M_PI * ((double) f32k * i) /  (double) POINT_1SEC);
      s64k = p_signal
        * cos(2 * M_PI * ((double) f64k * i) /  (double) POINT_1SEC);
//      s = s01k + s02k + s04k + s08k + s16k + s32k + s64k + noise;

      s = (s01k + s02k + s04k + s08k + s16k + s32k + s64k + noise) / 8 *
        32767.0;
//      s = s64k * ;
//      s = s + 71.0;
//      printf("%f\n", s);
      val = (unsigned short) (s);
//      printf("%05d %d\n", val, 4 + (i % 512) * 2);
      *((unsigned short *) (buf + 4 + (i % 512) * 2)) = htons(val);

      if((i + 1) % 512 == 0){
        int ret;
        *((int *) buf) = htonl(id);
        id += 512;
//        printf("id = %d, size = %d\n", id, sizeof(buf));

        if(flag == 0){
          ret = sendto(sock, buf, 1028, 0,
                       (struct sockaddr *)&addr0, sizeof(addr0));
        }else{
          ret = sendto(sock, buf, 1028, 0,
                       (struct sockaddr *)&addr1, sizeof(addr0));
        }
        
//        printf("ret = %d\n", ret);
//        exit(0);
      }
    }

    if(flag == 0){
      flag = 1;
    }else{
      flag = 0;
      printf("sleep\n");
      sleep(1);
    }
  }
}
