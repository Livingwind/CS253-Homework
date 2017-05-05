
#include<string>
#include<iostream>
#include<fstream>
#include<vector>

/*
 * A class to store and maintain usage data of various letters
 */
class Property{
	private:
		std::string value;
		int used;
	public:
		// Sets value to prop and used to 0
		Property(const std::string);

		// Prints the contents of value
		void print();
		std::string getValue(){ return value; }
		void incrUsed(){ used++; }
		bool operator==(const std::string s) const{ return value == s; }
		bool operator!=(const std::string s) const{ return !(*this == s); }
}; // CLASS: Property

Property::Property(const std::string prop){
	value = prop;
	used = 0;
} // Property(string);
	 
void Property::print(){ std::cout << value << ": " << used << "\n"; }
void printAll( Property*[676] );
/*
 * Checks a vector to see if it
 *	 contains a specific property value
 * Params - Property*[]: Array of Property's to search
 *	  - string property: name to search for
 * Return - pointer to found Property or nullptr on failure
 */
int indexProp(std::string name){
	return ((name[0] - 'A') * 26) + ( (name[1]-'a') );
}


/*
 * Parses the Unicode properties file and maps the pointer vector
 *	to the list of properties.
 * Params - string: file to open
 *	  - vector<Property*>: pointers to new Property objects
 *	  - Property*[]: array containing Property values
 * Return - 0 on error
 */
int processPropFile( std::string propfile, std::vector<Property*> &anums, Property* proplist[676] ){
	std::ifstream infile( propfile );
	if( !infile.is_open()){
		std::cerr << "Can't open properties file: " << propfile << "\n";
		return 1;
	}
	
	std::string line, codepoint, prop;
	int codenum, tmp;

	while( std::getline( infile, line, '\n' ) ){
		tmp = line.find(';');
		codepoint = line.substr(0, tmp);
		codenum = ::strtol(codepoint.c_str(), nullptr, 16);
		anums.resize(codenum+1);
	
		tmp = line.find(';', tmp+1);
		prop = line.substr( tmp+1, 2 );
		if( proplist[ indexProp( prop ) ] == nullptr )
			proplist[ indexProp( prop ) ] = new Property( prop );
		anums[codenum] = proplist[ indexProp( prop ) ];
	}
	return 0;
}

void printAll( Property* proplist[676] ){
	for( int i = 0; i < 676; i++ ){
		if( proplist[i] != nullptr )
			proplist[i]->print();
	}
}

int parseUTF8( std::string filename, const std::vector<Property*> anums ){
	std::ifstream infile( filename );
	if ( !infile.is_open() ){
		std::cerr << "Could not open file: " << filename << "\n";
		exit(-1);
	}
	char c;
	unsigned long int tmp;

	while ( infile.good() ){
		infile.get(c);
		if( (c & 0x80) == 0 ){
			tmp = static_cast<unsigned int>(c);
		}
		else if( (c & 0xE0) == 0xC0 ){
			tmp = ((c & 0x1F) << 6);	// xxxxx-000000
			infile.get(c);
			tmp |= ( c & 0x3F );		// xxxxx-yyyyyy
		}
		else if( (c & 0xF0) == 0xE0){
			tmp = ((c & 0x0F) << 12);	// xxxx-000000-000000
			infile.get(c);
			tmp |= ((c & 0x3F) << 6);	// xxxx-yyyyyy-000000
			infile.get(c);
			tmp |= (c & 0x3F);		// xxxx-yyyyyy-zzzzzz
		}
		else if( (c & 0xF8) == 0xF0 ){
			tmp = ((c & 0x07) << 18);
			infile.get(c);
			tmp |= ((c & 0x3F) << 12);
			infile.get(c);
			tmp |= ((c & 0x3F) << 6);
			infile.get(c);
			tmp |= (c & 0x3F);
		}
		else
			return c;
			
		if( anums[tmp] != nullptr )
			anums[tmp]->incrUsed();
		else
			return -1;
		tmp = 0;	
	}

	return 0;
}


int main(int argc, char* argv[]){
	/*
	 * (0) Find the location of the first file
	 * (1) Pass the file into the parsing function and build the property vector
	 */
	int fileLoc = 1;
	do{									// (0)
		if(!( fileLoc < argc )){
			std::cerr << "No files detected after options!\n";
			return -1;
		}
		if( argv[fileLoc][0] == '-' )
			fileLoc++;
		else
			break;
	}while(1);
	
	std::vector<Property*> anums;
	Property* proplist[676] = { nullptr };
	processPropFile( argv[fileLoc], anums, proplist );
	
	int err;
	for( int i = fileLoc+1; i < argc; i++){
		err = parseUTF8( argv[i], anums );
		if( err > 0 ){
			std::cerr << argv[0] << " encounted an improperly formatted byte: " << err << " in " << argv[i] << '\n';
			return -1;
		}
		else if( err == -1 ){
			std::cerr << argv[0] << " encounted an undefined property in: " << argv[i] << '\n';
			return -1;
		}	
	}

	std::string tmp;
	for(int i = 1; i < fileLoc; i++){
		tmp = argv[i]+1;
		if( tmp == "all")
			printAll( proplist );
		else if( proplist[indexProp(argv[i]+1)] != nullptr)
			proplist[indexProp(argv[i]+1)]->print();
		else{
			std::cerr << "Option not defined: " << argv[i] << "\n";
			return -1;
		}
	}
	
	for(int i = 0; i < 676; i++)
		delete proplist[i];
	return 0;
} // main()
