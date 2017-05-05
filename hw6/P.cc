/*
 * Christopher William Westerman
 * 3/17/2017
 * Provide a class structure to parse the properties file
 *  and collect data about a unicode string
 * ( Does not allow the use of C++ standard unicode facilities )
 */

#include "P.h"

P::P(){}
P::~P(){}
P::P( const string filename ){
    readfile( filename );
}
P::P( const P& o ):
    properties( o.properties ),
    mappings( o.mappings ){}
P& P::operator=( const P& o ){
    properties = o.properties;
    mappings = o.mappings;
    return *this;
}

void P::readfile( const string filename){
    clear();

    fstream fin( filename, fstream::in );
    if( !fin.is_open() )
        throw string( "Unable to open property file: ")+ filename + '\n';
    
    string line, prop, tmp_s;
    unsigned int codepoint, tmp_i;

    while( getline( fin, line, '\n' ) ){
        tmp_i = line.find(';');
        tmp_s = line.substr( 0, tmp_i );
        codepoint = ::strtol( tmp_s.c_str(), nullptr, 16 );
        mappings.resize( codepoint+1 );

        tmp_i = line.find(';', tmp_i+1 );
        prop = line.substr( tmp_i+1, 2 );

        mappings[ codepoint ] = prop;
        properties[ prop ] = 0;
    }
    
    fin.close();
}

set<string> P::props() const{
    set<string> ret;
    for( auto it : properties )
        ret.insert( it.first );
    return ret;
}

void P::count( int codepoint ){
    if( codepoint < 0 || codepoint > static_cast<int>( mappings.size() ) )
        return;

    string prop;
    if( ( prop = mappings[ codepoint ] ) == "" )
        return;
    
    auto it = properties.find( prop );
    if( it != properties.end() )
        it->second++;
    else
        return;            // Spec says not to do anything, but should throw an usage error
}

int P::count( const string propname ) const{
    auto it = properties.find( propname );
    if( it != properties.end() )
        return it->second;
    else
        return 0;
}

int P::size() const{
    return properties.size();
}

bool P::empty() const{
    return size() == 0;
}

void P::clear(){
    properties.clear();
    mappings.clear();
}
