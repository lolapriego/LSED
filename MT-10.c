#include "MT-10.h"

  int fila_ilum;
  int nv_energia;
  int contador;

  int retardo_reverberacion;
  int amplitud_reverberacion;

  int bufferCircular(int);

  // ===================
  // Funcion bucleMain
  // ===================
  void bucleMain(void){
    char opcion;

    printf("Seleccione una de las siguientes opciones\n");
    output("1) Caracterizacion de filtros\n");
    output("2) Ecualizacion Grafica\n");
    output("3) Incorporacion de Reverberacion Simple\n");
    output("====================\n");

    opcion=teclado();

    switch (opcion){
      case '1': printf("Caracterizacion de filtros\n");
                GestionCaracterizacion();
                break;
      case '2': printf("Ecualizacion Gráfica\n");
                GestionEcualizacion();
                break;
      case'3': printf("Incorporacian de Reverberacion Simple\n");
               GestionReverberacion();
               break;
      default: output("Tecla no valida\n");
    }
  }


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
    estado = 2;

    do{
      printf("\nBANDA:     32Hz   64Hz   125Hz   250Hz   500Hz   1kHz   2kHz\n");
      printf("Ganancia:  %d   %d   %d    %d    %d    %d   %d\n", filtros[0].gain, filtros[1].gain, filtros[2].gain, filtros[3].gain, filtros[4].gain, filtros[5].gain, filtros[6].gain);
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
    atenuacion = teclado() - '0';

    printf("\nSeleccione un retardo en escala de x100 milisegundos para el efecto reverberacion\n");
    retardo_reverberacion = (teclado() - '0') * 800; // pasamos la tecla seleccionada al número de interrupciones necesarias

    estado = 3;
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
    int j;

    for(i=0; i<7; i++){
      nv[i] = 0;
    }

    filtros[0].banda=32; // no habría que inicializarlo aquí, no variables globale son constantes!!
    filtros[0].ganancia=8;
    filtros[0].nv=0;
    filtros[0].gain = 1024;

    filtros[1].banda=64;
    filtros[1].ganancia=17;
    filtros[1].nv=0;
    filtros[1].gain = 1024;

    filtros[2].banda=125;
    filtros[2].ganancia=34;
    filtros[2].nv=0;
    filtros[2].gain = 1024;

    filtros[3].banda=250;
    filtros[3].ganancia=66;
    filtros[3].nv=0;
    filtros[3].gain = 1024;

    filtros[4].banda=500;
    filtros[4].ganancia=125;
    filtros[4].nv=0;
    filtros[4].gain = 1024;

    filtros[5].banda=1000;
    filtros[5].ganancia=227;
    filtros[5].nv=0;
    filtros[5].gain = 1024;

    filtros[6].banda=2000;
    filtros[6].ganancia=392;
    filtros[6].nv=0;
    filtros[6].gain = 1024;

    estado=0;
    filtro=0; //filtro por defecto

    for(i=0; i<2 ;i++)
      for(j = 0; j < 7; j++)
        historia[i][j] = 0;

    estadoMuestra=0;

    fila_ilum = 0;
    nv_energia = 0;
    contador = 0;

    retardo_reverberacion = 0;
    atenuacion = 0;
  }


  // ==========
  // Inicio del Hardware
  // ==========
  void hwInit(){
    initInt();
    DAC_ADC_init();
  }


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
    int energia;

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
      if (retardo_reverberacion == 0){
        DAC_dato ( 000000 + 0x800); // aquí se llamará la función de BufferCircular
      }
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


  // =============
  // recibe una tensión de entrada, y según el filtro en el que se encuentre el sistema
  // aplica el sistema de la documentación
  // =============
  int filtrado(int tension_ent){
    static int a [2][7] = {{-2029, -2011, -1970, -1878, -1660, -1115, 141} , {1006, 988, 955, 890, 772, 569, 239}};
    static int B0 = 1024;
    static int B1 = 0;
    static int B2 = -1024;
    int salida;
    int aux;

    aux = historia[0][filtro];

    salida = B0 * tension_ent -a[0][filtro] * historia[0][filtro] -a[1][filtro] * historia[1][filtro];
    historia[0][filtro] = salida >> 10;

    salida += B1 * historia[0][filtro] + historia[1][filtro] * B2;
    historia[1][filtro] = aux;

    salida = salida >> 10;
    salida = salida * filtros[filtro].ganancia;
    salida = salida >> 10;
    return salida;
  }


  // =============
  // llama a la función filtrado para cada filtro, y suma sus salidas según la ganancia seleccionada mediante
  // la interfaz. Además gestiona la salida del nv de energía por el vúmetro
  // =============
  int filtradoMultiple () {
    int output;
    int i;
    int salida_unica;
    int tension;
    int ganancia_energia [9] = {1024, 610, 364, 217, 129, 77, 46, 27, 21};

    output = 0;
    tension = leerADC();
    for(i=0; i<7 ;i++){
      filtro = i;

      salida_unica =  (filtrado(tension) * ganancia_energia[nv[i]]) >> 10;
      output += salida_unica;

      if(i==fila_ilum)
        nv_energia += salida_unica*salida_unica;
    }

    output = output >> 1;
    return output;
  }


  // =============
  //
  // =============
  void puertoExcitaFilaLeds(void){
    static UWORD led[9]={0x0000,0x0100,0x0300,0x0700,0x0F00,0x1F00,0x3F00,0x7F00,0xFF00};
    int i;
    UWORD valor = 1;
    UWORD valor_previo = 1;
   // Valor a escribir en el puerto de salida
    static UINT retVal = 3000; // Retardo introducido en microsegundos. (aprox. 3ms)
    int nivelEnergia;
    valor = fila_ilum <<4;
    puerto_S= puerto_S & 0xFF0F;
    puerto_S= valor | puerto_S;
    nivelEnergia=0;

    for(i=1; i < 8; i++){
      if( ( nEnergias[i-1] < nv_energia) && (nv_energia <= nEnergias[i]) ){
        nivelEnergia=i;
        break;
      }
    }

    if(nv_energia > nEnergias[8])
      nivelEnergia = 8;

    puerto_S = puerto_S & 0x00FF;
    puerto_S = puerto_S | led[nivelEnergia] | valor;
    set16_puertoS(puerto_S);
  }


  // =============
  // lee un dato del ADC lo almacena en un buffer, lo suma y lo devuelve
  // =============
  int bufferCircular () {
    int buffer[]
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


