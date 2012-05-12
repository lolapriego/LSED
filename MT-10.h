extern int nv[7];
extern int retardo_reverberacion;
extern int atenuacion_reverberacion;

extern int filtro;
extern int estado;

extern int fila_ilum;
extern int nv_energia;

int leerADC(void);
void DAC_dato(int dato);
int filtradoMultiple (void);
void puertoExcitaFilaLeds(void);
int bufferCircular (int tension, int *buffer);
int filtrado(int tension); //pendiente de hacer una sola matriz historial
char teclado(void);
int ADC_dato(void);

