
#include "utils.h"

void bubbleSort(uint16_t * samples, uint8_t total){

	for (uint8_t i = 0; i < total; i++){
		uint16_t aux;
		uint16_t maximum = 0;
		uint8_t where = 0;

		for (uint8_t j = i; j < total; j++){
			if(maximum < samples[j]){
				maximum = samples[j];

				where = j;
			}
		}

		if(where){
			aux = samples[i];
			samples[i] = samples[where];
			samples[where] = aux;	
		}
	}

	for (uint8_t i = 0; i < total; i++)
	{
		log_printf(__func__, debug9, "Sorted: %d", samples[i]);
	}
	
}
