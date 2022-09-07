#include <stdio.h>
#include <sys/random.h>
#include <stdlib.h>
#include <string.h>

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


int fillrand(float * arr, int N){
    int r;
    unsigned int max=4294967295;
    unsigned int tmp;
    for(int i=0;i<N;i++){
        r=getrandom(&tmp, sizeof(unsigned int), GRND_NONBLOCK);
        if(r!=4){
            printf("random returned something other than 4: %d\n",r);
        }
        arr[i] = (float)tmp/max;
    }
    return 0;
}

int fillLCR(float * arr, int N, int seed){
    int a=63, c=319, m=65537;
    int x[N];

    x[0]=seed;
    arr[0]=(float)x[0]/(m-1);
    for(int i=1;i<N;i++){
        //note largest can get before modulo is a*m+c, still less than 32bit integer supports
        x[i]  = (a*x[i-1]+c) % m;
        arr[i]=(float)x[i]/(m-1);
    }

    printf("%f\n", arr[0]);
    return 0;
}

int stats(float* arr,int N,float* covar, int nrange, char* name){

    char* a_name=concat(name,".arr");
    FILE * a_fp=fopen(a_name,"w");
    free(a_name);
    fwrite(arr, sizeof(float), N, a_fp);
    fclose(a_fp);

    float prev[nrange];
    for(int n=0; n<nrange; n++){
        prev[n] = 0.;
        covar[n] = 0.;
    }
    float now;
    float average=0.;
    for(int i=0; i<N; i++){
        now = arr[i];
        average += now;
        // in (nrange-1, nrange-2, ... 3, 2, 1)
        for(int n=nrange-1; n>0; n--){
            prev[n] = prev[n-1];
            covar[n] += now*prev[n];
        }
        prev[0] = now;
        covar[0] += now*prev[0];
    }

    /*
    prev[9]=prev[8]
    covar[9] +=now*prev[9]
    prev[8]=prev[7]
    covar[8] +=now*prev[8]
    prev[7]=prev[6]
    covar[7] +=now*prev[7]
    prev[6]=prev[5]
    covar[6] +=now*prev[6]
    ...
    prev[2]=prev[1]
    covar[2] +=now*prev[2]
    prev[1]=prev[0]
    covar[1] +=now*prev[1]
    prev[0] = now
    covar[0] += now*prev[0]
    */

    average = average/N;
    printf("average for %s is %f\n", name, average);
    for(int n=0; n<nrange; n++){
        covar[n] = (covar[n]/(N-n)) - average*average;
    }

    char* c_name=concat(name,".covar");
    FILE * c_fp=fopen(c_name,"w");
    free(c_name);
    fwrite(covar, sizeof(float), nrange, c_fp);
    fclose(a_fp);

    return 0;
}


int main(){
    int N = 1000000;  // num of random numbers in each sample/array
    int nrange = 10; // nrange-1 is highest n we will calculate "A(n)" for (covariance)

    int seed1 = 11111;
    int seed2 = 33333;
    int seed3 = 55555;

    float arr[N];
    float covar[nrange];

    fillrand(arr, N);        stats(arr,N,covar,nrange,"good1");
    fillrand(arr, N);        stats(arr,N,covar,nrange,"good2");
    fillrand(arr, N);        stats(arr,N,covar,nrange,"good3");
    fillLCR(arr, N, seed1);  stats(arr,N,covar,nrange,"lcr1");
    fillLCR(arr, N, seed2);  stats(arr,N,covar,nrange,"lcr2");
    fillLCR(arr, N, seed3);  stats(arr,N,covar,nrange,"lcr3");

    return 0;
}
