#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>
#include <vector>
using namespace std;
#define PI 3.14156
//paramaetri del programma. RICORDA: all non deve essere più piccolo di numjack.
int const all=10000, numjack=10000, numboot=10000;

//funzione che restituisce il quadrato di un double
double square(double m){
  return m*m;
}
//funzione sulla quale viene studiata la propagazione degli erori
double funzione(double m){
  //return exp(100*m);
  return (m-0.501)*(m-0.501)*(m-0.501)*(m-0.501)*(m-0.501)*(m-0.501)*(m-0.501)*(m-0.501);
  //return cos(1000*m);
  //return m*m*m*m*m*m*m*m*m*m*m*m*m;
  //return cos(m);
  //return sqrt(1/m);
}
class  Vecclass{
  vector <double> m_vector;
public:
  Vecclass(int a);
  ~Vecclass();
  void build_datasample();
  void do_jackknife_resampling(const Vecclass &d);
  void do_bootstrap_resampling(const Vecclass &d);
  template <class F>
  void build_fun_Vecclass(const Vecclass &d,F fun);
  double vector_med();
  double vector_stdev();
  template <class F>
  void fun_prop_err(F fun);
};

//costruttore di Vecclass, come argomento devi mettere la dimensione che vuoi assegnare al vettore m_vector
Vecclass::Vecclass(int a){
  m_vector.resize(a);
}

//distruttore
Vecclass::~Vecclass(){};

//funzione che estrae i dati dalla prima colonna di un file di testo per generare il campione di dati iniziale
void Vecclass::build_datasample(void){
  ifstream ifile("tantirandom.txt");
  if(ifile.fail()){cout << "file non aperto, il tuo datasample è vuoto." << endl;}
  string line;
  int j = 0;
  while (getline (ifile, line)){
    stringstream ss;
    ss << line;
    if(ss.fail()){cout << "la riga " << (j+1) << " del file non va bene, verrà saltata."<< endl << line << endl;}
    else if(j<(m_vector.size())){ss >> m_vector[j];}
    j+=1;
  }
  if(!(j==m_vector.size())){cout << "il vettore che scegli come dataset dovrebbe avere " << j << " componenti" << endl;}
  ifile.close();
  }
 
//funzione che crea il resampling jackknife dell'm_vector del Vecclass messo come argomento
void Vecclass::do_jackknife_resampling(const Vecclass &d){
  if(!(m_vector.size()==numjack)){cout << "il vettore che scegli per il ricampionamento jackknife deve avere " << numjack << " componenti" << endl;}
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
    m_vector[ijack]=((tot-sum[ijack])/(all-(all/numjack)));
  }
}
  
//funzione che crea il resampling bootstrap dell'm_vector del Vecclass messo come argomento
void Vecclass::do_bootstrap_resampling(const Vecclass &d){
  srand(time(NULL));
  if(!(d.m_vector.size()==all)){cout << "il ricampionamento lo devi fare sul dataset iniziale, furbone" << endl;}
  if(!(m_vector.size()==numboot)){cout << "il vettore che scegli per il ricampionamento bootstrap deve avere " << numboot << " componenti" << endl;}
  for(int i=0; i<(all*numboot); i+=1){
    m_vector[i/all]+=d.m_vector[rand()%all];
  }
  for(int i=0; i<numboot; i+=1){
    m_vector[i]/=all;
  }
}

//funzione che assegna alle componenti del vettore m_vector di una variabile Vecclass il quadrato delle componenti del vettore m_vector di un altra variabile Vecclass
template <class F>
void Vecclass::build_fun_Vecclass(const Vecclass &d, F fun){
  int a=d.m_vector.size();
  for(int i=0;i<a;i+=1){
    m_vector[i]=fun(d.m_vector[i]);
  }
}


//funzione che restituisce la media degli elementi del vettore m_vector
double Vecclass::vector_med(void){
  double med=0;
  int a=m_vector.size();
  for(int i=0; i<a;i+=1){
    med+=m_vector[i];
  }
  return med/a;
}

//funzione che restituisce la deviazione standard degli elementi del vettore m_vector
double Vecclass::vector_stdev(void){
  int a=m_vector.size();
  double med=0;
  for(int i=0; i<a;i+=1){
    med+=m_vector[i];
  }
  med/=a;
  double s=0;
  for(int i=0;i<a;i+=1){
    s+=square(m_vector[i]);
  }
  s/=a;
  return sqrt((s-square(med)));
}

//funzione che restituisce la propagazione degli errori standard
 template <class F>
 void Vecclass::fun_prop_err(F fun){
   double med=0;
   int a=m_vector.size();
   for(int i=0; i<a;i+=1){
     med+=m_vector[i];
   }
   med=med/a;
   double s=0;
   for(int i=0;i<a;i+=1){
     s+=square(m_vector[i]);
   }
   s/=a;
   s=s-square(med);
   double der_fun = (fun(med+0.000000000001)-fun(med))/0.000000000001;
   med=fun(med);
   cout << "utilizzando le formule di propagazione si ottiene " << med << " +/- " << sqrt(square(der_fun)*s)/sqrt(a-1) << endl;
 }

int main(){
  if(all<numjack){cout << "i parametri del programma sono sbagliati, 'numjack' non può essere più grande di 'all'"; return 0;}
  //genero un elemento di Vecclss con l'apposita funzione
  Vecclass vec_ds(all), vec_jk(numjack), vec_bs(numboot);
  vec_ds.build_datasample();
  vec_jk.do_jackknife_resampling(vec_ds);
  vec_bs.do_bootstrap_resampling(vec_ds);
  
  //medie ed errori nei tre metodi
  cout << "media ed errore naive " << vec_ds.vector_med() << " +/- " << vec_ds.vector_stdev()/sqrt(all -1) << endl;
  cout << "media ed errore jackknife " << vec_jk.vector_med() << " +/- " << vec_jk.vector_stdev()*sqrt(numjack-1) << endl;
  cout << "media ed errore bootstrap " << vec_bs.vector_med() << " +/- " << vec_bs.vector_stdev()*sqrt((double)all/(all-1)) <<endl << endl;
 
  //la funzione che voglio studiare è f(x)=x^2. Faccio il quadrato del dataset iniziale e delle medie sui campioni jackknife e bootstrap
  Vecclass fun_vec_ds(all), fun_vec_jk(numjack), fun_vec_bs(numboot);
  fun_vec_ds.build_fun_Vecclass(vec_ds, funzione);
  fun_vec_jk.build_fun_Vecclass(vec_jk, funzione);
  fun_vec_bs.build_fun_Vecclass(vec_bs, funzione);
  
  //medie ed errori del quadrato nei tre metodi (il primo è quello che presenta il bias troppo grande)
  cout << "media naive del quadrato= " << fun_vec_ds.vector_med() << " +/- " << fun_vec_ds.vector_stdev()/sqrt((double)all-1) << endl
       << "media jackknife del quadrato= " << fun_vec_jk.vector_med() << " +/- " << fun_vec_jk.vector_stdev()*sqrt((double)numjack-1) << endl
       << "media bootstrap del quadrato= " << fun_vec_bs.vector_med() << " +/- " << fun_vec_bs.vector_stdev()*sqrt((double)all/(all-1)) << endl;
  vec_ds.fun_prop_err(funzione);
  
  return 0;
}
