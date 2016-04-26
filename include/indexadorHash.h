/*
 * IndexadorHash.h
 *
 *  Created on: 26 de abr. de 2016
 *      Author: gacel
 */

#ifndef INDEXADORHASH_H_
#define INDEXADORHASH_H_

#include <unistd.h>
#include "indexadorInformacion.h"
#include "stemmer.h"
#include "tokenizador.h"

using namespace std;

class IndexadorHash
{

	friend ostream& operator<<(ostream& s, const IndexadorHash&);

	public:
		IndexadorHash(const string& fichStopWords, const string& delimitadores, const bool& detectComp, const bool& minuscSinAcentos, const string& dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm);

		IndexadorHash(const string& directorioIndexacion);

		IndexadorHash(const IndexadorHash&);

		~IndexadorHash();

		IndexadorHash& operator= (const IndexadorHash&);

		bool Indexar(const string& ficheroDocumentos);

		bool IndexarDirectorio(const string& dirAIndexar);

		bool GuardarIndexacion() const;

		bool RecuperarIndexacion (const string& directorioIndexacion);

		void ImprimirIndexacion() const;

		bool IndexarPregunta(const string& preg);

		bool DevuelvePregunta(string& preg) const;

		bool DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const;

		bool DevuelvePregunta(InformacionPregunta& inf) const;

		void ImprimirIndexacionPregunta();

		void ImprimirPregunta();

		bool Devuelve(const string& word, InformacionTermino& inf) const;

		bool Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc) const;

		bool Existe(const string& word) const;

		bool Borra(const string& word);

		bool BorraDoc(const string& nomDoc);

		void VaciarIndice();

		bool Actualiza(const string& word, const InformacionTermino& inf);

		bool Inserta(const string& word, const InformacionTermino& inf);

		int NumPalIndexadas() const;

		string DevolverFichPalParada () const;

		void ListarPalParada() const;

		int NumPalParada() const;

		string DevolverDelimitadores () const;

		bool DevolverCasosEspeciales () const;

		bool DevolverPasarAminuscSinAcentos () const;

		bool DevolverAlmacenarPosTerm () const;

		string DevolverDirIndice () const;

		int DevolverTipoStemming () const;

		bool DevolverAlmEnDisco () const;

		void ListarInfColeccDocs() const;

		void ListarTerminos() const;

		bool ListarTerminos(const string& nomDoc) const;

		void ListarDocs() const;

		bool ListarDocs(const string& nomDoc) const;

		stemmerPorter stemmer;

	private:
		string directorioIndice;
		tr1::unordered_map<string, InformacionTermino> indice;
		tr1::unordered_map<string, InfDoc> indiceDocs;
		string pregunta;
		tr1::unordered_map<string, InformacionTerminoPregunta> indicePregunta;
		InformacionPregunta infPregunta;
		tr1::unordered_set<string> stopWords;

		InfColeccionDocs informacionColeccionDocs;
		Tokenizador tok;
		string ficheroStopWords;
		int tipoStemmer;
		bool almacenarEnDisco;
		bool almacenarPosTerm;

	private:
		IndexadorHash();
		inline void ImprimirIndice() const
			{for(auto it = indice.begin(); it != indice.end(); ++it)	// A continuación aparecerá un listado del contenido del campo privado “índice” donde para cada término indexado se imprimirá: cout << termino << ‘\t’ << InformacionTermino << endl;
					cout << it->first << '\t' << it->second << "\n";}
		inline void ImprimeIndiceDocs() const
			{for(auto it = indiceDocs.begin(); it != indiceDocs.end(); ++it)// A continuación aparecerá un listado del contenido del campo privado “indiceDocs” donde para cada documento indexado se imprimirá: cout << nomDoc << ‘\t’ << InfDoc << endl;
				cout << it->first << '\t' << it->second << "\n";}
		inline void ImprimeIndicePregunta() const
			{for(auto it = indicePregunta.begin(); it != indicePregunta.end(); ++it)
				cout << it->first << '\t' << it->second << "\n";}
};

#endif /* INDEXADORHASH_H_ */
