// compilation safegaurds
#ifndef ICT_ERROR_H_
#define ICT_ERROR_H_
#include <iostream>
namespace ict {
   class Error {
      char* m_message;
   public:
   // constructors
	   Error();
	   Error(const char*);
   // destructor
	   virtual ~Error();
   // deleted constructor and operator=
	   Error(const Error&) = delete;
	   Error& operator=(const Error&) = delete;
		
   // operator= for c-style strings
	   void operator=(const char*);
   // methods.+
	   void clear();
	   bool isClear() const;
	   void message(const char* value);
   // cast overloads
	   operator const char*() const;
	   operator bool() const;
   };
   // operator << overload prototype for cout
   std::ostream& operator<<(std::ostream&, Error&);		
}

#endif
