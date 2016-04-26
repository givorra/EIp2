/*
 * indexadorInformacion.h
 *
 *  Created on: 10/03/2016
 *      Author: mlrg7
 */

#ifndef INDEXADORINFORMACION_H_
#define INDEXADORINFORMACION_H_

#include <iostream>
#include <string>
#include <unordered_map>
//#include <tr1/unordered_map>
#include <list>
using namespace std;
//using namespace std::tr1;


class InfColeccionDocs {

	friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);

	public:
		InfColeccionDocs (const InfColeccionDocs &);
		InfColeccionDocs ();
		~InfColeccionDocs ();
		InfColeccionDocs & operator= (const InfColeccionDocs &);

		long int getNumDocs()const;
		long int getNumTotalPal()const;
		long int getNumTotalPalSinParada()const;
		long int getNumTotalPalDiferentes()const;
		long int getTamBytes()const;

		void setNumDocs(long int);
		void setNumTotalPal(long int);
		void setNumTotalPalSinParada(long int);
		void setNumTotalPalDiferentes(long int);
		void setTamBytes(long int);

		void incrementarNumDocs();
		void incrementarNumTotalPal();
		void incrementarNumTotalPalSinParada();
		void incrementarNumTotalPalDiferentes();
		void incrementarTamBytes(long int);

		void decrementarNumDocs();
		void decrementarNumTotalPalDiferentes();
	private:
		long int numDocs; // No total de documentos en la colección
		long int numTotalPal; // No total de palabras en la colección
		long int numTotalPalSinParada; // No total de palabras sin stop-words en la colección
		long int numTotalPalDiferentes; // No total de palabras diferentes en la colección que no sean stop- words (sin acumular la frecuencia de cada una de ellas)
		long int tamBytes; // Tamaño total en bytes de la colección
};


class Fecha{

	friend ostream& operator<<(ostream& s, const Fecha& p);

	public:
		Fecha();
		Fecha(int,int,int,int,int,int);
		Fecha(struct tm *clock);
		~Fecha();
		bool operator <(const Fecha& fecha)const;
		bool operator<=(const Fecha& f) const;
		bool operator==(const Fecha& f) const;

		int dia;
		int mes;
		int anyo;
		int hora;
		int minuto;
		int segundo;
};


class InfDoc {

	friend ostream& operator<<(ostream& s, const InfDoc& p);

	public:
		InfDoc (const InfDoc &);

		//constructor inicializa el id a 0, cada fihero hacer un set!
		InfDoc ();
		~InfDoc ();
		InfDoc & operator= (const InfDoc &);

		long int getidDoc()const;
		int getNumPal()const;
		int getNumPalSinParada()const;
		int getNumPalDiferentes()const;
		int getTamBytes()const;
		Fecha getFechaModificacion()const;

		void setIdDoc(long int);
		void setNumPal(int);
		void setNumPalSinParada(int);
		void setNumPalDiferentes(int);
		void setTamBytes(int);
		void setFecha(Fecha);


		void incrementarNumPal();
		void incrementarNumPalSinParada();
		void incrementarNumPalDiferentes();


	private:

		long int idDoc;
		// Identificador del documento. El primer documento indexado en la colección será el identificador 1
		int numPal;
		// No total de palabras del documento
		int numPalSinParada;
		// No total de palabras sin stop-words del documento
		int numPalDiferentes;
		// No total de palabras diferentes que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
		int tamBytes;
		// Tamaño en bytes del documento
		Fecha fechaModificacion;
		// Atributo correspondiente a la fecha y hora de modificación del documento. El tipo “Fecha/hora” lo elegirá/implementará el alumno

};


class InfTermDoc {

	friend ostream& operator<<(ostream& s, const InfTermDoc& p);
	public:
		InfTermDoc (const InfTermDoc &);
		InfTermDoc (); // Inicializa ft = 0
		~InfTermDoc (); // Pone ft = 0
		InfTermDoc & operator= (const InfTermDoc &);

	// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
		bool operator == (const InfTermDoc &p)const;
		void almacenarposTerm(int);
		int getFt()const;
		void setFt(int);
		void incrementarFt();
		list<int> getPosTerm()const;
		void setPosTerm(list <int>);

	private:
		int ft; // Frecuencia del término en el documento
		list<int> posTerm; // Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true
	// Lista de números de palabra en los que aparece el término en el documento. Los números de palabra comenzarán desde cero (la primera palabra del documento). Se numerarán las palabras de parada. Estará ordenada de menor a mayor posición.
};


class InformacionTermino {

	friend ostream& operator<<(ostream& s, const InformacionTermino& p);

	public:
		InformacionTermino (const InformacionTermino &);
		InformacionTermino (); // Inicializa ftc = 0
		~InformacionTermino (); // Pone ftc = 0 y vacía l_docs
		InformacionTermino & operator= (const InformacionTermino &);
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase


		unordered_map<long int, InfTermDoc> getColeccionDocumentos() const;
		InfTermDoc* obtenerDoc(long int);
		long int consultarIdDocumento(InfTermDoc);
		bool addDoc(const long int&,const InfTermDoc&);
		bool eraseDoc(const long int&);
		bool existe(const long int&);
		int tamColeccionDocs()const;
		int getFtc()const;
		void setFtc(int);
		void incrementarFtc();

		int numTerminos;

	private:
		int ftc; // Frecuencia total del término en la colección
		unordered_map <long int, InfTermDoc> l_docs;
		// Tabla Hash que se accederá por el id del documento, devolviendo un objeto de la clase InfTermDoc que contiene toda la información de aparición del término en el documento
};



class InformacionTerminoPregunta {
	friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);

	public:
		InformacionTerminoPregunta (const InformacionTerminoPregunta &);
		InformacionTerminoPregunta ();
		~InformacionTerminoPregunta ();
		InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta&);
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase

		void incrementarFt();
		int getFt()const;
		list<int> getPosTerm()const;
		void setFt(int);
		void almacenarposTerm(int);

	private:
		int ft; // Frecuencia total del término en la pregunta
		list<int> posTerm; // Solo se almacenará esta información si el campo privado del indexador almacenarPosTerm == true

		// Lista de números de palabra en los que aparece el término en la pregunta. Los números de palabra comenzarán desde cero (la primera palabra de la pregunta). Se numerarán las palabras de parada. Estará ordenada de menor a mayor posición.
};


class InformacionPregunta {
	friend ostream& operator<<(ostream& s, const InformacionPregunta& p);


	public:
		InformacionPregunta (const InformacionPregunta &);
		InformacionPregunta ();
		~InformacionPregunta ();
		InformacionPregunta & operator= (const InformacionPregunta &);
		// Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
		long int getNumTotalPal()const;
		long int getNumTotalPalSinParada()const;
		long int getNumTotalPalDiferentes()const;
		void incrementarNumTotalPal();
		void incrementarNumTotalPalSinParada();
		void incrementarNumTotalPalDiferentes();

		void setNumTotalPal(long int);
		void setNumTotalPalSinParada(long int);
		void setNumTotalPalDiferentes(long int);


	private:
		long int numTotalPal; // No total de palabras en la pregunta
		long int numTotalPalSinParada; // No total de palabras sin stop-words en la pregunta
		long int numTotalPalDiferentes; // No total de palabras diferentes en la pregunta que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
};

#endif
