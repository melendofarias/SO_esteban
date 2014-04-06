#include "system.h"
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


void readStrFromUsr(int usrAddr, char *outStr);
void readBuffFromUsr(int usrAddr, char *outBuff, int byteCount);
void writeStrToUsr(char *str, int usrAddr);
void writeBuffToUsr(char *str, int usrAddr, int byteCount);
