



// A C program to find convex hull of a set of points
// Refer http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// for explanation of orientation()
#include <stdio.h>

// Define Infinite (Using INT_MAX caused overflow problems)
#define INF 10000

//TODO Point struct

typedef struct {
    double x;
    double y;
} Point;

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0)
        return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// Prints convex hull of a set of n points.
void convexHull(Point points[], int n)
{
    // Initialize
    int next[n];
    int i = 0;
    int l = 0;
    int p,q;
    // There must be at least 3 points
    if (n < 3)
        return;
    
    // Initialize Result
    for (; i < n; i++)
        next[i] = -1;
    
    // Find the leftmost point
    int left = -1;
    Point temp;
    for (i = 0; i < n; i++){
        
        if (left == -1){
            temp = points[i];
        }
        
        else if (points[i].x < left){
            temp = points[i];
            left = points[i].x;
        }
    }
    
    p = l;
    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again
    // TODO
    
    
    //    while (orientation == 0)
    Point start = temp;
    int start_ind = 0;
    for (i = 0; i < n; i++){
        if (points[i].x == start.x){
            start_ind = i;
            break;
            
        }
        
        
    }
    
        
        

    // Print Result
    for (i = 0; i < n ; i++)
    {
        printf("%lf , %lf\n",points[i].x,points[i].y);
        
        
        if (next[i] != -1){
            printf("%lf , %lf\n",points[i].x,points[i].y);
        }
            //cout << "(" << points[i].x << ", " << points[i].y << ")\n";
    }
}


// Driver program to test above functions
int main()
{
    Point points[] = { { 0.09, 0.42 }, { 0.19, 0.19 }, { 0.18, 0.81 }, { 0.24, 0.57 }, { 0.33, 0.62 },
            { 0.39, 0.79 }, { 0.50, 1.00 } , { 0.50, 0.40 }, { 0.60, 0.60 } , { 0.62, 0.18 }, { 0.70, 0.35 } , { 0.95, 0.59 },
            { 1.00, 0.77 }  , { 0.94, 0.76 }   };
    //cout << "The points in the convex hull are: \n";
    int n = sizeof(points) / sizeof(points[0]);
    convexHull(points, n);
    return 0;
}
