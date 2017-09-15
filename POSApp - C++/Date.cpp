#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <ctime>
#include "Date.h"
#include "POS.h"
using namespace std;

namespace ict{
  void Date::set(){
    time_t t = time(NULL);
    tm lt = *localtime(&t);
    m_day = lt.tm_mday;
    m_mon = lt.tm_mon + 1;
    m_year = lt.tm_year + 1900;
    if (dateOnly()){
      m_hour = m_min = 0;
    }
    else{
      m_hour = lt.tm_hour;
      m_min = lt.tm_min;
    }
  }

  int Date::value()const{
    return m_year * 535680 + m_mon * 44640 + m_day * 1440 + m_hour * 60 + m_min;
  }

  int Date::mdays()const{
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 }; 
    int mon = m_mon >= 1 && m_mon <= 12 ? m_mon : 13; 
    mon--; 
    return days[mon] + int((mon == 1)*((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0)); 
  }

  void Date::errCode(int errorCode) {
	  m_readErrorCode = errorCode;
  }
  
  void Date::set(int year, int mon, int day, int hour, int min) {
	  m_readErrorCode = NO_ERROR;
	  m_year = year;
	  m_mon = mon;
	  m_day = day;
	  m_hour = hour;
	  m_min = min;
  }

  Date::Date() {
	  m_dateOnly = false;
	  set();
  }

  Date::Date(int year, int month, int day) {
	  m_dateOnly = true;
	  set(year, month, day, 0, 0);
  }

  Date::Date(int year, int month, int day, int hour, int min) {
	  m_dateOnly = false;
	  set(year, month, day, hour, min);
  }

  bool Date::operator==(const Date& D) const {
	  return value() == D.value();
	
  }
 

  bool Date::operator!=(const Date& D) const {
	  return value() != D.value();
		
  }
  bool Date::operator<(const Date& D) const {
	  return value() < D.value();
  }
  bool Date::operator>(const Date& D) const {
	  return value() > D.value();
  }

  bool Date::operator<=(const Date& D) const {
	  return value() <= D.value();
  }

  bool Date::operator>=(const Date& D) const {
	  return value() >= D.value();
  }

  int Date::errCode() const {
	  return m_readErrorCode;
  }

  bool Date::bad() const {
	  if (m_readErrorCode != 0) {
		  return true;
	  }
	  else {
		  return false;
	  }
  }

  bool Date::dateOnly() const {
	  return m_dateOnly;
  }

  void Date::dateOnly(bool value) {
	  m_dateOnly = value;
	  if (value) {
		  m_hour = 0;
		  m_min = 0;
	  }
  }

  std::ostream& Date::write(std::ostream& ostr) const {
	  if (m_dateOnly) {
		  ostr << m_year << "/";
		  ostr.width(2);
		  ostr.fill('0');
		  ostr << m_mon;
		  ostr << "/";
		  ostr.width(2);
		  ostr.fill('0');
		  ostr << m_day;
		  return ostr;
	  }
	  else {
		  ostr << m_year << "/";
		  ostr.width(2);
		  ostr.fill('0');
		  ostr << m_mon;
		  ostr << "/";
		  ostr.width(2);
		  ostr.fill('0');
	      ostr << m_day;
		  ostr << ", " << m_hour << ":" << m_min;
		  return ostr;
	  }
  }

  std::istream& Date::read(istream& is) {
	  if (m_dateOnly) {
		  is >> m_year;
		  if (std::cin.get() != '/') {
			  m_readErrorCode = CIN_FAILED;
		  }
		  else {
			  is >> m_mon;
			  if (std::cin.get() != '/') {
				  m_readErrorCode = CIN_FAILED;
			  }
			  else {
				m_readErrorCode = NO_ERROR;
				  is >> m_day;
				  if (m_year >= MIN_YEAR && m_year <= MAX_YEAR) {
				m_readErrorCode = NO_ERROR;
					  if (m_mon > 0 && m_mon <= 12){
						m_readErrorCode = NO_ERROR;
						  if (m_day < 1 || m_day > mdays()) {
							  m_readErrorCode = DAY_ERROR;
						  }
					  }
					  else {
						  m_readErrorCode = MON_ERROR;
					  }
				  }
				  else {
					  m_readErrorCode = YEAR_ERROR;
				  }
			  }
		  }
	  }
	  else {
		  is >> m_year;
		  if (std::cin.get() != '/') {
			  m_readErrorCode = CIN_FAILED;
		  }
		  else {
			  is >> m_mon;
			  if (std::cin.get() != '/') {
				  m_readErrorCode = CIN_FAILED;
			  }
			  else {
				  is >> m_day;
				  if (std::cin.get() != ',') {
					  m_readErrorCode = CIN_FAILED;
				  }
				  else {
					  if (std::cin.get() != ' ') {
						  m_readErrorCode = CIN_FAILED;
					  }
					  else {
						  is >> m_hour;
						  if (std::cin.get() != ':') {
							  m_readErrorCode = CIN_FAILED;
						  }
						  else {
							  is >> m_min;
							  if (m_year >= MIN_YEAR && m_year <= MAX_YEAR) {
								  if (m_mon > 0 && m_mon <= 12) {
									  if (m_day > 0 && m_day <= mdays()) {
										  if (m_hour >= 0 && m_hour <= 23) {
											  if (m_min < 0 || m_min > 23) {
												  m_readErrorCode = MIN_ERROR;
											  }
										  }
										  else {
											  m_readErrorCode = HOUR_ERROR;
										  }
									  }
									  else {
										  m_readErrorCode = DAY_ERROR;
									  }
								  }
								  else {
									  m_readErrorCode = MON_ERROR;
								  }
							  }
							  else {
								  m_readErrorCode = YEAR_ERROR;
							  }
						  }
					  }
				  }
			  }
		  }
	  }
	  return is;
  }

  std::istream& operator>>(istream& is, Date& D) {
	  D.read(is);
	  return is;
  }
  std::ostream& operator<<(ostream& is, const Date& D) {
	  D.write(is);
	  return is;
  }

  int Date::year() const {
	  return m_year;
  }
  int Date::month() const {
	  return m_mon;
  }
  int Date::day() const {
	  return m_day;
  }
  void Date::year(int year) {
	  m_year = year;
  }
  void Date::month(int month) {
	  m_mon = month;
  }
  void Date::day(int day) {
	  m_day = day;
  }
}

