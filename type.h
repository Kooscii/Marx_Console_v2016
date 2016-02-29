#ifndef TYPE_H_
#define TYPE_H_

#include "stm32f10x.h"
#include "cmsis_os.h"
#include "rt_heap.h"

#define HEAP_SIZE (2000)
extern uint32_t heap_zone[HEAP_SIZE];


class InitHeapZone {
public:
	InitHeapZone() {
		_init_alloc((uint32_t)heap_zone, (uint32_t)&heap_zone[HEAP_SIZE-1]);
	};
};


class Param8 {
	private:
		int8_t value;
		int8_t val_default;
		int8_t min;
		int8_t max;
		float unit;

	public:
		Param8(int8_t _val, int8_t _min, int8_t _max, float _unit): value(_val), val_default(_val), min(_min), max(_max), unit(_unit), isUpdated(true) {if (_val>max || _val<min) value = min;};
		~Param8() {};
			
		bool isUpdated;

		bool _reset() {
			value = val_default;
			isUpdated = true;
			return true;
		};
		/* Set Value */
		bool _set_max(int8_t _max) {
			max = _max;
			if (value>max) {
				value = max;
				isUpdated = true;
			}
			return true;
		};
		bool _set_min(int8_t _min) {
			min = _min;
			if (value<min) {
				value = min;
				isUpdated = true;
			}
			return true;
		};
		bool _set_unit(float _unit) {
			unit = _unit;
			return true;
		};
		bool _set_rel(int8_t _delta) {
			int16_t _tmp;
			if ((value>=max && _delta>=0) || (value<=min && _delta<=0))
				return false;
			else {
				_tmp = (int16_t)value + (int16_t)_delta; 
				if (_tmp>(int16_t)max) 
					value = max; 
				else if (_tmp<(int16_t)min) 
					value = min;
				else
					value = (int16_t)_tmp;
				return true;
			}
		};
		bool _set_abs(int8_t _val) {
			if (_val<=max && _val>=min) {
				value = _val;
				return true;
			}
			else 
				return false;
		};
		
		/* Get Value*/
		int8_t _get_max() {
			return max;
		};
		int8_t _get_min() {
			return min;
		};
		float _get_unit() {
			return unit;
		}
		int8_t _get_val() {
			return value;
		};
};


class Param16 {
	private:
		int16_t value;
		int16_t val_default;
		int16_t min;
		int16_t max;
		float unit;

	public:
		Param16(int16_t _val, int16_t _min, int16_t _max, float _unit): value(_val), val_default(_val), min(_min), max(_max), unit(_unit), isUpdated(true) {if (_val>max || _val<min) value = min;};
		~Param16() {};
			
		bool isUpdated;

		bool _reset() {
			value = val_default;
			isUpdated = true;
			return true;
		};
		/* Set Value */
		bool _set_max(int16_t _max) {
			max = _max;
			if (value>max) {
				value = max;
				isUpdated = true;
			}
			return true;
		};
		bool _set_min(int16_t _min) {
			min = _min;
			if (value<min) {
				value = min;
				isUpdated = true;
			}
			return true;
		};
		bool _set_unit(float _unit) {
			unit = _unit;
			return true;
		};
		bool _set_rel(int16_t _delta) {
			int32_t _tmp;
			if ((value>=max && _delta>=0) || (value<=min && _delta<=0))
				return false;
			else {
				_tmp = (int32_t)value + (int32_t)_delta; 
				if (_tmp>(int32_t)max) 
					value = max; 
				else if (_tmp<(int32_t)min) 
					value = min;
				else
					value = (int32_t)_tmp;
				return true;
			}
		};
		bool _set_abs(int16_t _val) {
			if (_val<=max && _val>=min) {
				value = _val;
				return true;
			}
			else 
				return false;
		};
		
		/* Get Value*/
		int16_t _get_max() {
			return max;
		};
		int16_t _get_min() {
			return min;
		};
		float _get_unit() {
			return unit;
		}
		int16_t _get_val() {
			return value;
		};
};


class Func {
	private:
		void (*fp)();
	
	public:
		Func(void (*_fp)()): fp(_fp) {};
		~Func() {};
		
		void _exec() {(*fp)();};
};


#endif /* TYPE_H_ */
