#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")


/// <summary>
/// �T�[�o�[���̃v���O����
/// </summary>
/// <returns></returns>
int main() {
	WSADATA wsaData;
	// �������G���[
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WinSock�̏������Ɏ��s���܂���\n");
		return 1;
	}

	printf("�T�[�o�[���N�����܂�\n\n");

	u_short uport;
	printf("�|�[�g�ԍ�����͂��Ă�������:");
	scanf_s("%hd", &uport);

	// �\�P�b�g�쐬
	printf("�\�P�b�g�쐬:");
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		printf("���s\n");
		return 1;
	}
	printf("����\n");

	// �\�P�b�g�ɖ��O��t����
	printf("�\�P�b�g�̖��O�t��:");
	SOCKADDR_IN saddr;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("���s\n");
		return 1;
	}
	printf("����\n");

	// �N���C�A���g����̐ڑ��҂�
	if (listen(sock, 0) == SOCKET_ERROR) {
		printf("�N���C�A���g����̐ڑ��Ɏ��s���܂���\n");
		return 1;
	}

	printf("�N���C�A���g���̐ڑ��v����ҋ@...\n\n");

	// �ڑ��v��������܂Ńv���O�������~
	int size = (int)sizeof(saddr);
	SOCKET acceptSocket = accept(sock, (struct sockaddr*)&saddr, &size);

	// �\�P�b�g�̎擾���ł��Ȃ�����
	if (acceptSocket == -1) {
		printf("�ҋ@��Ԃ̃\�P�b�g�̎��ʎq���󂯎��܂���ł���\n");
		return 1;
	}
	// �ҋ@��Ԃ̃\�P�b�g�����
	closesocket(sock);

	printf("�`���b�g�J�n\n");
	printf("�N���C�A���g���̃��b�Z�[�W��҂��Ă��܂�\n");

	while (1) {
		int nRcv;
		char szBuf[1024];
		nRcv = recv(acceptSocket, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("��M --> %s\n", szBuf);

		printf("���M --> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(acceptSocket, szBuf, (int)strlen(szBuf), 0);
	}
	closesocket(acceptSocket);
	WSACleanup();

	return 0;
}