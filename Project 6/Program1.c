/*Dhruv Koul
  EECS 110 - Program 6
  Edited: 1/28/2012
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
    double x11 = 0, x12 = 0, x13 = 0, x14 = 0, x15 = -1.5, x16 = 0, y11 = 0, y12 = 0, y13 = 0, y14 = 0, y15 = -2.3, y16 = 0;
    double x21 = 4, x22 = 5, x23 = 0, x24 = 0, x25 = 2.4, x26 = 5, y21 = 0, y22 = 0, y23 = 5, y24 = 5, y25 = -0.5, y26 = 0;
    double x31 = 4, x32 = 2.5, x33 = 2.5, x34 = -2.5, x35 = 0.5, x36 = 2.5, y31 = 3, y32 = 2.5, y33 = 2.5, y34 = 2.5, y35 = 2.5, y36 = 0;
    double ans1 = 0, ans2 = 0, ans3 = 0, ans4 = 0, ans5 = 0, ans6 = 0;

    //calculate each of the distances for the points that exist in the table in the instruction sheet
    ans1 = distance(x11, y11, x21, y21);
    ans2 = distance(x12, y12, x22, y22);
    ans3 = distance(x13, y13, x23, y23);
    ans4 = distance(x14, y14, x24, y24);
    ans5 = distance(x15, y15, x25, y25);
    ans6 = distance(x16, y16, x26, y26);

    //distance table
    printf(" Point 1\t  Point2\t  Distance\n");
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x11, y11, x21, y21, ans1);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x12, y12, x22, y22, ans2);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x13, y13, x23, y23, ans3);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x14, y14, x24, y24, ans4);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x15, y15, x25, y25, ans5);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x16, y16, x26, y26, ans6);
    printf("\n\n");


    //calculate each of the perimeters for the triangles
    ans1 = tri_perimeter(x11, y11, x21, y21, x31, y31);
    ans2 = tri_perimeter(x12, y12, x22, y22, x32, y32);
    ans3 = tri_perimeter(x13, y13, x23, y23, x33, y33);
    ans4 = tri_perimeter(x14, y14, x24, y24, x34, y34);
    ans5 = tri_perimeter(x15, y15, x25, y25, x35, y35);
    ans6 = tri_perimeter(x16, y16, x26, y26, x36, y36);

    //perimeter table
    printf(" Point 1\t  Point 2\t  Point 3\t  Perimeter\n");
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x11, y11, x21, y21, x31, y31, ans1);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x12, y12, x22, y22, x32, y32, ans2);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x13, y13, x23, y23, x33, y33, ans3);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x14, y14, x24, y24, x34, y34, ans4);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x15, y15, x25, y25, x35, y35, ans5);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x16, y16, x26, y26, x36, y36, ans6);
    printf("\n\n");


    //calculate each of the areas for the triangles
    ans1 = tri_area(x11, y11, x21, y21, x31, y31);
    ans2 = tri_area(x12, y12, x22, y22, x32, y32);
    ans3 = tri_area(x13, y13, x23, y23, x33, y33);
    ans4 = tri_area(x14, y14, x24, y24, x34, y34);
    ans5 = tri_area(x15, y15, x25, y25, x35, y35);
    ans6 = tri_area(x16, y16, x26, y26, x36, y36);

    //perimeter table
    printf(" Point 1\t  Point 2\t  Point 3\t   Area\n");
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x11, y11, x21, y21, x31, y31, ans1);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x12, y12, x22, y22, x32, y32, ans2);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x13, y13, x23, y23, x33, y33, ans3);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x14, y14, x24, y24, x34, y34, ans4);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x15, y15, x25, y25, x35, y35, ans5);
    printf("(%.1f, %.1f)\t(%.1f, %.1f)\t(%.1f, %.1f)\t   %.2f\n", x16, y16, x26, y26, x36, y36, ans6);
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
