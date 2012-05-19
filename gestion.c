#include "MT-10.h"
#include "printf.h"

  // ===========
  // Gestion Caracterizacion
  // funcion que implementa la UI para la caracterizacion de filtros
  // ===========
  void GestionCaracterizacion(){
    int opcion;

    printf("Seleccione el filtro que desea caracterizar del 1 al 7 o cualquier otra tecla para volver al inicio\n");

    opcion=teclado() - '0';
    if(opcion >=1 && opcion <= 7){
      filtro= opcion - '0' -1;
      printf("Ha seleccionado el filtro: %d\n",filtro +1);
    }

    estado = 1;
  }


  // ========
  // GestionEcualizazion
  // funcion que implementa la UI para la gestion de los niveles de energia en la Ecualizacion
  // ========
   void GestionEcualizacion(){
    char opcion;
    int banda;
    char nivel;

    static int ganancia_energia [16] = {1024, 790, 610, 471, 364, 281, 217, 167, 129, 99, 77, 59, 46, 35, 27, 21};

    estado = 2;

    do{
      printf("\nBANDA:     32Hz   64Hz   125Hz   250Hz   500Hz   1kHz   2kHz\n");
      printf("Ganancia:  %d   %d   %d    %d    %d    %d   %d\n", ganancia_energia[nv[0]], ganancia_energia[nv[1]],
        ganancia_energia[nv[2]], ganancia_energia[nv[3]], ganancia_energia[nv[4]], ganancia_energia[nv[5]], ganancia_energia[nv[6]]);
      printf("====\nNivel)      %d      %d      %d       %d       %d       %d       %d\n", nv[0], nv[1], nv[2], nv[3], nv[4], nv[5], nv[6]);
      printf("\nSeleccione la banda de la que desea modificar su nv de energia\n\n o pulse E para salir\n");

      opcion = teclado();
      if (opcion == 'E')
        break;

      while(opcion <'1' || opcion>'7'){
        printf("Por favor seleccione una banda correcta, de %d a %d\n",1,7);
        opcion = teclado();
      }

      banda = opcion - '0' -1;

      do{ //con este bucle una vez seleccionada una banda permite modificarla hasta seleccionar la tecla 5
        printf("Seleccione %d o %d para modificar el nv de energia de la banda %d,\n===\n o %d para terminar\n", 8,9, banda +1, 5);
        nivel = teclado();
        if (nivel == '8' && nv[banda] != 0)
          nv[banda]--;
        else if (nivel =='9' && nv[banda] != 15)
          nv[banda]++;
        else
          printf("Error, pulse la tecla adecuada la proxima vez");
      } while(nivel != '5');

    } while (TRUE);
  }


  // ==============
  // GestionReverberacion
  // funcion que implementa la UI para la gestion de los parametros de la reverberacion:
  // atenuacion y retardo
  // ==============
  void GestionReverberacion(){

    printf ("\nSeleccione la atenuacion para el efecto reverberacion\n");
    atenuacion_reverberacion = teclado() - '0';

    printf("\nSeleccione un retardo en escala de\n x100 milisegundos para el efecto reverberacion\n");
    retardo_reverberacion = (teclado() - '0') * 800; // pasamos la tecla seleccionada al número de interrupciones necesarias

    printf("\nHa seleccionado una atenuacion de: %d \nY un retardo de: %d\n", atenuacion_reverberacion, retardo_reverberacion);
    estado = 3;
  }
