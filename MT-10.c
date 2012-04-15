#include "MT-10.h"


//función que se ejecuta después de init
//se ejecuta constantemente, sirve para seleccionar el estado del sistema
void bucleMain(void){
char opcion;

printf("Seleccione una de las siguientes opciones\n");
output("1) Caracterización de filtros\n");
output("2) Ecualización Gráfica\n");
output("3) Incorporación de Reverberación Simple\n");
output("---------------------------------\n");
opcion=teclado();

switch (opcion){
case '1': printf("Caracterización de filtros\n");
GestionCaracterizacion();
break;
case '2': printf("Ecualización Gráfica\n");
GestionEcualizacion();
break;
case'3': printf("Incorporación de Reverberación Simple\n");
GestionReverberacion();
break;
default: output("Tecla no válida\n");
}
}//buclemain


//función para seleccionar el filtro con el que se desea filtrar
void GestionCaracterizacion(){
char opcion;
printf("Seleccione el filtro que desea caracterizar u 8 para volver al menu inicio\n");
opcion=teclado();
if(opcion=='8')
return;
filtro= opcion - '0' -1;
printf("Ha seleccionado el filtro: %d\n",filtro +1);
estadoFiltrado = 1;
}



void GestionEcualizacion(){ //implementar interfaz
  char opcion;
  int banda;
  char nivel;
  estadoFiltrado = 2; 

  do{
  printf("\nBANDA:     32Hz   64Hz   125Hz   250Hz   500Hz   1kHz   2kHz\n");
  printf("Ganancia:  %d   %d   %d    %d    %d    %d   %d\n", filtros[0].gain, filtros[1].gain, filtros[2].gain, filtros[3].gain, filtros[4].gain, filtros[5].gain, filtros[6].gain);
  printf("(Nivel)      %d      %d      %d       %d       %d       %d       %d\n", nv[0], nv[1], nv[2], nv[3], nv[4], nv[5], nv[6]);
  printf("Seleccione la banda de la que desea modificar su nv de energia\n");
  
  do{
  opcion = teclado();
  if(opcion <'1' || opcion>'7')
    printf("Por favor seleccione una banda correcta, de %d a %d\n",1,7,8,9);
  }while (opcion <'1' || opcion>'7');
  
  banda = opcion - '0' -1;
  printf("Seleccione %d o %d para modificar el nv de energia de la banda\n", 8,9, banda +1);

  nivel = teclado();
  if (nivel == '8' && nv[banda] != 0)
    nv[banda]--;
  else if (nivel =='9' && nv[banda] != 7)
    nv[banda]++;
  else
    printf("Error, pulse la tecla adecuada la proxima vez");
  } while (opcion != '8');
}


void GestionReverberacion(){
}

void __init(){
swInit();
hwInit();
}



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

estadoFiltrado=0;
filtro=3; //filtro por defecto
for(i=0; i<2 ;i++){
for(j = 0; j < 7; j++){
historia[i][j] = 0;
}
}
estadoMuestra=0;

}

void hwInit(){
  initInt();
  DAC_ADC_init();


}


//------------------------------------------------------
// void rutina_tout0(void)
//
// Descripción:
// Función de atención a la interrupción para TIMER0
// Si se ha empezado a filtrar llama a la función filtrado con cada interrupción
//------------------------------------------------------
void rutina_tout0(void)
{
  mbar_writeShort(MCFSIM_TER0,BORRA_REF); // Reset del bit de fin de cuent
  if( estadoFiltrado == 1)
  DAC_dato(filtrado(leerADC()) + 0x800);
  else if (estadoFiltrado == 2)
  DAC_dato (filtradoMultiple() + 0x800 );

}

//------------------------------------------------------
// void initInt(void)
//
// Descripción:
// Función por defecto de inicialización de la interrupción del timer 0
//------------------------------------------------------
void initInt()
{
  mbar_writeByte(MCFSIM_PIVR,V_BASE); // Fija comienzo de vectores de interrupción en V_BASE.
  ACCESO_A_MEMORIA_LONG(DIR_VTMR0)= (ULONG)_prep_TOUT0; // Escribimos la dirección de la función para TMR0
  output("COMIENZA EL PROGRAMA\r\n");
  mbar_writeShort(MCFSIM_TMR0, (PRESCALADO-1)<<8|0x3D); // TMR0: PS=1-1=0 CE=00 OM=1 ORI=1 FRR=1 CLK=10 RST=1
  mbar_writeShort(MCFSIM_TCN0, 0x0000); // Ponemos a 0 el contador del TIMER0
  mbar_writeShort(MCFSIM_TRR0, CNT_INT1); // Fijamos la cuenta final del contador
  mbar_writeLong(MCFSIM_ICR1, 0x8888C888); // Marca la interrupción del TIMER0 como no pendiente
  sti(); // Habilitamos interrupciones
}




void iO () //para comprobar rápido que todo va bien, lee del ADC y debería sacar lo mismo por el DAC escalado x2
{
WORD tension1;
int lectura;
  double tension;
  lectura = ADC_dato();
if(lectura & 0x00000800)
lectura = lectura | 0xFFFFF000;

// Calcula la tensión correspondiente al valor leído
tension = ((double)lectura/FONDO_ESCALA);

tension1 = (tension * 0xFFF) ;
DAC_dato(tension1 + 0x800);
     
}




//lee un dato del ADC y devuelve la tensión
int leerADC(){
int lectura;
 lectura = ADC_dato();
if(lectura & 0x00000800)
lectura = lectura | 0xFFFFF000;
return lectura;
}






//dada una tensión de entrada saca por el DAC la tensión de salida filtrada

int filtrado(int tension_ent){

int salida;
int aux;
int a [2][7] = {{-2029, -2011, -1970, -1878, -1660, -1115, 141} , {1006, 988, 955, 890, 772, 569, 239}};

salida= B0*tension_ent + B1* historia[0][filtro] -a[0][filtro]* historia[0][filtro] + historia[1][filtro]*(-a[1][filtro]+B2);
aux = historia[0][filtro];
historia[0][filtro] = ( B0*tension_ent -a[0][filtro]* historia[0][filtro] -a[1][filtro]*historia[1][filtro] ) >>10;
historia[1][filtro] = aux;
salida = salida >> 10;
salida = salida * filtros[filtro].ganancia;
salida = salida >> 10;
return salida;
}

int filtradoMultiple () {
  int output;
  int i;
  int tension;
  output = 0;
  tension = leerADC();
  for(i=0; i<7 ;i++){
   filtro =i;
   output += (filtrado(tension) * nv[0]) >> 10;
 } 

  output = output >> 1;
  return output;
}

void puertoExcitaFilaLeds(void){
  static int contador = 0;
  UWORD valor_previo;
  UWORD var;
    UWORD valor = 1;  // Valor a escribir en el puerto de salida
  UINT retVal = 3000; // Retardo introducido en microsegundos. (aprox. 3ms)// Desplazamiento del bit hacia la izquierda
  int nivelEnergia=0;
  int buffer[24];
  if(estadoMuestra){
  if(contador<24){
  buffer [contador] = leerADC();
  contador++;
  }
  for(int i=0;i<24;i++){
  nivelEnergia+=(buffer[i]*buffer[i])/24;
  }
  if(nivelEnergia<Energias[0]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0x0000 | valor_previo);
    }
    if(nivelEnergia<Energias[1]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0x0100 | valor_previo);
    }
    if(nivelEnergia<Energias[2]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0x0300 | valor_previo);
    }
    if(nivelEnergia<Energias[3]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0x0700 | valor_previo);
    }
    if(nivelEnergia<Energias[4]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0x0F00 | valor_previo);
    }
    if(nivelEnergia<Energias[5]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0x1F00 | valor_previo);
    }
    if(nivelEnergia<Energias[6]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0x3F00 | valor_previo);
    }
    if(nivelEnergia<Energias[7]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(07F00 | valor_previo);
    }
      if(nivelEnergia>=Energias[7]){
    var = lee16_puertoE();
    valor_previo = var & 0x00FF;
    set16_puertoS(0xFF00 | valor_previo);
    }
    }
  for(valor = 0x0000; valor < 0x0070; valor = valor + 16){
    retardo(retVal);
    var = lee16_puertoE();
    valor_previo = var & 0xFF0F;
        set16_puertoS(valor | valor_previo);
  }
  // Desplazamiento del bit hacia la derecha 
  for(valor = 0x0060; valor<= 0x0000; valor = valor - 16){
    retardo(retVal);
    var = lee16_puertoE();
    valor_previo = var & 0xFF0F;
      set16_puertoS(valor | valor_previo);
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


