#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>
#include <vector>
using namespace std;

//paramaetri del programma
int const all=100, numjack=100, numboot=500;

//funzione che restituisce il quadrato di un double
double square(double m){
  return m*m;
}


class Vecclass{
  
  vector <double>  m_vector;
  double m_med;
  
public:
  
  //funzione che estrae i dati dalla prima colonna di un file di testo per generare il campione di dati iniziale
  void build_datasample(){
  ifstream ifile("test.txt");
  if(ifile.fail()){cout << "file non aperto, il tuo datasample è vuoto." << endl;}
  string line;
  int j = 1;
  while (getline (ifile, line)){
    stringstream ss;
    ss<<line;
    double a;
    ss >> a;
    if(ss.fail()){cout << "la riga " << j << " del file non va bene, verrà saltata."<< endl << line << endl;}
    else {m_vector.push_back(a);}
    j+=1;
  }
  ifile.close();
  }
  
  //funzione che crea il resampling jackknife dell'm_vector del Vecclass messo come argomento
  void do_jackknife_resampling(const Vecclass &d){
    double sum[numjack];
    for(int k=0; k<numjack; k+=1){
      sum[k]=0;
    }
    for(int isum=0; isum<all; isum+=1){
      sum[isum/(all/numjack)]+=d.m_vector[isum];
    }
    double tot=0;
    for(int t=0; t<numjack; t+=1){tot+=sum[t];}
    for(int ijack=0; ijack<numjack; ijack+=1){
      m_vector.push_back((tot-sum[ijack])/(all-(all/numjack)));
    }
  }
  
  //funzione che crea il resampling bootstrap dell'm_vector del Vecclass messo come argomento
  void do_bootstrap_resampling(const Vecclass &d){
    srand(time(NULL));
    
    double bootsample[numboot]={0};
    for(int i=0; i<(all*numboot); i+=1){
      bootsample[i/all]+=d.m_vector[rand()%100];
    }
    for(int i=0; i<numboot; i+=1){
      bootsample[i]/=all;
    }
    m_vector.assign(bootsample,bootsample+numboot);
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
};

int main(){
 //genero un elemento di Vecclss con l'apposita funzione
  Vecclass vec_ds, vec_jk, vec_bs;
 vec_ds.build_datasample();
 vec_jk.do_jackknife_resampling(vec_ds);
 vec_bs.do_bootstrap_resampling(vec_ds);
 
 //medie ed errori nei tre metodi
 cout << "media ed errore naive " << vec_ds.vector_med() << " +/- " << vec_ds.vector_stdev()/sqrt(all -1) << endl;
 cout << "media ed errore jackknife " << vec_jk.vector_med() << " +/- " << vec_jk.vector_stdev()*sqrt(numjack-1) << endl;
 cout << "media ed errore bootstrap " << vec_bs.vector_med() << " +/- " << vec_bs.vector_stdev()*sqrt((double)all/(all-1)) <<endl << endl;
 
 //la funzione che voglio studiare è f(x)=x^2. Faccio il quadrato del dataset iniziale e delle medie sui campioni jackknife e bootstrap
 Vecclass square_vec_ds, square_vec_jk, square_vec_bs;
 square_vec_ds.build_square_Vecclass(vec_ds);
 square_vec_jk.build_square_Vecclass(vec_jk);
 square_vec_bs.build_square_Vecclass(vec_bs);
 
 //medie ed errori del quadrato nei tre metodi (il primo è quello che presenta il bias troppo grande), l'ultimo è il calcolo con la propagazione degli errori
 cout << "media naive del quadrato= " << square_vec_ds.vector_med() << " +/- " << square_vec_ds.vector_stdev()/sqrt((double)all-1) << endl
      << "media jackknife del quadrato= " << square_vec_jk.vector_med() << " +/- " << square_vec_jk.vector_stdev()*sqrt((double)numjack-1) << endl
      << "media bootstrap del quadrato= " << square_vec_bs.vector_med() << " +/- " << square_vec_bs.vector_stdev()*sqrt((double)all/(all-1)) << endl
      << "quadrato della media e prop. errori = " << square(vec_ds.vector_med()) << " +/- " << 2*vec_ds.vector_med()*vec_ds.vector_stdev()/sqrt((double)all-1) << endl;
 
 return 0;
}
