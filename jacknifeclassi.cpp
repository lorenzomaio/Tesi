#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>
#include <vector>
using namespace std;

//funzione che restituisce il quadrato di un double
double square(double m){
  return m*m;
}


class Vecclass{
  
  vector <double>  m_vector;
  double m_med;
  
public:
  //funzione che assegna alle componenti del vettore m_vector di una variabile Vecclass le componenti di un array inserito come argomento.
  void build_Vecclass(int a, double c[]){
    for(int i=0;i<a;i+=1){
      m_vector.assign(c,c+a);
    }
  }
  //funzione che assegna alle componenti del vettore m_vector di una variabile Vecclass il quadrato delle componenti del vettore m_vector di un altra variabile Vecclass
    void build_square_Vecclass(const Vecclass &d){
    int a=d.m_vector.size();
    double b[a];
    for(int i=0;i<a;i+=1){
      b[i]=square(d.m_vector[i]);
    }
    m_vector.assign(b,b+a);
   }
  //funzione che restituisce la media degli elementi del vettore m_vector
   double vector_med(){
    m_med=0;
    int a=m_vector.size();
    for(int i=0; i<a;i+=1){
      m_med+=m_vector[i];
    }
    m_med/=a;
    return m_med;
   }
  //funzione che restituisce la deviazione standard degli elementi del vettore m_vector
  double vector_stdev(){
    int a=m_vector.size();
    double s=0;
    for(int i=0;i<a;i+=1){
      s+=square(m_vector[i]);
    }
    s/=a;
    return sqrt((s-square(m_med)));
  }
  
  void print_elements(){
    int a=10;
    for(int i=0; i<a; i+=1){
      cout << m_vector[i] << " ";
      }
    cout << endl;
  }
};

//parametri del programma
int const all=100, numjack=10; /*, numboot=500;*/

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
  
  //genero un elemento della classe Vecclass con l'apposita funzione
  Vecclass vec_col1;
  vec_col1.build_Vecclass(all, col1);
  
  //media ed errore naive
  cout << "media ed errore naive " << vec_col1.vector_med() << " +/- " << vec_col1.vector_stdev()/sqrt(all -1) << endl;
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
 
 //genero un elemento di Vecclss con l'apposita funzione
 Vecclass vec_jk;
 vec_jk.build_Vecclass(numjack, jk);
 
 
 //calcolo la media e l'errore sui nuovi campioni
 cout << "media ed errore naive " << vec_jk.vector_med() << " +/- " << vec_jk.vector_stdev()*sqrt(numjack-1) << endl;
 //cout << jk_med << " " << jk_err << endl;
 
 //la funzione che voglio studiare è f(x)=x^2. Costruisco un array dato dal quadrato di col1 ed uno dato dal quadrato di jk
 Vecclass square_vec_col1, square_vec_jk;
 square_vec_col1.build_square_Vecclass(vec_col1);
 square_vec_jk.build_square_Vecclass(vec_jk);
 
 //dal seguente cout si vede che la media fatta con il resampling jacknife approssima meglio <x>^2 rispetto alla media sui valori originali al quadrato,
 //quest'ultima, infatti, presenta un bias che non si riduce per N-->infty; quella ottenuta con il resampling, invece, per N grande si avvicina sempre più
 //ad f(X) +/- err(f(X)) dove l'erore è calcolato con la formula di propagazione degli errori.
 
 cout << "media naive del quadrato= " << square_vec_col1.vector_med() << " +/- " << square_vec_col1.vector_stdev()/sqrt((double)all-1) /*<< " questo è il calcolo di media ed errore sul quadrato dei dati iniziali"*/ << endl
      << "media jackknife del quadrato= " << square_vec_jk.vector_med() << " +/- " << square_vec_jk.vector_stdev()*sqrt((double)numjack-1) /*<< " questo è calcolato con il metodo jackknife" */<< endl;
   //      << "f(X) = " << square(naive_med) << " +/- " << 2*naive_med*naive_err/sqrt((double)all-1) << " questo è calcolato con la propagazione degli errori" << endl;
 
 //faccio il bootstrap resampling, come prima cosa devo creare dei nuovi campioni estraendo randomicamente elementi dall'array col1
 // srand(time(NULL));
 //  double bootsamples[numboot][all];
 // for(i=0;i<all*numboot;i+=1){
 //   bootsamples[i/all][i%all]=col1[rand()%100];
 // }
 // for(i=0;i<105;i+=1){cout << i/100  << " ";}
 // cout << endl;
 return 0;
}
