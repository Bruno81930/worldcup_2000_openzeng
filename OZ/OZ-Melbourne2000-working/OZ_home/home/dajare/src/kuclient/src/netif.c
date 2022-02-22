/*
 *	Header:
 *	File: netif.c
 *	Author: Noda Itsuki
 *	Date: 1997/04/1
 *	Changed by Yusuke Yamauchi (1999/06/12)
 *	EndHeader:
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#ifdef Solaris
#include <sys/filio.h>
#else
#include <sys/ioctl.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>

#include "libsclient.h"

Socket init_connection(char *host, int port)
{
    struct hostent	*host_ent ;
    struct in_addr	*addr_ptr ;
    struct sockaddr_in	cli_addr ;
    int	sockfd, one ;
    Socket	sock ;

    sock.socketfd = -1 ;

    if ((host_ent = (struct hostent *)gethostbyname(host)) == NULL) {
		/* Check if a numeric address */
		if(inet_addr(host) == 0)
		    return sock ;
	}
	else {
		addr_ptr = (struct in_addr *) *host_ent->h_addr_list ;
		host = inet_ntoa(*addr_ptr) ;
	}

    /* Open UDP socket. */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "Can't open socket.\n") ;
		return sock ;
    }

	one = 1 ;
	if (ioctl(sockfd, FIONBIO, &one) < 0) {
		fprintf(stderr, "Can't ioctl on socket.\n") ;
		return sock ;
	}

    /* Bind any local address. */
    bzero((char *) &cli_addr, sizeof(cli_addr)) ;
    cli_addr.sin_family = AF_INET ;
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    cli_addr.sin_port = htons(0) ;

    if (bind(sockfd, (struct sockaddr *)&cli_addr,
							    sizeof(cli_addr)) < 0) {
		fprintf(stderr, "Can't bind local address.\n") ;
		return sock ;
	}
    
    /* Fill in the structure with the address of the server. */
    sock.socketfd = sockfd ;
    bzero((char *) &sock.serv_addr, sizeof(sock.serv_addr)) ;
    sock.serv_addr.sin_family = AF_INET ;
    sock.serv_addr.sin_addr.s_addr = inet_addr(host) ;
    sock.serv_addr.sin_port = htons(port) ;

    return sock ;
}


Bool send_message(Socket sock, char *buf)
{
    int n ;

    n = strlen(buf) ;
    if (sendto(sock.socketfd, buf, n, 0,
		        (struct sockaddr *)&sock.serv_addr,
				sizeof(sock.serv_addr)) != n)
		return FALSE ;
    else
		return TRUE ;
}
	   

Bool receive_message(Socket *sock, char *buf, int size)
{
    int	n, servlen ;
    struct sockaddr_in serv_addr ;
    
    servlen = sizeof(serv_addr) ;
    n = recvfrom(sock->socketfd, buf, size, 0,
			 (struct sockaddr *)&serv_addr, &servlen) ;

	if (n < 0) {
		if (errno != ENOTSOCK && errno != EWOULDBLOCK) {
			fprintf(stderr, "Error: recvfrom.\n") ;
			exit(1) ;
		}
		return FALSE ;
	}
    else {
		sock->serv_addr.sin_port = serv_addr.sin_port ;
		buf[n] = '\0' ;

		if(n == 0)
		    return FALSE ;
		else
		    return TRUE ;
    }
}


void close_connection(Socket sock)
{
    close(sock.socketfd) ;
}

