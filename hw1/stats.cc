/*
 *	Christopher William Westerman
 *	1/24/2017
 *	This program takes input from standard input
 *		and counts the number of different character types.
 */
 
 //#define DEBUG
 
 #include <iostream>
 #include <string>
 using namespace std;

void parseCharType(int type[6]);
void printCharType(int type[6]);

int main(int argc, char* argv[]){
	int type[6];
	parseCharType(type);
	printCharType(type);
	 
	return 0;
}

/*
 * Prints the values stored in the
 *	array passed as a parameter.
 * Param - int[6](array information to be printed)
 * No returns
 */
void printCharType(int type[6]){
	cout 	<< "Control: " 		<< type[0]
			<< "\nLetter: "		<< type[1]
			<< "\nNumber: " 	<< type[2]
			<< "\nPunctuation: "<< type[3]
			<< "\nSymbol: "		<< type[4]
			<< "\nSpace: "		<< type[5]
			<< endl;
}

/*
 * Gathers character input from cin and
 * 	puts the values into the array parameter
 * Param - int[6](array to store type information)
 * No returns
 */
void parseCharType(int type[6]){
/*
 * This string array contains the information regarding type
 *	of each character in the ASCII table.
 * Each string is located at the array location associated with
 *	the characters value in the ASCII table.
 *	ie. 'NUL' = a[0] | 'A' = a[65] | 'DEL' = a[127]
 * Access with ASCII_INFO['<char>'][0/1]
 */
 const string ASCII_INFO[] = 	{	
								"Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc",
								"Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc","Cc",
								"Zs","Po","Po","Po","Sc","Po","Po","Po","Ps","Pe","Po","Sm","Po","Pd","Po","Po",
								"Nd","Nd","Nd","Nd","Nd","Nd","Nd","Nd","Nd","Nd","Po","Po","Sm","Sm","Sm","Po",
								"Po","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu",
								"Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Lu","Ps","Po","Pe","Sk","Pc",
								"Sk","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll",
								"Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ll","Ps","Sm","Pe","Sm","Cc" 
								};



	for(int i = 6; i >= 0; --i)
		type[i] = 0;
	char	c;
	 
	/*
	 * (0)	Loop through standard input until nothing gets found
	 * (1)	Find the characteristic matching the found character
	 */
	while( cin.get(c) ){		//(0)
		#ifdef DEBUG
		cout << c << ": "<< ASCII_INFO[(int)c][0] << endl;
		#endif
		switch(ASCII_INFO[(int)c][0]){		//(1)
			case 'C':	type[0]++; break;
			case 'L':	type[1]++; break;
			case 'N':	type[2]++; break;
			case 'P':	type[3]++; break;
			case 'S':	type[4]++; break;
			case 'Z':	type[5]++; break;
			default	:	cout << "'" << c << "' doesn't match to a type" << endl;
		}
	}
}

