#include "MT-10.h"

  // =============
  // lee un dato del ADC lo almacena en un buffer, lo suma y lo devuelve
  // =============
  int bufferCircular (int tension, int *buffer) {
    static int *puntero_buffer = 0;
    static int flag = 0; // marca cuando empezamos a sacar del buffer
    int muestra = 0;

    if ( !puntero_buffer)
      puntero_buffer = buffer;

    if( puntero_buffer < buffer + retardo_reverberacion){
      if( flag )
        muestra = *puntero_buffer;

      *puntero_buffer = tension / atenuacion_reverberacion;
      puntero_buffer++;
    }

    else{
      flag = 1;
      puntero_buffer = buffer;
      muestra = *puntero_buffer;
      *puntero_buffer = tension / atenuacion_reverberacion;
      puntero_buffer++;
    }

     return muestra;

  }
