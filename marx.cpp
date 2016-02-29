#include "marx.h"


Marx::Marx(uint8_t _sourcetype, uint8_t _stage_qty, int16_t * _val, int16_t * _min, int16_t * _max, float * _unit) {
	uint8_t i,j;
	
	SourceType = _sourcetype;
	Stage_Qty = _stage_qty;
	
	/* Initialize Entirety */
	for (i=0; i<7; i++)
		Entirety[i] = new Param16(_val[i], _min[i], _max[i], _unit[i]);
	
	/* Initialize Stage */
	for (j=0; j<Stage_Qty; j++)
		for (i=0; i<7; i++)
			Stage[j][i] = new Param16(_val[i]/Stage_Qty, _min[i]/Stage_Qty, _max[i]/Stage_Qty, _unit[i]);
	
	isMarxUpdated = true;
}


void Marx::SyncE2S(uint8_t _mp_type) {
	switch (_mp_type) {
		case MP_PULSEVOLTAGE:
			for (uint8_t i=0; i<Stage_Qty; i++) {
				this->setValue(i, _mp_type, getValue(_mp_type)/Stage_Qty);
			}
			break;
		case MP_POLARITY:
		case MP_PULSEWIDTH:
		case MP_PULSEDELAY:
		case MP_PS24V:
		case MP_TRIGGER:
			for (uint8_t i=0; i<Stage_Qty; i++) {
				this->setValue(i, _mp_type, getValue(_mp_type));
			}
			break;
		default:
			break;
	}
}

void Marx::SyncS2E(uint8_t _mp_type) {
//	switch (_mp_type) {
//		// sum
//		case MP_PULSEVOLTAGE: 
//			for (uint8_t i=0; i<Stage_cnt; i++){
//				_sum += Stages[i].getValue(_param);
//			}
//			Entirety.syncValue(_param, _sum);
//			break;
//		// equal
//		case MP_POLARITY:
//			Entirety.syncValue(_param, ptr->getValue(_param));
//			for (uint8_t i=0; i<Stage_cnt; i++){
//				Stages[i].syncValue(MP_PS24V,Entirety.getValue(_param));
//			}
//			break;
//		// separate
//		case MP_PULSEWIDTH:
//		case MP_PULSEDELAY:
//			break;
//		// or
//		case MP_PS24V:
//		case MP_TRIGGER:
//			for (uint8_t i=0; i<Stage_cnt; i++){
//				_sum += Stages[i].getValue(_param);
//			}
//			Entirety.syncValue(_param, !!_sum);
//			break;
//			break;
//		default:
//			break;
//	}
}


Console::Console(int16_t * _val, int16_t * _min, int16_t * _max, float * _unit) {
	uint8_t i=0;
	
	for (i=0; i<3; i++)
		pParameter[i] = new Param16(_val[i], _min[i], _max[i], _unit[i]);
	
	isConsoleUpdated = true;
}

