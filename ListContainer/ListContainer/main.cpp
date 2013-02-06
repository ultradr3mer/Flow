#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "ListContainer.h"

int main(){
	unsigned time;
	int count = 1000000;
	printf("Enter element Count:\n");
	scanf("%d",&count);

	printf("Benchmarking list with %d elements\n",count);

	time=clock();
	ListContainer<float> testCont;
	for (int i = 0; i < count; i++)
	{
		float* content = new float;
		testCont.Add(content);
	}
	time=clock()-time;
	printf("Generating list took %d miliseconds\n",time*1000/CLOCKS_PER_SEC);

	time=clock();
	testCont.InitReader();
	while (testCont.Read())
	{
		float value = (float)rand()/RAND_MAX;
		testCont.SetCurSortValue(value);
	}
	time=clock()-time;
	printf("Setting values took %d miliseconds\n",time*1000/CLOCKS_PER_SEC);

	time=clock();
	testCont.Sort();
	time=clock()-time;

	printf("\n");
	testCont.InitReader();
	int i = 0;
	while (testCont.Read())
	{
		i++;
		printf("%f\n",testCont.GetCurSortValue());

		if(i >= 50)
		{
			printf("...\n");
			break;
		}
	}

	printf("\nSorting took %d miliseconds\n\n",time*1000/CLOCKS_PER_SEC);

	system("PAUSE");
}