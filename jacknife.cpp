#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;

double square(double m){
  return m*m;
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
 
 //apro file di output
 
 ofstream ofile("jacknifemed.txt");
 if(ofile.fail()){cout << "bestia\n"; return 1;}
 
 //inizio a fare il jackknife
 
 int numjack=10, all=100;
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
   //ofile << jk[ijack] << endl;
 }
 
 //calcolo il valore di aspettazione sui 10 jacknife
 
 double expectation=0.;
 for(int i=0; i<numjack; i+=1){
   expectation+=jk[i];
 }
 expectation/=numjack;
 ofile << expectation << " dovrebbe essere uguale a ";
 
 //calcolo il valore di aspettazione naive
 
 double naivexp=0.;
 for(int i=0; i<all; i+=1){
   naivexp+=col1[i];}
 naivexp/=all;
 ofile << naivexp << endl;
 
 //calcolo l'errore sui jackknife
 
 double scquad=0, err=0, quadsum=0;
 for(i=0; i<numjack; i+=1){
   quadsum+=square(jk[i]);
 }
 scquad=(quadsum/numjack)-square(expectation);
 err=sqrt(scquad*(numjack-1));
 cout << err << " dovrebbe essere uguale a ";
 //calcolo l'errore naive
 
 double naive_scquad=0., naiverr=0., naive_quadsum=0.;
 for(int j=0; j<all; j+=1){
   naive_quadsum+=square(col1[j]);
 }
 naive_scquad=((naive_quadsum/all)-square(naivexp));
 naiverr=sqrt(naive_scquad/(all-1));
 cout << naiverr << endl;
 ofile.close();
 
 //calcolo il quadrato di ogni media jackknife
 
 double fj[all] /*,scol1[all]*/;
 for(int k=0;k<all;k+=1){
   fj[k]=0;
   fj[k]=square(jk[k]);
 }
 
 //calcolo la media degli fj
 
 double f_av=0;
 for(int k=0;k<all;k+=1){
    f_av+=fj[k]/all;
 }
 
 double avsfj=0;
 for(int k=0;k<all;k+=1){
     avsfj+=square(fj[k])/all;
 }
 //double sigma_fj=sqrt((all-1)*(avsfj-square(f_av)));
 //cout << sigma_fj << endl;
 return 0;
}
