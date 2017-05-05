/*
 * Christopher William Westerman
 * 3/17/2017
 * Provide a class structure to parse UTF-8 Unicode and
 * 	construct metrics of provided files
 * ( Does not allow the use of C++ standard unicode facilities )
 */

#include "U.h"

U::U(){
}

U::U( const U& other ):
	vprops( other.vprops ),
	stored_str( other.stored_str )
{
	for( int i = 0; i < PROP_COMBO; i++ )
		properties[i] = other.properties[i];
}

U::U( const std::string property_filename, const std::string data ) : U(){
	propfile( property_filename );
	readstring( data );
}

U &U::operator=(const U& other){
	for( int i = 0; i < PROP_COMBO; i++ )
		properties[i] = other.properties[i];
	vprops 		= other.vprops;
	stored_str 	= other.stored_str;
	return *this;
}

U::~U(){
}


/*
 * Parses the Unicode properties file and maps the pointer vector
 *	to the list of properties.
 * Params - string: file to open
 * Return - void
 */
void U::propfile( const std::string filename ){
	if( this->vprops.size() != 0 ){
		throw std::string( "Can't call propfile("+filename+"): Property file already processed.\n" );
	}

	std::ifstream infile( filename );
	if( !infile.is_open()){
		throw std::string( "Cannot open property file: " + filename + "\n" );
	}
	
	std::string line, codepoint, prop;
	unsigned int codenum, tmp, propindex;
	
	/*
 	 * (0) Split the line into parts containing the codepoint
 	 * 	and add space in the property vector for that value
 	 * (1) Collect the value (Aa-Zz) from the line and if it
 	 * 	doesn't exist, create a property in the properties
 	 * 	array with the corresponding value
 	 */
	while( std::getline( infile, line, '\n' ) ){
		// (0)
		tmp = line.find(';');
		codepoint = line.substr(0, tmp);
		codenum = ::strtol(codepoint.c_str(), nullptr, 16);
		vprops.resize(codenum+1);
		
		// (1)
		tmp = line.find(';', tmp+1);
		prop = line.substr( tmp+1, 2 );

		propindex = ( (prop[0] - 'A') * 26) + ( prop[1] - 'a' );
		if( properties[ propindex ].value == "" )
			properties[ propindex ].value = prop;
		vprops[ codenum ] = &properties[ propindex ];
	}
	infile.close();
}

/* Special function to be used in the test constructor
 */
void U::readstring( const std::string s_input ){
	char c;
	int s_loc;
	unsigned int vloc_str;
	unsigned long int codepoint;
	std::string stored_input;

	s_loc = 0;
	vloc_str = stored_str.size();

	/*
	 * (0) Begin parsing the characters from the string
	 * 	and build the Unicode codepoint.
	 * (1) Increment the times used counter of the object
	 * 	pointed to by the index corresponding to the codepoint
	 */
	while ( (c = s_input[s_loc]) != 0 ){
		s_loc++;
		stored_str.resize( vloc_str + 1 );

		//(0)
		if( (c & 0x80) == 0 ){
			stored_str[vloc_str] = c;
			codepoint = static_cast<unsigned int>(c);
		}
		else if( (c & 0xE0) == 0xC0 ){
			stored_str[vloc_str] = c;
			codepoint = ((c & 0x1F) << 6);		// xxxxx-000000
			c = s_input[s_loc];
			s_loc++;
			stored_str[vloc_str] += c;
			codepoint |= ( c & 0x3F );		// xxxxx-yyyyyy
		}
		else if( (c & 0xF0) == 0xE0){
			stored_str[vloc_str] = c;
			codepoint = ((c & 0x0F) << 12);		// xxxx-000000-000000
			c = s_input[s_loc];
			s_loc++;
			stored_str[vloc_str] += c;
			codepoint |= ((c & 0x3F) << 6);		// xxxx-yyyyyy-000000
			c = s_input[s_loc];
			s_loc++;
			stored_str[vloc_str] += c;
			codepoint |= (c & 0x3F);		// xxxx-yyyyyy-zzzzzz
		}
		else if( (c & 0xF8) == 0xF0 ){
			stored_str[vloc_str] = c;
			codepoint = ((c & 0x07) << 18);
			c = s_input[s_loc];
			s_loc++;
			stored_str[vloc_str] += c;
			codepoint |= ((c & 0x3F) << 12);
			c = s_input[s_loc];
			s_loc++;
			stored_str[vloc_str] += c;
			codepoint |= ((c & 0x3F) << 6);
			c = s_input[s_loc];
			s_loc++;
			stored_str[vloc_str] += c;
			codepoint |= (c & 0x3F);
		}
		else
			throw std::string( c + " does not respresent a valid Unicode leading character.\n" );
		
		//(1)	
		if( vprops[ codepoint ] != nullptr ){
			vprops[ codepoint ]->used++;
		}
		else
			throw std::string( "Encounted a character not included in the property file.\n" );
	
		vloc_str++;
	}
}


/*
 * Parses input files using the properties described in the
 * 	property file.
 * If no property file has been set up, the function will throw
 * 	a message regarding the character not being present.
 * Adds all hex values gathered to the accumulated string
 * Params - std::string file to read
 * Return - void
 */

void U::readfile( const std::string filename ){
	if( this->vprops.size() == 0 ){
		return;
	}

	std::ifstream infile( filename );
	if ( !infile.is_open() ){
		throw std::string( "Could not open file: "+filename+"\n" );
	}

	char c;
	unsigned int vloc_str;
	unsigned long int codepoint;

	vloc_str = stored_str.size();

	/*
	 * (0) Begin parsing the characters from the file
	 * 	and build the Unicode codepoint.
	 * (1) Increment the times used counter of the object
	 * 	pointed to by the index corresponding to the codepoint
	 */
	while ( infile.get(c) ){
		stored_str.resize( vloc_str + 1);

		//(0)
		if( (c & 0x80) == 0 ){
			stored_str[vloc_str] = c;
			codepoint = static_cast<unsigned int>(c);
		}
		else if( (c & 0xE0) == 0xC0 ){
			stored_str[vloc_str] = c;
			codepoint = ((c & 0x1F) << 6);		// xxxxx-000000
			infile.get(c);
			stored_str[vloc_str] += c;
			codepoint |= ( c & 0x3F );		// xxxxx-yyyyyy
		}
		else if( (c & 0xF0) == 0xE0){
			stored_str[vloc_str] = c;
			codepoint = ((c & 0x0F) << 12);		// xxxx-000000-000000
			infile.get(c);
			stored_str[vloc_str] += c;
			codepoint |= ((c & 0x3F) << 6);		// xxxx-yyyyyy-000000
			infile.get(c);
			stored_str[vloc_str] += c;
			codepoint |= (c & 0x3F);		// xxxx-yyyyyy-zzzzzz
		}
		else if( (c & 0xF8) == 0xF0 ){
			stored_str[vloc_str] = c;
			codepoint = ((c & 0x07) << 18);
			infile.get(c);
			stored_str[vloc_str] += c;
			codepoint |= ((c & 0x3F) << 12);
			infile.get(c);
			stored_str[vloc_str] += c;
			codepoint |= ((c & 0x3F) << 6);
			infile.get(c);
			stored_str[vloc_str] += c;
			codepoint |= (c & 0x3F);
		}
		else
			throw std::string(c + " does not respresent a valid Unicode leading character.\n");
		
		//(1)	
		if( vprops[codepoint] != nullptr ){
			vprops[codepoint]->used++;
		}
		else
			throw std::string("Encounted a character not included in the property file.\n");
	
		vloc_str++;
	}
	infile.close();
}

int		U::size() const	{ return stored_str.size(); }
std::string 	U::get() const	{
					std::string s = "";
					for( unsigned int i = 0; i < stored_str.size(); i++ )
						s += stored_str[i];
					return s;
				}
std::string	U::get( const int index ) const{
					if( index < 0 || index >= static_cast<int>(stored_str.size()) )
						throw std::string("Invalid index for get(index).\n");
					return stored_str[index];
				}
std::string	U::get( const int start, const int end ) const{
					if( start < 0 || end >= static_cast<int>(stored_str.size()) )
						throw std::string("Parameters of get(start, end).\n");
					std::string s = "";
					for( int i = start; i < end; i++ )
						s += stored_str[i];
					return s;
				}
int		U::propcount( std::string propname ) const {
					unsigned int propindex = ( (propname[0] - 'A') * 26) + ( propname[1] - 'a' );
					if( properties[ propindex ].value == "" )
						throw std::string("Property not included in the properties file.\n"); 
					return properties[ propindex ].used;
				}

std::set<std::string> U::props() const{
					std::set<std::string> propset;
					for( unsigned int i = 0; i < PROP_COMBO; i++){
						if( properties[i].value != "" )
							propset.insert( properties[i].value );
					}
					return propset;
				}
