#include "task.h"
extern "C" {
#include "u8g_arm.h"
}
#define LIST_X_OFFSET 12	// the x offset of item list displayed on the screen
#define STATUS_X_OFFSET 0	// the x offset of item list displayed on the screen


void item_print (u8g_t * lcd, uint8_t line, uint8_t pos, Item * _item) {	
	// as each item has its own type, this function is used to set different ways for print out every type
	switch (_item->getType()) {
		case ITEM_FOLDER:
		case ITEM_LABEL: {
			u8g_BufferStr(lcd, line, pos, _item->getLabel());
			break;
		}
		case ITEM_PARAM16: {
			u8g_BufferStr(lcd, line, pos, _item->getLabel(), _item->getValue()*_item->getUnit());
			break;
		}
		case ITEM_TOGGLE: {
			u8g_BufferStr(lcd, line, pos, _item->getLabel(), _item->getTag());
			break;
		}
		case ITEM_TIMES: {
			int16_t value=_item->getValue();
			switch (value) {
				case 0:	// if value == 0, which means repetition, display as string
					u8g_BufferStr(lcd, line, pos, _item->getLabel(), "重","频");
					break;
				case 1:	// now that 'if' statement is used here, why not display single pulse as string too
					u8g_BufferStr(lcd, line, pos, _item->getLabel(), "单","次");
					break;
				default:	// if value != 0, display as digital
					char str[7];
					uint8_t i=6;
					// int2str
					// start from the unit digit, put single digit into the end of str[]
					do {
						str[i--] = value%10 + 48;
						value = (int)(value/10);
					// until digit is 0
					} while (value != 0);
					value = i;
					// move the str from the end to the start
					for (i=0; i<6-value; i++)
						str[i] = str[value+i+1];
					// set the tail of the str to 0x00
					str[i] = 0x00;
					u8g_BufferStr(lcd, line, pos, _item->getLabel(), str, "次");
					break;
			}
			break;
		}
//		case PARAM_PULSEDELAY:
//			lcd->Print ( line, pos, lable, value/INV_DELAY_UNIT);
//			break;
//		case PARAM_REPEATINTERVAL:
//			lcd->Print ( line, pos, lable, value/INV_INTERVAL_UNIT);
//			break;
//		case PARAM_REPEATFREQ:
//			lcd->Print ( line, pos, lable, value/INV_FREQ_UNIT);
//			break;
//		case PARAM_REPEATTIMES:
//			// int2str
//			if (value == 1)
//				// now that 'if' statement is used here, why not display single pulse as string too
//				lcd->Print ( line, pos, lable, "单", "次");
//			else if (value) {
//				// if value != 0, display as digital
//				// start from the unit digit, put single digit into the end of str[]
//				do {
//					str[i--] = value%10 + 48;
//					value = (int)(value/10);
//				// until digit is 0
//				} while (value != 0);
//				value = i;
//				// move the str from the end to the start
//				for (i=0; i<11-value; i++)
//					str[i] = str[value+i+1];
//				// set the tail of the str to 0x00
//				str[i] = 0x00;
//				lcd->Print ( line, pos, lable, str, "次");
//			}
//			else
//				// if value == 0, which means repetition, display as string
//				lcd->Print ( line, pos, lable, "重", "频");
//			break;
//		case PARAM_PS24V:
//			lcd->Print ( line, pos, lable, value ? "24V开" : "24V关");
//			break;
//		case PARAM_TRIGGER:
//			lcd->Print ( line, pos, lable, value ? "允许" : "禁止");
//			break;
		default:
			break;
	}
}


void task_LCD (void const * arg)
{		
	static u8g_t LCD19264;
	static Menu_Node * ptr_page;		// temp pointer
	static Menu_Node * ptr_cursor;		// temp pointer
	static Menu_Node * item_list[] = {NULL, NULL, NULL, NULL, NULL};	// The 4 pointers pointing to the 4 items which should be displayed
	static bool isListUpdated = false, Status0=true, Status1=true;
	
	uint8_t i;
	uint8_t item_cnt;
	uint8_t blink=0;
	
	ptr_SPI1_Buffer_Tx = SPI1_Buffer_Tx;
	u8g_InitComFn(&LCD19264, &u8g_dev_st7920_192x32_hw_spi, u8g_com_hw_spi_fn);
	
	for(;;)
	{	
		LED_Set(GPIO_LED_GR, LED_ON);
		u8g_BufferStr(&LCD19264, 0x0f, 0x0f, NULL);
		/* ***************************************************************
		 * 						Parameter List
		 * ***************************************************************
					--------------------------
					|         -|- xxxx  00000|
					|           > xxxx  00000|
					|          |- xxxx  00000|
					|          |- xxxx  00000|
					--------------------------
		 * ***************************************************************/
		ptr_page = Main_Menu.getPage();
		ptr_cursor = Main_Menu.getCursor();
		
		/* let the list be in current page */
		if (item_list[0]->Tp != ptr_page) {	// list->parent shoud be ptr_page
			// it means the current page has changed, update the list
			// the first item doesn't need any condition because a page will always get a child
			item_list[i=0] = ptr_page->Tchild;
			// let other three pointer point to the next sibling
			while (item_list[i++]->Tnext!=NULL && i<4)
				item_list[i] = item_list[i-1]->Tnext;
			// if items are less than 4, let the rest pointer point to NULL
			for (; i<4; i++)
				item_list[i] = NULL;
		}
			
		/* find the current cursor first, set item_list properly */
		do {
			/* search inside list */
			i=0;
			// when list[i] = NULL the first condition is false, while should break without executing i++
			// so if the searching end up with reaching the tail of the list (not found), list[i-1] != NULL and list[i-1] != ptr_cursor
			// if the searching end up with locationg the ptr_cursor (found), i++ will be executed and list[i-1] == ptr_cursor
			while (item_list[i] != NULL && item_list[i++] != ptr_cursor);
			if (item_list[i-1] == ptr_cursor) break;	// if current cursor is found inside list, list has no need updating anymore
			
			/* if the cursor isn't in the list, we can come into conclusion that:
				1. the number of items in this page is more than 4
				2. list[0~3] != NULL
				3. the cursor should either be previous or next
				4. list will be updated
			   so we can move the entire list forwards and backwards until it covers the cursor */
			
			isListUpdated = true;
			/* search forwards (next) */
			// move entire list to the next item
			// if not found, list[3] should be NULL; otherwise list[3] = ptr_cursor
			while (item_list[3]->Tnext != NULL) { 
				for (i=0; i<4; i++)
					item_list[i] = item_list[i]->Tnext; 
				if (item_list[3] == ptr_cursor) break;
			} 
			if (item_list[3] == ptr_cursor) break;
			
			/* search backwards (prev) */
			// move entire list to the previous item
			// if not found, list[0] should be NULL; otherwise list[0] = ptr_cursor
			while (item_list[0]->Tprev != NULL) { 
				for (i=0; i<4; i++)
					item_list[i] = item_list[i]->Tprev; 
				if (item_list[0] == ptr_cursor) break;
			}
			if (item_list[0] == ptr_cursor) break;	// list[0] != NULL means cursor found and list[0] = ptr_cursor, break and update the rest of the list
			
			/* no result */	//theoretically it won't happen, because the cursor will always in a page's child list. If it happens, that means something wrong in menu.cpp
			// display "Error" and request reboot
		} while(0); /* cursor found, it should be in the list now */
		
		/* there are several flags determining whether each element on the screen should be refresh or not 
			1. Menu.isCursorChanged: curosr icon need refreshing, for item with cursor display " >"; for item without cursor display "  " to clear
			2. Item.isUpdated: this specific item need refreshing
			3. isListUpdated: all items in the list need refreshing, regardless of the former two conditions
			4. isMenuUpdated: force refreshing all
		 */
		
		/* display the item list on the screen */
		// chech all flags line by line (4 lines), to see if any elements need refreshing
		item_cnt=0;
		for (i=0; i<4; i++) {
			// if this item isn't NULL, otherwise print a blank line
			if (item_list[i] != NULL) {
				item_cnt++;
				// checking if this item need refreshing or not
				item_print (&LCD19264, i, LIST_X_OFFSET, item_list[i]->pItem);
				// checking if the cursor need refreshing
				if (item_list[i] == ptr_cursor) {
					if (i==0) u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, Main_Menu.isCursorSelected? "-*" : "->");
					else u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, Main_Menu.isCursorSelected? " *" : " >");
				}
				else {
					if (i==0) {
						if (item_list[i]->Tprev == NULL)
							u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, "%c%c", 0xA9, 0xD0);
						else
							u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, "%c%c", 0xA9, 0xE0);
					}
					else if (i==1 || i==2) {
						if (item_list[i+1]==NULL) u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, "%c%c", 0xA9, 0xB8);
						else u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, "%c%c", 0xA9, 0xC0);
					}
					else if (i==3 && item_list[i]->Tnext == NULL) 
						u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, "%c%c", 0xA9, 0xB8);
					else 
						u8g_BufferStr(&LCD19264, i, LIST_X_OFFSET-2, "%c%c", 0xA9, 0xC0);
				}
			}
		}
		
		/* ***************************************************************
		
		 * 						Status Monitor
		 * ***************************************************************
					--------------------------
					| PageLabel -|-          |
					|            |-          |
					|  Status0   |-          |
					|  Status1   |-          |
					--------------------------
		 * ***************************************************************/
		/* parent menu's labe */
			// Print Page Label
		u8g_BufferStr(&LCD19264, 0, STATUS_X_OFFSET, ptr_cursor->Tp->pItem->getLabel());
				
		/* Trigger Status & Blink Effect*/
		if (Main_Console.inTrigger()) {
			if (blink==0) {
				u8g_BufferStr(&LCD19264, 2, 2, "%s", "触发中");
			}
			if (Main_Console.Repetition() )
					u8g_BufferStr(&LCD19264, 3, 2, "%s", "重频");
				else
					u8g_BufferStr(&LCD19264, 3, 2, "%s%2d%s", "剩余", Main_Console.Times_Cnt, "次");
		}
		else {
			if (Status0 != Voltage_Source.getValue(MP_TRIGGER) || blink<=8) {
				Status0 = Voltage_Source.getValue(MP_TRIGGER);
				u8g_BufferStr(&LCD19264, 2, STATUS_X_OFFSET, "%s", Status0? "  注意    " : "          ");
			}
			if (Status1 != Voltage_Source.getValue(MP_TRIGGER) || blink<=8) {
				Status1 = Voltage_Source.getValue(MP_TRIGGER);
				u8g_BufferStr(&LCD19264, 3, STATUS_X_OFFSET, "%s", Status1? "已允许触发" : "          ");
			}
		}
		// blink effect
		if (blink > 8 && ((Status0 || Status1) || Main_Console.inTrigger())) {
			u8g_BufferStr(&LCD19264, 2, STATUS_X_OFFSET, "%s", "          ");
			u8g_BufferStr(&LCD19264, 3, STATUS_X_OFFSET, "%s", "          ");
		}
		blink=(blink+1)%16;
		
		
		/* Display */
		u8g_PrintStr(&LCD19264);
		
		/* LED Display */
		if (Voltage_Source.getValue(MP_TRIGGER))
			LED_Set(GPIO_LED_YL, LED_ON);
		else
			LED_Set(GPIO_LED_YL, LED_OFF);
		
		if (GPIO_ReadInputDataBit(GPIO_TFB))
			LED_Set(GPIO_LED_RD, LED_ON);
		else
			LED_Set(GPIO_LED_RD, LED_OFF);
		
		LED_Set(GPIO_LED_GR, LED_OFF);
		osDelay(100);
	}
}
