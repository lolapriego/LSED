//------------------------------------------------------------------------------
// m5272gpio.c
//
//   Funciones del m�dulo de entrada salida de prop�sito general de la
//   plataforma de desarrollo ENT2004CF
//
// Actualizada por: Javier Guill�n �lvarez
//------------------------------------------------------------------------------
#ifndef __M5272GPIO_C__
#define __M5272GPIO_C__

#include "m5272.h"
#include "m5272lib.c"

typedef struct
{
	UWORD puertoS; // Contendr� una copia del valor del puerto S para posibilitar su lectura
} TPuertoS;

//-------------------------------------------------------------------
// void set16_puertoS (UWORD valor)
//
// Rutina para sacar datos por el puerto de salida
//-------------------------------------------------------------------

void set16_puertoS (UWORD valor)
{
  UWORD* pchar;
  TPuertoS port;	
  port.puertoS = valor;
  pchar=(UWORD*)BASE_PUERTO_S;
  (*pchar)=port.puertoS;
}

//------------------------------------------
// UWORD lee16_puertoE (void)
//
// Descripci�n:
//   Lee los 16 bits del puerto de entrada
//------------------------------------------
UWORD lee16_puertoE (void)
{
  return *((UWORD*) BASE_PUERTO_E);
}

#endif