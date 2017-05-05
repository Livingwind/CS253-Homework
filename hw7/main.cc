#include "U.h"
#include "P.h"
#include <iostream>
#include <cassert>
using namespace std;
const string pub="/s/bach/a/class/cs253/pub/";   // ~ only works in shells

int main(){
    U u;
    assert(!u);
    u = "a";
    u += " \u263a";
    {
        U u2;
        const string space = " ";
        u2 += space;
        u2.append("\U0001f42e");
        u += u2;
    }
    const U u3 = u;
    assert(u3);
    assert(u3.size() == 5);
    assert(u3[0] == "a");
    assert(u3.codepoint(2) == 0x263a);
    assert(u3.codepoint(4) == 0x1f42e);
    assert(u3 == u);
    assert(u3 == "a \u263a \U0001f42e");
    assert("a \u263a \U0001f42e" == u3);
    assert("\u262e\u262e\u262e" != u3);
    cout << "A happy cow: " << u3 << endl;

    P p;
    assert(!p);
    p.readfile("UnicodeData.txt");
    assert(p);
    for( int i =0; i< u3.size(); i++ )
        p.count(u3.codepoint(i));
    assert(p.count("Ll")==1);
    assert(p.count("So")==2);
    assert(p.size() == 29);
    cout << "There are " << p.size() << " distinct properties.\n";
    cout << p << '\n';
}
