#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <cstring>
#include "Error.h"
using namespace std;
namespace ict{

	Error::Error() {
		m_message = nullptr;
	}

	Error::Error(const char* errorMessage) {
		m_message = nullptr;
		message(errorMessage);
	}
	
	void Error::operator=(const char* errorMessage) {
		clear();
		message(errorMessage);
	}

	Error::~Error() {
		delete[] m_message;
	}

	void Error::clear() {
		delete[] m_message;
		m_message = nullptr;
	}

	bool Error::isClear() const {
		if (m_message == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}

	void Error::message(const char* value) {
		delete[] m_message;
		int length = strlen(value);
		m_message = new char[length + 1];
		strcpy(m_message, value);
	}

	Error::operator const char*() const {
		return m_message;
	}

	Error::operator bool() const {
		return isClear();
	}

	ostream& operator<<(ostream& os, Error& err) {
		if (err.Error::isClear()) {
			return os;
		}
		else {
			os << (const char *)err;
			return os;
		}
	}
}