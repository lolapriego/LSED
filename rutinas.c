#include "MT-10.h"

#define BORRA_REF 0x0002 // Valor de borrado de interr. pendientes de tout1 para TER0

  // ===============
  // Rutina de atención con una frecuencia de 8Khz
  //
  // Descripción:
  // Función de atención a la interrupción para TIMER0
  // Si se ha empezado a filtrar llama a la función filtrado con cada interrupción
  // Igual para la ecualización
  // A su vez calcula el nv de energía y lo saca por el vúmetro cada 3ms
  // ================
  void rutina_tout0(void){
    int tension;
    static int buffer [7200];
    static int salida = 0;
    static int contador = 0;


    mbar_writeShort(MCFSIM_TER0,BORRA_REF); // Reset del bit de fin de cuent
    if( estado == 1){
      tension = filtrado(leerADC());
      DAC_dato(tension + 0x800);
      if(fila_ilum == filtro)
        nv_energia+= tension * tension;
    }

    else if (estado == 2){
      tension = filtradoMultiple();
      DAC_dato (tension + 0x800 );
    }
    else if (estado == 3){
      tension = bufferCircular(salida, buffer);
      salida = tension + leerADC();
      DAC_dato( salida + 0x800);
    }

  if(contador<24){
    contador++;
  }
  if (contador >= 24){ // cada 3 ms se ejecuta este bloque
    puertoExcitaFilaLeds();

    nv_energia=0;
    contador =0;

    if(fila_ilum<7)
      fila_ilum++;

    if(fila_ilum==7)
      fila_ilum=0;
    }
  }

  void rutina_int1(void){
  }

  void rutina_int2(void){
  }

  void rutina_int3(void){
  }

  void rutina_int4(void){
  }

  void rutina_tout1(void){
  }

  void rutina_tout2(void){
  }

  void rutina_tout3(void){
  }



