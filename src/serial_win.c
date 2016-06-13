
#include <stdio.h>
#include <windows.h>



HANDLE hCommSerial;





int serial_begin(char *sComPort, unsigned int baudrate) {
	
	char *strFormat, strPort[30];
	DCB dcbSerial;
	COMMTIMEOUTS tmSerial;
	unsigned int res;
	HANDLE hComm;
	
	strFormat = "\\\\.\\%s";
	sprintf(strPort, strFormat, sComPort);
	hComm = INVALID_HANDLE_VALUE;
	hComm = CreateFile(strPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if (hComm == INVALID_HANDLE_VALUE) {
		return 1;
	}
	
//	PurgeComm(hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	dcbSerial.DCBlength = sizeof(DCB);
	res = GetCommState(hComm, &dcbSerial);
	if (res == 0) {
		CloseHandle(hComm);
		return 1;
	}
	
	dcbSerial.BaudRate = baudrate;
	dcbSerial.ByteSize = 8;
	dcbSerial.StopBits = ONESTOPBIT;
	dcbSerial.Parity = NOPARITY;
	res = SetCommState(hComm, &dcbSerial);
	if (res == 0) {
		CloseHandle(hComm);
		return 2;
	}
	
	tmSerial.ReadIntervalTimeout = 0;
	tmSerial.ReadTotalTimeoutConstant = 0;
	tmSerial.ReadTotalTimeoutMultiplier = 0;
	tmSerial.WriteTotalTimeoutConstant = 0;
	tmSerial.WriteTotalTimeoutMultiplier = 0;
	res = SetCommTimeouts(hComm, &tmSerial);
	if (res == 0) {
		CloseHandle(hComm);
		return 3;
	}
	
	hCommSerial = hComm;
	return 0;
}




unsigned int serial_read(unsigned char *buf, unsigned int len) {
	
	unsigned int singleRead;
	unsigned int res;
	unsigned int index;
	
	index = singleRead = 0;
	while (index < len) {
		res = ReadFile(hCommSerial, buf + index, 1, (DWORD *) &singleRead, 0);
		if (singleRead == 0 || res == 0) {
			break;
		}
		index = index + 1;
	}
	return (index);
}




unsigned int serial_write(unsigned char *buf, unsigned int len) {
	
	unsigned int singleWrite;
	unsigned int res;
	unsigned int index;
	
	index = singleWrite = 0;
	while (index < len) {
		res = WriteFile(hCommSerial, buf + index, 1, (DWORD *) &singleWrite, 0);
		if (singleWrite == 0 || res == 0) {
			break;
		}
		index = index + 1;
	}
	FlushFileBuffers(hCommSerial);
	return (index);
}




void serial_end(void) {
	
	CloseHandle(hCommSerial);
	hCommSerial = INVALID_HANDLE_VALUE;
	return;
}



