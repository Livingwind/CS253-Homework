#include "U.h"
#include "P.h"
#include <iostream>
using namespace std;
const string pub="/s/bach/a/class/cs253/pub/";   // ~ only works in shells
int main() {
    try{
        U u;
        u.append("a+bc");
        P p;
        p.readfile(pub+"UnicodeData.txt");

        for (int i=0; i<u.size(); i++)
            p.count(u.codepoint(i));

        const U u2(u);
        const U u3 = u;
        const P p2(p);
        const P p3(p);

        cout << "Should be 4: " << u3.size() << '\n'
                << "Should be 3: " << p3.count("Ll") << '\n'
                 << "Should be : " << u.get(0,2) << '\n';
        try {
            u.readfile("/bogus");
        }
        catch (const string &msg) {
            cout << "I expected this: " << msg << '\n';
        }
        return 0;
    }
    catch (const string &msg) {
        cout << "Unexpected error: " << msg << '\n';
    }
}
