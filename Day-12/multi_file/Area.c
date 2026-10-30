#include <stdio.h>

#include "circle.h"
#include "square.h"
#include "rectangle.h"

int main(void)
{
    float radius, side, length, breadth;

    printf("Enter circle radius: ");
    scanf("%f", &radius);

    printf("Enter square side: ");
    scanf("%f", &side);

    printf("Enter rectangle length and breadth: ");
    scanf("%f %f", &length, &breadth);

    printf("\n----- AREA CALCULATION -----\n");

    printf("Area of Circle = %.2f\n",
           circle_area(radius));

    printf("Area of Square = %.2f\n",
           square_area(side));

    printf("Area of Rectangle = %.2f\n",
           rectangle_area(length, breadth));

    return 0;
}
