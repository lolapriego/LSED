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
      printf("%d) Atenuacion: %d   Reverberacion: %d\n");
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

      parametros ++;
    }

  }
