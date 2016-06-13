#ifndef MARX_H_
#define MARX_H_

#include "type.h"
#include "stm32f10x.h"

/* Marx Parameter Type */
#define MP_PULSEVOLTAGE		0	// Each stage's pulse voltage. Unit: 0.1 kv
#define MP_PULSEWIDTH		1	// Each stage's pulse width. Unit: 0.1 us
#define MP_PULSEDELAY		2	// Each stage's pulse rising edge delay. Unit 0.1 us
#define MP_TRIGGER			3	// Each stage's enable state of executing discharging while receiving trigger signal. 1 for enabled, 0 for disabled
#define MP_PS24V			4	// Each stage's on-off state of 24V power supply. 1 for turning on, 0 for turning off
#define MP_CAPCHARGING		5	// Each stage's enable state of charging capacitance. 1 for enabled, 0 for disabled
#define MP_POLARITY			6	// Each stage's polaroty. 1 for positive polarity, -1 for negative polarity, 0 for none

/* Console Parameter Type */
#define CP_TIMES			0	// Each stage's repetition setting, total times of triggers. Unit 1 times
#define CP_FREQ				1	// Each stage's repetition setting, trigger frequency. Unit 1
#define CP_FREQ_UNIT		2	// Each stage's repetition setting, trigger frequency. Unit Hz/kHz
#define CP_INTERVAL			3	// Each stage's repetition setting, the interval between two triggers. Unit: 1 s
 
/* Unit */
#define VOLTAGE_UNIT    (0.1)
#define CURRENT_UNIT    (0.1)
#define WIDTH_UNIT      (0.1)
#define DELAY_UNIT      (0.1)

#define TIMES_UNIT      (1)
#define INTERVAL_UNIT   (1)
#define FREQ_UNIT		(0.1)

/* Source Type */
#define VOLTAGE_SOURCE      0
#define CURRENT_SOURCE      1

class Marx;
class Marx_Param;


class Marx {
    private:
        uint8_t SourceType;                 // Voltage Source or Current Source
        uint8_t Stage_Qty;                  // Number of stages
     
    public:
        Marx(uint8_t _sourcetype, uint8_t _stage_qty, int16_t * _val, int16_t * _min, int16_t * _max, float * _unit);
        ~Marx() {};
		
		Param16 * Entirety[7];
		Param16 * Stage[16][7];
		bool isMarxUpdated;
			
		void SyncS2E(uint8_t _mp_type);
		void SyncE2S(uint8_t _mp_type);
		
		uint8_t getQty() {return Stage_Qty;};
		
		/* Value (Entirety): get, set */
		// get
		int16_t getValue(uint8_t _mp_type) {
			return Entirety[_mp_type]->_get_val();
		};
		// set abs
		void setValue(uint8_t _mp_type, int16_t _val) {
			Entirety[_mp_type]->_set_abs(_val);
			Entirety[_mp_type]->isUpdated = true;
			isMarxUpdated = true;
		};
		// set rel
		void setValue(uint8_t _mp_type, int16_t _val, int8_t _k) {
			Entirety[_mp_type]->_set_rel(_val*_k);
			Entirety[_mp_type]->isUpdated = true;
			isMarxUpdated = true;
		};	
		/* Value (Stage): get, set */
		// get
		int16_t getValue(uint8_t _idx, uint8_t _mp_type) {
			return Stage[_idx][_mp_type]->_get_val();
		};
		// set abs
		void setValue(uint8_t _idx, uint8_t _mp_type, int16_t _val) {
			Stage[_idx][_mp_type]->_set_abs(_val);
			Stage[_idx][_mp_type]->isUpdated = true;
			isMarxUpdated = true;
		};
		// set rel
		void setValue(uint8_t _idx, uint8_t _mp_type, int16_t _val, int8_t _k) {
			Stage[_idx][_mp_type]->_set_rel(_val*_k);
			Stage[_idx][_mp_type]->isUpdated = true;
			isMarxUpdated = true;
		};	
		
		/* isUpdated: get & set */
		// entirety
		bool isUpdated(uint8_t _mp_type) {
			return Entirety[_mp_type]->isUpdated;
		};
		void setUpdated(uint8_t _mp_type, bool state)  {
			Entirety[_mp_type]->isUpdated = state;
		};
		// stage
		bool isUpdated(uint8_t _idx, uint8_t _mp_type) {
			return Stage[_idx][_mp_type]->isUpdated;
		};
		void setUpdated(uint8_t _idx, uint8_t _mp_type, bool state)  {
			Stage[_idx][_mp_type]->isUpdated = state;
		};
};


class Console {
	private:
		bool trigger_status;
		bool repetition;
	
	public:
		Console(int16_t * _val, int16_t * _min, int16_t * _max, float * _unit);
		~Console() {};
			
		Param16 * pParameter[5];
		int16_t Times_Cnt;
		bool isConsoleUpdated;
			
		/* Value: get, set */
		// get 
		int16_t getValue(uint8_t _mp_type) {
			return pParameter[_mp_type]->_get_val();
		};
		// set abs
		void setValue(uint8_t _mp_type, int16_t _val) {
			pParameter[_mp_type]->_set_abs(_val);
			pParameter[_mp_type]->isUpdated = true;
			isConsoleUpdated = true;
		};
		// set rel
		void setValue(uint8_t _mp_type, int16_t _val, int8_t _k) {
			pParameter[_mp_type]->_set_rel(_val*_k);
			pParameter[_mp_type]->isUpdated = true;
			isConsoleUpdated = true;
		};	
		/* isUpdated: get & set */
		bool isUpdated(uint8_t _mp_type) {
			return pParameter[_mp_type]->isUpdated;
		};
		void setUpdated(uint8_t _mp_type, bool state)  {
			pParameter[_mp_type]->isUpdated = state;
		};
		
		bool inTrigger() {
			return trigger_status;
		};
		void setTrigger(bool state) {
			trigger_status = state;
		};
		bool Repetition() {
			return repetition; 
		};
		void setRepetition(bool state) {
			repetition = state;
		};
};

#endif /* MARX_H_ */
