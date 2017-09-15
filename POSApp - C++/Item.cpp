#define _CRT_SECURE_NO_WARNINGS  
// Lib includes
#include <iostream>
#include <cstring>
// inlcude Item and POS header files
#include "Item.h"
#include "POS.h"
#include "PosIO.h"
using namespace std;
namespace ict{
  // class Item implementaion
	Item::Item() {
		m_sku[0] = '\0';
		m_name = nullptr;
		m_price = 0;
		m_quantity = 0;
	}

	Item::Item(const char* sku, const char* name, double price, bool tax) {
		strncpy(m_sku, sku, MAX_SKU_LEN);

		int length = strlen(name);
		m_name = new char[length + 1];
		strcpy(m_name, name);
		
		m_quantity = 0;
		m_price = price;
		m_taxed = tax;
	}
	void Item::copy(const Item& item) {
		strncpy(m_sku, item.m_sku, MAX_SKU_LEN);
		if (this->m_name != nullptr) {
			delete m_name;
		}
		int length = strlen(item.m_name);
		m_name = new char[length + 1];
		strncpy(m_name, item.m_name, length + 1);

		m_price = item.m_price;
		m_taxed = item.m_taxed;
		m_quantity = item.m_quantity;
	}
	Item::Item(const Item& item) {
		copy(item);
	}
	Item::~Item() {
		delete[] m_name;
	}

	Item& Item::operator=(const Item& item) {
		if (this != &item && !item.isEmpty()) {
			copy(item);
		}
		return *this;
	} 

	void Item::sku(const char* sku) {
		strncpy(m_sku, sku, MAX_SKU_LEN);
	}

	void Item::price(const double price) {
		m_price = price;
	}

	void Item::name(const char* name) {
		int length = strlen(name);
		delete[] m_name;
		m_name = new char[length + 1];
		strcpy(m_name, name);
	}

	void Item::taxed(const bool tax) {
		m_taxed = tax;
	}

	void Item::quantity(const int quantity) {
		m_quantity = quantity;
	}

	const char* Item::sku() const {
		return (const char*)m_sku;
	}

	double Item::price() const {
		return m_price;
	}

	char* Item::name() const {
		return (char*)m_name;
	}

	bool Item::taxed() const {
		return m_taxed;
	}

	int Item::quantity() const {
		return m_quantity;
	}

	double Item::cost() const {
		if (m_taxed) {
			return m_price + (m_price * TAX);
		}
		else {
			return m_price;
		}
	}

	bool Item::isEmpty() const {
		if (m_sku[0] == '\0' && m_name == nullptr && m_price == 0 && m_quantity == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Item::operator==(const char* sku) {
		if (strcmp(m_sku, sku) == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	int Item::operator+=(const int quantity) {
		return m_quantity += quantity;
	}

	int Item::operator-=(const int quantity) {
		return m_quantity -= quantity;
	}

	double operator+=(double& d, const Item& I) {
		return d += I.quantity() * I.cost();
	}

	std::ostream& operator<<(std::ostream& ostr, const Item& item) {
		item.write(ostr, 1);
		return ostr;
	}
	std::istream& operator>>(std::istream& istr, Item& item) {
		item.read(istr);
		return istr;
	}
}
