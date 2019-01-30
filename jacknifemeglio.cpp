#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;

double square(double m){
  return m*m;
}

double med(int a, double c[]){
  double s=0;
  for(int i=0; i<a; i+=1){
    s+=c[i];
  }
  return s/ a;
}

double stdev(int a, double c[]){
  double s=0;
  for(int i=0; i<a; i+=1){
    s+=square(c[i]);
  }
  s/=a;
  return sqrt((s-square(med(a,c))));
}


int main(){
  double col1[100];
  //double col2[100];
  //double col3[100];
  ifstream ifile("test.txt");
  // controllo se il file è stato aperto correttamente
  if(ifile.fail()){cout << "bestia\n"; return 1;}
  // adesso voglio che i dati della prima riga siano messi come componenti 0 dei miei vettori.
  string line;
  int i = 0;
  int j = 1;
  while (getline (ifile, line)){
    stringstream ss;
    ss<<line;
    double a;
    ss >> a;
    if(ss.fail()){
      cout << "la riga " << j << " del file non va bene\n";
      cout << line << "\n";
    }
    else{
      col1[i]=a;
      
      i+=1;
    }
    j+=1;
  }
  ifile.close();
  
  int const all=100, numjack=10;
  
  //apro file di output
  ofstream ofile("jacknifemed.txt");
  if(ofile.fail()){cout << "bestia\n"; return 1;}
  
  //media ed errore naive
  
  cout << med(all, col1) << " " << stdev(all, col1)/sqrt(all-1) << endl;
  
  //faccio il jackknife resampling
  
  double jk[numjack], sum[numjack];
 for(int k=0; k<numjack; k+=1){
   jk[k]=0;
   sum[k]=0;
 }
 for(int isum=0; isum<all; isum+=1){
   sum[isum/(all/numjack)]+=col1[isum];
 }
 double tot=0;
 for(int t=0; t<numjack; t+=1){tot+=sum[t];}
 for(int ijack=0; ijack<numjack; ijack+=1){
   jk[ijack]=(tot-sum[ijack])/(all-(all/numjack));
 }
 
 //calcolo la media e l'errore sui nuovi campioni
 
 cout << med(numjack,jk) << " " << (stdev(numjack,jk)*sqrt(numjack-1)) << endl;
 
  return 0;
}
