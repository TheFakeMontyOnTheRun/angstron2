#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <vector>
#include <cassert> //thanks _Sticky_ from #armagetron !
#include <queue>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#ifdef NETWORKSUPPORT
       #include <sys/types.h>
       #include <ifaddrs.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
       #include <sys/types.h>
       #include <ifaddrs.h>

 extern int NET_InitOK = 0;

//most of those functions were taken from Beej's guide to networking. Im not particulary proud of this and I plan to 
//scrap those and replace it with GGZ 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void GlobalBroadcast()
{
   int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int broadcast = 1;
    //char broadcast = '1'; // if that doesn't work, try this

    he=gethostbyname("255.255.255.255");

    if (he == NULL) {  // get the host info
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // this call is what allows broadcast packets to be sent:
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }

    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(24245); // short, network byte order
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    std::memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    if ((numbytes=sendto(sockfd, "BZK2GAME", strlen("BZK2GAME"), 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        perror("sendto");
        exit(1);
    }

    printf("sent %d bytes to %s\n", numbytes,
        inet_ntoa(their_addr.sin_addr));

    close(sockfd);
}

extern void NET_WaitForPlayer();
extern void NET_SignalPlayer();

std::string WaitForBroadcast()
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[9];
    size_t addr_len;
    char s[INET6_ADDRSTRLEN];
	std::string _buffer;

    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, "24245", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, 8 , 0,
        (struct sockaddr *)&their_addr,(socklen_t*) &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

	std::string addr;
	addr=inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
    printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
    _buffer=buf;
	if (_buffer!="BZK2GAME")
	{
	std::cout << "received wrong broadcast:"<<_buffer<<std::endl;
	exit(0);
	}

    close(sockfd);
    //std::cout << addr<<std::endl;
    return addr;
}




//retorna o listening socket
int init_server(char*port,int howmany=1)
{
	int sockfd;
	addrinfo *servinfo;
	addrinfo *p;
	addrinfo hints;
	struct sigaction sa;
	int yes=1;
	
	int rv;

	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}
	
	
	freeaddrinfo(servinfo); // all done with this structure
	
	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}
	
		

	

	if (listen(sockfd, howmany) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	
	return sockfd;

}



//retorna o socket pro cliente
int waitforclient(int sockfd,std::string &otherip)
{
	struct sockaddr_storage their_addr; // connector's address information
	int new_fd;  // listen on sock_fd, new connection on new_fd
	socklen_t sin_size;
		
	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

char s[INET6_ADDRSTRLEN];
inet_ntop(their_addr.ss_family,	get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);

	//otherip=GetIncomingAddress(new_fd);
	otherip=s;
	std::cout << "o outro ip eh "<<otherip<<std::endl;
	
//		printf("recebi de %s\n",GetIncomingAddress(new_fd));
		return new_fd;
	}
}



int init_client(char *hostname,char * port)
{

	int sockfd;
	
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];



	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		exit(2);
	}
	
	
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);
	freeaddrinfo(servinfo); // all done with this structure
	return sockfd;
}
#else
int init_client(char *hostname,char * port)
{}

int waitforclient(int sockfd,std::string &otherip)
{}

int init_server(char*port,int howmany=1)
{}

std::string WaitForBroadcast()
{}

void *get_in_addr(struct sockaddr *sa)
{
}

void sigchld_handler(int s)
{}

void GlobalBroadcast()
{}

extern  int NET_InitOK = 0;
extern void NET_WaitForPlayer();
extern void NET_SignalPlayer();
#endif
