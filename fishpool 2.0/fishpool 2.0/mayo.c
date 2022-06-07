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
move_t* AllocateMove(move_t*);
point_t FindNearest(move_t*, pools_t*);

//move_t* BuildBT(move_t*, pools_t*, point_t);

int main()
{
	pools_t* pool1 = (pools_t*)malloc(sizeof(pools_t));
	if (pool1 == NULL) return(-1);
	pool1->center.x = 1;
	pool1->center.y = 1;
	printf_s("pool1 is %d ,%d \n", pool1->center.x, pool1->center.y);
	pool1->next = (pools_t*)malloc(sizeof(pools_t));
	if (pool1->next != NULL) exit;
	pool1->next->center.x = 4;
	pool1->next->center.y = 1;
	pool1->next->next = (pools_t*)malloc(sizeof(pools_t));
	if (pool1->next->next != NULL) exit;
	pool1->next->next->center.x = 6;
	pool1->next->next->center.y = 5;
	pool1->next->next->next = (pools_t*)malloc(sizeof(pools_t));
	if (pool1->next->next->next != NULL) exit;
	pool1->next->next->next->center.x = 1;
	pool1->next->next->next->center.y = 6;
	pool1->next->next->next->next = (pools_t*)malloc(sizeof(pools_t));
	if (pool1->next->next->next->next != NULL)exit;
	pool1->next->next->next->next->center.x = 15;
	pool1->next->next->next->next->center.y = 21;
	pool1->next->next->next->next->next = NULL;
	
	

	move_t* start = (move_t*)malloc(sizeof(move_t));
	start->CurrentP.x = 8;
	start->CurrentP.y = 5;

	point_t p1;
	p1= FindNearest(start, pool1);
	printf_s("the current point is :%d,%d \n", start->CurrentP.x, start->CurrentP.y);
	printf_s("the nearest point is :%d,%d \n\n", p1.x, p1.y);
	pool1 = pool1->next;
	
	
	  

}

double Dist(point_t p1, point_t p2)
{
	double delx = (double)p2.x - p1.x;
	double dely = (double)p2.y - p1.y;
	double dist = sqrt(pow(delx, 2) + pow(dely, 2));
	return dist;
}

move_t* AllocateMove(move_t* ptr)
{
	//allocate a move node 
	if (ptr == NULL)
	{
		ptr= malloc(sizeof(move_t));
		ptr->first = NULL;
		ptr->second = NULL;
	}
	return ptr;
	
}

point_t FindNearest(move_t* CurrentP, pools_t* pools)
{
	point_t nearest;
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
