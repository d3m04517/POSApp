#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <iomanip>
#include "PosApp.h"
using namespace std;

namespace ict {
	PosApp::PosApp(const char* filename, const char* billfname) {
		strcpy(m_filename, filename);
		strcpy(m_billfname, billfname);
		loadRecs();
	}

	int PosApp::menu() {
		int choice;
		cout << "The Sene-Store" << endl;
		cout << "1- List items" << endl;
		cout << "2- Add Perishable item" << endl;
		cout << "3- Add Non-Perishable item" << endl;
		cout << "4- Update item quantity" << endl;
		cout << "5- Show Item" << endl;
		cout << "6- POS" << endl;
		cout << "0- exit program" << endl;
		cout << "> ";
		cin >> choice;
		cout << endl;
		if (cin.fail()) {
			choice = 10;
		}
		cin.clear();
		cin.ignore(2000, '\n');
		return choice;
	}

	void PosApp::deallocateItems() {
		for (int i = 0; i < m_noOfItems; i++) {
			delete m_items[i];
		}
		m_noOfItems = 0;
	}

	void PosApp::loadRecs() {
		Item* p;
		char comma;
		char type;
		deallocateItems();
		std::fstream f;
		f.open(m_filename, ios::in);
		if (f.is_open()) {
			int i = 0;
			while (true) { 
				f >> type >> comma;
				if (f.eof()) {
					break;
				}
				if (type == 'P') {
					p = new Perishable;
					p->load(f);
				}
				else if (type == 'N') {
					p = new NonPerishable;
					p->load(f);
				}
				if (type == 'P' || type == 'N') {
					m_items[i] = p;
					m_noOfItems++;
				}
				i++;
	
			}
			f.close();
		}
		else {
			f.clear();
			f.close();
			std::ofstream o(m_filename);
			o.close();
		}
	}

	void PosApp::saveRecs() {
		fstream f(m_filename, std::ios::out | std::ios::trunc);
		f.close();
		std::fstream o;
		o.open(m_filename);
		if (!o.fail()) {
			for (int i = 0; i < m_noOfItems; i++) {
				if (m_items[i]->quantity() > 0) {
					m_items[i]->save(o);
					o << endl;
				}
			}
		}
		o.close();
		loadRecs();
	}

	int PosApp::searchItems(const char* sku) const {
		int index = -1;
		for (int i = 0; i < m_noOfItems; i++) {

			if (strcmp(m_items[i]->sku(), sku) == 0) {
				index = i;
				break;
			}
		}
		return index;
	}

	void PosApp::updateQty() {
		char* sku = new char[50];
		int qty;
		cout << "Please enter the SKU: ";
		cin >> sku;
		int i = searchItems(sku);
		if (i != -1) {
			m_items[i]->write(cout, false);
			cout << endl << endl;
			cout << "Please enter the number of purchased items: ";
			cin >> qty;
			m_items[i]->quantity(qty + m_items[i]->quantity());
			cout << "Updated!" << endl << endl;
		}
		else {
			cout << "Not found!" << endl;
		}
		delete[] sku;
	}

	void PosApp::addItem(bool isPerishable) {
		if (isPerishable) {
			Perishable *p = new Perishable;
			p->read(cin);
			if (cin.fail()) {
				cin.clear();
				cin.ignore(2000, '\n');
				p->write(cout, 0);
				cout << endl;
			}
			else {
				m_items[m_noOfItems] = p;
				m_noOfItems += 1;
				cout << "Item added." << endl << endl;
			}
		}
		else {
			NonPerishable *np = new NonPerishable;
			np->read(cin);
			if (cin.fail()) {
				cin.clear();
				cin.ignore(2000, '\n');
				np->write(cout, 0);
				cout << endl;
			}
			else {
				m_items[m_noOfItems] = np;
				m_noOfItems += 1;
				cout << "Item added." << endl << endl;
			}
		}
	}

	void PosApp::listItems() const {
		double total = 0;
		cout << " Row | SKU    | Item Name          | Price |TX |Qty |   Total |" << endl;
		cout << "-----|--------|--------------------|-------|---|----|---------|" << endl;
		for (int i = 0; i < m_noOfItems; i++) {
			int num = i + 1;
			cout.setf(ios::fixed);
			cout.width(4);
			cout << num << " |";
			cout.unsetf(ios::fixed);
			m_items[i]->write(cout, true);
			cout << endl;
			total += m_items[i]->cost() * m_items[i]->quantity();

		}
		cout << "-----^--------^--------------------^-------^---^----^---------^" << endl;
		cout << "                               Total Asset: $  |";
		cout.unsetf(ios::right);
		cout.setf(ios::fixed);
		cout.precision(2);
		cout.width(14);
		cout << total;
		cout << "|" << endl;
		cout << "-----------------------------------------------^--------------^" << endl;
		cout << endl;
	}

	void PosApp::truncateBillFile() {
		fstream f(m_billfname, std::ios::out | std::ios::trunc);
		f.close();
	}

	void PosApp::showBill() {
		double total = 0;
		char type;
		char comma;
		Date date;
		fstream f(m_billfname, ios::in);
		cout << setfill(' ');
		cout << "v--------------------------------------------------------v" << endl;
		cout << "| ";
		cout << (Date&)date;
		cout << "                                      |" << endl;
		cout << "| SKU    | Item Name          | Price |TX |Qty |   Total |" << endl;
		cout << "|--------|--------------------|-------|---|----|---------|" << endl;
		if (f.is_open()) {
			while (true) {
				cout << setfill(' ');
				f >> type >> comma;
				if (f.eof()) {
					break;
				}
				if (type == 'P') {
					Perishable *p = new Perishable;
					p->load(f);
					cout << "|";
					p->write(cout, true);
					cout << endl;
					total += p->cost() * p->quantity();
					delete p;
				}
				else {
					NonPerishable *np = new NonPerishable;
					np->load(f);
					cout << "|";
					np->write(cout, true);
					cout << endl;
					total += np->cost() * np->quantity();
					delete np;
				}
			}
			cout.unsetf(ios::right);
			cout << "^--------^--------------------^-------^---^----^---------^" << endl;
			cout << "|                               Total: $  |";
			cout.setf(ios::fixed);
			cout.precision(2);
			cout.width(14);
			cout << total << "|" << endl;
			cout.unsetf(ios::right);
			cout << "^-----------------------------------------^--------------^" << endl;
		}
		else {
			f.clear();
			f.close();
			truncateBillFile();
		}
	}

	void PosApp::addToBill(Item& I) {
		fstream f(m_billfname, ios::out|ios::app);
		int quantity = I.quantity();
		I.quantity(1);
		f << endl;
		I.save(f);
		f << endl;
		I.quantity(quantity - 1);
		f.close();
		saveRecs();
	}

	void PosApp::POS() {
		truncateBillFile();
		char* sku = new char[50];
		int index;
		while (true) {
			cout << "Sku: ";
			cin.getline(sku, MAX_SKU_LEN, '\n');
			index = searchItems(sku);
			if (sku[0] == '\0') {
				showBill();
				break;
			}
			else if (index != -1) {
				cout << "v------------------->" << endl;
				cout << "| " << m_items[index]->name() << endl;
				cout << "^------------------->" << endl;
				addToBill(*(m_items[index]));
			}
			else if (index == -1) {
				cout << "Not found!" << endl;
			}
		}
		delete[] sku;
	}

	void PosApp::run() {
		int choice;
		int found;
		char* sku = new char[50];
		while (true) {
			choice = menu();
			if (choice == 1) {
				listItems();
			}
			else if (choice == 2) {
				addItem(true);
			}
			else if (choice == 3) {
				addItem(false);
			}
			else if (choice == 4) {
				updateQty();
			}
			else if (choice == 5) {
				cout << "Please enter the SKU: ";
				cin >> sku;
				found = searchItems(sku);
				if (found != -1) {
					cout << "v-----------------------------------v" << endl;
					m_items[found]->write(cout, false);
					cout << endl;
					cout << "^-----------------------------------^" << endl;
					cout << endl;
				}
				else {
					cout << "Not found!" << endl;
				}
			}
			else if (choice == 6) {
				POS();
				cout << endl;
			}
			else if (choice == 0) {
				cout << "Goodbye!" << endl;
				break;
			}
			else {
				cout << "===Invalid Selection, try again===" << endl;
				cout << endl;
			}
		}
		delete[] sku;
	}
}
/*
ouputs:
-------------------------------------
update:
Please enter the SKU: 1313
Name:
Paper Tissue
Sku: 1313
Price: 1.22
Price after tax: 1.38
Quantity: 194
Total Cost: 267.45

Please enter the number of purchased items: 12
Updated!

------------------------------
Please enter the SKU: 3434
Not found!

-------------------------------------
add item:
Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 1200/10/12
Invalid Year in Date Entry

Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 2017/5/15
Item added.

--------------------------------------------
list item:
 Row | SKU    | Item Name          | Price |TX |Qty |   Total |
-----|--------|--------------------|-------|---|----|---------|
   1 | 1234   |Milk                |   3.99|  P|   2|     7.98|
   2 | 3456   |Paper Cups          |   5.99| TN|  38|   257.21|
   3 | 4567   |Butter              |   4.56| TP|   9|    46.38|
   4 | 1212   |Salted Butter       |   5.99|  P| 111|   664.89|
   5 | 1313   |Paper Tissue        |   1.22| TN| 206|   283.99|
   6 | 5656   |Honey               |  12.99| TP|  12|   176.14|
-----^--------^--------------------^-------^---^----^---------^
			       Total Asset: $  |       1436.59|
-----------------------------------------------^--------------^


--------------------------------------
printbill:
v--------------------------------------------------------v
| 2017/04/02, 12:42                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         22.05|
^-----------------------------------------^--------------^

-------------------------------------------------------
POS:
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1313
v------------------->
| Paper Tissue
^------------------->
Sku: 1234
v------------------->
| Milk
^------------------->
Sku: 7654
Not found!
Sku: 5656
v------------------->
| Honey
^------------------->
Sku:
v--------------------------------------------------------v
| 2017/04/02, 12:58                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 1234   |Milk                |   3.99|  P|   1|     3.99|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         32.03|
^-----------------------------------------^--------------^

------------------------------------------------------
run:
The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 5656
v-----------------------------------v
Name:
Honey
Sku: 5656
Price: 12.99
Price after tax: 14.68
Quantity: 10
Total Cost: 146.79
Expiry date: 2017/05/15
^-----------------------------------^

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 12345
Not found!

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> five

===Invalid Selection, try again===

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 0

Goodbye!

*/
