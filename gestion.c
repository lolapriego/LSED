#include "MT-10.h"
#include "printf.h"

 // ===========
  // Gestion Caracterizacion
  // ===========
  void GestionCaracterizacion(){
    char opcion;

    printf("Seleccione el filtro que desea caracterizar u 8 para volver al menu inicio\n");

    opcion=teclado();
    if(opcion=='8')
      return;
    filtro= opcion - '0' -1;
    printf("Ha seleccionado el filtro: %d\n",filtro +1);

    estado = 1;
  }


  // ========
  // GestionEcualizazion
  // ========
   void GestionEcualizacion(){
    char opcion;
    int banda;
    char nivel;

    static int ganancia_energia [9] = {1024, 610, 364, 217, 129, 77, 46, 27, 21};

    estado = 2;

    do{
      printf("\nBANDA:     32Hz   64Hz   125Hz   250Hz   500Hz   1kHz   2kHz\n");
      printf("Ganancia:  %d   %d   %d    %d    %d    %d   %d\n", ganancia_energia[nv[0]], ganancia_energia[nv[1]],
        ganancia_energia[nv[2]], ganancia_energia[nv[3]], ganancia_energia[nv[4]], ganancia_energia[nv[5]], ganancia_energia[nv[6]]);
      printf("(Nivel)      %d      %d      %d       %d       %d       %d       %d\n", nv[0], nv[1], nv[2], nv[3], nv[4], nv[5], nv[6]);
      printf("Seleccione la banda de la que desea modificar su nv de energia o pulse %d o %d para salir\n",8,9);

      opcion = teclado();
      if (opcion == '8' || opcion == '9')
        break;

      while(opcion <'1' || opcion>'7'){
        printf("Por favor seleccione una banda correcta, de %d a %d\n",1,7);
        opcion = teclado();
      }

      banda = opcion - '0' -1;

      do{ //con este bucle una vez seleccionada una banda permite modificarla hasta seleccionar la tecla 5
        printf("Seleccione %d o %d para modificar el nv de energia de la banda %d, o %d para terminar\n", 8,9, banda +1, 5);
        nivel = teclado();
        if (nivel == '8' && nv[banda] != 0)
          nv[banda]--;
        else if (nivel =='9' && nv[banda] != 8)
          nv[banda]++;
        else
          printf("Error, pulse la tecla adecuada la proxima vez");
      } while(nivel != '5');

    } while (opcion != '8');
  }


  // ==============
  // GestionReverberacion
  // ==============
  void GestionReverberacion(){

    printf ("\nSeleccione la atenuación para el efecto reverberación\n");
    atenuacion_reverberacion = teclado() - '0';

    printf("\nSeleccione un retardo en escala de x100 milisegundos para el efecto reverberacion\n");
    retardo_reverberacion = (teclado() - '0') * 800; // pasamos la tecla seleccionada al número de interrupciones necesarias

    estado = 3;
  }
