/* Find the last occurrence of a character */

#include <stdio.h>
#include "strings.h"

char *index(str,ch)
char *str, ch;

{
	int i, length = strlen(str);

/*
	for ( i=0; i < length; i++ )
*/
	for ( i=length-1; i >= 0; i-- )

		if ( *(str + i) == ch )
			return ( str + i );

	return (NULL);

}
