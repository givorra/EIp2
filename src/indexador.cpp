#include <iostream> 
#include <string>
#include <list> 
#include "indexadorHash.h"
#include "indexadorInformacion.h"
#include <sys/resource.h>
using namespace std;

///////////////////////////////////////////
// Igual que el indexador04.cpp pero almacenando información en disco duro
///////////////////////////////////////////



double getcputime(void) {
    struct timeval tim;
    struct rusage ru;

    getrusage(RUSAGE_SELF, &ru);

    tim=ru.ru_utime;
    double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
    tim=ru.ru_stime;
    t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
    return t;
}

//main
int main() {

    long double aa=getcputime();

    IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 1, false, true);

    a.IndexarDirectorio("corpus");
    cout << a <<"\n";

    cout << "Ha tardado " << getcputime() - aa << " segundos" << endl;

}
