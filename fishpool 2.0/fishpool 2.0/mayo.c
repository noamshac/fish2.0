#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <float.h>

typedef struct point {
	int x;
	int y;
	struct point_t* next;
}point_t;

typedef struct pools {
	point_t center;
	int size;
	// int visited;
	struct pools* next;
} pools_t;

typedef struct move {
	point_t CurrentP;
	double TimeSoFar;
	double Oil;
	struct move* first;
	struct move* second;
	
}move_t;

typedef struct twopools {
	pools_t* nearest;
	pools_t* second;
}twopools_t;

point_t* AllocatePoint(point_t*,int);
double Dist(point_t, point_t);
move_t* AllocateMove(move_t*,point_t);
pools_t* AllocatePool(pools_t*, point_t);
twopools_t* FindNearest(move_t*, pools_t*, point_t*);
//void route(move_t*, pools_t*);
void PrintTree(move_t*);

//move_t* BuildBT(move_t*, pools_t*, point_t);

int main()
{
	point_t S;
	S.x = 0;
	S.y = 0;

	point_t p1;
	p1.x = 1;
	p1.y = 1;
	point_t p2;
	p2.x = 4;
	p2.y = 1;
	point_t p3;
	p3.x = 1;
	p3.y = 6;
	point_t p4;
	p4.x = 6;
	p4.y = 5;
	point_t p5;
	p5.x = 9;
	p5.y = 4;
	point_t end;
	end.x = 15;
	end.y = 21;

	

	pools_t* pool1 = AllocatePool(NULL, p1);
	pool1->next = AllocatePool(pool1->next, p2);
	pool1->next->next = AllocatePool(pool1->next->next, p3);
	pool1->next->next->next = AllocatePool(pool1->next->next->next, p4);
	pool1->next->next->next->next = AllocatePool(pool1->next->next->next->next, p5);
	pool1->next->next->next->next->next = AllocatePool(pool1->next->next->next->next->next, end);
//	pool1->next->next->next->next->next->next = AllocatePool(pool1->next->next->next->next->next->next, p1);

	move_t* start = AllocateMove(NULL, S);

	
	
	//route(start, pool1);
	
	int listsize2 = 4;
	point_t* visitedP = AllocatePoint(NULL, listsize2);
	point_t* tempptr = visitedP;
	tempptr->x = p1.x;
	tempptr->y = p1.y;

	tempptr = tempptr->next;
	tempptr->x = p2.x;
	tempptr->y = p2.y;
	tempptr = tempptr->next;
	tempptr->x = p3.x;
	tempptr->y = p3.y;

	
	twopools_t* two ;
	two = FindNearest(start, pool1,visitedP);
	printf_s("the current point is :%d,%d \n", start->CurrentP.x, start->CurrentP.y);
	printf_s("the nearest point is :%d,%d \n", two->nearest->center.x, two->nearest->center.y);
	printf_s("the second nearest point is :%d,%d \n", two->second->center.x, two->second->center.y);
}


// calculate the distance between two given points by pitagoras
double Dist(point_t p1, point_t p2)
{
	double delx = (double)p2.x - p1.x;
	double dely = (double)p2.y - p1.y;
	double dist = sqrt(pow(delx, 2) + pow(dely, 2));
	return dist;
}


//allocate any given number of point nodes recusively 
point_t* AllocatePoint(point_t* ptr,int size)
{
	if (ptr == NULL && size > 0)
	{
		ptr = malloc(sizeof(point_t));
		if (ptr == NULL)return NULL;
		else
		{
			ptr->next = NULL;
			ptr->x = NULL;
			ptr->y = NULL;
			ptr->next = AllocatePoint(ptr->next, size - 1);
		}
	}
	return ptr;
}

//allocate a single move node 
move_t* AllocateMove(move_t* ptr, point_t point)
{
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

// allocate a single pool node
pools_t* AllocatePool(pools_t* ptr, point_t point)
{
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

twopools_t* FindNearest(move_t* CurrentP, pools_t* pools, point_t* visited)
{
	twopools_t* near = (twopools_t*)malloc(sizeof(twopools_t));
	point_t* ptr;
	if (near != NULL)
	{
		near->nearest = NULL; // why not null ---CurrentP
		near->second = NULL;
	}
	double dist1 = DBL_MAX;
	double dist2 = DBL_MAX;
	double temp = 0;
	while (pools != NULL)
	{
		ptr = visited;
		int flag = 1;
		while(ptr != NULL)
		{
			if (ptr->x == pools->center.x && ptr->y == pools->center.y) flag = 0;
			ptr = ptr->next;
		}

		if (flag)
		{
			temp = Dist(CurrentP->CurrentP, pools->center);
			if (temp < dist1)
			{
				dist2 = dist1;
				dist1 = temp;
				near->second = near->nearest;
				near->nearest = pools;
			}
			else if (temp < dist2)
			{
				dist2 = temp;
				near->second = pools;
			}
			
		}
		pools = pools->next;		
	}
	return near;
}
/*
void route(move_t* move, pools_t* pools)
{
	if (pools != NULL&& move!=NULL)
	{
		point_t near1 = FindNearest(move, pools);
	}
}
*/

void PrintTree(move_t* move)
{
	if (move != NULL)
	{
		printf_s("%d ,%d\n", move->CurrentP.x, move->CurrentP.y);
		PrintTree(move->first);
	}
	return;
}

/*
move_t* BuildBT(move_t* CurrentP, pools_t* Pools, point_t End)
{
	// find if reached end of branch 
	
	//check if reched z point

	// find near children

	//check if visited them
	
	//recursive child near

	//recursive child near B

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
