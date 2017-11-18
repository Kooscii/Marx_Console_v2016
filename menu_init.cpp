#include "menu.h"
#include "variables.h"


const char * const tag_freq_unit[] = {"Hz", "kHz"};
const char * const tag_times[] = {"��Ƶ", "����"};

void Menu_Init() {
	/* Root */
	Item * pitem_root = new Item(ITEM_LABEL, "Marx.Beta");
	Main_Menu.setRoot(new Menu_Node(-1, pitem_root));
	
	/* Trigger Mode Setting */
//	Item * pitem_ctrl = new Item(ITEM_FOLDER, "��Ƶ����");
//	Main_Menu.Menu_Insert(0, pitem_ctrl, -1);
//		// submenu
////		Main_Menu.Menu_Insert(1, new Item(ITEM_TOGGLE, Voltage_Source.Entirety[MP_TRIGGER], "����%8s", (char**)tag_onoff), 0);
//		Main_Menu.Menu_Insert(2, new Item(ITEM_TIMES, Main_Console.pParameter[CP_TIMES], "����%6s%s"), 0);
//		Main_Menu.Menu_Insert(3, new Item(ITEM_PARAM16, Main_Console.pParameter[CP_FREQ], "Ƶ��%8.1f"), 0);
//		Main_Menu.Menu_Insert(4, new Item(ITEM_TOGGLE, Main_Console.pParameter[CP_FREQ_UNIT], "��λ%8s", (char**)tag_freq_unit), 0);
	
	/* Voltage Source Setting */
	Item * pitem_voltsrc = new Item(ITEM_FOLDER, "��ѹԴ����");
	Main_Menu.Menu_Insert(10, pitem_voltsrc, -1);
		// submenu
		Main_Menu.Menu_Insert(11, new Item(ITEM_TOGGLE, Main_Console.pParameter[CP_TIMES], "����%8s", (char**)tag_times), 10);
		Main_Menu.Menu_Insert(12, new Item(ITEM_PARAM16, Main_Console.pParameter[CP_FREQ], "Ƶ��%8.2f"), 10);
		Main_Menu.Menu_Insert(13, new Item(ITEM_TOGGLE, Main_Console.pParameter[CP_FREQ_UNIT], "��λ%8s", (char**)tag_freq_unit), 10);
		Main_Menu.Menu_Insert(14, new Item(ITEM_PARAM16, Voltage_Source.Entirety[MP_PULSEWIDTH],   "����%6.1fus"), 10);
		Main_Menu.Menu_Insert(15, new Item(ITEM_PARAM16, Voltage_Source.Entirety[MP_PULSEVOLTAGE], "��ѹ%6.1fkV"), 10);
		Main_Menu.Menu_Insert(16, new Item(ITEM_PARAM16, Voltage_Source.Entirety[MP_PULSEDELAY],   "��ʱ%6.1fus"), 10);
	
	
	Item * pitem_currsrc = new Item(ITEM_FOLDER, "���Բ���");
	Main_Menu.Menu_Insert(1000, pitem_currsrc, -1);
//		// submenu
		Main_Menu.Menu_Insert(1001, new Item(ITEM_PARAM16, &dead_zone1, "����1%5.0fns"), 1000);
		Main_Menu.Menu_Insert(1002, new Item(ITEM_PARAM16, &dead_zone2, "����2%5.0fns"), 1000);


//	/* System Setting */
//	Item * pitem_setting = new Item(ITEM_FOLDER, "ϵͳ����");
//	Main_Menu.Menu_Insert(1000, pitem_setting, -1);
//	
//	/* Enter Debug mode */
//	Item * pitem_testing = new Item(ITEM_FOLDER, "����ģʽ");
//	Main_Menu.Menu_Insert(1100, pitem_testing, -1);
	
	
	// initialize page
	Main_Menu.setPage(Main_Menu.getRoot()->Tchild);
	Main_Menu.isPageChanged = true;
	// initialize cursor
	Main_Menu.setCursor(Main_Menu.getPage()->Tchild);
	Main_Menu.isCursorChanged = true;
	Main_Menu.isCursorSelected = false;
	
	Main_Menu.isMenuUpdated = true;
	
	Main_Menu.resetSpec();
}

