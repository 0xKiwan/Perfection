#ifndef _PERFECTION_UTIL_H
#define _PERFECTION_UTIL_H

/*
 * Determine if a given character is whitespace.
 *
 * @param c The character to check.
 * @return true if the character is whitespace, false otherwise.
 */
bool char_is_whitespace(char c);

/*
 * Determine if a given character is alphabetic.
 *
 * @param c The character to check.
 * @return true if the character is alphabetic, false otherwise.
 */
bool char_is_alphabetic( char ch );

/*
 * Determine if a given character is numeric.
 *
 * @param c The character to check.
 * @return true if the character is numeric, false otherwise.
 */
bool char_is_numeric( char ch );

/*
 * Determine if a given character is alphanumeric.
 *
 * @param c The character to check.
 * @return true if the character is alphanumeric, false otherwise.
 */
bool char_is_alphanumeric( char ch );

#endif // _PERFECTION_UTIL_H