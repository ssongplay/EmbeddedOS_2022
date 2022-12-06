#include "Common.h"


#define SERVERPORT 4039
#define BUFSIZE    512

typedef struct
{
  int msgNum;
  char buf[BUFSIZE -8];
  int len;
}MsgStruct;

//전역변수 영역
int clientCount = 0; // 접속한 클라이언트를 카운팅 변수
pthread_mutex_t mutex;  // 뮤텍스 전역변수 선언 (어디에서나 사용 가능)


void *ProcessClient(void *arg)
{

	MsgStruct data;
	int retval;
	SOCKET client_sock = (SOCKET)(long long)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	socklen_t addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr *)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1) {
		// 데이터 받기
		//retval = recv(client_sock, buf, BUFSIZE, 0);
    	retval = recv(client_sock, (MsgStruct *)&data, BUFSIZE, 0);
    
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

    	printf("%d %s %d\n", data.msgNum, data.buf, data.len);
		// 받은 데이터 출력
		// buf[retval] = '\0';
		//printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
    	strcpy(buf, "성공했습니다.\n");
    	// printf("[TCP/%s:%d,total client : %d] %s\n", addr, ntohs(clientaddr.sin_port),clientCount ,buf);
   

		// 데이터 보내기
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
	}

	// 소켓 닫기
	close(client_sock);
	pthread_mutex_lock(&mutex);
	clientCount--;
	pthread_mutex_unlock(&mutex);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n, 남은 클라이언트 = %d\n",
		addr, ntohs(clientaddr.sin_port), clientCount);   // 클라이언트 출력
	return 0;
}

int main(int argc, char *argv[])
{
  
  	//뮤텍스 초기화
  	pthread_mutex_init(&mutex, NULL);
  
	int retval;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
	pthread_t tid;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
   
    	// 클라이언트 카운트 증가
    	pthread_mutex_lock(&mutex);   // 뮤텍스 ON
    	clientCount++;                // 전역변수 변경 (클라이언트 카운트 증가)
    	pthread_mutex_unlock(&mutex); // 뮤텍스 OFF

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n, 접속중인 클라이언트 총 수 : %d\n",
			addr, ntohs(clientaddr.sin_port), clientCount);

		// 스레드 생성
		retval = pthread_create(&tid, NULL, ProcessClient,
			(void *)(long long)client_sock);
		if (retval != 0) { close(client_sock); }
	}

	// 소켓 닫기
	close(listen_sock);
 
  	// 뮤텍스 삭제
  	pthread_mutex_destroy(&mutex);
	return 0;
}