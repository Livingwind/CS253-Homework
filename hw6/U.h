/* Christopher William Westerman
 * 4/7/2017
 * Provide a class structure to parse UTF-8 Unicode and
 * 	construct metrics of provided files
 * ( Does not allow the use of C++ standard unicode facilities )
 */

#ifndef U_INCLUDE
#define U_INCLUDE

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class U {
    private:
		vector<unsigned int> stored_codepoints;

		void            readstring( string );
        string          encodeUTF8( unsigned int ) const;
        unsigned int    decodeUTF8( string ) const;
        void            processData( iostream& ins );

	public:	
		U();
        U( const string filename );
		U( const U& );
		U &operator=( const U& );
		~U();
		
		void    	readfile( const string filename );
        void        append( const string data );
		string	    get() const;
		string  	get( const int index ) const;
		string  	get( const int start, const int end_halfopen ) const;
        int         codepoint( const int index ) const;
		int 	    size() const;
        bool        empty() const;
        void        clear();
}; // CLASS: U

#endif
