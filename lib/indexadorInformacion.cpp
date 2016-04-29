/*
 * indexadorInformacion.cpp
 *
 *  Created on: 6 de abr. de 2016
 *      Author: gacel
 */

#include "indexadorInformacion.h"

Fecha::Fecha()
{
	dia = 0;
	mes = 0;
	anyo = 0;
	hora = 0;
	min = 0;
	seg = 0;
}

Fecha::Fecha(struct tm *clock)
{
	dia = clock->tm_mday;
	mes = clock->tm_mon;
	anyo = clock->tm_year;
	hora = clock->tm_hour;
	min = clock->tm_min;
	seg = clock->tm_sec;
}

bool Fecha::operator<(const Fecha& f)const {
	return anyo<f.anyo ||
  (anyo==f.anyo && mes<f.mes) ||
  (anyo==f.anyo && mes==f.mes && dia<f.dia) ||
  (anyo==f.anyo && mes==f.mes && dia==f.dia && hora<f.hora) ||
  (anyo==f.anyo && mes==f.mes && dia==f.dia && hora==f.hora && min<f.min) ||
  (anyo==f.anyo && mes==f.mes && dia==f.dia && hora==f.hora && min==f.min && seg<f.seg);
}
Fecha& Fecha::operator=(const Fecha& f)
{
	if(this != &f)
	{
		dia 	= f.dia;
		mes 	= f.mes;
		anyo 	= f.anyo;
		hora 	= f.hora;
		min 	= f.min;
		seg 	= f.seg;
	}
	return (*this);
}
/************************************** InformacionPregunta *******************************************/

InformacionPregunta::InformacionPregunta (const InformacionPregunta &ip):
numTotalPal(ip.numTotalPal),
numTotalPalDiferentes(ip.numTotalPalDiferentes),
numTotalPalSinParada(ip.numTotalPalSinParada)
{}

InformacionPregunta::InformacionPregunta():
numTotalPal(0),
numTotalPalDiferentes(0),
numTotalPalSinParada(0)
{}
InformacionPregunta::~InformacionPregunta ()
{
	numTotalPal 			= 0;
	numTotalPalDiferentes 	= 0;
	numTotalPalSinParada 	= 0;
}

InformacionPregunta& InformacionPregunta::operator=(const InformacionPregunta &ip)
{
	if(this != &ip)
	{
		this->numTotalPal			= ip.numTotalPal;
		this->numTotalPalDiferentes = ip.numTotalPalDiferentes;
		this->numTotalPalSinParada	= ip.numTotalPalSinParada;
	}
	return (*this);
}

ostream& operator<<(ostream& s, const InformacionPregunta& p)
{
	s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "<< "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
	return s;
}

/********************************* InformacionTerminoPregunta ****************************************/

InformacionTerminoPregunta::InformacionTerminoPregunta (const InformacionTerminoPregunta &itp):
ft(itp.ft)
{}

InformacionTerminoPregunta::InformacionTerminoPregunta ():
ft(0)
{}

InformacionTerminoPregunta::~InformacionTerminoPregunta ()
{
	ft = 0;
	posTerm.clear();
}
InformacionTerminoPregunta& InformacionTerminoPregunta::operator=(const InformacionTerminoPregunta& itp)
{
	if(this != &itp)
	{
		this->ft 		= itp.ft;
		this->posTerm = itp.posTerm;
	}
	return (*this);
}

ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
	s << "ft: " << p.ft;
	// A continuación se mostrarían todos los elementos de p.posTerm (“posicion TAB posicion TAB ... posicion,
	// es decir nunca finalizará en un TAB”): s << “\t“ << posicion;
	return s;
}

/***************************************** InfColeccionDocs ******************************************/


InfColeccionDocs::InfColeccionDocs():
numDocs(0),
numTotalPal (0),
numTotalPalSinParada(0),
numTotalPalDiferentes(0),
tamBytes(0)
{}

InfColeccionDocs::InfColeccionDocs(const InfColeccionDocs &icd):
numDocs(icd.numDocs),
numTotalPal (icd.numTotalPal),
numTotalPalSinParada(icd.numTotalPalSinParada),
numTotalPalDiferentes(icd.numTotalPalDiferentes),
tamBytes(icd.tamBytes)
{}

InfColeccionDocs::~InfColeccionDocs()
{
	numDocs = 0;
	numTotalPal = 0;
	numTotalPalSinParada = 0;
	numTotalPalDiferentes = 0;
	tamBytes = 0;
}

InfColeccionDocs& InfColeccionDocs::operator=(const InfColeccionDocs& icd)
{
	if(this != &icd)
	{
		numDocs 				= icd.numDocs;
		numTotalPal 			= icd.numTotalPal;
		numTotalPalSinParada 	= icd.numTotalPalSinParada;
		numTotalPalDiferentes 	= icd.numTotalPalDiferentes;
		tamBytes 				= icd.tamBytes;
	}
    return (*this);
}


ostream& operator<<(ostream& s, const InfColeccionDocs& p)
{
	s << "numDocs: " << p.numDocs << "\tnumTotalPal: " << p.numTotalPal <<"\tnumTotalPalSinParada: " <<
			p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes << "\ttamBytes: " <<
			p.tamBytes;
	return s;
}

/*********************************************** InfDoc *********************************************/


InfDoc::InfDoc():
fechaModificacion(),
idDoc(0),
numPal(0),
numPalSinParada(0),
numPalDiferentes(0),
tamBytes(0)
{}

InfDoc::InfDoc(const InfDoc &id):
fechaModificacion(id.fechaModificacion),
idDoc(id.idDoc),
numPal(id.numPal),
numPalSinParada(id.numPalSinParada),
numPalDiferentes(id.numPalDiferentes),
tamBytes(id.tamBytes)
{}

InfDoc::~InfDoc()
{
	idDoc = 0;
	numPal = 0;
	numPalSinParada = 0;
	numPalDiferentes = 0;
	tamBytes = 0;
}

InfDoc& InfDoc::operator=(const InfDoc& id)
{
	if(this != &id)
	{

		idDoc 				= id.idDoc;
		numPal 				= id.numPal;
		numPalSinParada 	= id.numPalSinParada;
		numPalDiferentes 	= id.numPalDiferentes;
		tamBytes 			= id.tamBytes;
		fechaModificacion 	= id.fechaModificacion;
	}
    return (*this);
}

ostream& operator<<(ostream& s, const InfDoc& p)
{
	s << "idDoc: " << p.idDoc << "\tnumPal: " <<	p.numPal << "\tnumPalSinParada: " << p.numPalSinParada <<
		"\tnumPalDiferentes: " << p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
	return s;
}

/*********************************************** InformacionTermino *********************************************/

InformacionTermino::InformacionTermino(const InformacionTermino &it):
ftc(it.ftc),
l_docs(it.l_docs)
{}

InformacionTermino::InformacionTermino():
ftc(0)
{}

InformacionTermino::~InformacionTermino()
{
	ftc = 0;
	this->l_docs.clear();
}

InformacionTermino& InformacionTermino::operator=(const InformacionTermino& it)
{
	if(this != &it)
	{
		this->ftc = it.ftc;
		this->l_docs=it.l_docs;
	}
    return (*this);
}

ostream& operator<<(ostream& s, const InformacionTermino& p)
{
	s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
	for (auto itLdocs = p.l_docs.begin(); itLdocs != p.l_docs.end(); ++itLdocs)
		s << "\t" << "Id.Doc: " << itLdocs->first << '\t' << itLdocs->second;

	return s;
}

/************************************************ InfTermDoc **************************************************/

InfTermDoc::InfTermDoc():
ft(0)
{}

InfTermDoc::InfTermDoc(const InfTermDoc &itd):
ft(itd.ft),
posTerm(itd.posTerm)
{}

InfTermDoc::~InfTermDoc()
{
	this->ft = 0;
	this->posTerm.clear();
}

InfTermDoc& InfTermDoc::operator= (const InfTermDoc& itd)
{
	if(this != &itd){
		this->~InfTermDoc();
		this->ft = itd.ft;
		this->posTerm=itd.posTerm;
	}
    return (*this);
}

ostream& operator<<(ostream& s, const InfTermDoc& p)
{
	s << "ft: " << p.ft << "\t";
	for (auto itPosTerm = p.posTerm.begin(); itPosTerm != p.posTerm.end();++itPosTerm)
	{
		if (itPosTerm == p.posTerm.begin())
			s << *itPosTerm;
		else
			s << "\t" << *itPosTerm;
	}
	return s;
}
