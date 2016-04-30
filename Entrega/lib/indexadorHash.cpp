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

void IndexadorHash::limpiarIndexador()
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
			Fecha fechaDoc(gmtime(&(statDocumento.st_mtime)));

			// COMPROBAMOS SI YA EXISTE EL DOCUMENTO
			if(itIndiceDocs != indiceDocs.end())
			{
				cerr << "AVISO: Este fichero ha sido previamente indexado." << "\n";
				//Fecha fechaDoc(gmtime(&(statDocumento.st_mtime)));

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
					informacionDocumento.tamBytes 			= statDocumento.st_size;
					informacionDocumento.fechaModificacion 	= fechaDoc;


					while(getline(documento, lineaDoc))
					{
						tok.Tokenizar(lineaDoc, tokens);				// Obtenemos los tokens de la linea
						informacionDocumento.numPal += tokens.size();	// Acumulamos el numero de palabras de la l

						// Rerorremos los tokens de la linea leida
						for(auto itTokens = tokens.begin(); itTokens != tokens.end(); ++itTokens)
						{

							stemmer.stemmer((*itTokens), tipoStemmer);

							auto itStopWords = stopWords.find((*itTokens));

							if(stopWords.find((*itTokens)) == stopWords.end())	// No es una stop word
							{

								++informacionDocumento.numPalSinParada;		// Incrementa palabras sin stop words
								auto itIndice = indice.find((*itTokens));

								if(itIndice == indice.end())					// Si el termino no existe
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

									auto itLdocs = itIndice->second.l_docs.find(informacionDocumento.idDoc);

									if(itLdocs != itIndice->second.l_docs.end())	// Si ya estÃ¡ en el documento
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
					indiceDocs.insert({nomDoc, informacionDocumento});		// AÃ±adimos el documento como indexado

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
	struct stat statDir;

	stat(directorioIndice.c_str(), &statDir);

	if(!S_ISDIR(statDir.st_mode))				// Si no existe directorio, lo creamos
		system(string("mkdir " +directorioIndice).c_str());

	ofstream f(directorioIndice+"/"+ficheroIndice, ofstream::out);

	if(f.good())
	{
		f << almacenarPosTerm << "\n";
		f << almacenarEnDisco << "\n";
		f << ficheroStopWords << "\n";
		for(auto itStopWords = stopWords.begin(); itStopWords != stopWords.end(); ++itStopWords)
			f << (*itStopWords) << " ";
		f << "\n";
		f << infPregunta.numTotalPal << "\n";
		f << infPregunta.numTotalPalDiferentes << "\n";
		f << infPregunta.numTotalPalSinParada << "\n";
		f << pregunta << "\n";
		f << indicePregunta.size() << "\n";		// Numero de elementos de la pregunta
		// Escribe todos los InformacionTerminoPregunta
		for(auto itIndicePregunta = indicePregunta.begin(); itIndicePregunta != indicePregunta.end(); ++itIndicePregunta)
		{
			f << itIndicePregunta->first << "\n";
			f << itIndicePregunta->second.ft << "\n";
			for(auto itPosTerm = itIndicePregunta->second.posTerm.begin(); itPosTerm != itIndicePregunta->second.posTerm.end(); ++itPosTerm)
				f << (*itPosTerm) << " ";
			f << "\n";
		}
		f << informacionColeccionDocs.numDocs << "\n";
		f << informacionColeccionDocs.numTotalPal << "\n";
		f << informacionColeccionDocs.numTotalPalDiferentes << "\n";
		f << informacionColeccionDocs.numTotalPalSinParada << "\n";
		f << informacionColeccionDocs.tamBytes << "\n";

		f << indice.size() << "\n";
		// Escribimos indice --> coleccion de terminos
		for(auto itIndice = indice.begin(); itIndice != indice.end(); ++itIndice)
		{
			f << itIndice->first << "\n";
			f << itIndice->second.ftc << "\n";
			f << itIndice->second.l_docs.size() << "\n";

			// Escribimos ldocs --> InfTermDocs de cada termino
			for(auto itLdocs = itIndice->second.l_docs.begin(); itLdocs != itIndice->second.l_docs.end(); ++itLdocs)
			{
				f << itLdocs->first << "\n";
				f << itLdocs->second.ft << "\n";
				for(auto itPosTerm = itLdocs->second.posTerm.begin(); itPosTerm != itLdocs->second.posTerm.end(); ++itPosTerm)
					f << (*itPosTerm) << " ";
				f << "\n";
			}
		}

		f << indiceDocs.size() << "\n";
		// Escribimos indiceDocs --> coleccion de documentos indexados
		for(auto itIndiceDocs = indiceDocs.begin(); itIndiceDocs != indiceDocs.end(); ++itIndiceDocs)
		{
			f << itIndiceDocs->first << "\n";
			f << itIndiceDocs->second.idDoc << "\n";
			f << itIndiceDocs->second.numPal << "\n";
			f << itIndiceDocs->second.numPalDiferentes << "\n";
			f << itIndiceDocs->second.numPalSinParada << "\n";
			f << itIndiceDocs->second.tamBytes << "\n";

			f << itIndiceDocs->second.fechaModificacion.anyo << " ";
			f << itIndiceDocs->second.fechaModificacion.mes << " ";
			f << itIndiceDocs->second.fechaModificacion.dia << " ";
			f << itIndiceDocs->second.fechaModificacion.hora << " ";
			f << itIndiceDocs->second.fechaModificacion.min << " ";
			f << itIndiceDocs->second.fechaModificacion.seg << "\n";
		}
		f << tok.CasosEspeciales() << "\n";
		f << tok.PasarAminuscSinAcentos() << "\n";
		f << tok.DelimitadoresPalabra() << "\n";

		f.close();
		return true;
	}
	else
	{
		cerr << "ERROR: No se ha podido crear fichero para guardar indexación";
		return false;
	}

}

bool IndexadorHash::RecuperarIndexacion(const string& directorioIndexacion)
{
	limpiarIndexador();
	ifstream f(directorioIndexacion+"/"+ficheroIndice, ifstream::in);
	directorioIndice = directorioIndexacion;
	Tokenizador tokAux(" ", false, false);
	if(f.good())
	{
		string dato = "";
		getline(f, dato);
		almacenarPosTerm = atoi(dato.c_str());
		getline(f, dato);
		almacenarEnDisco = atoi(dato.c_str());
		getline(f, ficheroStopWords);

		// Leer stop words
		getline(f, dato);
		list<string> tokens;
		tokAux.Tokenizar(dato, tokens);

		for(auto itStopWords = tokens.begin(); itStopWords != tokens.end(); ++itStopWords)
			stopWords.insert((*itStopWords));

		getline(f, dato);
		infPregunta.numTotalPal = atoi(dato.c_str());
		getline(f, dato);
		infPregunta.numTotalPalDiferentes = atoi(dato.c_str());
		getline(f, dato);
		infPregunta.numTotalPalSinParada = atoi(dato.c_str());
		getline(f, pregunta);

		// Indice pregunta
		getline(f, dato);			// Tamaño del indicePregunta
		for(int i = atoi(dato.c_str()); i != 0; i--)
		{
			string termino;
			InformacionTerminoPregunta infTermPreg;
			getline(f, termino);

			getline(f, dato);
			infTermPreg.ft = atoi(dato.c_str());

			getline(f, dato);
			tokAux.Tokenizar(dato, tokens);
			for(auto itTokens = tokens.begin(); itTokens != tokens.end(); ++itTokens)
				infTermPreg.posTerm.push_back(atoi((*itTokens).c_str()));

			indicePregunta.insert({termino, infTermPreg});
		}
		getline(f, dato);
		informacionColeccionDocs.numDocs				= atoi(dato.c_str());
		getline(f, dato);
		informacionColeccionDocs.numTotalPal			= atoi(dato.c_str());
		getline(f, dato);
		informacionColeccionDocs.numTotalPalDiferentes	= atoi(dato.c_str());
		getline(f, dato);
		informacionColeccionDocs.numTotalPalSinParada	= atoi(dato.c_str());
		getline(f, dato);
		informacionColeccionDocs.tamBytes				= atoi(dato.c_str());

		getline(f, dato);
		for(int i = atoi(dato.c_str()); i != 0; i--)
		{
			string termino;
			getline(f, termino);

			InformacionTermino infoTermino;
			getline(f, dato);
			infoTermino.ftc = atoi(dato.c_str());

			getline(f, dato);
			for(int i = atoi(dato.c_str()); i != 0; i--)
			{
				InfTermDoc infTermDoc;
				string idDoc;
				getline(f, idDoc);

				getline(f, dato);
				infTermDoc.ft = atoi(dato.c_str());
				getline(f, dato);
				tokAux.Tokenizar(dato, tokens);
				for(auto itTokens = tokens.begin(); itTokens != tokens.end(); ++itTokens)
				{
					infTermDoc.posTerm.push_back(atoi((*itTokens).c_str()));
				}
				// Añade registro a infoTermino.ldocs
				infoTermino.l_docs.insert({atoi(idDoc.c_str()), infTermDoc});
			}
			indice.insert({termino, infoTermino});
		}

		getline(f, dato);
		for(int i = atoi(dato.c_str()); i != 0; i--)
		{
			InfDoc infDoc;
			string nomDoc;
			getline(f, nomDoc);

			getline(f, dato);
			infDoc.idDoc = atoi(dato.c_str());
			getline(f, dato);
			infDoc.numPal = atoi(dato.c_str());
			getline(f, dato);
			infDoc.numPalDiferentes = atoi(dato.c_str());
			getline(f, dato);
			infDoc.numPalSinParada = atoi(dato.c_str());
			getline(f, dato);
			infDoc.tamBytes = atoi(dato.c_str());

			getline(f, dato);
			tokAux.Tokenizar(dato, tokens);
			auto itTokens = tokens.begin();
			++itTokens;
			infDoc.fechaModificacion.anyo 	= atoi((*itTokens).c_str());
			++itTokens;
			infDoc.fechaModificacion.mes 	= atoi((*itTokens).c_str());
			++itTokens;
			infDoc.fechaModificacion.dia 	= atoi((*itTokens).c_str());
			++itTokens;
			infDoc.fechaModificacion.hora 	= atoi((*itTokens).c_str());
			++itTokens;
			infDoc.fechaModificacion.min 	= atoi((*itTokens).c_str());
			++itTokens;
			infDoc.fechaModificacion.seg 	= atoi((*itTokens).c_str());
			++itTokens;
			indiceDocs.insert({nomDoc, infDoc});
		}
		getline(f, dato);
		tok.CasosEspeciales(atoi(dato.c_str()));
		getline(f, dato);
		tok.PasarAminuscSinAcentos(atoi(dato.c_str()));
		getline(f, dato);
		tok.DelimitadoresPalabra(dato);

		f.close();
	}
	else
	{
		cerr << "ERROR: No se ha podido abrir el archivo para lectura" << "\n";
		return false;
	}
	return true;
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
	int posTerm 						= 0;
	indicePregunta.clear();
	infPregunta.numTotalPal 			= 0;
	infPregunta.numTotalPalDiferentes 	= 0;
	infPregunta.numTotalPalSinParada 	= 0;

	pregunta = preg;

	list<string> tokensPregunta;
	tok.Tokenizar(preg, tokensPregunta);

	infPregunta.numTotalPal 			= tokensPregunta.size();

	if(infPregunta.numTotalPal == 0)
	{
		cerr << "ERROR: pregunta con 0 terminos" << "\n";
		return false;
	}

	for(auto itTokens = tokensPregunta.begin(); itTokens != tokensPregunta.end(); ++itTokens)
	{
		stemmer.stemmer((*itTokens), tipoStemmer);
		auto itStopWords = stopWords.find((*itTokens));

		if(itStopWords == stopWords.end())	// No es una stop word
		{
			string palabra = *itTokens;
			auto itIndicePregunta = indicePregunta.find((*itTokens));

			if(itIndicePregunta != indicePregunta.end())		// El termino ya esta indexado en la pregunta
			{
				++itIndicePregunta->second.ft;
				itIndicePregunta->second.posTerm.push_back(posTerm);
			}
			else												// El termino no esta indexado en la pregunta
			{
				InformacionTerminoPregunta infoTerminoPreg;
				infoTerminoPreg.ft = 1;
				infoTerminoPreg.posTerm.push_back(posTerm);

				indicePregunta.insert({(*itTokens), infoTerminoPreg});


				++infPregunta.numTotalPalDiferentes;
			}
			++infPregunta.numTotalPalSinParada;
		}
		++posTerm;
	}
	if(infPregunta.numTotalPalSinParada == 0)
	{
		cerr << "ERROR: pregunta con 0 terminos" << "\n";
		return false;
	}
	else
		return true;
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
	cout << "Terminos indexados en la pregunta: ";// << "\n";
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
	auto itIndiceDocs = indiceDocs.find(nomDoc);

	if(itIndiceDocs != indiceDocs.end())
	{
		auto itIndice = indice.find(word);
		if(itIndice != indice.end())
		{
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
	return stopWords.size();
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
 * Descripcion: El metodo comprueba que nomDoc este indexado, si estÃ¡, obtiene su id. A continuacion recorre el indice
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


