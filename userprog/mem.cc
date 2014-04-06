#include "mem.h"
/*
/ Para acceder a la memoria del usuario utilizar:
  Machine::ReadMem(int addr, int size, int *value)
  addr--> direccion de memoria virtual para leer.
  size--> numeros de bytes a leer 1, 2 o 4
  value--> donde escribe lo leido

  Machine::WriteMem(int addr, int size, int value).
  addr--> direccion virtual donde comienza a escribir
  size--> numeros de bytes a escribir (1, 2, or 4)
  value--> dato a ser escrito
*/


void readStrFromUsr(int usrAddr, char *outStr)
{
	int i = usrAddr;
	int aux=0;
	int j;
	machine->ReadMem(usrAddr,1,&j);
	while ((char)j != '\0' )
	{	
		outStr[aux]=(char)j;		
		aux++;
		i++;		
		machine->ReadMem(i,1,&j);
	}
}/*
void readBuffFromUsr(int usrAddr, char *outBuff, int byteCount)
{
	int i = usrAddr;
	int aux=0;
	int nbyte;
	int j;
	machine->ReadMem(usrAddr,,&j);
	while(nbyte!=byteCount)
	{
		outBuff[aux]=(char)j;
		aux++;
		nbyte++;
		machine->ReadMem(i++,1,&j);
		
	}

}


void writeStrToUsr(char *str, int usrAddr)
{
	int i=usrAddr;
	int aux=0;
	int j;
	while(str[aux]!='\0')
	{
		j = (int)str[aux];	
		machine->WriteMem(i,1,j);
		i++;
		aux++;
	}
}

void writeBuffToUsr(char *str, int usrAddr, int byteCount);*/
