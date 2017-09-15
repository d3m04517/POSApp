#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include <iostream>
#include "Perishable.h"
#include "Date.h"
using namespace std;

namespace ict {
	char Perishable::signature() const {
		return 'P';
	}

	Perishable::Perishable(){
		m_expiry.dateOnly(true);
	}

	std::fstream& Perishable::save(std::fstream& file) const {
		if (file.is_open()) {
			NonPerishable::save(file);
			file << ',' << m_expiry.year() << '/' << m_expiry.month() << '/' << m_expiry.day();
		}
		return file;
	}

	std::fstream& Perishable::load(std::fstream& file) {
		char ch;
		int day;
		int month;
		int year;
		if (file.is_open()) {
			NonPerishable::load(file);
			file >> ch >> year >> ch >> month >> ch >> day;
			m_expiry.year(year);
			m_expiry.month(month);
			m_expiry.day(day);
		}
		return file;
	}

	std::ostream& Perishable::write(std::ostream& os, bool linear) const {
		os.unsetf(ios::right);
		this->NonPerishable::write(os, linear);
		if (this->ok()) {
			if (!linear) {
				os << "Expiry date: ";
				os << (Date&)m_expiry;
			}
		}
		return os;
	}

	std::istream& Perishable::read(std::istream& istr) {
		cout << "Perishable ";
		NonPerishable::read(istr);
		if (!istr.fail()) {
			cout << "Expiry date (YYYY/MM/DD): ";
			istr >> (Date&)m_expiry;
			if (m_expiry.errCode() != 0) {
				istr.setstate(ios::failbit);
			}
			if (m_expiry.errCode() == 1) {
				this->error("Invalid Date Entry");
			}
			else if (m_expiry.errCode() == 2) {
				this->error("Invalid Year in Date Entry");
			}
			else if (m_expiry.errCode() == 3) {
				this->error("Invalid Month in Date Entry");
			}
			else if (m_expiry.errCode() == 4) {
				this->error("Invalid Day in Date Entry");
			}
		}
		return istr;
	}
}



