// INICIO PORTOS
// Material obtido da disciplina de TRSA
// J. Rosas 


#include <winsock2.h>
#include <iostream> 
#include <fstream>
#include <conio.h>
#include <stdio.h>

#include <interface_simulator.h>


//#define _USE_REAL_KIT_    
//#define USE_KIT_2D


char THE_KIT[512] = "carwash";

char  SERV_HOST_ADDR[1024] = "127.0.0.1";	   // endereco do servidor 

int   SERV_TCP_PORT = 9999;			// porta do servidor


/*
#ifdef USE_KIT_2D
  #undef SERV_TCP_PORT
  #define SERV_TCP_PORT 9999
#endif
*/


int socket_connected = 0;
SOCKET m_socket = NULL;


//must be called first time
void simulator_address(const char* the_kit, const char* http_adress, int port) {
	strcpy_s(THE_KIT, the_kit);
	strcpy_s(SERV_HOST_ADDR, http_adress);
	SERV_TCP_PORT = port;
}


void terminar_ligacao(void)
{

	socket_connected = 0;
	//#ifdef USE_KIT_2D
	if (m_socket) {
		closesocket(m_socket);	// desligar coneccao ao simulador
		WSACleanup();				// fazer house-keeping	
	}
	//#endif
}

void sighandle(int signum)
{
	if (signum) {};  //avoid warning
	// funcoes sig_kill (very nice)
	terminar_ligacao();
	printf("Exiting due to signal(%d)\n", signum);
	exit(0);
}



void estabelecer_ligacao(void)
{
	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR)
	{
		printf("\nError at WSAStartup()\n");
	}



	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET)
	{
		printf("Error at socket(): %d", WSAGetLastError());
		WSACleanup();
		// system ("PAUSE");
		return;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	clientService.sin_port = htons(SERV_TCP_PORT);

	if (connect(m_socket, (SOCKADDR*)& clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		printf("Failed to connect.");
		WSACleanup();
		//system("PAUSE");
		return;
	}
	socket_connected = 1;

}




void sendMessage(char* message)
{

#ifdef USE_KIT_2D  
	if (socket_connected == 0)
#endif
		estabelecer_ligacao();


	send(m_socket, message, strlen(message), 0);
	//printf("\nBytes Sent: %d\n",bytesSent);

//#ifndef USE_KIT_2D  
//	terminar_ligacao();
//#endif

}


void receiveMessage(char* message)
{
	int bytesRecv = recv(m_socket, message, 10000, 0);

	if (bytesRecv == -1)
	{
		printf("Connection error: %d.\n", WSAGetLastError());
		bytesRecv = 0;
	}

	message[bytesRecv] = '\0';
	//printf("Bytes Recv: %d\n" ,bytesRecv);
	//printf("%s\n",recvbuf);

#ifndef USE_KIT_2D  
	terminar_ligacao();
#endif


}





/*****************SIMULATOR ACCESS CODE *******************/
bool sim_create_DI_channel(uInt32 port)
{
	char buff[10000];
	char buff2[10000];
#ifdef USE_KIT_2D  
	sprintf(buff, "%s %d %d", "create_DI_channel\n", port, 0);
#else
	sprintf(buff, "GET http://%s:%d/%s/create_DI_channel/%d\r\n", SERV_HOST_ADDR, SERV_TCP_PORT, THE_KIT, port);
#endif
	sendMessage(buff);
	receiveMessage(buff2); //receive 'done'
	return(true);
}


bool sim_create_DO_channel(uInt32 port)
{
	char buff[10000];
#ifdef USE_KIT_2D  
	sprintf(buff, "%s %d %d", "create_DO_channel\n", port, 0);
#else
	sprintf(buff, "GET http://%s:%d/%s/create_DO_channel/%d\n", SERV_HOST_ADDR, SERV_TCP_PORT, THE_KIT, port);
#endif
	sendMessage(buff);
	receiveMessage(buff); //receive 'done'
	return(true);
}



uInt8 sim_ReadDigitalU8(uInt32 port)
{
	char buff[10000];
	char lixo[100];

	uInt32 value;
#ifdef USE_KIT_2D  
	sprintf(buff, "%s %02X %02X", "ReadDigitalU8\n", port, 0);
#else
	sprintf(buff, "GET http://%s:%d/%s/ReadDigitalU8/%d\n", SERV_HOST_ADDR, SERV_TCP_PORT, THE_KIT, port);
#endif	
	sendMessage(buff);
	receiveMessage(buff);

#ifdef USE_KIT_2D  
	sscanf(buff, "%s %02X %02X", lixo, &port, &value);
#else
	int idx = 0;
	while (buff[idx] != '\n') idx++;
	idx++;
	sscanf(buff + idx, "%d", &value);
#endif
	return((uInt8)value);
}


//cache port values written, so not to exhaust webconnection
bool hasfirstWrite[16] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };
unsigned char portValues[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void sim_WriteDigitalU8(uInt32 port, uInt8 data)
{
	char buff[10000];

	if (hasfirstWrite[port] == true) {
		if (portValues[port] == data)
			return;  // new value already there
	}

	portValues[port] = data;
	hasfirstWrite[port] = true;


#ifdef USE_KIT_2D  
	sprintf(buff, "%s %02X %02X", "WriteDigitalU8\n", port, data);
#else
	sprintf(buff, "GET http://%s:%d/%s/WriteDigitalU8/%d/%d\n", SERV_HOST_ADDR, SERV_TCP_PORT, THE_KIT, port, data);
#endif
	sendMessage(buff);

	receiveMessage(buff); //receive 'done'	
}


bool sim_close_channels()
{
	terminar_ligacao();
	return(true);
}





/*****************REAL KIT CODE *******************/


TaskHandle tasks[50];


bool close_channels(void)
{
#ifdef _USE_REAL_KIT_
	return(true);
#else		
	return sim_close_channels();
#endif 

}



bool createDigitalInput(uInt32 port)
{
#ifdef _USE_REAL_KIT_
	char taskName[100];
	char portName[100];
	//printf("\n1");
	TaskHandle task_handle;
	sprintf(taskName, "task_di__%d", port);
	//printf("\n2");
	sprintf(portName, "Dev3/port%d", port);
	//printf("\n3");
	DAQmxCreateTask(taskName, &task_handle);
	//printf("\n4");
	DAQmxCreateDIChan(task_handle, portName, "", DAQmx_Val_ChanForAllLines);
	//printf("\n5");
	DAQmxStartTask(task_handle);
	//printf("\n6");
	tasks[port] = task_handle;
	//printf("\n7 %x", task_handle);
	return(true);
#else		
	return sim_create_DI_channel(port); 
#endif 
}



bool createDigitalOutput(uInt32 port)
{
#ifdef _USE_REAL_KIT_
	char taskName[100];
	char portName[100];
	TaskHandle task_handle;
	sprintf(taskName, "task_di__%d", port);
	sprintf(portName, "Dev3/port%d", port);
	DAQmxCreateTask(taskName, &task_handle);
	DAQmxCreateDOChan(task_handle, portName, "", DAQmx_Val_ChanForAllLines);
	DAQmxStartTask(task_handle);
	tasks[port] = task_handle;
#else
	return sim_create_DO_channel(port);
#endif
	return(true);
}



uInt32 portos[20];


uInt8 readDigitalU8(uInt32 port)
{
	uInt8 data = 0;
#ifdef _USE_REAL_KIT_
	int32		error_0 = 0;
	char		errBuff_0[2048] = { '\0' };
	int32		read_0;
	TaskHandle task_handle = tasks[port];
	DAQmxReadDigitalU8(task_handle, 1, 10.0, DAQmx_Val_GroupByChannel, &data, 1, &read_0, NULL);
#else 
	data = sim_ReadDigitalU8(port);
#endif

	/*
		bool flag_error=false;
		if( (port==0) && !(portos[port] & 1<<2) && (data & 1<<2) && (portos[2]&1<<6))
			{printf("\nATTENTION: XX beyond left limit...\n");flag_error=true;}
		if( (port==0) && !(portos[port] & 1<<0) && (data & 1<<0) && (portos[2]&1<<7))
			{printf("\nATTENTION: XX beyond right limit...\n");flag_error=true;}
		if( (port==1) && !(portos[port] & 1<<3) && (data & 1<<3) && (portos[2]&1<<2))
			{printf("\nATTENTION: ZZ below lower limit...\n");flag_error=true;}
		if( (port==0) && !(portos[port] & 1<<6) && (data & 1<<6) && (portos[2]&1<<3))
			{printf("\nATTENTION: ZZ above upper limit...\n");flag_error=true;}
		if( (port==0) && !(portos[port] & 1<<5) && (data & 1<<5) && (portos[2]&1<<4))
			{printf("\nATTENTION: YY beyond OUTSIDE sensor...\n");	flag_error=true;}
		if( (port==0) && !(portos[port] & 1<<3) && (data & 1<<3) && (portos[2]&1<<5))
			{printf("\nATTENTION: YY beyond INSIDE sensor...\n");flag_error=true;}

		portos[port]=data;
		if(flag_error) {
			WriteDigitalU8(2, 0);
			exit(0);
		}
		*/
	return(data);
}


void writeDigitalU8(uInt32 port, uInt8 data)
{


	portos[port] = data;


#ifdef _USE_REAL_KIT_ 
	int         error_1 = 0;
	char        errBuff_1[2048] = { '\0' };
	int32		written_1;
	TaskHandle task_handle = tasks[port];
	DAQmxWriteDigitalU8(task_handle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, &data, &written_1, NULL);
#else 
	sim_WriteDigitalU8(port, data);
#endif

}


