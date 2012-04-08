//------------------------------------------------------------------------------
// InterrTimer_GNU.c
//
//   Ejemplo para aprender el manejo de interrupciones temporizadas
//   y fines de temporización mediante un temporizador con el sistema
//   ENT2004CF.
//
// Autores: Juan Manuel Montero, Rubén San Segundo y Ricardo de Córdoba
// Adaptado a C por: Javier Guillén Álvarez
//------------------------------------------------------------------------------
#include "m5272.h"
#include "m5272lib.c"

#define V_BASE 0x40				  // Dirección de inicio de los vectores de interrupción
#define DIR_VTMR0 4*(V_BASE+5)			  // Dirección del vector de TMR0

#define FREC_INT 8000				  // Frec. de interr. TMR0 = 8000 Hz 
#define PRESCALADO 2
#define CNT_INT1 MCF_CLK/(FREC_INT*PRESCALADO*16)	  // Valor de precarga del temporizador de interrupciones TRR0
#if CNT_INT1>0xFFFF
#error PRESCALADO demasiado pequeño para esa frecuencia (CNT_INT1>0xFFFF)
#endif
#define BORRA_REF 0x0002			  // Valor de borrado de interr. pendientes de tout1 para TER0



//------------------------------------------------------
// void initInt(void)
//
// Descripción:
//   Función por defecto de inicialización de la interrupción del timer 0
//------------------------------------------------------
void initInt()
{
  mbar_writeByte(MCFSIM_PIVR,V_BASE);			// Fija comienzo de vectores de interrupción en V_BASE.
  ACCESO_A_MEMORIA_LONG(DIR_VTMR0)= (ULONG)_prep_TOUT0; // Escribimos la dirección de la función para TMR0
  output("COMIENZA EL PROGRAMA\r\n");
  mbar_writeShort(MCFSIM_TMR0, (PRESCALADO-1)<<8|0x3D);		// TMR0: PS=1-1=0 CE=00 OM=1 ORI=1 FRR=1 CLK=10 RST=1
  mbar_writeShort(MCFSIM_TCN0, 0x0000);		// Ponemos a 0 el contador del TIMER0
  mbar_writeShort(MCFSIM_TRR0, CNT_INT1);	// Fijamos la cuenta final del contador
  mbar_writeLong(MCFSIM_ICR1, 0x8888C888);	// Marca la interrupción del TIMER0 como no pendiente
  sti();					// Habilitamos interrupciones
}




