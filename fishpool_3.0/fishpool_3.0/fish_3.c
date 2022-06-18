#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <float.h>
#include <string.h>

typedef struct point {
	int x;
	int y;
	//struct point* next;
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
	int size;
	struct move* next;
}move_t;

typedef struct TwoMoves {
	move_t* nearest;
	move_t* second;
}TwoMoves_t;

move_t* AllocatePath(int);
double Dist(point_t, point_t);
move_t* AllocateMove(move_t*, int, int, double, double, int);
pools_t* AllocatePool(pools_t*, point_t, int);
void FindNearest(move_t*, pools_t*, point_t*, TwoMoves_t*);  /// return was TwoMoves_t* ------
double route(move_t*, move_t*, pools_t*, point_t, double);
void UpdatePath(move_t*, point_t, double, double, int, char);
int HaventVisitAll(point_t*);
void printPath(move_t*);
void PrintTree(move_t*);
//pools_t* read_pools();



int main()
{
	pools_t* pools;
	point_t end;

	// call for function that scans the file to get size and creats a list of pools (returns its start ptr) 
	// end point = (size.x,size.y)
	int bmp_x = 110;
	int bmp_y = 150;
	point_t S;
	printf_s("Please Enter valid x,y start coordinate, bmp width is %d and height is %d\n", bmp_x, bmp_y);
	scanf_s("%d,%d", &S.x, &S.y);
	printf_s("%d , %d", S.x, S.y);
	double oil_s;
	printf_s("Please enter valid oil supply in range 1-1000\n");
	scanf_s("%lf", &oil_s);


	if (1)
	{
		// setting points representing pool centers manually
		point_t p1;
		p1.x = 9;
		p1.y = 27;
		point_t p2;
		p2.x = 100;
		p2.y = 30;
		point_t p3;
		p3.x = 50;
		p3.y = 45;
		point_t p4;
		p4.x = 13;
		p4.y = 59;
		point_t p5;
		p5.x = 100;
		p5.y = 98;
		//point_t end;
		end.x = bmp_x;
		end.y = bmp_y;
		// allocate manually a list of pool centers
		pools = AllocatePool(NULL, p1, 36);
		pools->next = AllocatePool(pools->next, p2, 10);
		pools->next->next = AllocatePool(pools->next->next, p3, 132);
		pools->next->next->next = AllocatePool(pools->next->next->next, p4, 73);
		pools->next->next->next->next = AllocatePool(pools->next->next->next->next, p5, 100);
		//pools->next->next->next->next->next = AllocatePool(pools->next->next->next->next->next, end);
	//	pools->next->next->next->next->next->next = AllocatePool(pools->next->next->next->next->next->next, p1);

	}
	

	// allocate a list with 'n' nodes of move type - n is number of pool centers 
	int listsize2 = 4; //'n'
	move_t* path = AllocatePath(listsize2);
	// update the start move with values from user and best time 
	UpdatePath(path, S, oil_s, 0, -1,'a');
	double best_time= 2000;

	printf_s("main\n\n");
	
	best_time = route(path, path, pools, end, best_time);// declare before 
	
	//printPath(path);
}

/*pools_t* read_pools()
{
	pools_t* pools=NULL;
	FILE* pools_file= NULL;
	char str[50];
	fopen_s(&pools_file, "pools.txt", "rt");
	if (pools_file==NULL)
	{
		printf_s("cant open file");//fix text
		exit;//check if this is write order
	}

	while (fgets(str, 50, pools_file)) {
		for (int i = 0; i < strlen(str); i++) {
			if (str[i] >= '0' && str[i] <= '9') {

				printf_s("%s", str);
			}
		}
	}
	fclose(pools_file);

	return pools;
}*/

// calculate the distance between two given points ** add check for (-1)
double Dist(point_t p1, point_t p2)
{
	double delx = (double)p2.x - p1.x;
	double dely = (double)p2.y - p1.y;
	double dist = sqrt(pow(delx, 2) + pow(dely, 2));
	return dist;
}


//allocate any given number of point nodes  
move_t* AllocatePath(int size)
{

	move_t* head = NULL;
	move_t* ptr = NULL;
	int i;
	for (i = 0; i <= size; i++)
	{
		ptr = malloc(sizeof(move_t));
		if (ptr != NULL)
		{
			ptr->CurrentP.x = -1;
			ptr->CurrentP.y = -1;
			ptr->Oil = -1;
			ptr->TimeSoFar = -1;
			ptr->next = head;
			head = ptr;
		}

	}
	return head;
}


// allocate a single pool node
pools_t* AllocatePool(pools_t* ptr, point_t point, int size)
{
	if (ptr == NULL)
	{
		ptr = malloc(sizeof(pools_t));
		if (ptr != NULL)
		{
			ptr->next = NULL;
			ptr->center = point;
			ptr->size = size;
		}
	}
	return ptr;
}


//allocate a single move node   *** why send the ptrPath
move_t* AllocateMove(move_t* ptr, int x, int y, double oil, double time, int size)
{
	ptr = malloc(sizeof(move_t));
	if (ptr != NULL)
	{
		ptr->next = NULL;
		ptr->CurrentP.x = x;
		ptr->CurrentP.y = y;
		ptr->Oil = oil;
		ptr->TimeSoFar = time;
	}
	return ptr;
}


//
void FindNearest(move_t* CurrentP, pools_t* pools, move_t* path, TwoMoves_t* two) /// return was TwoMoves_t* --------, 
{
	//TwoMoves_t two;

	// will need to free after each call or allocate once and send to func
	pools_t* ptrPools = pools;
	move_t* ptrPath;
	double dist1 = DBL_MAX;
	double dist2 = DBL_MAX;
	double temp = 0;
	// runs over all possible pool points  
	while (ptrPools != NULL)
	{
		ptrPath = path;
		int Not_in_path = 1;
		//checks if the current pool point is inside path
		while (ptrPath != NULL)
		{
			if (ptrPath->CurrentP.x == ptrPools->center.x && ptrPath->CurrentP.y == ptrPools->center.y) Not_in_path = 0;
			ptrPath = ptrPath->next;
		}

		// if not in path allredy
		if (Not_in_path)
		{
			// calc dist
			temp = Dist(CurrentP->CurrentP, ptrPools->center);
			printf_s("\t(%d,%d)------ distance is : %.2lf\n", ptrPools->center.x, ptrPools->center.y, temp);
			printf_s("\n");
			// if this pool is closer than current nearest.point
			// make second->point cell hold the nearest point cords
			//and nearest->point the cords of this pool
			if (temp < dist1)
			{
				dist2 = dist1;
				dist1 = temp;
				two->second->CurrentP = two->nearest->CurrentP;
				two->second->size = two->nearest->size;
				two->nearest->CurrentP = ptrPools->center;
				two->nearest->size = ptrPools->size;
			}
			// if closer only than second update as folowes 
			else if (temp < dist2)
			{
				dist2 = temp;
				two->second->CurrentP = ptrPools->center; //
				two->second->size = ptrPools->size;
			}

		}
		ptrPools = ptrPools->next;
		// if non is found two.nearest and two.second will both hold (-1,-1)
	}
	return;
}


// updates temp point list with a given point
void UpdatePath(move_t* path, point_t point, double oil, double time,int size, char C)
{
	move_t* temp;
	temp = path;
	if (C == 'r')
	{

		// for each point in the list 
		while (temp != NULL)
		{
			//printf_s("%d , %d\t", temp->x == point.x, temp->y == point.y);
			// if its the target point update cords to -1		 not -remove itand allocate a new one
			if (temp->CurrentP.x == point.x && temp->CurrentP.y == point.y)
			{
				temp->CurrentP.x = -1;
				temp->CurrentP.y = -1;
				temp->Oil = -1;
				temp->TimeSoFar = -1;
				return;
			}
			temp = temp->next;     // hafritata ken ovedet
		}
	}
	// adding to path the next node
	else if (C == 'a')
	{
		while (temp != NULL)
		{

			//if there is space add			
			if (temp->CurrentP.x == -1 && temp->CurrentP.y == -1)
			{
				temp->CurrentP.x = point.x;
				temp->CurrentP.y = point.y;
				temp->Oil = oil;
				temp->TimeSoFar = time;
				return;
			}
			temp = temp->next;     // hafritata kanire ovedet
		}

	}
	
	return;
}



double route(move_t* path, move_t* Current, pools_t* pools, point_t end,double best_time)
{

	printf_s("current point is: (%d,%d) \n\n", Current->CurrentP.x, Current->CurrentP.y);

	// if can finish from current point. send end as current move point
	int canFinish = (Current->Oil / 0.2 >= Dist(Current->CurrentP, end)) ? 1 : 0;
	if (canFinish)
	{
		printf_s("finished!\n");
		// maybe update
		//test without a pointer
		move_t last_move;
		last_move.CurrentP = end;
		last_move.Oil = Current->Oil - (Dist(Current->CurrentP, end) * 0.2);
		last_move.TimeSoFar = Current->TimeSoFar + (Dist(Current->CurrentP, end) / 0.2);

		// if time is shorter PRINT
		if (last_move.TimeSoFar <= best_time)
		{
			best_time = last_move.TimeSoFar;
			// write to text file
			// make copy of pools.bmp with path painted 
			//printPath(path);
			// print last move
			printPath(path);
			printf_s("Time=%.2lf (%d,%d) oil=%.2lf\n\n\n", last_move.TimeSoFar, last_move.CurrentP.x, last_move.CurrentP.y, last_move.Oil);
			printf_s("finished! and in shorter time\n");
		}
			
		return best_time;
	}

	// if cant go directly to end find and update in local dynamic twomoves (move->point/NULL)
	TwoMoves_t* two = malloc(sizeof(TwoMoves_t));
	if (two == NULL) {
		printf_s("faild to assign memory");
		exit(1);
	}
	else
	{
		two->nearest = AllocateMove(two->nearest, -1, -1, -1, -1, -1);
		two->second = AllocateMove(two->nearest, -1, -1, -1, -1, -1);
		FindNearest(Current, pools, path, two);

		if (two->nearest->CurrentP.x != -1 && two->second->CurrentP.x != -1)
		{
			printf_s("the two nearest are:\t");
			printf_s("[(%d,%d)", two->nearest->CurrentP.x, two->nearest->CurrentP.y);
			printf_s("(%d,%d)]\n\n", two->second->CurrentP.x, two->second->CurrentP.y);
		}

		// if there is a nearest:	
		if (two->nearest->CurrentP.x != -1)
		{

			double oil;
			double time;

			// check if can reach nearest
			int canreach = (Current->Oil / 0.2 >= Dist(Current->CurrentP, two->nearest->CurrentP)) ? 1 : 0;
			if (canreach)
			{
				// add to path the nearest stats (point,oil,time)
				oil = Current->Oil - (Dist(Current->CurrentP, two->nearest->CurrentP) * 0.2) + (two->nearest->size * 0.2); // add oil from pool
				time = Current->TimeSoFar + (Dist(Current->CurrentP, two->nearest->CurrentP) / 0.2) + (two->nearest->size); // add time extracting
			   //is it necesary to update two. nearest move stats ---- yes cuase it will be the next frame "current"
			   //Current->CurrentP = two->nearest->CurrentP;  dont***
				two->nearest->Oil = oil;
				two->nearest->TimeSoFar = time;
				//
				UpdatePath(path, two->nearest->CurrentP, oil, time, two->nearest->size, 'a');
				printf_s("call for route with nearest\n");
				// call for route after "going" to nearest
				best_time = route(path, two->nearest, pools, end, best_time);
				// back from recursion 1
				printf_s("back from route with nearest\n");
				// removing nearest from path
				UpdatePath(path, two->nearest->CurrentP, -1, -1, -1, 'r');
			}

			//printPath(path);
			//printf_s("current point is: (%d,%d) \n\n", Current->CurrentP.x, Current->CurrentP.y);
			// back from recursion 1

			// if there is a second 
			if (two->second->CurrentP.x != -1)
			{

				//Current->Oil = 200;

				// check if can reach second
				canreach = (Current->Oil / 0.2 >= Dist(Current->CurrentP, two->second->CurrentP)) ? 1 : 0;
				if (canreach)
				{
					oil = Current->Oil - (Dist(Current->CurrentP, two->second->CurrentP) * 0.2) + (two->second->size * 0.2);
					time = Current->TimeSoFar + (Dist(Current->CurrentP, two->second->CurrentP) / 0.2) + (two->nearest->size);
					two->second->Oil = oil;
					two->second->TimeSoFar = time;
					// add to path the current  ****
					UpdatePath(path, two->second->CurrentP, oil, time, two->second->size, 'a');
					printf_s("call for route with nsecond\n");
					// call for route after "going" to second
					best_time = route(path, two->second, pools, end, best_time);
					// back from recursion 2
					printf_s("back from route with second\n");
					// removing second from path
					UpdatePath(path, two->second->CurrentP, -1, -1, -1, 'r');
				}

				else
				{
					printf_s("out of options from second\n");
				}
				//printf_s("current point is: (%d,%d) \n\n", Current->CurrentP.x, Current->CurrentP.y);
			}
			
			return best_time;
			// remove current from path ???
		}
		else
		{
			printf_s("no nearest ");
			printf_s("for point: (%d,%d) \n\n", Current->CurrentP.x, Current->CurrentP.y);
			return best_time;
		}

		free(two->nearest);
		free(two->second);
		free(two);
	}

	return best_time;
	/*else
	{
		printf_s("path is full there are no possible moves\n");
	}*/	
}



// prints the path from the start (can print from any point on but will get only the start)
void printPath(move_t* path)
{
	move_t* temp = path;
	while (temp != NULL && temp->CurrentP.x!=-1)
	{
		
		printf_s(" Time=%.2lf (%d,%d) oil=%.2lf ",temp->TimeSoFar, temp->CurrentP.x, temp->CurrentP.y, temp->Oil);
		temp = temp->next;
		(temp != NULL) ? printf_s("->") : printf_s("\n\n");
	}
}




