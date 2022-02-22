
///////////////////////////////////////////////////////////////////////

#include "common.hxx"

#if OS_TYPE == WINDOWS
#  include <process.h>
#endif
#include "Mutex.hxx"
#include "basic.hxx"
#include "PlayerClient.hxx"
#include "Server.hxx"
#include "Window.hxx"
#include "WindowsWindow.hxx"

///////////////////////////////////////////////////////////////////////


const int MAX_TEAM_NAME = 32;
typedef std::map<std::string, std::string> Map;

struct Parameter	// チームごとに共有すること。
{
	const char* host;
	int port;
	char teamName[MAX_TEAM_NAME + 1];
	int side;	// サイドを区別する値で 0 か 1
	int uniformNumber;	// -1 の場合 connect、そうでないばあい reconnect
	int retry;
	Window* window;
	Map parameters;
	Parameter() {
		host = "localhost";
		port = 6000;
		strcpy(teamName, getDefaultTeamName());
		side = 0;
		uniformNumber = -1;
		retry = 3;
		window = 0;
	}
};
Parameter g_parameter;
Parameter g_parameter2;
Mutex g_glovalMutex;
volatile int g_numberOfClients = 0;
void runClient(void* arg)
{
	g_glovalMutex.lock();
	Parameter* p = (Parameter*)arg;
	PlayerClient* client = newPlayerClient(p->parameters);
	if(p->window)
		p->window->add(client, p->side);
	g_glovalMutex.unlock();
	bool connected = false;
	for(int i=0; !connected && i<p->retry; i++) {
		if(p->uniformNumber == -1)
			connected = client->connect(p->host, p->port, p->teamName);
		else
			connected = client->reconnect(p->host, p->port, p->teamName, p->uniformNumber);
		
	}
	if(connected)
		client->go();
	g_glovalMutex.lock();
	if(p->window)
		p->window->remove(client);
	delete client;
	g_numberOfClients--;
	printf("finished.\n");
	g_glovalMutex.unlock();
}

int main(int argc, char** argv)
{
#if OS_TYPE == WINDOWS
	static WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	WSAStartup(wVersionRequested, &wsaData);
#endif
	
	Server::theServer.readConfigFile("server.conf");
	flagHelper.initialize();
	
	int c = 0;
#if MULTI_THREAD
	int number = MAX_PLAYER;
	bool twice = false;
#endif
	for(int i = 1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-file") == 0) {
				if(i+1 >= argc) {
					fprintf(stderr, "error: option '%s'\n", argv[i]);
				} else {
					Server::theServer.readConfigFile(argv[++i]);
					flagHelper.initialize();
				}
			}
			else if(strcmp(argv[i], "-r") == 0) {
				if(i+1 >= argc) {
					fprintf(stderr, "error: option '%s'\n", argv[i]);
				} else {
					int n = atoi(argv[++i]);
					if(n < 1 || MAX_PLAYER < n)
						fprintf(stderr, "error: option %s %d\n", argv[i-1], n);
					g_parameter.uniformNumber = n-1;
				}
			}
			else if(strcmp(argv[i], "-p") == 0) {
				if(i+2 >= argc) {
					fprintf(stderr, "error: option '%s'\n", argv[i]);
				} else {
					
					g_parameter.parameters[argv[++i]] = argv[++i];
				}
			}
#if MULTI_THREAD
			else if(strcmp(argv[i], "-2") == 0) {
				twice = true;
			}
			else if(strcmp(argv[i], "-n") == 0) {
				if(i+1 >= argc) {
					fprintf(stderr, "error: option '%s'\n", argv[i]);
				} else {
					int n = atoi(argv[++i]);
					if(n <= 0 || MAX_PLAYER < n)
						fprintf(stderr, "error: option '%s %d'\n", argv[i-1], n);
					number = n;
				}
			}
#else
			else if(strcmp(argv[i], "-2") == 0 || strcmp(argv[i], "-n") == 0) {
				fprintf(stderr, "error: option '%s' is only with multi thread version.\n", argv[i]);
			}
#endif
			else {
				fprintf(stderr, "error: option '%s'\n", argv[i]);
				fprintf(stderr, "usage: [<option>]... [<host> [<port> [<team name>]]]\n");
				fprintf(stderr, "option:\n");
				fprintf(stderr, "-file <server.conf>    read server configuration file.\n");
				fprintf(stderr, "-r <uniform number>    reconnect as <uniform number>th client.\n");
				fprintf(stderr, "-n <number>            start up <number> client(s).\n");
				fprintf(stderr, "-2                     start up two teams.\n");
				fprintf(stderr, "-p <name> <value>      set parameter <name> to <value>.\n");
			}
		} else {
			switch(c++) {
			case 0:
				g_parameter.host = argv[i];
				break;
			case 1:
				g_parameter.port = atoi(argv[i]);
				break;
			case 2:
				strcpy(g_parameter.teamName, argv[i]);
				break;
			default:
				fprintf(stderr, "error: argument '%s'\n", argv[i]);
				break;
			}
		}
	}
	printf("%s/%s/%d\n", (const char*)g_parameter.teamName, (const char*)g_parameter.host, (int)g_parameter.port);
#if MULTI_THREAD
	if(twice)
		printf("%d member(s) x 2 teams\n", number);
	else
		printf("%d member(s)\n", number);
#endif
	
#if !MULTI_THREAD
	g_numberOfClients = 1;
	runClient(&g_parameter);
#else
	g_numberOfClients = twice ? number*2 : number;
#if WITH_WINDOW
	WindowImplemented window(g_parameter.teamName);
	g_parameter.window = &window;
	if(!twice) {
		for(int j=0; j<number; j++) {
			_beginthread(&runClient, 0, &g_parameter);
		}
		window.go();
	} else {
		g_parameter2 = g_parameter;
		strcat(g_parameter.teamName, "-A");
		strcat(g_parameter2.teamName, "-B");
		g_parameter.parameters["key"] = "1234";
		g_parameter2.parameters["key"] = "9876";
		g_parameter2.side = 1;
		int j;
		for(j=0; j<number; j++) {
			_beginthread(&runClient, 0, &g_parameter);
		}
		for(j=0; j<number; j++) {
			_beginthread(&runClient, 0, &g_parameter2);
		}
		window.go();
	}
	while(g_numberOfClients > 0) {
		::Sleep(200);
	}
#else
	if(!twice) {
		for(int j=1; j<number; j++) {
			_beginthread(&runClient, 0, &g_parameter);
		}
		if(number > 0)
			runClient(&g_parameter);
	} else {
		g_parameter2 = g_parameter;
		strcat(g_parameter.teamName, "-A");
		strcat(g_parameter2.teamName, "-B");
		g_parameter.parameters["key"] = "0";
		g_parameter2.parameters["key"] = "1";
		g_parameter2.side = 1;
		int j;
		for(j=0; j<number; j++) {
			_beginthread(&runClient, 0, &g_parameter);
		}
		for(j=1; j<number; j++) {
			_beginthread(&runClient, 0, &g_parameter2);
		}
		if(number > 0)
			runClient(&g_parameter2);
	}
	while(g_numberOfClients > 0) {
		::Sleep(200);
	}
#endif
#endif
	return 0;
}

#ifdef _MSC_VER	// {
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int /*nCmdShow*/)
{
	int argc = 0;
	char* cmdline = new char[strlen(lpCmdLine)+1];
	strcpy(cmdline, lpCmdLine);
	char** argv = new char*[strlen(lpCmdLine)+2];

	char* p = cmdline;
	argv[argc++] = "soccerclient";

	for(;;) {
		if(*p == '\0')
			break;
		argv[argc++] = p;
		while(*p != '\0' && !isspace(*p))
			p++;
		if(*p == '\0')
			break;
		*p++ = '\0';
		while(*p && isspace(*p))
			p++;
	}
	argv[argc] = 0;

	int result = main(argc, argv);

	delete[] cmdline;
	delete[] argv;
	return result;

}
#endif	// }
