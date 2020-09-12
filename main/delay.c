#include "delay.h"
//#include "xtensa/hal.h"

void delay_us(unsigned int time_us)//lleva de argumento el tiempo en us de delay requerido
{	
	unsigned int cuenta_anterior =0;
	unsigned int cuenta_actual;
	unsigned int delta_cuentas=0;
	
	cuenta_actual= xthal_get_ccount(); //cuenta actual
	if(cuenta_actual>cuenta_anterior){ //si no hubo overflow en ccount
		delta_cuentas= cuenta_actual-cuenta_anterior;
		while(delta_cuentas<=us_to_counts(time_us)){
			cuenta_anterior=cuenta_actual;
			cuenta_actual=xthal_get_ccount();
			delta_cuentas= cuenta_actual-cuenta_anterior;
		}
	}else{
		delta_cuentas= (0xffffffff-cuenta_anterior)+cuenta_actual;
		while(delta_cuentas<=us_to_counts(time_us)){
		cuenta_anterior=cuenta_actual;
		cuenta_actual=xthal_get_ccount();
		delta_cuentas=cuenta_actual-cuenta_anterior;// actualizo variable delta_cuentas
		}
	}
}
