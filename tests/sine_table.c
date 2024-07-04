#include <stdio.h>
#include <math.h>
int main(void)
{
int angle_degree;
double angle_radian, pi, value;
printf("Compute a table of the sine function\n\n");
pi = 4.0*atan(1.0); /* could also just use pi = M_PI */
printf("Value of PI = %f \n\n", pi);
printf("Angle\tSine\n");
angle_degree = 0;/* initial angle value */
while (angle_degree <= 360) { /* loop til angle_degree > 360 */
angle_radian = pi * angle_degree / 180.0;
value = sin(angle_radian);
printf ("%3d\t%f\n ", angle_degree, value);
angle_degree += 10; /* increment the loop index */
}
return 0;
}

//need to run gcc -o sine test.c -lm to compile