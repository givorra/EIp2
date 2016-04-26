

#include "indexadorHash.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <algorithm>
#include <fstream>

//COMPROBADO
IndexadorHash::IndexadorHash(const string& fichStopWords, const string& delimitadores, const bool& detectComp, const bool& minuscSinAcentos, const string& dirIndice, const int& tStemmer, const bool& almEnDisco, const bool& almPosTerm)
{

	//QUE ES DETECTCOMP
	this->ficheroStopWords = fichStopWords;

	ifstream f(fichStopWords,ifstream::in);
	if(f.is_open()){
		string cadena;
		f >> cadena;
		int cont = 0;
		while(!f.eof())
		{
			stopWords.insert(cadena);
			f >> cadena;
		}
		f.close();
	}else
	{
		cerr << "sERROR: No existe el archivo: " << ficheroStopWords << "\n";
	}

	tok.DelimitadoresPalabra(delimitadores);
	tok.CasosEspeciales(detectComp);
	tok.PasarAminuscSinAcentos(minuscSinAcentos);


	if(dirIndice=="")
		directorioIndice = get_current_dir_name();
	else directorioIndice = dirIndice;


	tipoStemmer = tStemmer;
	almacenarEnDisco = almEnDisco;
	almacenarPosTerm = almPosTerm;
}

//COMPROBADO
IndexadorHash::IndexadorHash(const string& directorioIndexacion)
{
	RecuperarIndexacion(directorioIndexacion);
}

//COMPROBADO
IndexadorHash::IndexadorHash(const IndexadorHash& otro)
{
	indice = otro.indice;
	indiceDocs = otro.indiceDocs;
	informacionColeccionDocs = otro.informacionColeccionDocs;
	pregunta = otro.pregunta;
	indicePregunta = otro.indicePregunta;
	infPregunta = otro.infPregunta;
	ficheroStopWords = otro.ficheroStopWords;
	stopWords = otro.stopWords;
	tok = otro.tok;
	directorioIndice = otro.directorioIndice;
	tipoStemmer = otro.tipoStemmer;
	almacenarEnDisco = otro.almacenarEnDisco;
	almacenarPosTerm = otro.almacenarPosTerm;
}

//COMPROBADO
IndexadorHash& IndexadorHash::operator= (const IndexadorHash& otro)
{
	if(this != &otro)
		//this->~IndexadorHash();

		indice = otro.indice;
		indiceDocs = otro.indiceDocs;
		informacionColeccionDocs = otro.informacionColeccionDocs;
		infPregunta = otro.infPregunta;
		pregunta = otro.pregunta;
		indicePregunta = otro.indicePregunta;
		ficheroStopWords = otro.ficheroStopWords;
		stopWords = otro.stopWords;
		tok = otro.tok;
		directorioIndice = otro.directorioIndice;
		tipoStemmer = otro.tipoStemmer;
		almacenarEnDisco = otro.almacenarEnDisco;
		almacenarPosTerm = otro.almacenarPosTerm;
    return (*this);
}

//COMPROBADO
IndexadorHash::~IndexadorHash()
{

	this->VaciarIndice();
	//indice.clear();
	//indiceDocs.clear();
	pregunta = "";
	//indicePregunta.clear();
	//stopWords.clear();
	ficheroStopWords = "";
	directorioIndice = "";
	tipoStemmer = 0;
	almacenarEnDisco = false;
	almacenarPosTerm = false;

}
//---------------------------------
//COMPROBADO
ostream& operator<<(ostream& s, const IndexadorHash& p) {
	s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << endl;
	s << "Tokenizador: " << p.tok << endl;
	s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << endl;
	s << "Stemmer utilizado: " << p.tipoStemmer << endl;
	s << "Informacion de la coleccion indexada: " << p.informacionColeccionDocs << endl;
	s << "Se almacenara parte del indice en disco duro: " << p.almacenarEnDisco << endl;
	s << "Se almacenaran las posiciones de los terminos: " << p.almacenarPosTerm<<endl;
	return s;
}

//COMPROBADO
void IndexadorHash::ImprimirIndexacion() const {
	cout << "Terminos indexados: " << endl;
	for(const auto& n: indice)
		 cout << n.first << '\t' << n.second << endl;
	// A continuación aparecerá un listado del contenido del campo privado “índice” donde para cada término indexado se imprimirá: cout << termino << ‘\t’ << InformacionTermino << endl;

	cout << "Documentos indexados: " << endl;

	for(const auto& n: indiceDocs)
		 cout << n.first << '\t' << n.second << endl;
	// A continuación aparecerá un listado del contenido del campo privado “indiceDocs” donde para cada documento indexado se imprimirá: cout << nomDoc << ‘\t’ << InfDoc << endl;
	}

//COMPROBADO
void IndexadorHash::ImprimirIndexacionPregunta() {
	cout << "Pregunta indexada: " << pregunta << endl;
	cout << "Terminos indexados en la pregunta: " << endl;
	for(const auto& n: indicePregunta)
		 cout << n.first << '\t' << n.second << endl;
	// A continuación aparecerá un listado del contenido de“indicePregunta” donde para cada término indexado se imprimirá: cout << termino << ‘\t’ << InformacionTerminoPregunta << endl;
	cout << "Informacion de la pregunta: " << infPregunta << endl;
}

//COMPROBADO
void IndexadorHash::ImprimirPregunta() {
	cout << "Pregunta indexada: " << pregunta << endl;
	cout << "Informacion de la pregunta: " << infPregunta << endl;
}

//COMPROBADO
string IndexadorHash::DevolverFichPalParada() const
{
	return ficheroStopWords;
}

//COMPROBADO
bool IndexadorHash::DevolverAlmacenarPosTerm() const
{
	return almacenarPosTerm;
}

//COMPROBADO
string IndexadorHash::DevolverDirIndice() const
{
	return directorioIndice;
}

//COMPROBADO
int IndexadorHash::DevolverTipoStemming() const
{
	return tipoStemmer;
}

//COMPROBADO
bool IndexadorHash::DevolverAlmEnDisco() const
{
	return almacenarEnDisco;
}

//COMPROBADO
string IndexadorHash::DevolverDelimitadores() const
{
	return tok.DelimitadoresPalabra();
}

//COMPROBADO
bool IndexadorHash::DevolverCasosEspeciales() const
{
	return tok.CasosEspeciales();
}

//COMPROBADO
bool IndexadorHash::DevolverPasarAminuscSinAcentos() const
{
	return tok.PasarAminuscSinAcentos();
}

//COMPROBADO
bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const
{
	if(!pregunta.empty())
	{
		inf = infPregunta;
		return true;
	}
	return false;
}

//COMPROBADO
void IndexadorHash::ListarInfColeccDocs() const
{
	cout << informacionColeccionDocs << endl;
}

//COMPROBADO
void IndexadorHash::ListarTerminos() const
{
	for(const auto& n : indice)
		cout << n.first << '\t' << n.second << endl;
}

//COMPROBADO
void IndexadorHash::ListarPalParada() const
{
	for(const auto& n: stopWords)
		cout << n << endl;
}

//COMPROBADO
void IndexadorHash::ListarDocs() const
{
	for(const auto& n : indiceDocs)
		cout << n.first << '\t' << n.second << endl;
}

//COMRPOBADO
bool IndexadorHash::ListarDocs(const string& nomDoc) const
{
	auto n = indiceDocs.find(nomDoc);

	if(n != indiceDocs.end()){
		cout << n->first << '\t' << n->second << "\n";
		return true;
	}

	return false;
}

//COMPROBADO
bool IndexadorHash::Existe(const string& word) const
{
	return (indice.find(word) != indice.end());
}

//COMPROBADO
bool IndexadorHash::Borra(const string& word)
{
	return (indice.erase(word) != 0);
}

//CAMBIADO
bool IndexadorHash::Inserta(const string& word, const InformacionTermino& inf)
{
	/*
	if(Existe(word))
	{
		return false;
	}else
	{
		//indice[word] = inf;
		indice.insert({word, inf});
		numeroEnIndice++;
		return true;
	}
	*/

	unordered_map<string, InformacionTermino>::iterator it = indice.find(word);

	//Comprobamos si esta indexado.
	if(it == indice.end()){
		indice.insert({word, inf});
		return true;
	}

	return false;
}

//COMPROBADO
bool IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf)
{
	if(Existe(word))
	{
		indice[word] = inf;
		return true;
	}else
	{
		cerr << "ERROR: En actualiza no existe la palabra "<<word<<endl;
		return false;

	}
}

//COMPROBADO
int IndexadorHash::NumPalIndexadas() const
{
	return indice.size();
}

//COMPROBADO
int IndexadorHash::NumPalParada() const
{
	stopWords.size();
}

//COMPROBADO PREGUNTAR PROFE
void IndexadorHash::VaciarIndice()
{
	indice.clear();
	indiceDocs.clear();
	indicePregunta.clear();
}

//COMPROBADO
bool IndexadorHash::DevuelvePregunta(string& preg) const
{
	if(!pregunta.empty()){
		preg = pregunta;
		return true;
	}

	return false;
}

//----PROBLEMICAS? -----
//COMPROBADO-LIOSO
bool IndexadorHash::ListarTerminos(const string& nomDoc) const
{
	auto it = indiceDocs.find(nomDoc);
	InformacionTermino infTermino;
	unordered_map<long int, InfTermDoc>::iterator auxIt;


	//Comprobamos si el documento esta en la coleccion.
	if(it != indiceDocs.end()){
		//Conseguimos la id del documento.
		long int idDoc = it->second.getidDoc();

		for(auto it2 = indice.begin(); it2 != indice.end(); ++it){
			//Conseguimos la InformacionTermino
			infTermino = it2->second;
			auxIt = infTermino.getColeccionDocumentos().find(idDoc);

			if(auxIt != infTermino.getColeccionDocumentos().end()){
				cout << it2->first << '\t' << infTermino << "\n";
			}
		}
		return true;
	}

	return false;
}

//COMPROBADO
bool IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const
{
	unordered_map<string, InformacionTermino>::const_iterator temp =
				indice.find(word);
		if (temp != indice.end())
		{
			inf = temp->second;
			return true;
		}
		inf = InformacionTermino();
		return false;
}

//COMPROBADO-LIOSO
bool IndexadorHash::Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc) const
{


	if(Existe(word)){

		auto it = indice.find(word);
		auto it2 = indiceDocs.find(nomDoc);

		if(it2 != indiceDocs.end()){

			long int idDoc = it2->second.getidDoc();
			unordered_map<long int, InfTermDoc> ldocs = it->second.getColeccionDocumentos();
			auto it3 = ldocs.find(idDoc);

			if(it3 != ldocs.end()){
				InfDoc = it3->second;
				return true;
			}
		}
	}

	return false;
}

//COMPROBADO
bool IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const
{
	auto it = indicePregunta.find(word);
	if(it != indicePregunta.end()){
		inf = it->second;
		return true;
	}
	return false;
}

//COMPROBADO
bool IndexadorHash::IndexarDirectorio(const string& dirAIndexar) {

	struct stat dir;
	int err = stat(dirAIndexar.c_str(), &dir);

	if(err == -1 || !S_ISDIR(dir.st_mode)){
		//cerr << "No existe el directorio " + dirAIndexar << "\n";
		return false;
	} else
	{
		//cout << dirAIndexar<< endl;
		string cmd="find "+ dirAIndexar + " -follow -type f -not -name \"*.tk\" | sort > lista_fich_indexar";
		system(cmd.c_str());
		return Indexar("lista_fich_indexar");

		/*
		string cmd="find "+ dirAIndexar +" -follow -type f |sort > lista_fich_indexar";
		system(cmd.c_str());
		return Indexar("lista_fich_indexar");
	*/
	}
}


//COMPROBADO
bool IndexadorHash::BorraDoc(const string& nomDoc)
{

	//Comprobmamos si tenemos el nombre en el indice de documentos
	//n es un puntero al indice de documentos.
	 auto n = indiceDocs.find(nomDoc);

	 //Si lo tenemos, procederemos a localizarlo
	 if(n != indiceDocs.end())
	 {
		 /*Obtenemos el id del documentos, mediante el segundo campo de indice Documento
		  * Que es InfDoc y tiene como atributo el id del Documento.
		  */
		 long int idDocumento = n->second.getidDoc();

		 for(auto n2 = indice.begin(); n2 != indice.end(); ++n2)
		 {
			 InformacionTermino *infTerm = &(n2->second);
			 InfTermDoc *infTermDoc = infTerm->obtenerDoc(idDocumento);


			 /*Si es diferente de null es que en ese termino aparece el documento asociado ene l_docs
			  * sino pasamos a la siguiente iteración para ver otro termino.
			  */
			 if(infTermDoc != NULL)
			 {
				 //Modificamos la frecuencia total del termino.
				 infTerm->setFtc(infTerm->getFtc() - infTermDoc->getFt());
				 //Modificamos num de palabras de parada en la coleccion.
				 informacionColeccionDocs.setNumTotalPalSinParada(informacionColeccionDocs.getNumTotalPalSinParada() - infTermDoc->getFt());
				 //Eliminamos el documento de l_docs;
				 infTerm->eraseDoc(idDocumento);

				 //Si no tiene nada la lista de documentos.l_docs
				 if(infTerm->tamColeccionDocs() == 0)
				 {
					 //Hay una palabra diferente menos.
					 informacionColeccionDocs.decrementarNumTotalPalDiferentes();
					 //Tenemos que borrar el termino del indice ya que nadie lo contiene
					 indice.erase(n2->first);
				 }
			 }
		 }

		 informacionColeccionDocs.decrementarNumDocs();
		 informacionColeccionDocs.setNumTotalPal(informacionColeccionDocs.getNumTotalPal() - n->second.getNumPal());


		 informacionColeccionDocs.setTamBytes(informacionColeccionDocs.getTamBytes() - n->second.getTamBytes());

		 indiceDocs.erase(nomDoc);
		 return true;
	 }
	 //Si no lo tenemos, devolvemos false, y no lo habremos borrado.
	 return false;

}

//COMPROBADO
bool IndexadorHash::IndexarPregunta(const string& preg)
{

	list<string> todosTerminos;
	this->pregunta  = preg;
	//vaciar campos.
	indicePregunta.clear();
	infPregunta.~InformacionPregunta();

	//Si la preguntatieene terminos
	if(!pregunta.empty())
	{
		tok.Tokenizar(preg, todosTerminos);
		//cout << "Este es el tamaño de la lista :" << todosTerminos.size()<< endl;
		//recorremos la lista de terminos de la pregunta
		for(string terminoPreg : todosTerminos)
		{
			//cout << "ESTA ES LA PALABRA : "<<terminoPreg << endl;
			stemmer.stemmer(terminoPreg, tipoStemmer);

			//cout << "ESTA ES LA PALABRA DESPUES DEL STEMMER: "<<terminoPreg << endl;
			auto it = stopWords.find(terminoPreg);

			if(it == stopWords.end())
			{//No es una stopWord vamo s aver si esta indezada
				auto it2 = indicePregunta.find(terminoPreg);

				//Si esta indexada
				if(it2 != indicePregunta.end())
				{
					//cout << "Si que esta indexadaaaa" << terminoPreg << endl;
					//cout << infPregunta.getNumTotalPal()<< endl;
					InformacionTerminoPregunta infTermPreg = it2->second;
					infTermPreg.incrementarFt();
					if(almacenarPosTerm) infTermPreg.almacenarposTerm(infPregunta.getNumTotalPal());
					indicePregunta[terminoPreg]=infTermPreg;
				}else//No lo esta
				{
					InformacionTerminoPregunta infTermPreg;
					infTermPreg.incrementarFt();

					if(almacenarPosTerm) infTermPreg.almacenarposTerm(infPregunta.getNumTotalPal());

					indicePregunta[terminoPreg] = infTermPreg;
					//aumentamos el numero de palabras total diferentes
					infPregunta.incrementarNumTotalPalDiferentes();

				}
				infPregunta.incrementarNumTotalPalSinParada();
			}
			//En cualquier caso aunmentamos el numero total de palabras en la informacion de la pregunta
			infPregunta.incrementarNumTotalPal();
		}
		return true;
	}else
	{
		cerr << "ERROR : No hay términos que indexar en la pregunta" << endl;
	}
	return false;

}

//COMPROBADO-DIFISIL
/*bool IndexadorHash::Indexar(const string& ficheroDocumentos)
{
	ifstream f(ficheroDocumentos);
	bool indexadosTodos = true;
	bool previamenteIndex = false;
	string doc = "";
	cout << "Por ahora va bien la cosa" << endl;
	if(f.is_open())
	{
		cout << "Se ha abierto el fichero" << endl;
		f>> doc;
		//Si alguno no se puede indexar, poner indexadosTodos a false;
		cout << "He leido algo : " << doc << endl;

		while(!f.eof())
		{
			if(!doc.empty())
			{//-------------AQUI PASA EL MEOLLO --------
				//------Paso 1: Comprobar que exista.
				struct stat attrib;
				if(stat(doc.c_str(), &attrib) == -1){
					cerr << "ERROR: El fichero " << doc << " no ha sido encontrado." << "\n";
					return false;
				}
				cout << "Exite el ficheroo? si llegamos aqui esque no " << endl;
				//------Paso 2: Comprobar que no esté indexado ya.
				long int idDoc;
				auto it = indiceDocs.find(doc);
				if(it != indiceDocs.end())
				{
					cerr << "Documento ya indexado : "<< doc << endl;
					//Archivo indezado.
					Fecha fechaNew(gmtime(&(attrib.st_mtime)));
					if(it->second.getFechaModificacion() < fechaNew)
					{
						cerr << "Reindexamos"<< endl;
						idDoc = it->second.getidDoc();
						if(!BorraDoc(it->first))
						{
							cerr << "ERROR: No se ha borrado el documento: "<< it->first << endl;
							return false;
						}

					}
					previamenteIndex = true;
				}else
				{
					previamenteIndex = false;
				}
				//------Paso 3: Indexar rellenando la informacion del documento, los bytes fecha
				InfDoc informacionDocumento;
				if(previamenteIndex)
				{
					informacionDocumento.setIdDoc(idDoc);
				}else
				{
					informacionColeccionDocs.incrementarNumDocs();
					// +1????
					informacionDocumento.setIdDoc(informacionColeccionDocs.getNumDocs());
				}
				informacionDocumento.setTamBytes(attrib.st_size);
				Fecha fecha(gmtime(&(attrib.st_mtime)));
				informacionDocumento.setFecha(fecha);

				informacionColeccionDocs.setTamBytes(informacionColeccionDocs.getTamBytes() + informacionDocumento.getTamBytes());
				cout << "Voy a tokenizar " << endl;
				//-----Paso 4: Rellenar numPal, numPalSinParada, numPalDiferentes
				if(tok.Tokenizar(doc))
				{
					ifstream file(doc + ".tk");
					cout << "Estoy dentro del if del tokenizar " << endl;

					if(file.is_open())
					{
						string term;
						file >> term;
						cout << "Estoy dentro del if de que el fichero esta abierto " << endl;

						while(!file.eof())
						{
							cout << "Estoy en el while " << endl;

							stemmer.stemmer(term, tipoStemmer);
							auto iteratorStopWord = stopWords.find(term);
							if(iteratorStopWord == stopWords.end())
							{
								cout << "Estoy en  no es stopWord" << endl;

								//No es una stopWord
								//Ahora a ver si está indexada la palabra ya o no
								auto iteratorIndice = indice.find(term);
								if(iteratorIndice == indice.end())
								{
									cout << "Estoy en  no esta indexada" << endl;

									//No Esta indexada
									InformacionTermino inforTerm;
									inforTerm.incrementarFtc();

									InfTermDoc infTermDoc;
									infTermDoc.incrementarFt();
									//Si está a true almacenamos la posicion
									if(almacenarPosTerm) infTermDoc.almacenarposTerm(informacionDocumento.getNumPal());

									inforTerm.addDoc(informacionDocumento.getidDoc(),infTermDoc);
									indice[term]=inforTerm;
									informacionDocumento.incrementarNumPalDiferentes();
									informacionDocumento.incrementarNumPalDiferentes();

								}else
								{
									cout << "Estoy en si esta indexada" << endl;
									//Si está indexada
									//incrementamos la frecuencia en la que aparece el termino
									InformacionTermino *infTerm = &(iteratorIndice->second);
									cout << "He sacado la informacion de termino" << endl;
									infTerm->incrementarFtc();
									cout << "He incrementado la frecuencia" << endl;
									//y obtenemos la informacion del documento
									InfTermDoc *infTermDoc = infTerm->obtenerDoc(informacionDocumento.getidDoc());
									cout << "He obtenido el documento" << endl;
									//Comprobamos si es el documento que estamos indexando
									if(infTermDoc == NULL)
									{
										cout << "Estoy en es null" << endl;
										//No tenemos ese documento por lo tanto debemos crearlo
										InfTermDoc ninfTerm;
										infTerm->addDoc(informacionDocumento.getidDoc(), ninfTerm);

										infTermDoc = infTerm->obtenerDoc(informacionDocumento.getidDoc());
										informacionDocumento.incrementarNumPalDiferentes();
									}else
									{
										cout << "Estoy en  no es null" << endl;
										//Contine el documento que estamos indexando
									}
									infTermDoc->incrementarFt();
									if(almacenarPosTerm) infTermDoc->almacenarposTerm(informacionDocumento.getidDoc());
								}

								informacionDocumento.incrementarNumPalSinParada();
								informacionColeccionDocs.incrementarNumTotalPalSinParada();
							}else
							{
								cout << "Estoy en  es stopWord" << endl;

								//es una stopWord
							}

							informacionDocumento.incrementarNumPal();
							informacionColeccionDocs.incrementarNumTotalPal();

							term = "";
							file >> term;
						}
					}else
					{
						cerr << "ERROR: No se pudo leer el fichero tokenizado de " << doc << "\n";
									return false;
					}
				}else
				{
					cerr<< "Error: No se pudo tokenizar el fichero." << endl;
					return false;
				}
				cout << "AQUI ACABA EL MEOLLO ? " << endl;
				//------------AQUI ACABA EL MEOLLO----------
			}
			doc = "";
			f >> doc;
		}
		cout << " He salido del lio" << endl;

		f.close();
		cout << "He cerrado el foch" << endl;

	}else
	{
		cerr << "ERROR No se pudo abrir el fichero de documentos a indexar"<< endl;
	}
	return false;
}*/

/*bool
IndexadorHash::Indexar(const string &ficheroDocumentos){
	//Variable para saber si funciono bien todo.
	bool todoOk = true;

	//Abrimos el documento que nos proporcionan por parametro.
	ifstream inputFile(ficheroDocumentos);

	//Comprobamos si lo hemos abierto.
	if(inputFile.is_open()){
		//Vamos obteniendo los documentos a indexar.
		string documentoAIndexar = "";
		while(!inputFile.eof()){
			documentoAIndexar = "";
			getline(inputFile, documentoAIndexar);
			if(documentoAIndexar.length() > 0){
				if(IndexarDoc(documentoAIndexar) == false){
					todoOk = false;
				}
			}
		}

		//Cerramos el documento.
		inputFile.close();

		//Si llegamos a este punto, es que la operacion ha sido realizada
		//con exito.

		if(todoOk == false){
			return false;
		}

		return true;
	} else{
		cerr << "ERROR: No se pudo abrir el fichero que contiene los documentos a indexar" << "\n";
	}

	return false;
}
*/
bool
IndexadorHash::Indexar(const string &ficheroDocumentos){
	//Variable para almacenar el id del documento.
	ifstream inputFile(ficheroDocumentos);
	bool todoOk = true;

	//Comprobamos si lo hemos abierto.
	if(inputFile.is_open())
	{
		//Vamos obteniendo los documentos a indexar.
		string documentoAIndexar = "";
		while(!inputFile.eof() )
		{
			documentoAIndexar = "";
			getline(inputFile, documentoAIndexar);
			
			if(documentoAIndexar.length() > 0)
			{

				long int idDoc;
				//Obtenemos los datos del documento que nos han pasado
				//y comprobamos si existe.
				struct stat attrib;
				if(stat(documentoAIndexar.c_str(), &attrib) == -1)
				{
					cerr << "ERROR: El fichero " << documentoAIndexar << " no ha sido encontrado." << "\n";
					return false;
				}

				//Una vez hecho lo anterior, tenemos que comprobar si dicho fichero
				//ha sido indexado previamente
				bool archivoIndexado = false;

				auto it = indiceDocs.find(documentoAIndexar);

				//Comprobamos si ha sido indexado previamete.
				if(it != indiceDocs.end())
				{
					//Esto significa que ha sido indexado previamente.
					cerr << "AVISO: Este fichero ha sido previamente indexado." << "\n";
					archivoIndexado = true;

					//Vamos a comprobar si este documento es mas reciente que el
					//que indexamos anteriormente.
					Fecha fechaFichero(gmtime(&(attrib.st_mtime)));

					Fecha fechaNew(gmtime(&(attrib.st_mtime)));
					if(it->second.getFechaModificacion() < fechaNew)
					{
						//Reindexamos.
						cout << "Documento " << documentoAIndexar << " repetido. Volvemos a reindexarlo.\n";

						//Primero, obtenemos el idDoc.
						idDoc = it->second.getidDoc();

						//Borramos el documento antiguo.
						if(BorraDoc(it->first))
						{
							//Se consiguio borrar el documento.
						} else{
							cerr << "ERROR: No se pudo borrar el documento " << it->first << " para su reindexacion.\n";
							return false;
						}
					}
				}

				//Indexamos el documento.
				InfDoc informacionDocumento;

				//Indicamos el id del documento en funcion de si estaba indexado o no.
				if(archivoIndexado)
				{
					//Establecemos el idDoc que tenia anteriormente.
					informacionDocumento.setIdDoc(idDoc);
				} else
				{
					//Indicamos el id del documento.
					informacionColeccionDocs.incrementarNumDocs();
					informacionDocumento.setIdDoc(informacionColeccionDocs.getNumDocs());
				}

				//Indicamos el tamanyo del fichero y la fecha de modificacion.
				informacionDocumento.setTamBytes(attrib.st_size);
				Fecha fechaFichero(gmtime(&(attrib.st_mtime)));
				informacionDocumento.setFecha(fechaFichero);

				//Aumentamos el numero de bytes en la coleccion de documentos.
				informacionColeccionDocs.setTamBytes(informacionColeccionDocs.getTamBytes() + informacionDocumento.getTamBytes());

				//Nos falta rellenar numPal, numPalSinParada y numPalDiferentes.
				//Para rellenar dichas variables, debemos de indexar, y para ello
				//primero tenemos que tokenizar.
				if(tok.Tokenizar(documentoAIndexar))
				{
					//Una vez hemos tokenizado, tenemos que abrir dicho documento.
					ifstream inputFile(documentoAIndexar + ".tk");
					

					//Comprobamos si lo hemos abierto.
					if(inputFile.is_open())
					{
					//Variable que utilizaremos para ir leyendo los terminos.
						string termino = "";
						inputFile >> termino;

						while(!inputFile.eof())
						{
							//Obtenemos el termino.
							//termino = "";
							//inputFile >> termino;
							//getline(inputFile, termino);
							//Le aplicamos el stemmer.
							stemmer.stemmer(termino, tipoStemmer);

							//Ahora comprobamos que no sea una stopWord.
							auto itStopWord = stopWords.find(termino);
							if(itStopWord != stopWords.end()){
								//Es una stopWord.
							} else
							{
								//No es una stopWord.
								//Tenemos que ver si tenemos dicho termino ya indexado o no.
								auto it2 = indice.find(termino);

								if(it2 != indice.end())
								{

									//Lo hemos encontrado.
									//Aumentamos la frecuencia de aparicion en la coleccion.
									InformacionTermino *infTer = &(it2->second);
									infTer->incrementarFtc();
									//cout << "Intentando imprimir infTerm: "<<*infTer<<endl;
									//Ahora actualizamos sus datos con respecto al documento.
									InfTermDoc *infTermDocumento = infTer->obtenerDoc(informacionDocumento.getidDoc());
									//Comprobamos si dicho termino tiene el documento que estamos indexando.
									if(infTermDocumento != NULL){
										//Contiene el documento que estamos indexando.
									} else{
										//No contiene el documento que estamos indexando.
										//Tenemos que crearlo.
										InfTermDoc nuevoInfTermDoc;

										//Lo metemos en la coleccion de documentos en los que aparece el termino.
										infTer->addDoc(informacionDocumento.getidDoc(), nuevoInfTermDoc);

										//Actualizamos nuestro infTermDocumento.
										infTermDocumento = infTer->obtenerDoc(informacionDocumento.getidDoc());

										//Aumentamos el numero de palabras diferentes.
										informacionDocumento.incrementarNumPalDiferentes();
									}

									//Aumentamos la frecuencia del temrino en el documento.
									infTermDocumento->incrementarFt();

									//Comprobamos si tenemos que almacenar posiciones.
									if(almacenarPosTerm){
										infTermDocumento->almacenarposTerm(informacionDocumento.getNumPal());
									}
								}else{
									//No lo hemos encontrado.
									InformacionTermino infTermino;
									infTermino.incrementarFtc();

									InfTermDoc infTermDocumento;
									infTermDocumento.incrementarFt();
									if(almacenarPosTerm){
										infTermDocumento.almacenarposTerm(informacionDocumento.getNumPal());
									}

									infTermino.addDoc(informacionDocumento.getidDoc(), infTermDocumento);
									indice.insert({termino, infTermino});

									informacionDocumento.incrementarNumPalDiferentes();
									informacionColeccionDocs.incrementarNumTotalPalDiferentes();
								}

								//Aumentamos el numero total de palabras sin stopWords tanto en la
								//informacion del documento como en la información de la coleccion.
								informacionDocumento.incrementarNumPalSinParada();
								informacionColeccionDocs.incrementarNumTotalPalSinParada();
							}
							//cout << informacionDocumento.getNumPal() << endl;
							//cout << "Voy a incrementar el numero de palabras " << endl;
							informacionDocumento.incrementarNumPal();
							informacionColeccionDocs.incrementarNumTotalPal();

							inputFile >> termino;
						}

					//Cerramos dicho fichero.
						inputFile.close();

					//Insertamos el documento al indice de documentos indexados.
						indiceDocs.insert({documentoAIndexar, informacionDocumento});
					} else{
						cerr << "ERROR: No se pudo leer el fichero tokenizado de " << documentoAIndexar  << "\n";
						return false;
					}
				} else{
					cerr << "ERROR: No se pudo tokenizar el fichero " << documentoAIndexar << "\n";
					return false;
				}

			}
		}

		inputFile.close();

		if(todoOk == false)
		{
			return false;
		}
		return true;
	} else{
		cerr << "ERROR: No se pudo abrir el fichero que contiene los documentos a indexar" << "\n";
	}

	return false;
}


//------------------------------------------------------------------
//REPASAR
bool IndexadorHash::GuardarIndexacion() const
{
	struct stat attrib;
	bool todoBien = true;
	ofstream outputFile;

	//Vamos a comprobar si existe.
	int err = stat(directorioIndice.c_str(), &attrib);
	if(err == -1){
		//No existe dicho directorio, hay que crearlo.
		system(string("mkdir " + directorioIndice).c_str());
	}

	outputFile.open(directorioIndice + "/guardarIndexacion");
	//----------------Guardar parametros del indexadorHash-------
	if(outputFile.is_open()){
		//Guardamos los parametros del indexador.
		outputFile << almacenarEnDisco << "\n";
		outputFile << almacenarPosTerm << "\n";
		outputFile << tipoStemmer << "\n";
		outputFile << directorioIndice << "\n";
		outputFile << ficheroStopWords << "\n";
		outputFile << tok.DelimitadoresPalabra() << "\n";
		outputFile << tok.PasarAminuscSinAcentos() << "\n";
		outputFile << tok.CasosEspeciales();
		outputFile << informacionColeccionDocs.getNumDocs() << "\n";
		outputFile << informacionColeccionDocs.getNumTotalPal() << "\n";
		outputFile << informacionColeccionDocs.getNumTotalPalSinParada() << "\n";
		outputFile << informacionColeccionDocs.getNumTotalPalDiferentes() << "\n";
		outputFile << informacionColeccionDocs.getTamBytes() << "\n";


		//Cerramos el fichero.
		outputFile.close();

		return true;
	} else{
		cerr << "ERROR: No se pudo guardar los parametros del indexador." << "\n";
	}

	return false;
/*
	//------------------Guardar terminos del indexador------------

	//f.open(directorioIndice +"/guardarIndexacion");

	if(f.is_open())
	{
		//Recorremos el indice
		for(auto n = indice.begin(); n != indice.end(); ++n)
		{
			InformacionTermino infTerm = n->second;
			f << n->first << '\t' << infTerm.getFtc() << '\t' << infTerm.tamColeccionDocs() << '\t';

			for(auto p: infTerm.getColeccionDocumentos())
			{
				InfTermDoc infTermDoc = p.second;
				f << p.first << '\t' << infTermDoc.getFt() << '\t' << infTermDoc.getPosTerm().size();

				for(auto m : infTermDoc.getPosTerm())
					f << '/t' << m;
			}
			f<< "\n";
		}
	}else
	{
		todoBien=false;
		cerr << "ERROR: No se pueden guardar los parametros del indexadorHash"<<endl;
	}

	//--------------Guardar documentos indexador------------
	//f.open(directorioIndice +"/guardarIndexacion");
	if(f.is_open())
	{
		for(auto n = indiceDocs.begin(); n != indiceDocs.end(); ++n)
		{
			InfDoc infDoc = n->second;
			f<< n->first << '\t' << infDoc.getidDoc() << '\t' << infDoc.getNumPal() <<'\t' << infDoc.getNumPalDiferentes() <<'\t' << infDoc.getNumPalSinParada() <<'\t' << infDoc.getTamBytes() << '\t' << infDoc.getFechaModificacion()<<"\n";
		}
		//f.close();

	}else
	{
		cerr << "ERROR: No se pudo guardar los documentos del indexador." << "\n";
		todoBien = false;
	}

	//--------------Guardar info pregunta--------------
	//f.open(directorioIndice +"/guardarIndexacion");
	if(f.is_open())
	{

		f << pregunta <<"\n";
		f << infPregunta.getNumTotalPal() <<"\n";
		f << infPregunta.getNumTotalPalDiferentes()<<"\n";
		f << infPregunta.getNumTotalPalSinParada()<<"\n";

		//f.close();
	}else
	{
		cerr << "ERROR: No se pudo guardar la informacion de la pregunta." << "\n";
		todoBien = false;
	}

	//--------------Guardar indice pregunta-----------

	f.open(directorioIndice +"/guardarIndexacion");
	if(f.is_open())
		{
			for(auto n: indicePregunta)
			{
				InformacionTerminoPregunta infTermPreg = n.second;
				f << n.first << '\t' << infTermPreg.getFt() << '\t' << infTermPreg.getPosTerm().size();

				list<int> posTerm = infTermPreg.getPosTerm();

				for(auto p = posTerm.begin(); p != posTerm.end(); ++p)
					f << '\t' << *p;
			}
			//f.close();
		}else
		{
			cerr << "ERROR: No se pudo guardar el indice de la pregunta." << "\n";
			todoBien = false;
		}
		f.close();
		return true;
		*/
}


//FALTA
bool IndexadorHash::RecuperarIndexacion(const string& directorioIndexacion)
{
	indice.clear();
	indiceDocs.clear();
	indicePregunta.clear();
	stopWords.clear();
	ifstream inputFile;

	inputFile.open(directorioIndexacion + "/guardarIndexacion");

	//Comprobamos si hemos abierto dicho documento.
	if(inputFile.is_open()){
		//Vamos recuperando la informacion.
		//Recuperamos la variable almacenarEnDisco.
		inputFile >> almacenarEnDisco;

		//Recuperamos la variable almacenarPosTerm
		inputFile >> almacenarPosTerm;

		//Recuperamos el tipo de stemmer.
		inputFile >> tipoStemmer;

		//Recuperamos el directorioIndice
		//getline(inputFile, directorioIndice);
		inputFile >> directorioIndice;
		//Recuperamos el fichero de stopWords.
		inputFile >> ficheroStopWords;
		//getline(inputFile, ficheroStopWords);

		//Recuperamos los delimitadores de palabra.
		string delimitadoresPalabra;
		//getline(inputFile, delimitadoresPalabra);
		inputFile >> delimitadoresPalabra;
		//Recuperamos el pasar a minusculas sin acentos.
		bool minusculasSinAcentos;
		inputFile >> minusculasSinAcentos;

		//Recuperamos los casos especiales.
		bool casosEspeciales;
		inputFile >> casosEspeciales;

		//Recuperamos la informacion de la coleccion de documentos.
		long int aux;

		inputFile >> aux;
		informacionColeccionDocs.setNumDocs(aux);

		inputFile >> aux;
		informacionColeccionDocs.setNumTotalPal(aux);

		inputFile >> aux;
		informacionColeccionDocs.setNumTotalPalSinParada(aux);

		inputFile >> aux;
		informacionColeccionDocs.setNumTotalPalDiferentes(aux);

		inputFile >> aux;
		informacionColeccionDocs.setTamBytes(aux);

		//Creamos el tokenizador a partir de los parametros leidos.
		Tokenizador auxTok(delimitadoresPalabra, casosEspeciales, minusculasSinAcentos);
		tok = auxTok;



		//Cerramos el documento.
		inputFile.close();

		return true;
	} else{
		cerr << "ERROR: No se han podido cargar los parametros del indexador." << "\n";
	}

	return false;

}
//QUEHASEMOH

IndexadorHash::IndexadorHash()
{
	// Este constructor se pone en la parte privada porque no se permitirá
	//crear un indexador sin inicializarlo convenientemente. La inicialización
	//la decidirá el alumno
}


//-------------------------------METODOS ADICIONALES-------------
