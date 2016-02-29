#include "variables.h"

InitHeapZone HeapZone;

Menu Main_Menu;

//					   v	w		d		t  	24	cc	p	
int16_t volt_pval[] = {0, 	1, 		0, 		0, 	0, 	0, 	0};
int16_t volt_pmin[] = {0, 	1, 		0, 		0, 	0, 	0, 	0};
int16_t volt_pmax[] = {9999, 	999, 	9999, 	1, 	1, 	1, 	1};
float volt_punit[] =  {0.1,	0.1,	0.1,	1, 	1, 	1, 	1};
Marx Voltage_Source(VOLTAGE_SOURCE, 1, volt_pval, volt_pmin, volt_pmax, volt_punit);

int16_t curr_pval[] = {0, 	1, 		0, 		0, 	0, 	0, 	0};
int16_t curr_pmin[] = {0, 	1, 		0, 		0, 	0, 	0, 	0};
int16_t curr_pmax[] = {9999, 	999, 	9999, 	1, 	1, 	1, 	1};
float curr_punit[] =  {0.1,	0.1,	0.1,	1, 	1, 	1, 	1};
Marx Current_Source(CURRENT_SOURCE, 1, curr_pval, curr_pmin, curr_pmax, curr_punit);

//						  t		f		i
int16_t console_pval[] = {1, 	1, 		0};
int16_t console_pmin[] = {0, 	1, 		0};
int16_t console_pmax[] = {999, 	100, 	9999};
float console_punit[] =  {1,	0.01,	1};
Console Main_Console(console_pval, console_pmin, console_pmax, console_punit);
