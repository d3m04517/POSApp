#ifndef ICT_ITEM_H__
#define ICT_ITEM_H__
// inlcude PosIO and POS header files
#include "PosIO.h"
#include "POS.h"
namespace ict{
  // class Item
	class Item: public PosIO {
		char m_sku[MAX_SKU_LEN + 1];
		char* m_name;
		double m_price;
		bool m_taxed;
		int m_quantity;
	public:
		Item();
		Item(const char*, const char*, double, bool tax = true);
		//copy constructor and operator
		void copy(const Item&);
		Item& operator=(const Item&);
		Item(const Item&);
		virtual ~Item();
		//setters
		void sku(const char*);
		void price(const double);
		void name(const char*);
		void taxed(const bool);
		void quantity(const int);
		//getters
		const char* sku() const;
		double price() const;
		char* name() const;
		bool taxed() const;
		int quantity() const;
		double cost() const;
		bool isEmpty() const;
		//member operators
		bool operator==(const char*);
		int operator+=(int);
		int operator-=(int);
	};
  // end class Item
  // operator += 
  double operator+=(double& d, const Item& I);
  // operator << and operator >>
  std::ostream& operator<<(std::ostream&, const Item&);
  std::istream& operator>>(std::istream&, Item&);
}


#endif
