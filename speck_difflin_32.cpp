#include <cstdlib>
#include <stdint.h>
#include <iostream>
#include <cmath>
#include <omp.h>
#include <time.h>
using namespace std;
#define ROR(x, r) ((x >> r) | (x << (16 - r)))
#define ROL(x, r) ((x << r) | (x >> (16 - r)))
//#define KE(x, y, l, k) (x = ROR(l, 7), x += y, x ^= k, y = ROL(y, 2), y ^= x)
#define R(x, y, k) (x = ROR(x, 7), x += y, x ^= k, y = ROL(y, 2), y ^= x)
//#define R(x, y) (x = ROR(x, 7), x += y, y = ROL(y, 2), y ^= x)
#define ROUNDS 10
bool maskdot(int mask, int value, int n){
  bool tmp=0;
  for(int i=0; i<n; i++){
    if( ((mask>>i)&0x1) == 1) tmp ^= ((value>>i)&0x1);
  } 
  return tmp;
}


int main(){
  srand(time(NULL));


  for(int exper=0; exper<128;exper++){
    
  int k[11];   //k[T]
        k[0]=rand()&0xffff;
    int l[23]; //keywords l[m+T-2]
	l[0]=rand()&0xffff;
	l[1]=rand()&0xffff;
	l[2]=rand()&0xffff;
  for(int i=0; i<ROUNDS-1; i++){
     l[i+3]=((k[i]+ROR(l[i],7))&0xffff)^i;
     k[i+1]=ROL(k[i],2)^l[i+3];
  }
  unsigned int leftdiff=0x2800;
  unsigned int rightdiff=0x0010;
  unsigned int leftmask=0x3810;
  unsigned int rightmask=0x3010;                                                                       
  long long int counter = 0;

  for(int i=0; i<=0x7fff; i++){
      //#pragma omp parallel for shared(counter) 
  for(int j=0; j<=0xffff; j++){
    uint16_t left = rand()&0xffff; uint16_t leftprime = left ^ leftdiff;
    uint16_t right = rand()&0xffff; uint16_t rightprime = right ^ rightdiff;
    for(int r=1; r<=ROUNDS; r++){
      R(left,right,k[r-1]);
      R(leftprime,rightprime,k[r-1]);
    }
    if( (maskdot(leftmask,(left ^ leftprime),16) ^ maskdot(rightmask,(right ^ rightprime),16)) == 0){
      // #pragma omp atomic 
       counter++;}
    else {
       //#pragma omp atomic
       counter--;}
    
  }}

    //cout << hex << leftdiff << " " << rightdiff << " " << leftmask << " " << rightmask << " ";
        if(counter > 0) cout << "2^" << (log2(counter)-31) << "+";       //correlation
        else cout << "2^" << (log2(-counter)-31) << "+";
    
 }
return 0;
}
