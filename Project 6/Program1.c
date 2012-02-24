/*Dhruv Koul
  EECS 110 - Program 6
  Edited: 2/23/2012 (for revisions)
*/

/*This is the combined triangle.c file with all of the functions previously defined in exercises 3 through 5. This
file contains the distance, tri_perimeter and tri_area functions that we defined, and in the main function we call
all of those functions and print results in those tables as requested.*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>   //need this library for the square root function

double distance(double x1, double y1, double x2, double y2);    //required function prototype
double tri_perimeter(double x1, double y1, double x2, double y2, double x3, double y3);     //required function prototype
double tri_area(double x1, double y1, double x2, double y2, double x3, double y3);      //required function prototype

int main()
{
    //define the variables that will be used and printed to the table.
    double x1[6] = {0, 0, 0, 0, -1.5, 0};
    double x2[6] = {4, 5, 0, 0, 2.4, 5};
    double x3[6] = {4, 2.5, 2.5, -2.5, 0.5, 2.5};
    double y1[6] = {0, 0, 0, 0, -2.3, 0};
    double y2[6] = {0, 0, 5, 5, -0.5, 0};
    double y3[6] = {3, 2.5, 2.5, 2.5, 2.5, 0};
    double ans[6] = {0, 0, 0, 0, 0, 0};

    int i = 0;
    while (i < 6)
    {
        ans[i] = distance(x1[i], y1[i], x2[i], y2[i]);
        i++;
    }

    //distance table
    printf(" Point 1\t  Point2\t  Distance\n");
    i = 0;
    while (i < 6)
    {
        printf("(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x1[i], y1[i], x2[i], y2[i], ans[i]);
        i++;
    }
    printf("\n\n");


    //calculate each of the perimeters for the triangles
    i = 0;
    while (i < 6)
    {
        ans[i] = tri_perimeter(x1[i], y1[i], x2[i], y2[i], x3[i], y3[i]);
        i++;
    }


    //perimeter table
    printf(" Point 1\t  Point 2\t  Point 3\t  Perimeter\n");
    i = 0;
    while (i < 6)
    {
        printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x1[i], y1[i], x2[i], y2[i], x3[i], y3[i], ans[i]);
        i++;
    }
    printf("\n\n");


    //calculate each of the areas for the triangles
    i = 0;
    while (i < 6)
    {
        ans[i] = tri_area(x1[i], y1[i], x2[i], y2[i], x3[i], y3[i]);
        i++;
    }


    //perimeter table
    printf(" Point 1\t  Point 2\t  Point 3\t   Area\n");
    i = 0;
    while (i < 6)
    {
        printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x1[i], y1[i], x2[i], y2[i], x3[i], y3[i], ans[i]);
        i++;
    }
    printf("\n");


    return 0;		//exit program
}

double distance(double x1, double y1, double x2, double y2)
{
    //calculate the change in x and change in y
    double differenceX = x2 - x1;
    double differenceY = y2 - y1;

    //calculate (change in x)^2 and (change in y)^2
    double squareX = differenceX * differenceX;
    double squareY = differenceY * differenceY;

    //distance = squareroot of (squared change in x + squared change in y
    double dist = sqrt(squareX + squareY);

    return dist;    //return the distance as the value for this function call.
}

double tri_perimeter(double x1, double y1, double x2, double y2, double x3, double y3)
{
    //find the length of each side of the triangle using the previous distance formula from exercise 3
    double dist1 = distance(x1, y1, x2, y2);
    double dist2 = distance(x2, y2, x3, y3);
    double dist3 = distance(x1, y1, x3, y3);

    //calculate and return the perimeter of the triangle
    double perimeter = dist1 + dist2 + dist3;
    return perimeter;
}

double tri_area(double x1, double y1, double x2, double y2, double x3, double y3)
{
    double length1 = distance(x1, y1, x2, y2);
    double length2 = distance(x2, y2, x3, y3);
    double length3 = distance(x1, y1, x3, y3);
    double half_perimeter = (length1 + length2 + length3) / 2;

    double area = 0;
    area = sqrt(half_perimeter * (half_perimeter - length1) * (half_perimeter - length2) * (half_perimeter - length3));
    return area;
}
