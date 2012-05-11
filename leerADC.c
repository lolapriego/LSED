#include "MT-10.h"

  // =============
  // lee un dato del ADC, le aplica la máscara para la extensión de signo y devuelve la lectura
  // =============
  int leerADC(){
    int lectura;
    lectura = ADC_dato();
    if(lectura & 0x00000800)
      lectura = lectura | 0xFFFFF000;

    return lectura;
  }
