#include "../inc/util.h"

// Implementation for util.h char_is_whitespace
bool char_is_whitespace(char c) 
{
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

// Implementation for util.h char_is_alphabetic
bool char_is_alphabetic( char ch )
{
	return ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' );
}

// Implementation for util.h char_is_numeric
bool char_is_numeric( char ch )
{
	return ch >= '0' && ch <= '9';
}

// Implementation for util.h char_is_alphanumeric
bool char_is_alphanumeric( char ch )
{
    return ch_is_alphabetic( ch ) || ch_is_numeric( ch );
}