/*
 * tokenizador.h
 *
 *  Created on: 29/01/2016
 *      Author: mlrg7
 */

#ifndef TOKENIZADOR_H_
#define TOKENIZADOR_H_

#include <iostream>
#include <string>
#include <list>
using namespace std;


class Tokenizador {

	friend ostream& operator<<(ostream&, const Tokenizador&);

public:
	Tokenizador (const Tokenizador&);
	Tokenizador (const string& );
	Tokenizador (const string& , const bool&, const bool& );
	Tokenizador ();
	~Tokenizador();
	Tokenizador& operator= (const Tokenizador&);

	void Tokenizar (const string& str, list<string>& tokens) const;
	bool Tokenizar (const string& i, const string& f) const;
	bool Tokenizar (const string & i) const;
	bool TokenizarListaFicheros (const string& i) const;
	bool TokenizarDirectorio (const string& i) const;

    void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);
    string DelimitadoresPalabra() const { return delimiters; }
	void DelimitadoresPalabra(const string& nuevoDelimiters) { delimiters = nuevoDelimiters; }
	void CasosEspeciales (const bool& nuevoCasosEspeciales){ casosEspeciales = nuevoCasosEspeciales;}
	bool CasosEspeciales ()const{return casosEspeciales;}
	void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos){pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;}
	bool PasarAminuscSinAcentos () const{return pasarAminuscSinAcentos;}

private:
	string delimiters;
	bool casosEspeciales;
	bool pasarAminuscSinAcentos;
	string pasarMinusculasNoAcentos(const string& cadena) const;
	int calculaIndice(int lastPos,const string& str , string xception )const ;
	int procesaNumeros(int lastPos,const string& str , string xception,bool& )const ;
	void procesar(const string& str, list<string>& tokens, string::size_type &lastPos, const string excep)const;
	void hacerPushBack(int inicio, int final, const string& cadena, list<string>& tokens)const;

};
#endif /* TOKENIZADOR_H_ */
