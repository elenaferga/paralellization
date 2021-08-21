using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <ctime>

int main()
{

  ifstream leer;
  ofstream centros, mpf;
  int i, j;
  double a, aux, cuenta;
  unsigned t0, t1;
  string cadena;
  vector<double> x, y, z, mvir, contador;
  double caja = 400;

  leer.open("MiniUchuu_halolist_z0p001.ascii");
  t0 = clock();
  //Con esto ignoro las 65 primeras filas (header) del ascii
  j = 1;
  while (j != 65)
  {
    getline(leer, cadena);
    j++;
  }

  //Con esto me quedo con las columnas x y z Mvir del ascii y las meto en un vector
  //El contador j tiene en cuenta el número de halos que hay.
  //El contador i va a llevar la cuenta de las columnas para cada uno de los halos. Hay 81 columnas (se enumeran desde i=0 hasta i=80), cuando se
  //llegue a la ultima y pasamos a la siguiente fila, reiniciamos el contador. Así hasta que no se haya acabado de leer el fichero.
  while (!leer.eof())
  {
    for (i = 0; i <= 80; i++)
    {
      leer >> a;
      if (i == 17)
      {
        x.push_back(a);
      }
      else if (i == 18)
      {
        y.push_back(a);
      }
      else if (i == 19)
      {
        z.push_back(a);
      }
      else if (i == 10)
      {
        mvir.push_back(a);
      }
    }
    i = 0;
  }

  double mlimite;

  for (i = 0; i != mvir.size() - 1; i++)
  {
    mlimite = mvir[i];
    cuenta = 0;
  
  #pragma omp parallel for private(j, mvir, mlimite ) shared(cuenta)
    for (j = 0; j != mvir.size() - 1; j++)
    {
      if (mvir[j] >= mlimite)
      {
        cuenta++;
      }
    }
    contador.push_back(cuenta);
  }
  mpf.open("masapdf.txt");
  mpf << "#Masa" << "\t" << "Probabilidad" << "\n";

  for (i = 0; i != mvir.size() - 1; i++)
  {
    mpf << mvir[i] << "\t" << contador[i] / (caja * caja * caja) << "\n";
  }

  return 0;
}