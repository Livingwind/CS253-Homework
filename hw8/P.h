/* Christopher William Westerman
 * 4/7/2017
 * Provide a class structure to parse UTF-8 Unicode and
 * 	construct metrics of provided files
 * ( Does not allow the use of C++ standard unicode facilities )
 */

#ifndef P_INCLUDE
#define P_INCLUDE

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

class P {
	private:
        map<string, unsigned int> properties;
        vector<string> mappings;

    public:
        P();
        P( const string filename );
        P( const P& );
        P &operator=( const P& );
        ~P();

        operator bool() const;

        void        readfile( const string filename );
        set<string> props() const;
        void        count( int );
        int         count( const string propname ) const;
        int         size() const;
        bool        empty() const;
        void        clear();
};

ostream& operator<<( ostream&, const P& );

#endif
