#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <float.h>

typedef struct point {
	int x;
	int y;
	
}point_t;

typedef struct pools {
	point_t center;
	int size;
	struct pools* next;
} pools_t;

typedef struct move {
	point_t CurrentP;
	double TimeSoFar;
	double Oil;
	struct move* first;
	struct move* second;
	
}move_t;


double Dist(point_t, point_t);
move_t* AllocateMove(move_t*,point_t);
pools_t* AllocatePool(pools_t*, point_t);
point_t FindNearest(move_t*, pools_t*);

//move_t* BuildBT(move_t*, pools_t*, point_t);

int main()
{
	point_t p1;
	p1.x = 0;
	p1.y = 0;
	point_t p2;
	p2.x = 1;
	p2.y = 1;
	point_t p3;
	p3.x = 4;
	p3.y = 1;
	point_t p4;
	p4.x = 6;
	p4.y = 5;
	point_t p5;
	p5.x = 1;
	p5.y = 6;
	point_t p6;
	p6.x = 15;
	p6.y = 21;

	point_t pr;
	pr.x = 3;
	pr.y = 6;

	pools_t* pool1 = AllocatePool(NULL, p1);
	pool1->next = AllocatePool(pool1->next, p2);
	pool1->next->next = AllocatePool(pool1->next->next, p3);
	pool1->next->next->next = AllocatePool(pool1->next->next->next, p4);
	pool1->next->next->next->next = AllocatePool(pool1->next->next->next->next, p5);
	pool1->next->next->next->next->next = AllocatePool(pool1->next->next->next->next->next, p6);
//	pool1->next->next->next->next->next->next = AllocatePool(pool1->next->next->next->next->next->next, p1);

	move_t* start = AllocateMove(NULL, pr);

	point_t pc;
	pc= FindNearest(start, pool1);
	printf_s("the current point is :%d,%d \n", start->CurrentP.x, start->CurrentP.y);
	printf_s("the nearest point is :%d,%d \n\n", pc.x, pc.y);
	  

}

double Dist(point_t p1, point_t p2)
{
	double delx = (double)p2.x - p1.x;
	double dely = (double)p2.y - p1.y;
	double dist = sqrt(pow(delx, 2) + pow(dely, 2));
	return dist;
}

move_t* AllocateMove(move_t* ptr, point_t point)
{
	//allocate a move node 
	if (ptr == NULL)
	{
		ptr = malloc(sizeof(move_t));
		if (ptr != NULL)
		{
			ptr->first = NULL;
			ptr->second = NULL;
			ptr->CurrentP = point;
		}
	}
	return ptr;
}

pools_t* AllocatePool(pools_t* ptr, point_t point)
{
	//allocate a move node 
	if (ptr == NULL)
	{
		ptr = malloc(sizeof(pools_t));
		if (ptr != NULL)
		{
			ptr->next = NULL;
			ptr->center = point;
		}
	}
	return ptr;
}

point_t FindNearest(move_t* CurrentP, pools_t* pools)
{
	point_t nearest=CurrentP->CurrentP;
	double dist1 = DBL_MAX;
	double temp = 0;
	while (pools != NULL)
	{
		temp = Dist(CurrentP->CurrentP, pools->center);
		if (temp < dist1)
		{
			dist1 = temp;
			nearest = pools->center;
		}
		pools = pools->next;
	}
	return nearest;
}

/*
move_t* BuildBT(move_t* CurrentP, pools_t* Pools, point_t End)
{
	// find if reached end of branch 
	
	//check if reched z point

	// find nearest children

	//check if visited them
	
	//recursive child nearest

	//recursive child nearest B

	if (CurrentP == NULL) // Got to the bottom of the BST - insert child
	{
		CurrentP = AllocateMove(CurrentP);
		if (CurrentP == NULL)
			return NULL;
		else
		{
			CurrentP->first = AllocateMove(NULL);
			CurrentP->second = AllocateMove(NULL);
		}
	}
	else // Locating a place for the new childs
	{

		//find the two closest points and update them to first and second



		//if (root->data > newData)
		//	root->left = InsertDataToBST(root->left, newData);
		//else
		//	root->right = InsertDataToBST(root->right, newData);
	}
	return CurrentP;
}
*/
