#include "U.h"
#include <iostream>
using namespace std;
const string pub="/s/bach/a/class/cs253/pub/";   // ~ only works in shells
int main() {
	try{
    U foo(pub+"UnicodeData.txt", "a³+b³≠c³");
    cout << "Should be 8: " << foo.size() << '\n'
         << "Should be 2: " << foo.propcount("Sm") << '\n'
         << "Should be b³: " << foo.get(3,5) << '\n';
	cout << "NewLines: " << foo.propcount("Cc") << '\n';

	const U bar = foo;
	cout << bar.get() << '\n';
	int i,j;
	cin >> i >> j;
	cout << bar.get(i) << '\n';
	cout << bar.get(i, j) << '\n';
	string s;
	cin >> s;
	int propnum = bar.propcount(s);
	cout << "Prop " << s << ": " << propnum << '\n';
	std::set<std::string> somestuff =  bar.props();
	for( std::set<std::string>::iterator it = somestuff.begin();
		it != somestuff.end(); ++it ){
		cout << *it << '\n';
	}
	
	}
	catch( string s ){
		cout << s;
	}

    return 0;
}
