#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;


void printhead(){

  cout << "(set-logic QF_ABV)" << endl;
  cout << "(set-info :smt-lib-version 2.5)" << endl;
  //cout << "(set-option :produce-models true)" <<endl;
}
void declare(int r){
  for(int i=1; i<=r; i++){
      cout << "(declare-fun a_" << i << " () (_ BitVec 16)) " ;
      cout << "(declare-fun b_" << i << " () (_ BitVec 16)) " ;
      cout << "(declare-fun c_" << i << " () (_ BitVec 16)) " ;
      cout << "(declare-fun d_" << i << " () (_ BitVec 16)) " ;
      cout << "(declare-fun z_" << i << " () (_ BitVec 16)) " ;
  }
  cout << "(declare-fun a_" << r+1 << " () (_ BitVec 16)) " ;
  cout << "(declare-fun b_" << r+1 << " () (_ BitVec 16)) " ;  
  cout << "(declare-fun Weight () (_ BitVec 16)) " ; 
  cout << endl;
  //define hamming weight
  cout <<"(define-fun w_H0 ((x (_ BitVec 16))) (_ BitVec 16)"<<endl;
  cout <<"      (bvadd (bvand x #x5555 )"<<endl;
  cout <<"             (bvand (bvlshr x #x0001) #x5555 )))"<<endl;
  cout <<"(define-fun w_H1 ((x (_ BitVec 16))) (_ BitVec 16)"<<endl;
  cout <<"      (bvadd (bvand x #x3333 )"<<endl;
  cout <<"             (bvand (bvlshr x #x0002) #x3333 )))"<<endl;
  cout <<"(define-fun w_H2 ((x (_ BitVec 16))) (_ BitVec 16)"<<endl;
  cout <<"      (bvadd (bvand x #x0f0f )"<<endl;
  cout <<"             (bvand (bvlshr x #x0004) #x0f0f)))"<<endl;
  cout <<"(define-fun w_H3 ((x (_ BitVec 16))) (_ BitVec 16)"<<endl;
  cout <<"      (bvadd (bvand x #x00ff )"<<endl;
  cout <<"             (bvand (bvlshr x #x0008) #x00ff )))"<<endl;
  cout <<"(define-fun w_H ((x (_ BitVec 16))) (_ BitVec 16) (w_H3 (w_H2 (w_H1 (w_H0 x)))))"<<endl;
  }

void cczformula(int r){
  for(int i=1; i<=r; i++){
    cout << "(assert (= ((_ extract 15 15)z_"<<i<<") (_ bv0 1)))" << endl;
  }
  for(int i=1; i<=r; i++){
    cout << "(assert (= z_"<<i<<" (bvxor (bvlshr (bvxor ((_ rotate_right 7) a_"<<i<<") (bvxor c_"<<i<<" d_"<<i<<")) #x0001) (bvlshr z_"<<i<<" #x0001))))" << endl;
  }
  for(int i=1; i<=r; i++){
    for(int j=0; j<=15; j++){
       cout << "(assert (= (bvxor ((_ rotate_right 7) a_"<<i<<") d_"<<i<<") (bvand (bvxor ((_ rotate_right 7) a_"<<i<<") d_"<<i<<") z_"<<i<<")))" << endl;
    }
    for(int j=0; j<=15; j++){
       cout << "(assert (= (bvxor c_"<<i<<" d_"<<i<<") (bvand (bvxor c_"<<i<<" d_"<<i<<") z_"<<i<<")))" << endl;
    }
  }
}

void linearlayer(int r){
  for(int i=1; i<=r; i++){
    cout << "(assert (= ((_ rotate_right 2) b_"<<i+1<<") (bvxor b_"<<i<<" c_"<<i<<")))" <<endl;
    cout << "(assert (= d_"<<i<<" (bvxor b_"<<i+1<<" a_"<<i+1<<")))" <<endl;
  }
}

void objectivefun(int r, int bound){
  cout << "(assert (= ";
  for(int i=1; i<=r; i++){
    if(i<r)  cout << "(bvadd (w_H z_"<<i<<") ";
    else cout << "(w_H z_"<<r<<")";
  }
  for(int i=1; i<r; i++){
    cout << ")";
  }
  cout <<" (_ bv"<<bound<<" 16)))"<<endl;
}

void tail(int r){
  cout << "(check-sat)" <<endl;
  cout << "(exit)" <<endl;

}


int main(int argc, char** argv){

int ROUND = atoi(argv[1]);
int BOUND = atoi(argv[2]);
printhead();
declare(ROUND);
cczformula(ROUND);
linearlayer(ROUND);
objectivefun(ROUND,BOUND);
tail(ROUND);
return 0;
}
