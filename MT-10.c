#include "MT-10.h"


//funciÃ³n que se ejecuta despuÃ©s de init
//se ejecuta constantemente, sirve para seleccionar el estado del sistema
void bucleMain(void){
char opcion;

printf("Seleccione una de las siguientes opciones\n");
output("1) CaracterizaciÃ³n de filtros\n");
output("2) EcualizaciÃ³n GrÃ¡fica\n");
output("3) IncorporaciÃ³n de ReverberaciÃ³n Simple\n");
output("---------------------------------\n");
opcion=teclado();

switch (opcion){
case '1': printf("CaracterizaciÃ³n de filtros\n");
	GestionCaracterizacion();
	break;
case '2': printf("EcualizaciÃ³n GrÃ¡fica\n");
	GestionEcualizacion();
	break;
case'3': printf("IncorporaciÃ³n de ReverberaciÃ³n Simple\n");
	GestionReverberacion();
	break;
default: output("Tecla no vÃ¡lida\n");
}
}//buclemain


//funciÃ³n para seleccionar el filtro con el que se desea filtrar
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
  //interfaz pendiente para energías;
  estadoFiltrado = 2;
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

	filtros[0].banda=32; // no habrÃ­a que inicializarlo aquÃ­, no variables globale son constantes!!
	filtros[0].ganancia=8;
	filtros[0].nv=0;
	filtros[1].banda=64;
	filtros[1].ganancia=17;
	filtros[1].nv=0;
	filtros[2].banda=125;
	filtros[2].ganancia=34;
	filtros[2].nv=0;
	filtros[3].banda=250;
	filtros[3].ganancia=66;
	filtros[3].nv=0;
	filtros[4].banda=500;
	filtros[4].ganancia=125;
	filtros[4].nv=0;
	filtros[5].banda=1000;
	filtros[5].ganancia=227;
	filtros[5].nv=0;
	filtros[6].banda=2000;
	filtros[6].ganancia=392;
	filtros[6].nv=0;

	estadoFiltrado=0;
	filtro=0; //filtro por defecto
	for(i=0; i<2 ;i++){
	  for(j = 0; j < 7; j++){
	    historia[i][j] = 0;
	  }
	}	
}

void hwInit(){
  initInt();
  DAC_ADC_init();


}


//------------------------------------------------------
// void rutina_tout0(void)
//
// DescripciÃ³n:
//   FunciÃ³n de atenciÃ³n a la interrupciÃ³n para TIMER0
// Si se ha empezado a filtrar llama a la funciÃ³n filtrado con cada interrupciÃ³n
//------------------------------------------------------
void rutina_tout0(void)
{
  mbar_writeShort(MCFSIM_TER0,BORRA_REF); 	// Reset del bit de fin de cuent
  if( estadoFiltrado == 1)
  DAC_dato(filtrado(leerADC(), filtro) + 0x800);
  else if (estadoFiltrado == 2)
  DAC_dato (filtradoMultiple() + 0x800 );

}

//------------------------------------------------------
// void initInt(void)
//
// DescripciÃ³n:
//   FunciÃ³n por defecto de inicializaciÃ³n de la interrupciÃ³n del timer 0
//------------------------------------------------------
void initInt()
{
  mbar_writeByte(MCFSIM_PIVR,V_BASE);			// Fija comienzo de vectores de interrupciÃ³n en V_BASE.
  ACCESO_A_MEMORIA_LONG(DIR_VTMR0)= (ULONG)_prep_TOUT0; // Escribimos la direcciÃ³n de la funciÃ³n para TMR0
  output("COMIENZA EL PROGRAMA\r\n");
  mbar_writeShort(MCFSIM_TMR0, (PRESCALADO-1)<<8|0x3D);		// TMR0: PS=1-1=0 CE=00 OM=1 ORI=1 FRR=1 CLK=10 RST=1
  mbar_writeShort(MCFSIM_TCN0, 0x0000);		// Ponemos a 0 el contador del TIMER0
  mbar_writeShort(MCFSIM_TRR0, CNT_INT1);	// Fijamos la cuenta final del contador
  mbar_writeLong(MCFSIM_ICR1, 0x8888C888);	// Marca la interrupciÃ³n del TIMER0 como no pendiente
  sti();					// Habilitamos interrupciones
}




void iO () //para comprobar rÃ¡pido que todo va bien, lee del ADC y deberÃ­a sacar lo mismo por el DAC escalado x2
{
WORD tension1;
int lectura;
  double tension;
  lectura = ADC_dato();
if(lectura & 0x00000800)
lectura = lectura | 0xFFFFF000;

// Calcula la tensiÃ³n correspondiente al valor leÃ­do
tension = ((double)lectura/FONDO_ESCALA);

tension1 = (tension * 0xFFF) ;
DAC_dato(tension1 + 0x800);
     
}




//lee un dato del ADC y devuelve la tensiÃ³n 
int leerADC(){
int lectura;
 lectura = ADC_dato();
if(lectura & 0x00000800)
lectura = lectura | 0xFFFFF000;
return lectura;
}






//dada una tensiÃ³n de entrada saca por el DAC la tensiÃ³n de salida filtrada

int filtrado(int tension_ent, int filtro){

int  salida;
int aux;
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
  output = 0;
   output += filtrado(leerADC(), filtro);
  //output = output >> 1;
  return output;
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


