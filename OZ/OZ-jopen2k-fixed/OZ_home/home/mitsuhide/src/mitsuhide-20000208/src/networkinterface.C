#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include "networkinterface.h"


// コンストラクタ
NetworkInterface::NetworkInterface(void) {
  sock.socketfd = -1;
}


// 接続
bool NetworkInterface::connect(char* host, int port) {
  struct hostent *host_ent;
  struct in_addr *addr_ptr;
  int one;

  // ネットワークアドレスを得る  
  if ((host_ent = (struct hostent *)gethostbyname(host)) == NULL) {
    /* Check if a numeric address */
    if (inet_addr(host) == 0) {
      fprintf(stderr, "Can't find the host.\n");
      return false;
    } 
  } else {
    addr_ptr = (struct in_addr *)*host_ent->h_addr_list;
    host = inet_ntoa(*addr_ptr);
  }

  // ソケットを開く
  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stderr, "Can't open socket.\n") ;
    return false;
  }

  // シグナルの設定
  signal(SIGIO, SIG_IGN);
  if (fcntl(sockfd, F_SETOWN, getpid()) < 0) {
     fprintf(stderr, "Can't fcntl on socket.\n") ;
     return false;
  }
  one = fcntl(sockfd, F_GETFL, 0);
  one |= FASYNC;
   if (fcntl(sockfd, F_SETFL, one) < 0) {
     fprintf(stderr, "Can't fcntl on socket.\n") ;
     return false;
  }
  one = 1 ;
  if (ioctl(sockfd, FIONBIO, &one) < 0) {
    fprintf(stderr, "Can't ioctl on socket.\n") ;
    return false;
  }
  
  // ソケットのバインド
  struct sockaddr_in cli_addr;
  bzero((char *) &cli_addr, sizeof(cli_addr));
  cli_addr.sin_family = AF_INET;
  cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  cli_addr.sin_port = htons(0);
  if (bind(sockfd, (struct sockaddr *)&cli_addr,
	   sizeof(cli_addr)) < 0) {
    fprintf(stderr, "Can't bind local address.\n");
    return false;
  }
    
  // サーバアドレスの書き込み
  sock.socketfd = sockfd;
  bzero((char *) &sock.serv_addr, sizeof(sock.serv_addr));
  sock.serv_addr.sin_family = AF_INET;
  sock.serv_addr.sin_addr.s_addr = inet_addr(host);
  sock.serv_addr.sin_port = htons(port);
}


// メッセージを送信する
bool NetworkInterface::send_message(char* msg) {
  // ソケットのチェック
  if (sock.socketfd == -1) {
    fprintf(stderr, "Cant send message.\n");
    return false;  
  }

  int n = strlen(msg);
  if (sendto(sock.socketfd, msg, n, 0,
	     (struct sockaddr *)&sock.serv_addr,
	     sizeof(sock.serv_addr)) != n) {
    fprintf(stderr, "Cant send message.\n");
    return false;
  } else
    return true;
}


// メッセージを受信する
bool NetworkInterface::receive_message(char* msg, int size) {
  // ソケットのチェック
  if (sock.socketfd == -1) {
    fprintf(stderr, "Cant receive message.\n");
    return false;  
  }

  struct sockaddr_in serv_addr;   
  int servlen = sizeof(serv_addr);
  int n = recvfrom(sock.socketfd, msg, size, 0,
	           (struct sockaddr *)&serv_addr, &servlen);
  if (n < 0) {
    if (errno != ENOTSOCK && errno != EWOULDBLOCK && errno != 0) {
      fprintf(stderr, "Warning: recvfrom Error.[%d]\n", errno);
    }
    return false;
  } else {
    sock.serv_addr.sin_port = serv_addr.sin_port;
    msg[n] = '\0';
    if(n == 0) {
      fprintf(stderr, "Receive no message.\n");
      return false;
    } else
      return true;
  }
}  



