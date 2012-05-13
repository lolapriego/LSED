#include "MT-10.h"
#include "printf.h"

  void GestionAvanzada(){
    char opcion;

    do{
      printf("Seleccione una de las siguientes opciones avanzada\n");
      output("1) Seleccion de pre-sets de Reverberacion\n");
      output("2) Seleccion de pre-sets de Ecualizacion\n");
      output("E) Volver al menu principal")
      output("====================\n");

      opcion=teclado();

      switch (opcion){
        case '1': printf("Seleccion de parametros de Reverberacion\n");
                  GestionAvanzadaReverberacion();
                  break;

        case '2': printf("Configuracion de Ecualizacion\n");
                  GestionAvanzadaEcualizacion();
                  break;

        case 'E': printf("Incorporacian de Reverberacion Simple\n");
                  GestionReverberacion();
                  break;

        default: output("====\nTecla no valida, por favor vuelva a introducir una opcion\n");
      }
    }while(opcion!= 'E');
  }

  void GestionAvanzadaReverberacion(){
    static int atenuaciones[9] = {3, 4, 5, 2, 0, 0, 0, 0, 0};
    static int retardos[9] = { 800, 1600, 800, 3200, 0, 0, 0, 0, 0};
    static int parametros = 4;

    int i;
    int opcion;

    printf("\n=========\nSeleccione alguno de los siguientes parametros de reverberacion a aplicar:\n");
    for (i=0, i< parametros; i++)
      printf("%d) Atenuacion: %d   Reverberacion: %d\n", i + 1, atenuaciones[i], retardos[i]);
    printf("O pulse E para añadir un parametro de reverberacion y aplicarlo\n");

    opcion = teclado() - '0';
    while ((opcion<= 0 || opcion > parametros)&&opcion!= 21){
      printf("Por favor pulse una tecla correcta\n");
      opcion = teclado();
    }

    if(opcion >0 && opcion <= parametros){
      atenuacion_reverberacion = atenuaciones[opcion -1];
      retardo_reverberacion = retardos[opcion - 1];

      estado = 3;
    }

    else if(opcion == 21 && parametros < 9){
      GestionReverberacion();
      atenuaciones[parametros] = atenuacion_reverberacion;
      retardos[parametros] = retardo_reverberacion;

      estado = 3;
      parametros ++;
    }

  }

  void GestionAvanzadaEcualizacion(){
    static int ecualizaciones[9][7] = {{0,1,2,3,4,5,6},{0,15,0,15,0,15,0},{15,13,11,9,7,5,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
    static int parametros = 3;

    int i;
    int opcion;

    printf("\n=========\nSeleccione alguno de los siguientes parametros de ecualizacion a aplicar:\n");

    for (i=0, i< parametros; i++){
      printf("\n====BANDA:     32Hz   64Hz   125Hz   250Hz   500Hz   1kHz   2kHz=====\n");
      printf("%d)Nivel      %d      %d      %d       %d       %d       %d       %d\n", i + 1, ecualizaciones[i][0], ecualizaciones[i][1], ecualizaciones[i][2]
      ecualizaciones[i][3], ecualizaciones[i][4], ecualizaciones[i][5], ecualizaciones[i][6]);
    }

    printf("O pulse E para añadir un parametro de reverberacion y aplicarlo\n");

    opcion = teclado() - '0';
    while ((opcion<= 0 || opcion > parametros)&&opcion!= 21){
      printf("Por favor pulse una tecla correcta\n");
      opcion = teclado();
    }

    if(opcion >0 && opcion <= parametros){
      nv = ecualizaciones [opcion - 1];

      estado = 2;
    }

    else if(opcion == 21 && parametros < 9){
      printf("=====\nIntroduzca un nuevo parametro de ecualizacion, pulse de forma pausa el nivel para las distintas bandas de frecuencia\n");
      for(i = 0;i < 7; i++){
        printf("Nivel para la banda del filtro%d\n", i + 1);
        do{
          opcion = teclado() - '0';
        } while(opcion< 0 || opcion > 9);
        ecualizaciones[parametros][i] = opcion;
      }
      nv = ecualizaciones[parametros];

      estado = 2;
      parametros ++;
    }
  }
