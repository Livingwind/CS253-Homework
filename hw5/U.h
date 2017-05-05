/*
 * Christopher William Westerman
 * 3/17/2017
 * Provide a class structure to parse UTF-8 Unicode and
 * 	construct metrics of provided files
 * ( Does not allow the use of C++ standard unicode facilities )
 */

#ifndef U_INCLUDE
#define U_INCLUDE

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>

const int PROP_COMBO = 676;

class U {
	private:
		class Property{
			public:
				std::string value;
				int used;

				Property(){
					value = "";
					used = 0;
				}
				Property( const std::string s ){
					value = s;
					used = 0;
				}
				Property &operator=(const Property& other){
					value = other.value;
					used = other.used;
					return *this;
				}
		};

		Property properties[ PROP_COMBO ];
		std::vector<Property*> vprops;
		std::vector<std::string> stored_str;

		void readstring( std::string );

	public:	
		U();
		U( const U& );
		U( const std::string property_filename, const std::string data );
		U &operator=( const U& );
		~U();
		
		void 		readfile( const std::string filename );
		void 		propfile( const std::string property_filename );
		int 		size() const;
		std::string	get() const;
		std::string	get( const int index ) const;
		std::string	get( const int start, const int end_halfopen ) const;
		int		propcount( std::string property_name ) const;

		std::set<std::string> props() const;
}; // CLASS: U

#endif
