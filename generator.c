#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#define  k  2 //number of alphabets
#define  n  3 //number of states

#define FALSE 0
#define TRUE 1;
int f[n]; //Array to hold flags
int s[k*n]; //ICDFA sequence
int last[k*n];
int final[(int)pow(2,n)][n];


int isfull()
{

	for(int j = 1; j <= n-1; j++){
		//if( ( f[j] + 1 ) <= f[j+1]-1) { 
	    for(int l = f[j]+1; l <= f[j+1]-1; l++){
	
	    	if(s[l] != j) return FALSE;
	    }
	/*}else{
		for(int l = f[j+1]-1; l <= f[j]+1; l++){
			if(s[l]!= j) return FALSE;
		}
	}*/
    	}
	for( int l = f[n-1]; l < k*n; l++){
		if(s[l] != n-1) return FALSE;
	}
    return TRUE;
   
  
 //for (j=1, j<=n-1;j++)
 //   {
 //    for (l=f[j]+1,l<f[j+1]-1; l++)
 //      if (s[l]!=j ) return 0;
 //   }
 
}

int isallzero(){

	int allzero = 1;
	for(int i = 0; i < k*n; i++){
		if(s[i] != 0) allzero = 0;
	}

	return allzero;
}

void decimal_to_binary(int decimal){
	int index = decimal;
	int i = 0;
	do{
	  final[index][i++] =decimal % 2;
	  decimal = decimal /2;

	}while(decimal> 0);

}


void nextflag(int i){
	printf("Creating nextflag....\n");
        if(i == 1){
               f[i] = f[i]-1;
        }else{
                if(f[i] -1 == f[i-1]){
                        f[i] = k * i -1;
                        nextflag(i-1);
                }else{
                    f[i] = f[i]-1 ;
                }
        }
}

void initflag(){
        //intialize the flag
        //f[0] = 0;
        //f[1] = 1;
        //f[2] = 3;
        //f[3] = 5;
	f[0] = 0;
	//final[0] = 0;
	for(int i = 1; i < n; i++){
		f[i] = (k * i) - 1;
		//f[i] = 0;
	}

	for(int i =1; i < (int)pow(2,n); i++){
		decimal_to_binary(i);	
	}
	
}

void printflag(int index){

        for(int i = 0; i < n; i++){
		if( final[index][i] == 1)
	                printf("[%d] ",f[i]);
		else
			printf("%d ", f[i]);	
        }
        printf("\t");
}

void printsequence(){
        for(int i = 0; i < k*n; i++){
                printf("%d ",s[i]);
        }
        //printf("\n");
}

int isFlaged(int j){
        for(int i = 1; i < n; i++){
                if(j == f[i]) return i;
        }
        return -1;
}

void translate(){
	for(int i = 0; i < n; i++){
		for(int j = 0; j<k; j++){
			printf("{state(%d) R(%d) State(%d)} , ",i,j,s[i+j]);
		}
	}
	printf("\n");
}

void nexticdfa(int a, int b, int z){
        int i = a * k + b;//a = 2 , k =2 , b = 1
        if(a < n-1){
                while(isFlaged(i) >= 0){
                        for(int kk = i + 1; kk <= k*n-1; kk++)
                            if(isFlaged(kk)< 0) s[kk] = 0;
                        b = b - 1;
                        i = i - 1;
                }
        }
        
	// f[j] = the nearest flag not exceeding i
        int j;
	j=1;
        for(int x = n-1; x >= 0; x--){
		if(f[x] <= i ){
                	j = x;
                	break;
            	}      
        }

        //printf("\nI'm J:%d, and I'm I:%d, and I'm b:%d \n",j,i,b);
      	if(s[i] == s[f[j]] ){
	        s[i] = 0;
                if(b == 0) nexticdfa(a-1,k-1,z);
	        else  nexticdfa(a,b-1,z);
        }else{
               	s[i] = s[i] + 1;
        }
        
}

void reset(){
    for(int i = 0; i < n*k; i++){
	s[i] = 0;
        //if(isFlaged(i)>= 0) s[i] = isFlaged(i);
    } 
	for(int i = 0; i< n; i++){
		s[f[i]] = i;
	}
}
int islast() {
	int index = 0;
	int i;
	for(i= 1; i <= n-1; i++){
		//printf("%d ", i);
		if(s[index] != i) return FALSE;
		index++;
	}
	for(i = k-1; i <= (k-1) * (n+1); i++){
		//printf("%d ", n-1);
		if(s[index] != n-1) return FALSE;
		index++;
	}
	//printf("\n");
	return TRUE;
}
int Ngenerator(int m, int j){
	

}
int main(){
	int i  = 1;
        initflag();
	reset();
	
	for(int f = 0; f < (int)pow(2,n); f++){
		printsequence();
		printf("\t +- \t");
		printflag(f);
		printf("(%d) n=%d, k=%d\n",i,n,k);
		i++;
	}
	
	//for(int j = 0; j < 2; j++){
	do{
		do{
			nexticdfa(n-1,k-1,i);
			for(int f = 0; f < (int)pow(2,n); f++){
				printsequence();
				printf("\t +- \t");
				printflag(f);
				printf("(%d) n=%d, k=%d\t\n",i,n,k);
				//translate();
		        	i++;
			}
		}while(!isfull() && !islast());
	
		if(islast()) break;
	
		nextflag(n-1);
		reset();

		for(int f = 0; f < (int)pow(2,n); f++){
			printsequence();
			printf("\t* - \t");
			printflag(f);
			printf("(%d) n=%d, k=%d\t\n",i,n,k);
			//translate();
			i++;
		}
		
	}while(1);
	
        
	
	// This is the code to generate all the 2^n possible choice of
	// final states
	for(int i = 0; i < (int)pow(2,n) ; i++){
		printf("%d)",i);
		for(int j = 0; j < n; j++){
			printf("%d ",final[i][j]);
		}
		printf("\n");
	}
	
	//printf("%d\n",(int)pow(2,3));
        return 0;
}



