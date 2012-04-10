//------------------------------------------------------------------------------
// ADC_GNU.c
//
//   Programa sencillo para mostrar el manejo de un conversor Anal�gico
//   Digital utilizando QSPI (Queued Serial Peripheral Interface).
//
// Autores: Juan Manuel Montero y Rub�n San Segundo!!!!!
// Adaptado a C por: Javier Guill�n �lvarez
//------------------------------------------------------------------------------
#include "m5272lib.c"
#include "m5272adc_dac.c"
#include "printf.c"

#define FONDO_ESCALA 0xFFF	// Valor de lectura m�xima del ADC
#define V_MAX 5			// Valores de tensi�n m�xima del ADC

//------------------------------------------------------
// void __init(void)
//
// Descripci�n:
//   Funci�n por defecto de inicializaci�n del sistema
//
//------------------------------------------------------
void __init(void)
{
  DAC_ADC_init();
}

//------------------------------------------------------
// void bucleMain(void)
//
// Descripci�n:
//   Funci�n del programa principal
//
//------------------------------------------------------
void leeADC(void)
{
  int lectura;
  double tension;

  // Imprime intrucciones
  output("\r\nCONECTE EN LA ENTRADA ANAL�GICA 1 UNA TENSI�N ENTRE 0 Y 5 V.\r\n");
  output("PULSE UNA TECLA PARA REALIZAR LA MEDIDA...\r\n");

  // Espera a recibir un car�cter
  inch();

  // Lee un dato de la entrada anal�gica 1
//while(TRUE)
//
  lectura = ADC_dato();
printf("%d",lectura);
if(lectura & 0x00000800){
output("mask/n/n");
lectura = lectura | 0xFFFFF000;
printf("%d",lectura);

}
  // Calcula la tensi�n correspondiente al valor le�do
  tension = V_MAX * ((double)lectura/FONDO_ESCALA);


  // Imprime la primera parte del mensaje de resultado
  output("VALOR MEDIDO ENTRE 0V (000000000000) Y 5V (111111111111) ES: ");
printf("%d",tension);
  // Imprime 'tensi�n' con 2 cifras decimales sin signo
  outNumDec(10,tension,2,SIN_SIGNO); 
  output("V (");

  // Imprime el dato le�do en binario
  outNum(2, lectura, SIN_SIGNO);

  // Imprime el final del mensaje de resultado
  output(")\r\n");
}

//---------------------------------------------------------
// Definici�n de rutinas de atenci�n a la interrupci�n
// Es necesario definirlas aunque est�n vac�as
void rutina_int1(void){}
void rutina_int2(void){}
void rutina_int3(void){}
void rutina_int4(void){}
void rutina_tout0(void){}
void rutina_tout1(void){}
void rutina_tout2(void){}
void rutina_tout3(void){}
