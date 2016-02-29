#ifndef MENU_H_
#define MENU_H_

#include "type.h"
#include "configuration.h"

#define BTN_UP		0
#define BTN_DOWN	1
#define BTN_LEFT	2
#define BTN_RIGHT	3
#define BTN_CANCEL	4
#define BTN_OK		5

class Menu_Node;
class Menu;

#define ITEM_PARAM8		1
#define ITEM_PARAM16	2
#define ITEM_TOGGLE		3
#define ITEM_FUNC		4
#define ITEM_FOLDER		5
#define ITEM_LABEL		6
#define ITEM_TIMES		7	// special for parameter CP_TIMES

class Item {
	private:
		uint8_t item_type;
		Param8 * pm8;
		Param16 * pm16;
		Func * fp;
		
		const char * label;
		char ** tag;
	
	public:
		Item(uint8_t _type, Param8 * _pm8, const char * _lbl): 
			item_type(_type), pm8(_pm8), label(_lbl), isItemUpdated(true) 
				{pm16=NULL; fp=NULL; tag=NULL;};
		Item(uint8_t _type, Param8 * _pm8, const char * _lbl, char ** _tag): 
			item_type(_type), pm8(_pm8), label(_lbl), tag(_tag), isItemUpdated(true) 
				{pm16=NULL; fp=NULL;};
		Item(uint8_t _type, Param16 * _pm16, const char * _lbl): 
			item_type(_type), pm16(_pm16), label(_lbl), isItemUpdated(true) 
				{pm8=NULL; fp=NULL; tag=NULL;};
		Item(uint8_t _type, Param16 * _pm16, const char * _lbl, char ** _tag): 
			item_type(_type), pm16(_pm16), label(_lbl), tag(_tag), isItemUpdated(true) 
				{pm8=NULL; fp=NULL;};
		Item(uint8_t _type, Func * _op, const char * _lbl): 
			item_type(_type), fp(_op), label(_lbl), isItemUpdated(true) 
				{pm8=NULL; pm16=NULL; tag=NULL;};
		Item(uint8_t _type, const char * _lbl): 
			item_type(_type), label(_lbl), isItemUpdated(true) 
				{pm8=NULL; pm16=NULL; fp=NULL; tag=NULL;};
		~Item() {};
		
		bool isItemUpdated;
			
		/* Type: get */
		uint8_t getType() {return item_type;};
		/* Value: get & set */
		// get
		int16_t getValue() {
			if (pm8!=NULL) 
				return pm8->_get_val();
			else if (pm16!=NULL) 
				return pm16->_get_val();
			else
				return 0;
		};
		// set value absolutely (value)
		void setValue(int16_t _val) {
			if (pm8!=NULL) {
				pm8->_set_abs(_val);
				pm8->isUpdated = true;
			}
			else if (pm16!=NULL) {
				pm16->_set_abs(_val);
				pm16->isUpdated = true;
			}
		};
		// set value relatively (value, coefficient)
		void setValue(int16_t _val, int8_t _k) {
			if (pm8!=NULL) {
				pm8->_set_rel(_val*_k);
				pm8->isUpdated = true;
			}
			else if (pm16!=NULL) {
				pm16->_set_rel(_val*_k);
				pm16->isUpdated = true;
			}
		};
		/* Unit: get */
		float getUnit() {
			if (pm8!=NULL) {
				return pm8->_get_unit(); 
			}
			else if (pm16!=NULL) {
				return pm16->_get_unit(); 
			}
			return 0;
		}
		/* Label&Tag: get */
		const char * getLabel() {
			return label;
		};
		// only toggle has tags
		char * getTag() {
			if (item_type==ITEM_TOGGLE && tag!=NULL) {
				if (pm8!=NULL)
					return tag[pm8->_get_val()]; 
				else if (pm16!=NULL)
					return tag[pm16->_get_val()]; 
			} 
			return NULL;
		};
		/* action */
		void Exec() {
			if (item_type==ITEM_TOGGLE) {
				if (pm8!=NULL) {
					pm8->_set_abs((pm8->_get_val()+1)%2); 
					pm8->isUpdated = true;
				}
				else if (pm16!=NULL) {
					pm16->_set_abs((pm16->_get_val()+1)%2); 
					pm16->isUpdated = true;
				}
			}
			else if (item_type==ITEM_FUNC) 
				fp->_exec();
		};
		/* isUpdated: get & set */
		bool isUpdated() {
			if (pm8!=NULL) 
				return pm8->isUpdated;
			else if (pm16!=NULL) 
				return pm16->isUpdated;
			return false;
		};
		void setUpdated(bool state)  {
			if (pm8!=NULL) 
				pm8->isUpdated = state;
			else if (pm16!=NULL) 
				pm16->isUpdated = state;
		};
};


class Menu_Node {
	private:
		int16_t item_id;
		Menu_Node * cursor_history;
		
	public:
		Item * pItem;
		Menu_Node * Tp;			// node of parent-menu
		Menu_Node * Tchild;		// node of sub-menu
		Menu_Node * Tprev;		// node of previous menu item
		Menu_Node * Tnext;		// node of next menu item

		Menu_Node(int16_t _id, Item * _item=NULL, Menu_Node * parent_ptr=NULL, Menu_Node * sib_ptr=NULL):
			item_id(_id), cursor_history(NULL), pItem(_item), Tp(parent_ptr), Tchild(NULL), Tprev(sib_ptr), Tnext(NULL) {};
		~Menu_Node() {};
			
		/* Get */
		int16_t getID() {return item_id;};
//		Item * getItem() {return pItem;};
		Menu_Node * getCursorHistory() {return cursor_history? cursor_history : Tchild;};
		void setCursorHistory (Menu_Node * ch) {cursor_history = ch;};
		
		/* Methods for item context */
		bool isToggle() {return (pItem->getType()==ITEM_TOGGLE);};
		bool isFolder() {return (pItem->getType()==ITEM_FOLDER);};
		bool isParam() {return (pItem->getType()==ITEM_PARAM16 || pItem->getType()==ITEM_PARAM8 || pItem->getType()==ITEM_TIMES);};
		bool isFunc() {return (pItem->getType()==ITEM_FUNC);};
		bool isLabel() {return (pItem->getType()==ITEM_LABEL);};
};


class Menu {
    private:
		Menu_Node * root;
		Menu_Node * page;
		Menu_Node * cursor;
		bool spec;
    
    public:
		/* Constructor and Destructor */
		Menu() {};
		~Menu() {};
			
		bool isMenuUpdated;
		bool isPageChanged;
		bool isCursorChanged;
		bool isCursorSelected;
		
		Menu_Node * Menu_Search (Menu_Node * ptr, int16_t item_id);
		bool Menu_Insert (int16_t item_id, Item * _item, int16_t parent_id=-1);
		
		/* Root: get, set */
		Menu_Node * getRoot() {
			return root;
		};
		void setRoot(Menu_Node * _root) {
			root = _root;
		};
		
		/* Page: get, set, next, prev */
		Menu_Node * getPage() {
			return page;
		};
		void setPage(Menu_Node * ptr) {
			page = ptr;
			cursor = page->getCursorHistory(); 
			isPageChanged = true;
		};
		void nextPage() {
			if (page->Tnext) {
				page = page->Tnext; 
				cursor = page->getCursorHistory(); 
				isPageChanged = isCursorChanged = true;
			}
		};
		void prevPage() {
			if (page->Tprev) {
				page = page->Tprev;
				cursor = page->getCursorHistory();
				isPageChanged = isCursorChanged = true;
			}
		};
		
		/* Cursor: get, set, next, prev, p, child */
		Menu_Node * getCursor() {
			return cursor;
		};
		void setCursor(Menu_Node * ptr) {
			cursor = ptr;
			cursor->Tp->setCursorHistory(cursor);
			isCursorChanged = true;
		};
		void nextCursor() {
			if (cursor->Tnext) {
				cursor = cursor->Tnext;
				cursor->Tp->setCursorHistory(cursor);
				isCursorChanged = true;
			}
		};
		void prevCursor() {
			if (cursor->Tprev) {
				cursor = cursor->Tprev; 
				cursor->Tp->setCursorHistory(cursor); 
				isCursorChanged = true;
			}
		};
//		void pCursor() {
//			if (page!=root) {
//				
//				
//				
//				cursor = cursor->Tprev; 
//				page->setCursorHistory(cursor); 
//				isCursorChanged = true;
//			}
//		};
//		void childCursor() {
//			if (cursor->Tprev)
//				cursor = cursor->Tprev; 
//				page->setCursorHistory(cursor); 
//				isCursorChanged = true;
//		};
		
		/* Input Behavior */
		bool onButton(uint8_t);
		bool onEncoder(int16_t);
		bool setSpec() {
			if (spec)
				return false;
			else {
				spec=true; 
				return true;
			}
		};
		bool resetSpec() {
			if (!spec)
				return false;
			else {
				spec=false; 
				return true;
			}
		};
};

void Menu_Init();

#endif /* MENU_H_ */
