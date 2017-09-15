#ifndef ICT_NONPERISHABLE_H__
#define ICT_NONPERISHABLE_H__
#include <fstream>
#include "Item.h"
#include "Error.h"

namespace ict{
	class NonPerishable : public Item {
		Error m_err;

	protected:
		bool ok() const;
		void error(const char*);
		virtual char signature() const;

	public: 
		std::fstream& save(std::fstream&) const;
		std::fstream& load(std::fstream&);
		std::ostream& write(std::ostream&, bool) const;
		std::istream& read(std::istream&);
	};
}








#endif