#include "m5272lib.c"
#include "m5272lcd.c"
#include "printf.c"
#include "m5272adc_dac.c"
#include "teclado.c"
#include "m5272.h"

#include "MT-10.h"

#include "bufferCircular.c"
#include "gestion.c"
#include "rutinas.c"
#include "leerADC.c"
#include "filtrar.c"
#include "outputVumetro.c"

#define V_BASE 0x40 // Direcci�n de inicio de los vectores de interrupci�n
#define DIR_VTMR0 4*(V_BASE+5) // Direcci�n del vector de TMR0

#define FREC_INT 8000 // Frec. de interr. TMR0 = 8000 Hz
#define PRESCALADO 2
#define CNT_INT1 MCF_CLK/(FREC_INT*PRESCALADO*16) // Valor de precarga del temporizador de interrupciones TRR0
#if CNT_INT1>0xFFFF
#error PRESCALADO demasiado peque�o para esa frecuencia (CNT_INT1>0xFFFF)
#endif

#define V_MAX 5

  int fila_ilum;
  int nv_energia; //marca el nv de energía de la señal a sacar por el vúmetro


  int nv[7]; //marca el nv de energía seleccionado en ecualización
  int estado; //variable global que marca el estado del sistema para filtrar
  int filtro; //variable global que marca el filtro seleccionado en el sistema

  int retardo_reverberacion;
  int atenuacion_reverberacion;

  void initInt(void);

  void swInit(void);
  void hwInit(void);


  // ===================
  // Funcion bucleMain
  // ===================
  void bucleMain(void){
    char opcion;

    printf("Seleccione una de las siguientes opciones\n");
    output("1) Caracterizacion de filtros\n");
    output("2) Ecualizacion Grafica\n");
    output("3) Incorporacion de Reverberacion Simple\n");
    output("4) Gestion de Reverberacion --- Avanzado---");
    output("====================\n");

    opcion=teclado();

    switch (opcion){
      case '1': printf("Caracterizacion de filtros\n");
                GestionCaracterizacion();
                break;

      case '2': printf("Ecualizacion Gráfica\n");
                GestionEcualizacion();
                break;

      case '3': printf("Incorporacian de Reverberacion Simple\n");
                GestionReverberacion();
                break;

      case '4': printf("Gestion de parametros de Reverberacion\n");
                GestionAvanzadaReverberacion();
                break;

      default: output("Tecla no valida\n");
    }
  }




  // ==============
  // inicio del Sistema
  // ==============
  void __init(){
    swInit();
    hwInit();
  }


  // ==============
  // inicia las variables de SW
  // ==============
  void swInit(){
    int i;

    for(i=0; i<7; i++){
      nv[i] = 0;
    }

    estado=0;
    filtro=0; //filtro por defecto

    fila_ilum = 0;
    nv_energia = 0;

    retardo_reverberacion = 0;
    atenuacion_reverberacion = 0;
  }


  // ==========
  // Inicio del Hardware
  // ==========
  void hwInit(){
    initInt();
    DAC_ADC_init();
  }


  // ===========
  // Inicializa los registros para la interrupción del timer 0
  //
  // Descripción:
  // Función por defecto de inicialización de la interrupción del timer 0
  // =============
  void initInt(){
    mbar_writeByte(MCFSIM_PIVR,V_BASE); // Fija comienzo de vectores de interrupción en V_BASE.
    ACCESO_A_MEMORIA_LONG(DIR_VTMR0)= (ULONG)_prep_TOUT0; // Escribimos la dirección de la función para TMR0

    output("COMIENZA EL PROGRAMA\r\n");

    mbar_writeShort(MCFSIM_TMR0, (PRESCALADO-1)<<8|0x3D); // TMR0: PS=1-1=0 CE=00 OM=1 ORI=1 FRR=1 CLK=10 RST=1
    mbar_writeShort(MCFSIM_TCN0, 0x0000); // Ponemos a 0 el contador del TIMER0
    mbar_writeShort(MCFSIM_TRR0, CNT_INT1); // Fijamos la cuenta final del contador
    mbar_writeLong(MCFSIM_ICR1, 0x8888C888); // Marca la interrupción del TIMER0 como no pendiente
    sti(); // Habilitamos interrupciones
  }

