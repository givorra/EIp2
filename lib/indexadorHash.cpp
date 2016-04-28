/*
 * IndexadorHash.cpp
 *
 *  Created on: 26 de abr. de 2016
 *      Author: gacel
 */

#include "../include/indexadorHash.h"

IndexadorHash::IndexadorHash(const string& fichStopWords, const string& delimitadores, const bool& detectComp,
		const bool& minuscSinAcentos, const string& dirIndice, const int& tStemmer, const bool& almEnDisco,
		const bool& almPosTerm)
{
	(*this).ficheroStopWords = fichStopWords;

	ifstream f(fichStopWords, ifstream::in);
	if(f.good()){
		string cadena;
		f >> cadena;
		int cont = 0;
		while(!f.eof())
		{
			stopWords.insert(cadena);
			f >> cadena;
		}
		f.close();
	}
	else
	{
		cerr << "sERROR: No existe el archivo: " << ficheroStopWords << "\n";
	}

	(*this).tok.DelimitadoresPalabra(delimitadores);
	(*this).tok.PasarAminuscSinAcentos(minuscSinAcentos);
	(*this).tok.CasosEspeciales(detectComp);

	if(dirIndice=="")
		directorioIndice = get_current_dir_name();
	else
		directorioIndice = dirIndice;

	tipoStemmer = tStemmer;
	almacenarEnDisco = almEnDisco;
	almacenarPosTerm = almPosTerm;
	//indice.clear();
}

IndexadorHash::IndexadorHash(const string& directorioIndexacion)
{
	RecuperarIndexacion(directorioIndexacion);
}

IndexadorHash::IndexadorHash(const IndexadorHash& ih):
indice(ih.indice),
indiceDocs(ih.indiceDocs),
informacionColeccionDocs(ih.informacionColeccionDocs),
pregunta(ih.pregunta),
indicePregunta(ih.indicePregunta),
infPregunta(ih.infPregunta),
ficheroStopWords(ih.ficheroStopWords),
stopWords(ih.stopWords),
tok(ih.tok),
directorioIndice(ih.directorioIndice),
tipoStemmer(ih.tipoStemmer),
almacenarEnDisco(ih.almacenarEnDisco),
almacenarPosTerm(ih.almacenarPosTerm)
{}

IndexadorHash::~IndexadorHash()
{
	VaciarIndice();
	pregunta			= "";
	ficheroStopWords 	= "";
	directorioIndice 	= "";
	tipoStemmer 		= 0;
	almacenarEnDisco 	= false;
	almacenarPosTerm 	= false;
}

IndexadorHash& IndexadorHash::operator= (const IndexadorHash& ih)
{
	if(this != &ih)
	{
		indice 						= ih.indice;
		indiceDocs 					= ih.indiceDocs;
		informacionColeccionDocs 	= ih.informacionColeccionDocs;
		infPregunta 				= ih.infPregunta;
		pregunta 					= ih.pregunta;
		indicePregunta 				= ih.indicePregunta;
		ficheroStopWords 			= ih.ficheroStopWords;
		stopWords 					= ih.stopWords;
		tok 						= ih.tok;
		directorioIndice 			= ih.directorioIndice;
		tipoStemmer 				= ih.tipoStemmer;
		almacenarEnDisco 			= ih.almacenarEnDisco;
		almacenarPosTerm 			= ih.almacenarPosTerm;
	}
    return (*this);
}

bool IndexadorHash::Indexar(const string& ficheroDocumentos)
{
	ifstream fichDocumentos(ficheroDocumentos, ifstream::in);
	string nomDoc;		// Nombre documento a indexar
	string lineaDoc;	// Linea a tokenizar
	bool indexar = false;	// Indica si se ha indexado
	struct stat statDocumento;
	int idDocAIndexar;
	list<string> tokens;
	int posTerm;		// Posicion del termino en el documento

	if(fichDocumentos.good())
	{
		ifstream documento;
		nomDoc = "";						// Inicializa nomDoc

		while(getline(fichDocumentos, nomDoc))			// Leemos primer documento a indexar
		{
			auto itIndiceDocs = indiceDocs.find(nomDoc);
			idDocAIndexar = 0;
			indexar = false;
			posTerm = 0;

			stat(nomDoc.c_str(), &statDocumento);

			// COMPROBAMOS SI YA EXISTE EL DOCUMENTO
			if(itIndiceDocs != indiceDocs.end())
			{
				cerr << "AVISO: Este fichero ha sido previamente indexado." << "\n";
				Fecha fechaDoc(gmtime(&(statDocumento.st_mtime)));

				// Guardamos id del documento indexado anteriormente
				if(itIndiceDocs->second.fechaModificacion < fechaDoc)
				{
					cout << "Documento " << nomDoc << " repetido. Volvemos a reindexarlo.\n";
					idDocAIndexar = itIndiceDocs->second.idDoc;
					indexar = true;
				}

				//Borramos el documento antiguo.
				if(!BorraDoc(nomDoc))
				{
					cerr << "ERROR: No se pudo borrar el documento " << nomDoc << " para su reindexacion.\n";
					return false;
				}
			}
			else
				indexar = true;		// Si no existe, decimos que lo indexe

			// INDEXAMOS SI HAY QUE INDEXAR
			if(indexar)
			{
				documento.open(nomDoc.c_str(), ifstream::in);
				if(documento.good())	// Abre documento a indexar
				{
					InfDoc informacionDocumento;

					if(idDocAIndexar != 0)
						informacionDocumento.idDoc = idDocAIndexar;
					else
					{
						++informacionColeccionDocs.numDocs;
						informacionDocumento.idDoc = informacionColeccionDocs.numDocs;
					}
					informacionDocumento.tamBytes = statDocumento.st_size;


					while(getline(documento, lineaDoc))
					{
						tok.Tokenizar(lineaDoc, tokens);				// Obtenemos los tokens de la linea
						informacionDocumento.numPal += tokens.size();	// Acumulamos el numero de palabras de la l

						// Rerorremos los tokens de la linea leida
						for(auto itTokens = tokens.begin(); itTokens != tokens.end(); ++itTokens)
						{

							stemmer.stemmer((*itTokens), tipoStemmer);

							auto itStopWords = stopWords.find((*itTokens));

							if(itStopWords == stopWords.end())	// No es una stop word
							{

								++informacionDocumento.numPalSinParada;		// Incrementa palabras sin stop words
								if(!Existe((*itTokens)))					// Si el termino no existe
								{

									InformacionTermino informacionTerminoGlobal;
									InfTermDoc informacionTerminoDocumento;
									++informacionDocumento.numPalDiferentes;			// Incrementa palabras diferentes DOCUMENTO
									++informacionColeccionDocs.numTotalPalDiferentes;	// Incrementa palabras diferentes COLECCION

									// Inicializa termino en documento
									informacionTerminoDocumento.ft = 1;
									informacionTerminoDocumento.posTerm.push_back(posTerm);

									// Inserta el termino como global
									//informacionTerminoGlobal.~InformacionTermino();
									informacionTerminoGlobal.ftc = 1;
									informacionTerminoGlobal.l_docs.insert({informacionDocumento.idDoc, informacionTerminoDocumento});
									string termino = (*itTokens);
									indice.insert({termino, informacionTerminoGlobal});
								}
								else						// Si el termino ya existe
								{
									// Cargamos InformacionTermino
									auto itIndice = indice.find((*itTokens));
									auto itLdocs = itIndice->second.l_docs.find(informacionDocumento.idDoc);

									if(itLdocs != itIndice->second.l_docs.end())	// Si ya está en el documento
									{
										++itLdocs->second.ft;	// Incrementa frecuencia en el documento
										itLdocs->second.posTerm.push_back(posTerm);
									}
									else		// Si existe pero no en el documento actual
									{
										// Inserta un nuevo registro documento - InfoTermDoc
										++informacionDocumento.numPalDiferentes;
										InfTermDoc informacionTerminoDocumento;
										informacionTerminoDocumento.ft = 1;
										informacionTerminoDocumento.posTerm.push_back(posTerm);
										itIndice->second.l_docs.insert({informacionDocumento.idDoc, informacionTerminoDocumento});
									}
									++itIndice->second.ftc;	// Incrementa frecuencia del termino global
								}
							}
							++posTerm;	// Incrementamos posicion del termino
						}

						lineaDoc = "";
						//getline(documento, lineaDoc);
					}

					// Actualizamos la informacion de la coleccion de documentos
					informacionColeccionDocs.numTotalPal 			+= informacionDocumento.numPal;
					informacionColeccionDocs.numTotalPalSinParada 	+= informacionDocumento.numPalSinParada;
					informacionColeccionDocs.tamBytes				+= informacionDocumento.tamBytes;
					indiceDocs.insert({nomDoc, informacionDocumento});		// Añadimos el documento como indexado

					documento.close();
				}
				else
				{
					cerr << "ERROR: El fichero " << nomDoc << " no ha sido encontrado." << "\n";
					this->~IndexadorHash();
					return false;
				}
			}
		}
		fichDocumentos.close();
	}
	else
	{
		cerr << "sERROR: No existe el archivo: " << ficheroDocumentos << "\n";
	}
	return true;
}

bool IndexadorHash::IndexarDirectorio(const string& dirAIndexar)
{
	struct stat dir;
	int err = stat(dirAIndexar.c_str(), &dir);

	if(err == -1 || !S_ISDIR(dir.st_mode))
	{
		//cerr << "No existe el directorio " + dirAIndexar << "\n";
		return false;
	}
	else
	{
		string cmd="find "+ dirAIndexar + " -follow -type f -not -name \"*.tk\" | sort > lista_fich_indexar";
		system(cmd.c_str());
		return Indexar("lista_fich_indexar");
	}
}

bool IndexadorHash::GuardarIndexacion() const
{

}

bool IndexadorHash::RecuperarIndexacion (const string& directorioIndexacion)
{

}

void IndexadorHash::ImprimirIndexacion() const
{
	cout << "Terminos indexados: " << "\n";
	ImprimeIndiceDocs();
	cout << "Documentos indexados: " << "\n";
	ImprimirIndice();
}

bool IndexadorHash::IndexarPregunta(const string& preg)
{

}

bool IndexadorHash::DevuelvePregunta(string& preg) const
{
	if(!pregunta.empty())
	{
		preg = pregunta;
		return true;
	}
	return false;
}

bool IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const
{
	auto it = indicePregunta.find(word);
	if(it != indicePregunta.end())
	{
		inf = it->second;
		return true;
	}
	return false;
}

bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const
{
	if(!pregunta.empty())
	{
		inf = infPregunta;
		return true;
	}
	return false;
}

void IndexadorHash::ImprimirIndexacionPregunta()
{
	cout << "Pregunta indexada: " << pregunta << "\n";
	cout << "Terminos indexados en la pregunta: " << "\n";
	ImprimeIndicePregunta();
	cout << "Informacion de la pregunta: " << infPregunta << "\n";
}

void IndexadorHash::ImprimirPregunta()
{
	cout << "Pregunta indexada: " << pregunta << "\n";
	cout << "Informacion de la pregunta: " << infPregunta << "\n";
}

bool IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const
{
	auto temp = indice.find(word);
	if (temp != indice.end())
	{
		inf = temp->second;
		return true;
	}
	inf = InformacionTermino();
	return false;
}

bool IndexadorHash::Devuelve(const string& word, const string& nomDoc, InfTermDoc& InfDoc) const
{
	if(Existe(word))
	{
		auto itIndiceDocs = indiceDocs.find(nomDoc);

		if(itIndiceDocs != indiceDocs.end())
		{
			auto itIndice = indice.find(word);

			long int idDoc = itIndiceDocs->second.idDoc;
			auto itLdocs = itIndice->second.l_docs.find(idDoc);

			if(itLdocs != itIndice->second.l_docs.end())
			{
				InfDoc = itLdocs->second;
				return true;
			}
		}
	}
	return false;
}

bool IndexadorHash::Existe(const string& word) const
{
	return (indice.find(word) != indice.end());
}

bool IndexadorHash::Borra(const string& word)
{
	return (indice.erase(word) != 0);
}

bool IndexadorHash::BorraDoc(const string& nomDoc)
{
	auto itIndiceDocs = indiceDocs.find(nomDoc);

	if(itIndiceDocs != indiceDocs.end())	// Si existe el documento
	{
		int idDoc = itIndiceDocs->second.idDoc;
		for(auto itIndice = indice.begin(); itIndice != indice.end(); ++itIndice)	// Recorremos todos los terminos que aparecen en el documento
		{	// Resta la frecuencia del termino (en informacion del termino) y borra el registro de ldocs.
			auto itLdocs = itIndice->second.l_docs.find(idDoc);
			if(itLdocs != itIndice->second.l_docs.end())
			{
				itIndice->second.ftc -= itLdocs->second.ft;
				itIndice->second.l_docs.erase(itLdocs);
				// Si este termino solo aparecia en nomDoc, se elimina del indice de terminos
				if(itIndice->second.l_docs.empty())
				{
					indice.erase(itIndice);
					--informacionColeccionDocs.numTotalPalDiferentes;
				}
			}

		}
		// Resta totales de la informacion de la coleccion
		informacionColeccionDocs.numDocs 				-= 1;
		informacionColeccionDocs.numTotalPal 			-= itIndiceDocs->second.numPal;
		//informacionColeccionDocs.numTotalPalDiferentes 	-= itIndiceDocs->second.numPalDiferentes;
		informacionColeccionDocs.numTotalPalSinParada 	-= itIndiceDocs->second.numPalSinParada;
		informacionColeccionDocs.tamBytes 				-= itIndiceDocs->second.tamBytes;

		indiceDocs.erase(itIndiceDocs);
		return true;
	}
	return false;
}

void IndexadorHash::VaciarIndice()
{
	indice.clear();
	indiceDocs.clear();
	indicePregunta.clear();
}

bool IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf)
{
	if(Existe(word))
	{
		indice[word] = inf;
		return true;
	}
	else
	{
		cerr << "ERROR: En actualiza no existe la palabra "<<word<<endl;
		return false;
	}
}

bool IndexadorHash::Inserta(const string& word, const InformacionTermino& inf)
{
	auto it = indice.find(word);

	//Comprobamos si esta indexado.
	if(it == indice.end())
	{
		indice.insert({word, inf});
		return true;
	}
	return false;
}

int IndexadorHash::NumPalIndexadas() const
{
	return indice.size();
}

string IndexadorHash::DevolverFichPalParada () const
{
	return ficheroStopWords;
}

void IndexadorHash::ListarPalParada() const
{
	for(auto it = stopWords.begin(); it != stopWords.end(); ++it)
		cout << (*it) << "\n";
}

int IndexadorHash::NumPalParada() const
{
	stopWords.size();
}

string IndexadorHash::DevolverDelimitadores () const
{
	return tok.DelimitadoresPalabra();
}

bool IndexadorHash::DevolverCasosEspeciales () const
{
	return tok.CasosEspeciales();
}

bool IndexadorHash::DevolverPasarAminuscSinAcentos () const
{
	return tok.PasarAminuscSinAcentos();
}

bool IndexadorHash::DevolverAlmacenarPosTerm() const
{
	return almacenarPosTerm;
}

string IndexadorHash::DevolverDirIndice () const
{
	return directorioIndice;
}

int IndexadorHash::DevolverTipoStemming () const
{
	return tipoStemmer;
}

bool IndexadorHash::DevolverAlmEnDisco () const
{
	return almacenarEnDisco;
}

void IndexadorHash::ListarInfColeccDocs() const
{
	cout << informacionColeccionDocs << "\n";
}

void IndexadorHash::ListarTerminos() const
{
	for(auto it = indice.begin(); it != indice.end(); ++it)
		cout << it->first << '\t' << it->second << "\n";
}
/*
 * Descripcion: El metodo comprueba que nomDoc este indexado, si está, obtiene su id. A continuacion recorre el indice
 * de terminos para imprimir los que aparezcan en el documento.
 */
bool IndexadorHash::ListarTerminos(const string& nomDoc) const
{
	auto itIndiceDocs = indiceDocs.find(nomDoc);

	if(itIndiceDocs != indiceDocs.end())	// Comprueba que exista el documento
	{
		for(auto itIndice = indice.begin(); itIndice != indice.end(); ++itIndice)	// Recorre lista de terminos indexados
		{
			auto itLdocs = itIndice->second.l_docs.find(itIndiceDocs->second.idDoc);
			if(itLdocs != itIndice->second.l_docs.end())
				cout << itLdocs->first << '\t' << itIndice->second << "\n";
		}
		return true;
	}
	return false;
}

void IndexadorHash::ListarDocs() const
{
	for(auto itIndiceDocs = indiceDocs.begin(); itIndiceDocs != indiceDocs.end(); ++itIndiceDocs)
			cout << itIndiceDocs->first << '\t' << itIndiceDocs->second << endl;
}

bool IndexadorHash::ListarDocs(const string& nomDoc) const
{
	auto itIndiceDocs = indiceDocs.find(nomDoc);

	if(itIndiceDocs != indiceDocs.end())
	{
		cout << itIndiceDocs->first << '\t' << itIndiceDocs->second << "\n";
		return true;
	}
	return false;

}

ostream& operator<<(ostream& s, const IndexadorHash& p)
{
	s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << endl;
	s << "Tokenizador: " << p.tok << endl;
	s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << endl;
	s << "Stemmer utilizado: " << p.tipoStemmer << endl;
	s << "Informacion de la coleccion indexada: " << p.informacionColeccionDocs << endl;
	s << "Se almacenara parte del indice en disco duro: " << p.almacenarEnDisco << endl;
	s << "Se almacenaran las posiciones de los terminos: " << p.almacenarPosTerm<<endl;
	return s;
}

