#include "m5272lib.c"
#include "m5272lcd.c"
#include "printf.c"
#include "m5272adc_dac.c"
#include "teclado.c"
#include "m5272.h"


#define V_BASE 0x40 // Dirección de inicio de los vectores de interrupción
#define DIR_VTMR0 4*(V_BASE+5) // Dirección del vector de TMR0

#define FREC_INT 8000 // Frec. de interr. TMR0 = 8000 Hz
#define PRESCALADO 2
#define CNT_INT1 MCF_CLK/(FREC_INT*PRESCALADO*16) // Valor de precarga del temporizador de interrupciones TRR0
#if CNT_INT1>0xFFFF
#error PRESCALADO demasiado pequeño para esa frecuencia (CNT_INT1>0xFFFF)
#endif
#define BORRA_REF 0x0002 // Valor de borrado de interr. pendientes de tout1 para TER0

#define NUM_FILT 7
#define NUM_FILT 7
#define V_MAX 5
#define FONDO_ESCALA 0xFFF
#define B0 1024
#define B1 0
#define B2 -1024


typedef struct{ //realmente son constantes, pensar donde se usan y quitarlo de global!
int banda;
int ganancia;
int nv;
int gain;
}t_filtro;

typedef struct {
WORD variableAux;
}
TpuertoSalida;
TpuertoSalida puerto;

t_filtro filtros[NUM_FILT];

int nv[7];
int historia[2][7]; //pendiente de retocar para que no sea global podría ser static en filtrado
int estadoFiltrado; //variable global que marca el estado del sistema para filtrar
int filtro; //variable global que marca el filtro seleccionado en el sistema
int nEnergias [8] = {200,559,1567,4386,12280, 34374,96223,269354, 753992,};
int estadoMuestra;



void initInt(void);
void GestionMenuPrincipal (void);
void GestionCaracterizacion(void);
void GestionEcualizacion(void);
void GestionReverberacion(void);

void swInit(void);
void hwInit(void);
void outputDAC(int *senal, int numMuestras);
void iO (void);

void rutina_tout0(void);
void rutina_int1(void);
void rutina_int2(void);
void rutina_int3(void);
void rutina_int4(void);
void rutina_tout1(void);
void rutina_tout2(void);
void rutina_tout3(void);

int leerADC(void);
int filtrado(int tension); //pendiente de hacer una sola matriz historial
int filtradoMultiple (void);
void puertoExcitaFilaLeds(void);

