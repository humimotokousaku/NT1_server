#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")


/// <summary>
/// サーバー側のプログラム
/// </summary>
/// <returns></returns>
int main() {
	WSADATA wsaData;
	// 初期化エラー
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WinSockの初期化に失敗しました\n");
		return 1;
	}

	printf("サーバーを起動します\n\n");

	u_short uport;
	printf("ポート番号を入力してください:");
	scanf_s("%hd", &uport);

	// ソケット作成
	printf("ソケット作成:");
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("失敗\n");
		return 1;
	}
	printf("成功\n");

	// ソケットに名前を付ける
	printf("ソケットの名前付け:");
	SOCKADDR_IN saddr;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("失敗\n");
		return 1;
	}
	printf("成功\n");

	// クライアントからの接続待ち
	if (listen(sock, 0) == SOCKET_ERROR) {
		printf("クライアントからの接続に失敗しました\n");
		return 1;
	}

	printf("クライアント側の接続要求を待機...\n\n");

	// 接続要求が来るまでプログラムを停止
	int size = (int)sizeof(saddr);
	SOCKET acceptSocket = accept(sock, (struct sockaddr*)&saddr, &size);

	// ソケットの取得ができなかった
	if (acceptSocket == -1) {
		printf("待機状態のソケットの識別子を受け取れませんでした\n");
		return 1;
	}
	// 待機状態のソケットを解放
	closesocket(sock);

	printf("チャット開始\n");
	printf("クライアント側のメッセージを待っています\n");

	while (1) {
		int nRcv;
		char szBuf[1024];
		nRcv = recv(acceptSocket, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("受信 --> %s\n", szBuf);

		printf("送信 --> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(acceptSocket, szBuf, (int)strlen(szBuf), 0);
	}
	closesocket(acceptSocket);
	WSACleanup();

	return 0;
}