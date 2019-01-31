#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;

//funzione che restituisce il quadrato di un double

double square(double m){
  return m*m;
}

//funzione che restituisce il puntatore ad un array i cui elementi sono date dal quadrato degli elementi dell'array inserito come argomento

double square_arr[]={0};
void array_square(int a, double c[]){;
  for (int i=0; i<a; i+=1){
   square_arr[i]=square(c[i]);
  }
}

//funzione che restituisce la media degli elementi dell'array inserito come secondo argomento; l'intero da inserire come primo argomento è il numero degli elementi dell'array

double med(int a, double c[]){
  double s=0;
  for(int i=0; i<a; i+=1){
    s+=c[i];
  }
  return s/ a;
}

//funzione che restituisce la deviazione standard degli elementi dell'array inserito come secondo argomento; l'intero da inserire come primo argomento è il numero degli elementi dell'array

double stdev(int a, double c[]){
  double s=0;
  for(int i=0; i<a; i+=1){
    s+=square(c[i]);
  }
  s/=a;
  return sqrt((s-square(med(a,c))));
}
//parametri del programma
int const all=100, numjack=100;

int main(){
  
  //genero l'array prendendo i dati dalla prima colonna di un file di input
  
  double col1[all];
  ifstream ifile("test.txt");
  if(ifile.fail()){cout << "file non aperto\n"; return 1;}
  //i dati della prima riga siano messi come componenti 0 dei miei vettori.
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
  if(ofile.fail()){cout << "file non aperto\n"; return 1;}
  
  //media ed errore naive
  double naive_med, naive_err;
  naive_med=med(all,col1);
  naive_err=stdev(all,col1);
  //cout << naive_med  << " " << naive_err << endl;
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
 double jk_med, jk_err;
 jk_med=med(numjack,jk);
 jk_err=stdev(numjack,jk)*sqrt(numjack-1);
 //cout << jk_med << " " << jk_err << endl;
 
 //la funzione che voglio studiare è f(x)=x^2. Costruisco un array dato dal quadrato di col1 ed uno dato dal quadrato di jk
 
 double scol1[all], sjk[numjack];
 array_square(all, col1);
 for(int i=0; i<all; i+=1){
   scol1[i] =square_arr[i];
 }
 array_square(numjack, jk);
 for(int i=0; i<numjack; i+=1){
   sjk[i] =square_arr[i];
 }
 
 //dal seguente cout si vede che la media fatta con il resampling jacknife approssima meglio <x>^2 rispetto alla media sui valori originali al quadrato,
 //quest'ultima, infatti, presenta un bias che non si riduce per N-->infty; quella ottenuta con il resampling, invece, per N grande si avvicina sempre più
 //ad f(X) +/- err(f(X)) dove l'erore è calcolato con la formula di propagazione degli errori.
 
 cout << "media naive = " << med(all, scol1) << " +/- " << stdev(all,scol1)/sqrt((double)all-1) << " questo è il calcolo di media ed errore sul quadrato dei dati iniziali" << endl
      << "media jackknife = " << med(numjack, sjk) << " +/- " << stdev(numjack,sjk)*sqrt((double)numjack-1) << " questo è calcolato con il metodo jackknife" << endl
      << "f(X) = " << square(naive_med) << " +/- " << 2*naive_med*naive_err/sqrt((double)all-1) << " questo è calcolato con la propagazione degli errori" << endl;
 
  return 0;
}
