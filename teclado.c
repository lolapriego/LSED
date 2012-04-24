//------------------------------------------------------------------------------
// Teclado_GNU.c
//
//  Fichero que proporciona la funci�n para leer una tecla de teclado
//
// Autores: Juan Manuel Montero y Rub�n San Segundo.
// Adaptado a C por: Javier Guill�n �lvarez
// Retocado: M� Dolores Priego Porcuna e Ines Fern�ndez Yuste
//------------------------------------------------------------------------------
#include "m5272lib.c"
#include "m5272gpio.c"

#define NUM_FILAS 4
#define NUM_COLS 4
#define EXCIT 1


UWORD puerto_S=0;


//------------------------------------------------------
// char teclado(void)
//
// Descripci�n:
//   Explora el teclado matricial y devuelve la tecla 
//   pulsada
//------------------------------------------------------
char teclado(void)
{
  //char tecla;
  //static UWORD puerto_S=0;
  BYTE fila, columna, fila_mask;
  static char teclas[4][4] = {{"123C"},
                              {"456D"},
                              {"789E"},
                              {"A0BF"}};
  // Bucle de exploraci�n del teclado
  while(TRUE){

    // Excitamos una columna
    for(columna = NUM_COLS - 1; columna >= 0; columna--){
      //set16_puertoS(EXCIT << columna);	
      puerto_S = puerto_S & 0xFFF0;
      puerto_S= (EXCIT << columna) | puerto_S;
      set16_puertoS(puerto_S);		// Se env�a la excitaci�n de columna
      retardo(1150);				// Esperamos respuesta de optoacopladores

      // Exploramos las filas en busca de respuesta
      for(fila = NUM_FILAS - 1; fila >= 0; fila--){
        fila_mask = EXCIT << fila;		// M�scara para leer el bit de la fila actual
        if(lee16_puertoE() & fila_mask){		// Si encuentra tecla pulsada,
          while(lee16_puertoE() & fila_mask);	//   Esperamos a que se suelte
          retardo(1150);			//   Retardo antirrebotes
          return teclas[fila][columna];	//   Devolvemos la tecla pulsada
        }
      }
      // Siguiente columna
    }
    // Exploraci�n finalizada sin encontrar una tecla pulsada
  }
  // Reiniciamos exploraci�n
}


