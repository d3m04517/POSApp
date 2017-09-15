#ifndef POSAPP_H
#define POSAPP_H
#include "Item.h"
#include "NonPerishable.h"
#include "Perishable.h"
namespace ict {
	class PosApp {
		char m_filename[128];
		char m_billfname[128];
		Item* m_items[MAX_NO_ITEMS];
		int m_noOfItems;
		int menu();
		void deallocateItems();
		void loadRecs();
		void saveRecs();
		int searchItems(const char*) const;
		void updateQty();
		void addItem(bool);
		void listItems()const;
		void truncateBillFile();
		void showBill();
		void addToBill(Item&);
		void POS();
	public:
		PosApp(const char*, const char*);
		void run();
	};
}

#endif