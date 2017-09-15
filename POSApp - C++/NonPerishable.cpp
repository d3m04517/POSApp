#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "NonPerishable.h"
using namespace std;
namespace ict {
	bool NonPerishable::ok() const {
		return m_err.isClear();
	}

	void NonPerishable::error(const char* message) {
		m_err.message(message);
	}

	char NonPerishable::signature() const {
		return 'N';
	}

	std::fstream& NonPerishable::save(std::fstream& file) const {
		if (file.is_open()) {
			file << signature() << ',' << sku() << ',' << name() << ',' << price() << ',' << taxed() << ',' << quantity();
		}
		return file;
	}

	std::fstream& NonPerishable::load(std::fstream& file) {
		int quantity;
		bool tax;
		double price;
		char* sku = new char[50];
		char* name = new char[50];
		char ch;
		if (file.is_open()) {
			file.getline(sku, 20, ',');
			file.getline(name, 20, ',');
			file >> price >> ch >> tax >> ch >> quantity;
			this->sku(sku);
			this->name(name);
			this->price(price);
			this->taxed(tax);
			this->quantity(quantity);
		}
		delete[] sku;
		delete[] name;
		return file;
	}

	std::ostream& NonPerishable::write(ostream& ostr, bool linear) const {
		if (!ok()) {
			ostr << (Error&)m_err << endl;
		}
		else {
			if (linear) {
				ostr.setf(ios::left);
				ostr << " ";
				ostr.width(MAX_SKU_LEN);
				ostr  << sku();
				ostr << '|';
				ostr.width(20);
				ostr << name();
				ostr << '|';
				ostr.width(7);
				ostr.precision(2);
				ostr.unsetf(ios::left);
				ostr.setf(ios::fixed);
				ostr << price();
				ostr << '|';
				if (taxed()) {
					ostr << ' ' << 'T' << signature() << '|';
				}
				else {
					ostr << ' ' << ' ' << signature() << '|';
				}
				ostr.width(4);
				ostr << quantity();
				ostr << '|';
				ostr.width(9);
				ostr.precision(2);
				ostr << cost() * quantity();
				ostr << '|';
				ostr.unsetf(ios::fixed);
			}
			else {
				ostr.unsetf(ios::right);
				ostr << "Name:" << endl;
				ostr.setf(ios::fixed);
				ostr << name() << endl;
				ostr << "Sku: " << sku() << endl;
				ostr.precision(2);
				ostr << "Price: " << price() << endl;
				if (taxed()) {
					ostr << "Price after tax: " << cost() << endl;
				}
				else {
					ostr << "Price after tax: N/A" << endl;
				}
				ostr << "Quantity: " << quantity() << endl;
				ostr.precision(2);
				ostr << "Total Cost: " << cost() * quantity() << endl;
			}
		}
		return ostr;
	}

	std::istream& NonPerishable::read(std::istream& is) {
		char* sku = new char[50];
		char* name = new char[50];
		double price;
		char taxed;
		bool isTaxed;
		int length;
		int quantity;
		if (!is.fail()) {
			cout << "Item Entry:" << endl;
			cout << "Sku: ";
			is >> sku;
			length = strlen(sku);
			sku[length] = '\0';
			cout << "Name:" << endl;
			is >> name;
			length = strlen(name);
			name[length] = '\0';
			cout << "Price: ";
			is >> price;
			if (!is.fail()) {
				
				cout << "Taxed: ";
				is >> taxed;
				if (taxed != 'y' && taxed != 'n') {
					is.setstate(ios::failbit);
				}
				if (taxed == 'y') {
					isTaxed = true;
				}
				else if (taxed == 'n') {
					isTaxed = false;
				}
				if (!is.fail()) {
					cout << "Quantity: ";
					is >> quantity;
					if (!is.fail()) {
						this->sku(sku);
						this->name(name);
						this->price(price);
						this->taxed(isTaxed);
						this->quantity(quantity);
					}
					else {
						error("Invalid Quantity Entry");
					}
				}
				else {
					error("Invalid Taxed Entry, (y)es or (n)o");
				}
			}
			else {
				error("Invalid Price Entry");
			}
		}
		delete[] sku;
		delete[] name;
		return is;
	}
}
