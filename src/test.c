#include <stdio.h>

int GCD(int a , int b)
{ 
	if(a%b==0) 
		return b; 
	else 
		return GCD(b,a%b); 
}

int main(void)
{
	
	FILE *fp;
	fp = fopen("result.txt","a");
	int i,j=0;
	for(i=1;i<200;i++)
	{
		for(j=1;j<200;j++)
		{
			printf("i=%d,j=%d\n",i,j);
			fprintf(fp, "gcd(%d,%d)=%d\n", i, j, GCD(i,j));			
		}	
	}
	fclose(fp);
	return 0;
}