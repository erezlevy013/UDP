#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ADDR "127.0.0.1"
#define PORT 1234


typedef struct sockaddr_in SocAdd;
typedef struct sockaddr sockaddr;

int CreateServer()
{
	SocAdd serverAdd;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if( sock < 0 ) 
	{
		perror("socket failed");
		return -1;
	}
	memset(&serverAdd, 0, sizeof(serverAdd));
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_addr.s_addr = INADDR_ANY;
	serverAdd.sin_port = htons(PORT);

	if( bind(sock, (sockaddr *) &serverAdd, sizeof(serverAdd)) < 0 )
	{
		perror("bind failed");
		return -2;
	}
	return sock;
}

int Recvfrom(int _sock, SocAdd _clientAdd)
{
	unsigned int clientAdd_len = sizeof(_clientAdd);
	char buffer[4096];
	int read_bytes;
	read_bytes = recvfrom( _sock, buffer, sizeof(buffer), 0,(sockaddr *) &_clientAdd, &clientAdd_len );
	if( read_bytes < 0 ) 
	{
		perror("recvfrom failed");	
		return -1;
	}
	buffer[read_bytes] = '\0';
	printf("Client: %s \n", buffer);
	return 0;
}

int SentTo(int _sock, SocAdd _clientAdd)
{
	int sent_bytes;
	char data[] = "Wellcom";
	sent_bytes = sendto(_sock, data, sizeof(data) , 0,(sockaddr *) &_clientAdd, sizeof(_clientAdd));
	if (sent_bytes < 0) 
	{
		perror("sendto failed");
		return -3;
	}
	printf("Message sent.\n");
	return 0;
}

int ServerStart(int _sock)
{
	SocAdd clientAdd;
	if( Recvfrom(_sock, clientAdd) < 0 )
	{
		return -1;
	}
	if(SentTo(_sock, clientAdd) < 0 )
	{
		return -1;
	}
	return 0;
}

int main()
{
	int sock;
	sock = CreateServer();
	ServerStart( sock );
	close(sock);
	return 0;
}
