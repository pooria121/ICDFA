#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gmp.h>

#define n 3
#define k 2
int f[n]; //int
int s[n*k];
mpz_t max;
mpz_t min;

void init(){
	/* 
	* For using gmp library, all the variables should
	* get initilized before being use.
	* this is the place where we initilize the variables
	*/
	//mpz_array_init(f[0]

	mpz_init(max);
	mpz_init(min);

}

void computeN(mpz_t result, long m, long j){
	if ( m == n-1 ){
		mpz_ui_pow_ui(result, n, k*n-1-j);
		//return (int)pow(n,     k*n-1-j);
	}else{
		mpz_t result1,spare;
		mpz_init_set_ui(result1,0);
		mpz_init_set_ui(spare,0);
		//mpz_long sum = 0;
		mpz_set_ui(result,0);
		for(long i = 0; i<= (m+1) * k - j -2; i++){
			computeN(result1, m+1,j+i+1);
			mpz_set_ui(spare,0);
			mpz_ui_pow_ui(spare,m+1,i);
			mpz_mul(result1,result1,spare);
			mpz_add(result,result1,result);
			//sum += (long)pow((m+1),i) * computeN(m+1,j+i+1);
		}
		//return sum;
		mpz_clear(result1);
		mpz_clear(spare);

	}	
}

void generate(mpz_t m){
	//long ss = 1;
	mpz_t ss,mult;
	mpz_init_set_ui(ss,1);
	mpz_init_set_ui(mult,1);

	mpz_t p,result;
	mpz_init(p);
	mpz_init_set_ui(result,0);
	f[0] = 0;
	for(long i = 1; i < n; i++){
		long j = k * i - 1;
		//int p = (long)pow(i,j-f[i-1]-1);
		mpz_ui_pow_ui(p,i,j-f[i-1]-1);
		computeN(result,i,j); 
		mpz_mul(mult,p,ss); // p * s
		//gmp_printf("p=%Zd,p*ss=%Zd|",p,mult);
		mpz_mul(mult,mult,result); //
		while( j >= i-1 && mpz_cmp(m,mult) >= 0){//m >= p * ss * computeN(i,j)){
			//m = m - computeN(i,j) * p * ss;
			//gmp_printf("m=%Zd,mult=%Zd,result=%Zd,",m,mult,result);
			mpz_sub(m,m,mult);
			j = j - 1;
			//p = p/i;
			 mpz_tdiv_q_ui(p,p,i);
			//gmp_printf("m=%Zd\n",m);
			computeN(result,i,j);
			mpz_mul(mult,p,ss); // p * s
			mpz_mul(mult,mult,result); //

			

		}
		//ss = ss * (long)pow(i,j - f[i-1] - 1);
		mpz_ui_pow_ui(mult,i,j-f[i-1] -1);
		mpz_mul(ss,ss,mult);
		f[i] = j;
	}
	//the rest
	
	long i  = k * n - 1;
	long j  = n-1;

	while(j > 0){
		while(i >= f[j]){
			//s[i] = m %(j+1);
			mpz_tdiv_r_ui(result,m,j+1);
			
			s[i] = mpz_get_si(result);
			//m = m / (j+1);
			mpz_tdiv_q_ui(m,m,j+1);
			i = i -1;
			
		}
		i = i - 1;
		j = j - 1;
	}

	//now we have to apply the flags (sum them up)
	for(i = 0; i < n; i++){
		s[f[i]] = i;
	}

}

int main(){
	init();

	mpz_set_ui(max, 0);
	
	mpz_t spare;
	mpz_init(spare);
	
	for(int i = 0; i < k; i++){
		mpz_set_ui(spare,0);
		computeN(spare,1,i);
		mpz_add(max,max,spare);
	}
	gmp_printf("Maximum Number of ICDFs:%Zd\n",max);
	//mpz_clear(max);
	//mpz_clear(spare);
	
	//int r = computeN(n,k);
	//printf("Maximum number of ICDFs:%ld\n",max);
	srand ( time(NULL) );
	//long chose = 120;// rand() % max;

	gmp_randstate_t state;
	
	mpz_t chose;
	mpz_init(chose);
	gmp_randinit_default (state);
	gmp_randseed_ui(state, (long) time(NULL));
	mpz_urandomm (chose, state ,max);
	//mpz_set_ui(chose,215);
	//mpz_init_set_ui(chose,0);
	gmp_printf("ICDF #=%Zd has been chosen.\n", chose);
	generate(chose);
	printf("n=%d,k=%d \tFlags: ",n,k);
	for(int i = 0; i < n; i++){
		printf("%d ", f[i]);
	}
	printf("\nCanonical string:");
	for(int i = 0; i < n*k; i++){
		printf("%d ",s[i]);
	}
	
	printf("\n");
	
	return 0;
}


