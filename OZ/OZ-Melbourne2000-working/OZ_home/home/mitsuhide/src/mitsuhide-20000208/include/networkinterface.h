#include <sys/types.h>
#include <netinet/in.h>

typedef struct {
  int socketfd;                 // ソケットファイルディスクリプタ
  struct sockaddr_in serv_addr; // サーバアドレス
} Socket;

class NetworkInterface {
private:
  Socket sock; // ソケット

public:
  NetworkInterface(void);
  bool connect(char* host, int port); 
  bool send_message(char* msg);
  bool receive_message(char* msg, int size);     
};
