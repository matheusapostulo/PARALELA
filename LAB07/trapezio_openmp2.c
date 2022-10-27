#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double Local_trap(double a, double b, int n);

double f(double x){
    return exp(x);
}

int main(int argc, char* argv[])
{
    double global_result = 0.0;
    double a,b;
    int n;
    int thread_count;
    
    thread_count = strtol(argv[1], NULL, 10);
    printf("Entre com o valor a, b e n\n");
    scanf("%lf %lf %d", &a, &b, &n);
    
    #pragma omp parallel num_threads(thread_count)
    {
        double my_result = 0.0;
        my_result += Local_trap(a, b, n);
        
	int my_rank = omp_get_thread_num();
        int thread_count = omp_get_num_threads();

        printf("Com n = %d trapezios, nossa estimativa\n", n);
        printf("Da integral de %f para %f = %.14e\n", a , b, my_result);

	printf("THREAD %d executou de um total de %d THREADS\n\n",my_rank, thread_count);
      
        #pragma omp critical
        { 
          global_result += my_result;
        }
    }
    
    printf("\nResultado final = %.14e\n", global_result);
}

double Local_trap(double a, double b, int n){
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    
    h = (b-a)/n;
    local_n = n/thread_count;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    my_result = (f(local_a) + f(local_b))/2.0;
    for( i = 1; i<= local_n-1; i++){
        x = local_a + i*h;
        my_result += f(x);
    }
    
    my_result = my_result*h;
    
    return my_result;
}
