#include "myCompare.h"

#include <string.h>
#include <stdlib.h>

/* Quick and dirty swap of the address of 2 arrays of unsigned int
*/
void swap( unsigned **first, unsigned **second)
{
	unsigned *temp;
	temp=*first;
	*first=*second;
	*second=temp;
}
/* A function which returns how similar 2 strings are
   Assumes that both point to 2 valid null terminated array of chars.
   Returns the similarity between them.
*/

double strCompare(char *str1,char *str2)
{
	double c1,c2;
	c1=strCompare_side1(str1,str2);
	c2=strCompare_side1(str2,str1);

	return (c1+c2)/2;
}


double strCompare_side1( char *str1, char *str2)
{
	size_t len1=strlen(str1), len2=strlen(str2);
	float lenLCS;
	unsigned j, k, *previous, *next;
	if( len1==0 || len2==0)
		return 0;
	previous=(unsigned *)calloc( len1+1, sizeof(unsigned));
	next=(unsigned *)calloc( len1+1, sizeof(unsigned));
	for(j=0; j<len2; ++j){
		for(k=1; k<=len1; ++k)
			if( str1[k-1] == str2[j])
				next[k]=previous[k-1]+1;
			else next[k]=previous[k]>=next[k-1]?previous[k]:next[k-1];
		swap( &previous, &next);
	}
	lenLCS=(float)previous[len1];
	free(previous);
	free(next);
	return lenLCS=lenLCS/len1*100;
}