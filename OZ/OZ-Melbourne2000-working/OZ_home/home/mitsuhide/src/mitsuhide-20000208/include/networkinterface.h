#include <sys/types.h>
#include <netinet/in.h>

typedef struct {
  int socketfd;                 // $B%=%1%C%H%U%!%$%k%G%#%9%/%j%W%?(B
  struct sockaddr_in serv_addr; // $B%5!<%P%"%I%l%9(B
} Socket;

class NetworkInterface {
private:
  Socket sock; // $B%=%1%C%H(B

public:
  NetworkInterface(void);
  bool connect(char* host, int port); 
  bool send_message(char* msg);
  bool receive_message(char* msg, int size);     
};
