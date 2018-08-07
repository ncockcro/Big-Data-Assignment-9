// Assignment9.c
#include<stdio.h>
#include<string.h>
#include<mpi.h>

double f(double x);
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);

int main() {
int my_rank, comm_sz, n, local_n;
double a = 0, b = 1.0, h, local_a, local_b;
double local_int, total_int;
int source;
int userInput;

MPI_Init(NULL, NULL);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

if(my_rank == 0) {
printf("Enter a number: ");
scanf("%d", &n);
}

h = (b-a)/n; // height of each trapezoid
local_n = n/comm_sz; // number of trapezoids assign to each datanode

local_a = a + my_rank*local_n*h;
local_b = local_a + local_n*h;
local_int = Trap(local_a, local_b, local_n, h);

if(my_rank!=0) {
	MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}
else {
	total_int = local_int;
	for(source = 1; source<comm_sz; source++) {
		MPI_Recv(&local_int,1,MPI_DOUBLE, source, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		total_int = total_int+local_int;
	}
}

if(my_rank == 0) {
	printf("With n=%d our estimate\n",n);
	printf("of the integral from  %f to %f = %.15e",a,b,total_int);
}
MPI_Finalize();
return 0;
}

double f(double x) {
return 4 / (1 + x * x); // Integral of x^2
}

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) { // trapezoidal rule
double estimate, x;
int i;
estimate = (f(left_endpt) + f(right_endpt)) / 2;
for(i = 1; i < trap_count; i++) {
	x = left_endpt+i*base_len;
	estimate += f(x);
}
estimate = estimate * base_len;
return estimate;
}
