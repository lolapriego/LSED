  int retardo_reverberacion;
  int atenuacion_reverberacion;

 

 // =============
  // lee un dato del ADC lo almacena en un buffer, lo suma y lo devuelve
  // =============
 INLINE int bufferCircular (int tension, int * puntero_buffer, int * buffer) {

    if( puntero_buffer < buffer + retardo_reverberacion){
      * puntero_buffer = tension / atenuacion_reverberacion;
      puntero_buffer ++;
    }
    else{
      puntero_buffer = buffer;
      *puntero_buffer = tension / atenuacion_reverberacion;
    }

    return buffer[retardo_reverberacion -1];
  }
