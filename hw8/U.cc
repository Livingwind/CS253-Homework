/*
 * Christopher William Westerman
 * 3/17/2017
 * Provide a class structure to parse UTF-8 Unicode and
 * 	construct metrics of provided files
 * ( Does not allow the use of C++ standard unicode facilities )
 */

#include "U.h"

static char add_leading_byte( iostream &ins ){
    char c;
    if( !ins.get(c) )
        throw string( "Expected valid unicode byte but get() failed." );
    if( (c & 0xC0) != 0x80 )
        throw string( "Encountered bad following byte: " )+c+".\n";
    return c;
}

string U::encodeUTF8( unsigned int codepoint ) const{
    string encoding;

    if( codepoint < 0x0080 ){
        encoding += static_cast<char>(codepoint);                                   // 0xxxxxxx
    }
    else if( codepoint < 0x0800 ){
        encoding += static_cast<char>( 0xC0 | ( ( codepoint & 0x7C0 ) >> 6 ) );     // xxxxx-xxxxxx -> 0xxxxx
        encoding += static_cast<char>( 0x80 | ( ( codepoint & 0x3F ) ) );
    }
    else if( codepoint < 0x10000 ){
        encoding += static_cast<char>( 0xE0 | ( ( codepoint & 0xF000 ) >> 12 ) );   // xxxx-xxxxxx-xxxxxx -> 0xxxx
        encoding += static_cast<char>( 0x80 | ( ( codepoint & 0xFC0 ) >> 6 ) );
        encoding += static_cast<char>( 0x80 | ( ( codepoint & 0x3F ) ) );
    }
    else{
        encoding += static_cast<char>( 0xF0 | ( ( codepoint & 0x1C000 ) >> 18 ) );  // xxx-xxxxxx-xxxxxxx-xxxxxxx -> 0xxx
        encoding += static_cast<char>( 0x80 | ( ( codepoint & 0x3F000 ) >> 12 ) );
        encoding += static_cast<char>( 0x80 | ( ( codepoint & 0xFC0 ) >> 6 ) );
        encoding += static_cast<char>( 0x80 | ( ( codepoint & 0x3F ) ) );
    }

    return encoding;
}

// Validation must happen before calling this function.
unsigned int U::decodeUTF8( string s ) const{
    unsigned int codepoint = 0;

    switch( s.size() ){
        case 1:
            codepoint |= static_cast<unsigned int>(s[0]);
            break;
        case 2:
            codepoint |= ( s[0] & 0x1F ) << 6;      // xxxxx-000000
            codepoint |= ( s[1] & 0x3F );
            break;
        case 3:
            codepoint |= ( s[0] & 0x0F ) << 12;     // xxxx-000000-000000
            codepoint |= ( s[1] & 0x3F ) << 6;
            codepoint |= ( s[2] & 0x3F );
            break;
        case 4:
            codepoint |= ( s[0] & 0x07 ) << 18;     // xxx-000000-000000-000000
            codepoint |= ( s[1] & 0x3F ) << 12;
            codepoint |= ( s[2] & 0x3F ) << 6;
            codepoint |= ( s[3] & 0x3F );
            break;
        default:
            throw string("Something went wrong decoding");
    }

    return codepoint;
}

U::U(){}
U::U( const string filename ){
    readfile( filename );
}
U::U( const U& o):
    stored_codepoints( o.stored_codepoints )
{}
U& U::operator=( const U& o ){
    stored_codepoints = o.stored_codepoints;
    return *this;
}
U& U::operator=( const string& s ){
    clear();
    append( s );
    return *this;
}
U::~U(){}

// Helper function for readfile/append
void U::processData( iostream& ins ){
    char c;
    string s;
    while( ins.get(c) ){
        s.clear();
        if( ( c & 0x80 ) == 0 ){
            s += c;
            stored_codepoints.push_back( decodeUTF8(s) );
        }
        else if( ( c & 0xE0 ) == 0xC0 ){
            s += c;
            s += add_leading_byte( ins );
            stored_codepoints.push_back( decodeUTF8(s) );
        }
        else if( ( c & 0xF0 ) == 0xE0 ){
            s += c;
            s += add_leading_byte( ins );
            s += add_leading_byte( ins );
            stored_codepoints.push_back( decodeUTF8(s) );
        }
        else if( ( c & 0xF8 ) == 0xF0 ){
            s += c;
            s += add_leading_byte( ins );
            s += add_leading_byte( ins );
            s += add_leading_byte( ins );
            stored_codepoints.push_back( decodeUTF8(s) );
        }
        else
            throw string( "Encountered a poorly formatted leading byte: " )+c+".\n";
    }
}

void U::readfile( const string filename ){
    fstream fin( filename, fstream::in );
    if( !fin.is_open() )
            throw string( "Unable to open file: " )+ filename+".\n";
    processData( fin );
}

void U::append( const string data ){
    stringstream ins( data );
    processData( ins );
}

string U::get() const{
    string s;
    for( auto it = stored_codepoints.begin(); it != stored_codepoints.end(); ++it )
        s += encodeUTF8(*it);
    return s;
}
string U::get( const int index ) const{
    if( index < 0 ||
            index >= static_cast<int>(stored_codepoints.size()) )
        throw string( "Invalid index passed to get(int)\n" );

    return encodeUTF8( stored_codepoints[index] );
}
string U::get( const int begin, const int end ) const{
    if( begin >= end || begin < 0 ||
            end >= static_cast<int>(stored_codepoints.size()) )
        throw string( "Invalid range of arguments for get(start,end)\n" );

    string s;
    for( int i = begin; i < end; i++ )
        s += encodeUTF8( stored_codepoints[i] );
    return s;
}

int U::front() const{
    if(empty())
        throw string( "No codepoints available" );
    return codepoint(0);
}

int U::back() const{
    if(empty())
        throw string( "No codepoints available" );
    return codepoint(size()-1);
}

int U::codepoint( const int index ) const{  
    if( index < 0 ||
            index >= static_cast<int>(stored_codepoints.size()) )
        throw string( "Invalid index passed to codepoint(int)\n" );

    return stored_codepoints[index];
}

int U::size() const{
    return stored_codepoints.size();
}

bool U::empty() const{
    return size() == 0;
}

void U::clear(){
    stored_codepoints.clear();
}

/*
 * ITERATOR METHODS
 */

U::iterator U::begin() const{
    return iterator( this, 0 );
}

U::iterator U::end() const{
    return iterator( this, size() );
}


/*
 *   OPERATORS
 */

U U::operator+( const U& rhs ) const {
    U u = *this;
    u += rhs;
    return u;
}
U U::operator+( const string& rhs ) const {
    U u = *this;
    u += rhs;
    return u;
}
U operator+( const string& lhs, const U& rhs ){ return rhs + lhs; }

U& U::operator+=( const string& s ){
    append( s );
    return *this;
}
U& U::operator+=( const U& u ){
    for( auto it : u.stored_codepoints )
        stored_codepoints.push_back(it);
    return *this;
}

string U::operator[]( int index ) const { return get( index ); }
U::operator bool() const { return !empty(); }

bool U::operator==( const U& rhs ) const{
    return stored_codepoints == rhs.stored_codepoints;
}
bool U::operator==( const string& rhs ) const{
    return get() == rhs;
}
bool U::operator!=( const U& rhs ) const{ return !(*this==rhs); }
bool U::operator!=( const string& rhs ) const{ return !(*this==rhs); }

bool operator==( const string& lhs, const U& rhs ){ return rhs == lhs; }
bool operator!=( const string& lhs, const U& rhs ){ return !(rhs==lhs); }

ostream& operator<<( ostream& os, const U& rhs){
    for( int i = 0; i < rhs.size(); i++ )
        os << rhs[i];
    return os;
}
