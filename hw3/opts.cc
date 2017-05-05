
#include<string>
#include<iostream>
#include<fstream>
#include<vector>

namespace opts{
	/*
	 * A class to store and maintain usage data of various letters
	 */
	class Property{
		private:
			std::string value;
			int used;

			Property* link;
		public:
			// Sets value to prop and used to 0
			Property(const std::string);
			~Property(){ delete link; }

			// Prints the contents of value
			void print();
			Property* next(){ return link; }
			Property* setNext( Property* tmp ){ link = tmp; return link; }
			// Standard pre/post increment on used
			Property &operator++(){ used++; return *this; }
			Property operator++(int){ const Property tmp = *this; ++*this; return tmp; }
			bool operator==(const std::string s) const{ return value == s; }
			bool operator!=(const std::string s) const{ return !(*this == s); }
	}; // CLASS: Property
	
	Property::Property(const std::string prop){
		value = prop;
		used = 0;
		link = nullptr;
	} // Property(string);
	 
	void Property::print(){ std::cout << value << ": " << used << "\n"; }
	/*
	 * Checks a vector to see if it
	 *	 contains a specific property value
	 * Params - Property*: list to search
	 *	  - string property: name to search for
	 * Return - pointer to found Property or nullptr on failure
	 */
	Property* containsProperty(Property*, std::string name);
	/*
	 * Builds the property vector by adding newly found entries to vector
	 *	and linking them to the pointer array.
	 * Params - ifstream: file to read from
	 *	  - Property*: list to add to
	 *	  - Property*[127]: array of pointers to properties
	 * Return - true if an error has occured
	 */
	bool buildPropVector(std::ifstream&, Property*, Property*[127]);
	
	void printAllProps(Property* head){
		Property* tmp = head;
		while( tmp != nullptr ){
			tmp->print();
			tmp = tmp->next();
		}
	}
	Property* addtoback(Property* head, std::string prop){
		if(head == nullptr)
			return head = new Property(prop);
	
		Property* tmp = head;
		while ( tmp->next() != nullptr )
			tmp = tmp->next();
		return tmp->setNext( new Property( prop ) );
	}

	Property* containsProperty(Property* head, std::string name){
		Property *tmp = head;
		while ( tmp != nullptr ){
			if(*tmp == name)
				return tmp;
			tmp = tmp->next();
		}
		return nullptr;
	} // containsProperty
	
	bool buildPropList(std::ifstream &propfile, Property *proplist, Property *anums[127]){
		std::string line, prop;
		Property* tmp;
		int hex;
		
		while(propfile >> line){
			hex = static_cast<int>(::strtol(line.c_str(), nullptr, 16));
			propfile >> line;
			prop = line;
			
			tmp = containsProperty(proplist, prop);
			if(tmp == nullptr)
				tmp = addtoback( proplist, prop );

			anums[hex] = tmp;
		};

		return 0;
	} // buildPropVector
	
} // NAMESPACE opts

int main(int argc, char* argv[]){
	argc = argc;
	argv = argv;
	//opts::Property *anums[127] = {nullptr};
	opts::Property *proplist = nullptr;
	/*
	 * (0) Find the location of the first file
	 * (1) Pass the file into the parsing function and build the property vector
	 */
	/*int fileLoc = 1;
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
	*/
	//std::ifstream propfile( argv[fileLoc] );
	//opts::buildPropList(propfile, proplist, anums);
	
	opts::addtoback( proplist, "Wz" );
	opts::addtoback( proplist, "Qr" );
	opts::printAllProps( proplist );
	delete proplist;
	return 0;
} // main()

