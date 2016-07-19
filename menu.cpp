#include "menu.h"
#include "variables.h"

bool Menu::onButton(uint8_t button) {
	switch (button) {
		case BTN_UP:
			prevCursor();
			break;
		case BTN_DOWN:
			nextCursor();
			break;
		case BTN_LEFT:
			if (isCursorSelected) {
				cursor->pItem->setValue(-100, 1);
			}
			else {
				if (page->Tp->getID()==-1 && page->Tprev && page->Tprev->getID()<1000) {
					prevPage();
					if (cursor->isToggle() || true) {
						isCursorSelected = false;
						isCursorChanged = true;
					}
				}
			}
			break;
		case BTN_LEFT_DEBUG:
			if (page->Tp->getID()==-1 && page->Tprev) {
				prevPage();
				if (cursor->isToggle() || true) {
					isCursorSelected = false;
					isCursorChanged = true;
				}
			}
			break;
		case BTN_RIGHT:
			if (isCursorSelected) {
				cursor->pItem->setValue(100, 1);
			}
			else {
				if (page->Tp->getID()==-1 && page->Tnext && page->Tnext->getID()<1000) {
					nextPage();
					if (cursor->isToggle() || true) {
						isCursorSelected = false;
						isCursorChanged = true;
					}
				}
			}
			break;
		case BTN_RIGHT_DEBUG:
			if (page->Tp->getID()==-1 && page->Tnext) {
				nextPage();
				if (cursor->isToggle() || true) {
					isCursorSelected = false;
					isCursorChanged = true;
				}
			}
			break;
		case BTN_CANCEL:
			// if cursor is selected, unselect it
			if (isCursorSelected) {
				isCursorSelected = false;
				isCursorChanged = true;
			}
			// if cursor isn't selected, back to parent menu. if parent is root, do nothing
			else if (cursor->Tp->Tp!=root){
				setCursor(cursor->Tp);
				setPage(page->Tp);
			}
			break;
		case BTN_OK:
			if (isCursorSelected) {
				isCursorSelected = false;
				isCursorChanged = true;
			}
			// if it's a folder and it has a child, enter submenu (child)
			else if (cursor->isFolder() && cursor->Tchild){
				setCursor(cursor->Tchild);
				setPage(cursor->Tp);
			}
			// if this parameter is a toggle, than just flip it
			else if (cursor->isToggle()) {
				cursor->pItem->Exec();
				cursor->pItem->isItemUpdated = true;
				/* special for freq max value */
				if (Main_Console.getValue(CP_FREQ_UNIT)) {	// if unit is khz, maxium freq is 2.00 khz
					Main_Console.pParameter[CP_FREQ]->_set_max(500);
				}
				else {	// if unit is hz, maxium freq is 99.99 hz
					Main_Console.pParameter[CP_FREQ]->_set_max(9999);
				}
			}
			// otherwise, select it
			else if (cursor->isParam()){
				isCursorSelected = true;
				isCursorChanged = true;
			}
		default:
			return false;
	}
	return true;
}


bool Menu::onEncoder(int16_t delta){
	if (isCursorSelected) {
		if (spec) 
			cursor->pItem->setValue(delta, 10);
		else 
			cursor->pItem->setValue(delta, 1);
		cursor->pItem->isItemUpdated = true;
	}
	else {
		if (delta>0)
//			while (delta--) Cursor_next();
			nextCursor();
		else 
//			while (delta++) Cursor_prev();
			prevCursor();
	}
	return true;
}


Menu_Node * Menu::Menu_Search(Menu_Node * ptr, int16_t item_id) {
    Menu_Node * item_ptr;
    
    if (ptr != NULL) {
        // if the node is not empty, then search it
        if (item_id != ptr->getID()) {
            // if this node's ID isn't the target, search it's sibling tree
            item_ptr = Menu_Search (ptr->Tnext, item_id);
            if (item_ptr != NULL)
                // if the sibling tree return a target ptr, return the ptr
                return item_ptr;
            else {
                // if the sibling tree return null, search it's child tree
                return Menu_Search (ptr->Tchild, item_id);
            }
        }
        else
            // if this node's ID is the target, return the ptr
            return ptr;
        }
    else
        // if the node is empty, return null
        return NULL;
}


bool Menu::Menu_Insert (int16_t item_id, Item * _item, int16_t parent_id ) {
    Menu_Node * parent_ptr;
    Menu_Node * sib_ptr;
    
    // start searching from root
    parent_ptr = Menu_Search(root, parent_id);
    if (parent_ptr != NULL) {
        // if the parent exist
        sib_ptr = parent_ptr->Tchild;
        // I do not use sentinel here, so the boundary condition is more complicated
        if (sib_ptr == NULL) {
            // if the parent has no child, insert item
            parent_ptr->Tchild = new Menu_Node(item_id, _item, parent_ptr);
        }
        else {
            // if the parent has children, find the deepest child
            while (sib_ptr->Tnext != NULL) {
                sib_ptr = sib_ptr->Tnext;
            }   //untill no sibling
			sib_ptr->Tnext = new Menu_Node(item_id, _item, parent_ptr, sib_ptr);
        }
        return true;
    }
    else
        return false;
}

