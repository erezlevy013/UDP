#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ADDR "127.0.0.1"
#define PORT "1234"

typedef struct sockaddr_in SocAdd;
typedef struct sockaddr sockaddr;
int CreateClient()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) 
	{
		perror("socket failed");
		return -1;
	}
	return sock;
}
int SentTo(int _sock, SocAdd _sin)
{
	int sent_bytes;
	memset(&_sin, 0, sizeof(_sin));
	_sin.sin_family = AF_INET;
	_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	_sin.sin_port = htons(1234);
	sent_bytes = sendto(_sock, "Hi from client", 15 , 0,(sockaddr *) &_sin, sizeof(_sin));
	if (sent_bytes < 0) 
	{
		perror("sendto failed");
		return -3;
	}
	printf("Message sent.\n");
	return 0;
}
int Recvfrom(int _sock, SocAdd _sin)
{
	unsigned int clientAdd_len = sizeof(_sin);
	char buffer[4096];
	int read_bytes;
	read_bytes = recvfrom( _sock, buffer, sizeof(buffer), 0,(sockaddr *) &_sin, &clientAdd_len );
	if( read_bytes < 0 ) 
	{
		perror("recvfrom failed");	
		return -1;
	}
	buffer[read_bytes] = '\0';
	printf("Client: %s \n", buffer);
	return 0;
}
int ClientStart(int _sock)
{
	SocAdd clientAdd;
	if(SentTo(_sock, clientAdd) < 0 )
	{
		return -1;
	}
	if( Recvfrom(_sock, clientAdd) < 0 )
	{
		return -1;
	}
	
	return 0;
}

int main()
{
	int sock;
	sock = CreateClient();
	ClientStart( sock );
	close(sock);
	return 0;
}
