
#include "indexadorInformacion.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <algorithm>
//#include <cctype>

using namespace std;


//******************__CLASE INFORMACION TERMINO__*********************CLOSE

InformacionTermino::InformacionTermino(const InformacionTermino &i)
{

	if (this != &i) {
			this->ftc = i.ftc;
			this->l_docs = i.l_docs;
		}
}

InformacionTermino::InformacionTermino()
{
	ftc = 0;
	this->l_docs.clear();
}

InformacionTermino::~InformacionTermino()
{
	ftc = 0;
	this->l_docs.clear();
}

InformacionTermino& InformacionTermino::operator= (const InformacionTermino& other)
{
	if(this != &other)
	{
		this->ftc = other.ftc;
		this->l_docs=other.l_docs;
	}

    return *this;
}

//VA
ostream& operator<<(ostream& s, const InformacionTermino &p) {

	unordered_map<long int, InfTermDoc>::const_iterator i;
	s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
	for ( i = p.l_docs.begin(); i != p.l_docs.end(); i++)
		s << "\t" << "Id.Doc: " << i->first << '\t' << i->second;

	return s;
}
//----- metodos propios ------

InfTermDoc* InformacionTermino::obtenerDoc(long int idDoc)
{
	//cout << idDoc << endl;
	//cout << "LLEGO AQUI "<< endl;
	//cout << "esto es lo que recibo por parametro : " << idDoc << endl;
	//listarLdocs();
	//std::unordered_map<long int,InfTermDoc>::iterator it = l_docs.find(idDoc);
	auto it = l_docs.find(idDoc);
	//cout << "LLEGO AQUI TAMBIEN "<< endl;
	if(it != l_docs.end())
	{
		return &(it->second);
	}
	return NULL;
}

long int InformacionTermino::consultarIdDocumento(InfTermDoc term)
{
	for(auto& x : l_docs)
			if(x.second == term)
				return x.first;
}

bool InformacionTermino::addDoc(const long int& id,const InfTermDoc& infTermDoc)
{
	if(existe(id))
	{
		return false;
	}else
	{
		l_docs[id] = infTermDoc;
		numTerminos++;
		return true;
	}
}

bool InformacionTermino::eraseDoc(const long int& id)
{
	return(l_docs.erase(id) != 0);
}

bool InformacionTermino::existe(const long int & id)
{
	return (l_docs.find(id) != l_docs.end());
}

unordered_map<long int, InfTermDoc> InformacionTermino::getColeccionDocumentos() const
{
	return l_docs;
}

int InformacionTermino::tamColeccionDocs()const
{
	return l_docs.size();
}

int InformacionTermino::getFtc() const
{
	return ftc;
}

void InformacionTermino::setFtc(int ftc)
{
	this->ftc = ftc;
}

void InformacionTermino::incrementarFtc()
{
	this->ftc++;
}



//******************__CLASE FECHA__*********************CLOSE

Fecha::Fecha() {


}

Fecha::Fecha(int dia, int mes, int anyo, int hora, int min, int seg) {
	this->dia = dia;
	this->mes = mes;
	this->anyo = anyo;
	this->hora = dia;
	this->minuto = dia;
	this->segundo = dia;

}

Fecha::~Fecha() {
	dia = mes = anyo = hora = minuto = segundo = 0;
}

bool Fecha::operator==(const Fecha& f) const {
   return dia==f.dia && mes==f.mes && anyo==f.anyo &&
          hora==f.hora && minuto==f.minuto && segundo==f.segundo;
}

bool Fecha::operator <(const Fecha& f)const {
	return anyo<f.anyo ||
	          (anyo==f.anyo && mes<f.mes) ||
	          (anyo==f.anyo && mes==f.mes && dia<f.dia) ||
	          (anyo==f.anyo && mes==f.mes && dia==f.dia && hora<f.hora) ||
	          (anyo==f.anyo && mes==f.mes && dia==f.dia && hora==f.hora && minuto<f.minuto) ||
	          (anyo==f.anyo && mes==f.mes && dia==f.dia && hora==f.hora && minuto==f.minuto && segundo<f.segundo);
}

bool Fecha::operator<=(const Fecha& f) const {
   return *this<f || *this==f;
}

Fecha::Fecha(struct tm *clock){
	anyo = clock->tm_year;
	mes = clock->tm_mon;
	dia = clock->tm_mday;
	hora = clock->tm_hour;
	minuto = clock->tm_min;
	segundo = clock->tm_sec;
}

ostream&
operator<<(ostream &s, const Fecha &p){
	s << p.dia << '/' << p.mes << '/' << p.anyo
			<< ' ' << p.hora << ':' << p.minuto<< ':' << p.segundo;
	return s;
}
//******************__CLASE INF TERM DOC__*************************CLOSE

InfTermDoc::InfTermDoc()
{
	ft = 0;
}

InfTermDoc::~InfTermDoc()
{
	this->ft = 0;
	this->posTerm.clear();
}

InfTermDoc::InfTermDoc(const InfTermDoc &o)
{
	ft = o.ft;
	posTerm = o.posTerm;
}

InfTermDoc& InfTermDoc::operator= (const InfTermDoc& other)
{
	if(this != &other){
		this->~InfTermDoc();
		this->ft = other.ft;
		this->posTerm=other.posTerm;
	}
    return (*this);
}

bool InfTermDoc::operator ==(const InfTermDoc &p) const
{
    return this->ft == p.ft && this->posTerm == p.posTerm;
}

ostream& operator<<(ostream& s, const InfTermDoc& p) {

	s << "ft: " << p.ft << "\t";
		for (list<int>::const_iterator i = p.posTerm.begin(); i != p.posTerm.end();
				i++) {
			if (i == p.posTerm.begin())
				s << *i;
			else
				s << "\t" << *i;
		}
		//s << endl;
		return s;
}

//----- metodos propios ------


void InfTermDoc::almacenarposTerm(int pos)
{
	posTerm.push_back(pos);
}

int InfTermDoc::getFt()const
{
	return ft;
}

void InfTermDoc::setFt(int nft)
{
	this->ft = nft;
}

void InfTermDoc::incrementarFt()
{
	this->ft++;
}

list<int> InfTermDoc::getPosTerm()const
{
	return posTerm;
}

void InfTermDoc::setPosTerm(list <int> posTermNew)
{
	posTerm = posTermNew;
}


//******************__CLASE INF DOC__*************************

//Se muestra la fecha?????
ostream& operator<<(ostream& s, const InfDoc& p) {
	s << "idDoc: " << p.getidDoc() << "\tnumPal: " << p.getNumPal() << "\tnumPalSinParada: " << p.getNumPalSinParada() << "\tnumPalDiferentes: " << p.getNumPalDiferentes() << "\ttamBytes: " << p.getTamBytes();
	return s;
}

InfDoc::InfDoc()
{
	idDoc = 0;
	numPal = 0;
	numPalSinParada = 0;
	numPalDiferentes = 0;
	tamBytes = 0;
	//falta la fecha
}

InfDoc::InfDoc(const InfDoc &d) : fechaModificacion(d.fechaModificacion)
{
	this->idDoc = d.idDoc;
	this->numPal = d.numPal;
	this->numPalSinParada = d.numPalSinParada;
	this->numPalDiferentes = d.numPalDiferentes;
	this->tamBytes = d.tamBytes;
}

InfDoc::~InfDoc()
{
	idDoc = 0;
	numPal = 0;
	numPalSinParada = 0;
	numPalDiferentes = 0;
	tamBytes = 0;

}

InfDoc& InfDoc::operator= (const InfDoc& other)
{
	if(this != &other)
	{
		this->~InfDoc();

		idDoc = other.idDoc;
		numPal = other.numPal;
		numPalSinParada = other.numPalSinParada;
		numPalDiferentes = other.numPalDiferentes;
		tamBytes = other.tamBytes;
		fechaModificacion = other.fechaModificacion;
	}
    return (*this);
}

//----- metodos propios ------

long int InfDoc::getidDoc()const
{
	return idDoc;
}

int InfDoc::getNumPal()const
{
	return numPal;
}

int InfDoc::getNumPalSinParada()const
{
	return numPalSinParada;
}

int InfDoc::getNumPalDiferentes()const
{
	return numPalDiferentes;
}

int InfDoc::getTamBytes()const
{
	return tamBytes;
}

Fecha InfDoc::getFechaModificacion()const
{
	return fechaModificacion;
}

void InfDoc::setNumPal(int n)
{
	this->numPal = n;
}

void InfDoc::setNumPalSinParada(int n)
{
	this->numPalSinParada = n;
}

void InfDoc::setNumPalDiferentes(int n)
{
	this->numPalDiferentes = n;
}

void InfDoc::setTamBytes(int nuevoTam)
{
	this->tamBytes = nuevoTam;
}

void InfDoc::setIdDoc(long int n)
{
	idDoc = n;
}

void InfDoc::setFecha(Fecha f)
{
	fechaModificacion = f;
}
void InfDoc::incrementarNumPal()
{
	this->numPal++;
}

void InfDoc::incrementarNumPalSinParada()
{
	this->numPalSinParada++;
}

void InfDoc::incrementarNumPalDiferentes()
{
	this->numPalDiferentes++;
}


//******************__CLASE INF COLECCION DOCS__*************************
//Sacar fecha???
ostream& operator<<(ostream& s, const InfColeccionDocs& p){
s << "numDocs: " << p.getNumDocs() << "\tnumTotalPal: " << p.getNumTotalPal() << "\tnumTotalPalSinParada: " << p.getNumTotalPalSinParada() << "\tnumTotalPalDiferentes: " << p.getNumTotalPalDiferentes() << "\ttamBytes: " << p.getTamBytes();
	return s;
}

InfColeccionDocs::InfColeccionDocs()
{
	numDocs = 0;
	numTotalPal = 0;
	numTotalPalSinParada = 0;
	numTotalPalDiferentes = 0;
	tamBytes = 0;
}

InfColeccionDocs::InfColeccionDocs(const InfColeccionDocs &other)
{
	numDocs = other.numDocs;
	numTotalPal = other.numTotalPal;
	numTotalPalSinParada = other.numTotalPalSinParada;
	numTotalPalDiferentes = other.numTotalPalDiferentes;
	tamBytes = other.tamBytes;
}

InfColeccionDocs::~InfColeccionDocs()
{
	numDocs = 0;
	numTotalPal = 0;
	numTotalPalSinParada = 0;
	numTotalPalDiferentes = 0;
	tamBytes = 0;
}

InfColeccionDocs& InfColeccionDocs::operator= (const InfColeccionDocs& other)
{
	if(this != &other)
	{
		this->~InfColeccionDocs();
		numDocs = other.numDocs;
		numTotalPal = other.numTotalPal;
		numTotalPalSinParada = other.numTotalPalSinParada;
		numTotalPalDiferentes = other.numTotalPalDiferentes;
		tamBytes = other.tamBytes;
	}
    return (*this);
}

//----- metodos propios ------

long int InfColeccionDocs::getNumDocs()const
{
	return numDocs;
}

long int InfColeccionDocs::getNumTotalPal()const
{
	return numTotalPal;
}

long int InfColeccionDocs::getNumTotalPalSinParada()const
{
	return numTotalPalSinParada;
}

long int InfColeccionDocs::getNumTotalPalDiferentes()const
{
	return numTotalPalDiferentes;
}

long int InfColeccionDocs::getTamBytes()const
{
	return tamBytes;
}

void InfColeccionDocs::setNumDocs(long int n)
{
	this->numDocs = n;
}

void InfColeccionDocs::setNumTotalPal(long int n)
{
	this->numTotalPal = n;
}

void InfColeccionDocs::setNumTotalPalSinParada(long int n)
{
	this->numTotalPalSinParada = n;
}

void InfColeccionDocs::setNumTotalPalDiferentes(long int n)
{
	this->numTotalPalDiferentes = n;
}

void InfColeccionDocs::setTamBytes(long int n)
{
	this->tamBytes = n;
}

void InfColeccionDocs::incrementarNumDocs()
{
	this->numDocs++;
}

void InfColeccionDocs::incrementarNumTotalPal()
{
	this->numTotalPal++;
}

void InfColeccionDocs::incrementarNumTotalPalSinParada()
{
	this->numTotalPalSinParada++;
}

void InfColeccionDocs::incrementarNumTotalPalDiferentes()
{
	this->numTotalPalDiferentes++;
}

void InfColeccionDocs::incrementarTamBytes(long int n)
{
	this->tamBytes = tamBytes + n;
}

void InfColeccionDocs::decrementarNumDocs()
{
	numDocs = numDocs-1;;
}

void InfColeccionDocs::decrementarNumTotalPalDiferentes()
{
	numTotalPalDiferentes = numTotalPalDiferentes-1;
}


//******************__CLASE INFORMACION TERMINO PREGUNTA__*************************
//REPASO DIFERENTE
ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
	s << "ft: " << p.getFt();
	// A continuación se mostrarían todos los elementos de p.posTerm (“posicion TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB”): s << “\t“ << posicion;

	for(int posicion: p.posTerm)
	{
		s << "\t" << posicion;
	}

	return s;
}

InformacionTerminoPregunta::InformacionTerminoPregunta()
{
	ft =0;
}

InformacionTerminoPregunta::InformacionTerminoPregunta(const InformacionTerminoPregunta&t)
{
	ft = t.ft;
	posTerm = t.posTerm;
}

InformacionTerminoPregunta::~InformacionTerminoPregunta()
{
	this->ft = 0;
	this->posTerm.clear();
}

InformacionTerminoPregunta& InformacionTerminoPregunta::operator= (const InformacionTerminoPregunta& other)
{
	if(this != &other)
		this->~InformacionTerminoPregunta();

		ft = other.ft;
		posTerm = other.posTerm;
    return (*this);
}

//----- metodos propios ------implementado

int InformacionTerminoPregunta::getFt()const
{
	return ft;
}

list<int> InformacionTerminoPregunta::getPosTerm()const
{
	return posTerm;

}

void InformacionTerminoPregunta::setFt(int ft)
{
	this->ft = ft;
}

void InformacionTerminoPregunta::incrementarFt()
{
	this->ft++;
}

void InformacionTerminoPregunta::almacenarposTerm(int pos)
{
	posTerm.push_back(pos);
}


//******************__CLASE INFORMACION PREGUNTA__*************************

InformacionPregunta::InformacionPregunta()
{
	numTotalPal = 0;
	numTotalPalDiferentes = 0;
	numTotalPalSinParada = 0;
}

InformacionPregunta::InformacionPregunta(const InformacionPregunta &i)
{
	numTotalPal = i.numTotalPal;
	numTotalPalDiferentes = i.numTotalPalDiferentes;
	numTotalPalSinParada = i.numTotalPalSinParada;
}

InformacionPregunta::~InformacionPregunta()
{
	numTotalPal = 0;
	numTotalPalDiferentes = 0;
	numTotalPalSinParada = 0;
}

InformacionPregunta& InformacionPregunta::operator= (const InformacionPregunta& other)
{
	if(this != &other)
	{
		numTotalPal = other.numTotalPal;
		numTotalPalDiferentes = other.numTotalPalDiferentes;
		numTotalPalSinParada = other.numTotalPalSinParada;
	}
    return (*this);
}

ostream& operator<<(ostream& s, const InformacionPregunta& p){
	s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: " << p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
	return s;
}

//----- metodos propios ------


long int InformacionPregunta::getNumTotalPal()const
{
	return numTotalPal;
}

long int InformacionPregunta::getNumTotalPalSinParada()const
{
	return numTotalPalSinParada;
}

long int InformacionPregunta::getNumTotalPalDiferentes()const
{
	return numTotalPalDiferentes;
}

void InformacionPregunta::incrementarNumTotalPal()
{
	numTotalPal++;
}

void InformacionPregunta::incrementarNumTotalPalSinParada()
{
	numTotalPalSinParada++;
}

void InformacionPregunta::incrementarNumTotalPalDiferentes()
{
	numTotalPalDiferentes++;
}

void InformacionPregunta::setNumTotalPal(long int n)
{
	this->numTotalPal = n;
}

void InformacionPregunta::setNumTotalPalSinParada(long int n)
{
	this->numTotalPalSinParada = n;
}

void InformacionPregunta::setNumTotalPalDiferentes(long int n)
{
	this->numTotalPalDiferentes = n;
}
