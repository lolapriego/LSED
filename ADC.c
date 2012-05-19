//------------------------------------------------------------------------------
// ADC_GNU.c
//
//   Programa sencillo para mostrar el manejo de un conversor Analógico
//   Digital utilizando QSPI (Queued Serial Peripheral Interface).
//
// Autores: Juan Manuel Montero y Rubén San Segundo modificado por
// Mª Dolores Priego Porcuna
// Adaptado a C por: Javier Guillén Álvarez
//------------------------------------------------------------------------------
#include "m5272lib.c"
#include "m5272adc_dac.c"
#include "printf.c"

#define FONDO_ESCALA 0xFFF	// Valor de lectura máxima del ADC
#define V_MAX 5			// Valores de tensión máxima del ADC

void leeADC(void)
{
  int lectura;
  double tension;

  // Imprime intrucciones
  output("\r\nCONECTE EN LA ENTRADA ANALÓGICA 1 UNA TENSIÓN ENTRE 0 Y 5 V.\r\n");
  output("PULSE UNA TECLA PARA REALIZAR LA MEDIDA...\r\n");

  // Espera a recibir un carácter
  inch();

  // Lee un dato de la entrada analógica 1
//while(TRUE)
//
  lectura = ADC_dato();
printf("%d",lectura);
if(lectura & 0x00000800){
output("mask/n/n");
lectura = lectura | 0xFFFFF000;
printf("%d",lectura);

}
  // Calcula la tensión correspondiente al valor leído
  tension = V_MAX * ((double)lectura/FONDO_ESCALA);


  // Imprime la primera parte del mensaje de resultado
  output("VALOR MEDIDO ENTRE 0V (000000000000) Y 5V (111111111111) ES: ");
printf("%d",tension);
  // Imprime 'tensión' con 2 cifras decimales sin signo
  outNumDec(10,tension,2,SIN_SIGNO);
  output("V (");

  // Imprime el dato leído en binario
  outNum(2, lectura, SIN_SIGNO);

  // Imprime el final del mensaje de resultado
  output(")\r\n");
}

