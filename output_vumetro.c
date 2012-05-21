#include "MT-10.h"

  // =============
  //
  // =============
  void puertoExcitaFilaLeds(void){
    int nEnergias [9] = {200,559,1567,4386,12280, 34374,96223,269354, 753992};
    static UWORD led[9]={0x0000,0x0100,0x0300,0x0700,0x0F00,0x1F00,0x3F00,0x7F00,0xFF00};

    int i;
    UWORD valor = 1;
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
