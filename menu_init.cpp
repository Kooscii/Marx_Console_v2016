#include "menu.h"
#include "variables.h"


const char * const tag_onoff[] = {"关", "开"};

void Menu_Init() {
	/* Root */
	Item * pitem_root = new Item(ITEM_LABEL, "Marx.Beta");
	Main_Menu.setRoot(new Menu_Node(-1, pitem_root));
	
	/* Trigger Mode Setting */
	Item * pitem_ctrl = new Item(ITEM_FOLDER, "重频设置");
	Main_Menu.Menu_Insert(0, pitem_ctrl, -1);
		// submenu
		Main_Menu.Menu_Insert(1, new Item(ITEM_TIMES, Main_Console.pParameter[CP_TIMES], "次数%6s%s"), 0);
		Main_Menu.Menu_Insert(2, new Item(ITEM_PARAM16, Main_Console.pParameter[CP_FREQ], "频率%6.2fHz"), 0);
		Main_Menu.Menu_Insert(3, new Item(ITEM_TOGGLE, Voltage_Source.Entirety[MP_TRIGGER], "触发%8s", (char**)tag_onoff), 0);
	
	/* Voltage Source Setting */
	Item * pitem_voltsrc = new Item(ITEM_FOLDER, "电压源参数");
	Main_Menu.Menu_Insert(10, pitem_voltsrc, -1);
		// submenu
//		Main_Menu.Menu_Insert(11, new Item(ITEM_TOGGLE,  Voltage_Source.Entirety[MP_TRIGGER],      "触发%8s", (char**)tag_onoff), 10);
		Main_Menu.Menu_Insert(12, new Item(ITEM_PARAM16, Voltage_Source.Entirety[MP_PULSEVOLTAGE], "电压%6.1fkV"), 10);
		Main_Menu.Menu_Insert(13, new Item(ITEM_PARAM16, Voltage_Source.Entirety[MP_PULSEWIDTH],   "脉宽%6.1fkV"), 10);
		Main_Menu.Menu_Insert(14, new Item(ITEM_PARAM16, Voltage_Source.Entirety[MP_PULSEDELAY],   "延时%6.1fkV"), 10);
	
	/* Current Source Setting */
	Item * pitem_currsrc = new Item(ITEM_FOLDER, "电流源参数");
	Main_Menu.Menu_Insert(20, pitem_currsrc, -1);
		// submenu
//		Main_Menu.Menu_Insert(21, new Item(ITEM_TOGGLE,  Current_Source.Entirety[MP_TRIGGER],      "触发%8s", (char**)tag_onoff), 20);
		Main_Menu.Menu_Insert(22, new Item(ITEM_PARAM16, Current_Source.Entirety[MP_PULSEVOLTAGE], "电压%6.1fkV"), 20);
		Main_Menu.Menu_Insert(23, new Item(ITEM_PARAM16, Current_Source.Entirety[MP_PULSEWIDTH],   "脉宽%6.1fkV"), 20);
		Main_Menu.Menu_Insert(24, new Item(ITEM_PARAM16, Current_Source.Entirety[MP_PULSEDELAY],   "延时%6.1fkV"), 20);
	
	/* System Setting */
	Item * pitem_setting = new Item(ITEM_FOLDER, "系统设置");
	Main_Menu.Menu_Insert(1000, pitem_setting, -1);
	
	/* Enter Debug mode */
	Item * pitem_testing = new Item(ITEM_FOLDER, "测试模式");
	Main_Menu.Menu_Insert(1100, pitem_testing, -1);
	
	
	// initialize page
	Main_Menu.setPage(Main_Menu.getRoot());
	Main_Menu.isPageChanged = true;
	// initialize cursor
	Main_Menu.setCursor(Main_Menu.getPage()->Tchild);
	Main_Menu.isCursorChanged = true;
	Main_Menu.isCursorSelected = false;
	
	Main_Menu.isMenuUpdated = true;
	
	Main_Menu.resetSpec();
}

