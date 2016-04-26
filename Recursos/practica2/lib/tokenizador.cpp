#include "tokenizador.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <algorithm>
//#include <cctype>

using namespace std;

const string excepURL = "_:/.?&-=#@";
const string excepNUM = ",.";
const string numbers = "1234567890";
const string excepEMAIL = ".-_@";
const string excepHTTP = "http https ftp";
const string excepACRO = ".";
const string excepSIGNOS = "%$ºª";

Tokenizador::Tokenizador() :
		delimiters(",;:.-/+*\\ '\"{}[]()<>�!�?&#=\t\n\r@"), casosEspeciales(true), pasarAminuscSinAcentos(false)
{}

ostream& operator<<(ostream& os, const Tokenizador& t)
{
    os << "DELIMITADORES: " << t.delimiters << " TRATA CASOS ESPECIALES: " << t.casosEspeciales <<" PASAR A MINUSCULAS Y SIN ACENTOS: " << t.pasarAminuscSinAcentos;
    return os;
}

Tokenizador::Tokenizador(const Tokenizador &other)
{
    delimiters = other.delimiters;
    casosEspeciales = other.casosEspeciales;
    pasarAminuscSinAcentos = other.pasarAminuscSinAcentos;
}

// ??? no se si lo tendre que quitar.
Tokenizador::Tokenizador (const string& delimitadoresPalabra) :
    delimiters(delimitadoresPalabra)
{}

Tokenizador::Tokenizador (const string& delimitadoresPalabra, const bool& casosEspeciales, const bool& pasarAminusculasin) :
    delimiters(delimitadoresPalabra), casosEspeciales(casosEspeciales), pasarAminuscSinAcentos(pasarAminusculasin)
{}

Tokenizador::~Tokenizador ()
{
    delimiters = "";
}

Tokenizador& Tokenizador::operator= (const Tokenizador& other)
{
	if(this != &other)
		this->delimiters = other.delimiters;
		this->casosEspeciales=other.casosEspeciales;
		this->pasarAminuscSinAcentos=other.pasarAminuscSinAcentos;
    return (*this);
}

void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const
{
	//Coste de O(n*d) siendo n el tama�o de str y y d el tama�o del string de delimitadores.
    int indice;
    string::size_type lastPos = str.find_first_not_of(delimiters,0);
    string::size_type pos;
    bool num = true;

    if(casosEspeciales)
    	pos	  = str.find_first_of(" " +delimiters,lastPos);
    else
    	pos	  = str.find_first_of(delimiters,lastPos);


    if(!tokens.empty())
    	tokens.clear();

    //cout << "Dentro de tokenizar"<< endl;
    while(string::npos != pos || string::npos != lastPos)
	{
    	//cout << lastPos << "    " << pos << endl;
    	if(casosEspeciales)
    	{
			if(excepHTTP.find(str.substr(lastPos, str.find_first_of(':',lastPos) - lastPos), lastPos)!= string::npos)//Si empieza por http, https o ftp
    		{
				//procesar(str,tokens,lastPos, excepURL);
				indice = calculaIndice(lastPos,str,excepURL);
				hacerPushBack(lastPos, indice,str,tokens);
				lastPos = str.find_first_not_of(" "+delimiters, indice);

			}else if(numbers.find(str[lastPos])!=string::npos || ((str[lastPos] == ',' ||str[lastPos] == '.') && numbers.find(str[lastPos+1])!= string::npos))
			{
				indice = procesaNumeros(lastPos, str, excepNUM, num);

				if(indice == -1 && (str[str.size()-1] == ',' || str[str.size()-1] == '.' ))
				{
					hacerPushBack(lastPos,str.size()-1, str, tokens);
					lastPos = str.find_first_not_of(" "+delimiters, indice);
				}else if(num){

					if((str[lastPos-1] == '.' || str[lastPos-1] == ',') /*&& (delimiters.find('.') == string::npos || delimiters.find(',')== string::npos)*/)//cuando lastpos esta en el siguiente numero a la coma porque es delimitador el punto
						tokens.push_back("0" + str.substr(lastPos-1, indice - (lastPos-1)));
					else if (str[lastPos] == ',' || str[lastPos] == '.')//para .16 (cuando lastpos es el que tiene el coma)(no es delimitador el punto)
						tokens.push_back("0" + str.substr(lastPos, pos - lastPos));
					else//cualquier otro caso
						tokens.push_back(str.substr(lastPos, indice - lastPos));

					//Si el indice por donde voy a cortar es punto lo meto
					if(str[indice] == '.' || str[indice]==',')
						lastPos = str.find_first_not_of(" "+delimiters, indice+1);
					else
						lastPos = str.find_first_not_of(" " + delimiters, indice);
				}else
				{
					hacerPushBack(lastPos,indice, str, tokens);
					lastPos = str.find_first_not_of(" "+delimiters, indice);
				}

			}else if(excepEMAIL.find(str[pos]) != string::npos)//email
			{
				procesar(str,tokens,lastPos, excepEMAIL);
			}else//AQUI ES CUANDO NO TIENE EL DELIMITADOR PUESTO LOS ACRONIOMS Por lo tanto
			{
				//cuando el punto no es delimitador entrarÃ¡ aqui.
				if(str[pos-1] == '.' && str[pos] == ' ' && str.substr(lastPos, (pos -1 )-lastPos).find('.') != string::npos)//casos de terminar en punto
				{
					hacerPushBack(lastPos,pos -1,str,tokens);
					//tokens.push_back(str.substr(lastPos, (pos-1) - lastPos));
					lastPos = str.find_first_not_of(" "+delimiters, pos);
				}else if(str[lastPos] == '.' && str[lastPos+1] != '.' && str.substr(lastPos+1, pos - (lastPos)).find('.') != string::npos)//caso de empezar en punto
				{
					if(pasarAminuscSinAcentos)
						tokens.push_back(pasarMinusculasNoAcentos(str.substr(lastPos+1, pos - (lastPos+1))));
					else
						tokens.push_back(str.substr(lastPos+1, pos - (lastPos+1)));

					lastPos = str.find_first_not_of(" "+delimiters, pos);
				}else if(str[lastPos] == '-' && delimiters.find('-') != string::npos)
				{
					if(pasarAminuscSinAcentos)
						tokens.push_back(pasarMinusculasNoAcentos(str.substr(lastPos+1, pos - lastPos)));
					else
						tokens.push_back(str.substr(lastPos+1, pos - lastPos));
					lastPos = str.find_first_not_of(" "+delimiters, pos);
				}else{
					hacerPushBack(lastPos,pos,str,tokens);
					//tokens.push_back(str.substr(lastPos, pos - lastPos));
					lastPos = str.find_first_not_of(" "+delimiters, pos);
				}
			}
			pos = str.find_first_of(" " + delimiters, lastPos);
    	}else
    	{
    		hacerPushBack(lastPos,pos,str,tokens);
			//tokens.push_back(str.substr(lastPos, pos - lastPos));
			lastPos = str.find_first_not_of(delimiters, pos);
			pos = str.find_first_of(delimiters, lastPos);

    	}
	}
}

bool Tokenizador::Tokenizar(const string& i, const string& f) const
{
	//i.size()*tokens.size()
	ifstream entr;
    ofstream salid;
    string cadena;
    list<string> tokens;
    list<string>::iterator itS;
    //cout << i << endl;
    entr.open(i.c_str());
    salid.open(f.c_str());

    if(!entr){
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }else{
    	while(!entr.eof()){
            cadena = "";
            getline(entr, cadena);
            if(cadena.length() != 0)
            {
                Tokenizar(cadena, tokens);
                for(itS= tokens.begin() ; itS != tokens.end() ; itS++)
                    salid << (*itS) << endl;
            }
        }
    }
    entr.close();
    salid.close();
    return true;
}

bool Tokenizador::Tokenizar (const string & i) const
{
	/*
	string f;
	string s;
	if(i[i.size()-1] == '\r')
	{
		s = i.substr(0,i.size()-1);
		f = i.substr(0,i.size()-1)+".tk";
	}else
	{
		s = i;
		f = i+".tk";
	}
*/
	return Tokenizar(i, i+".tk");
	//if(Tokenizar( s, f)) return true;
	//else return false;
}

bool Tokenizador::TokenizarListaFicheros (const string& i) const
{
	bool correcto = false;
    ifstream entr;
    string cadena;
    entr.open(i.c_str());
    string aux;
    if(!entr){
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }else{
    	if(i == ".lista_fich")
    	{
    		cadena = "";
    		getline(entr, cadena);
    	}

        while(!entr.eof()){
            cadena = "";
            getline(entr, cadena);
            if(cadena.length() != 0){
            			if(Tokenizar(cadena)) correcto = true;
            }
        }
    }

    return correcto;
}

bool Tokenizador::TokenizarDirectorio (const string& i) const
{
	//cout << i << endl;
    struct stat dir;
    // Compruebo la existencia del directorio
    int err = stat(i.c_str(), &dir);

    if(err == -1 || !S_ISDIR(dir.st_mode)){
    	//cerr << "ERROR: No existe el directorio: " << i << endl;
        return false;
    }
    else {
    // Hago una lista en un fichero con find>fich
        string cmd = "find "+ i +" -follow |sort > .lista_fich";
        system(cmd.c_str());
        return TokenizarListaFicheros(".lista_fich");
    }
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters)
{
    /* A�ade al final de delimiters los nuevos delimitadores que aparezcan en
     * nuevoDelimiters (no se almacenan caracteres repetidos)
     * */
	/*string aux = delimiters + nuevoDelimiters;
	char anterior = 'a';
	sort(aux.begin(), aux.end());

	for( int i = 0; i< aux.size(); i++)
	{
		if(aux[i] != anterior) delimiters += aux[i];
		anterior = aux[i];
	}*/

	string del = nuevoDelimiters;
	string aux = "";
	char anterior = 'a';

	for( int i = 0; i< del.size(); i++)
	{
		if(delimiters.find(del[i]) == string::npos)
			delimiters += del[i];
	}
}
//---------------------------------------------PRIVADOS---------------------------------------------//
string Tokenizador::pasarMinusculasNoAcentos(const string& cadena)const
{
	string cad = "";
	for(int i = 0; i< cadena.size(); i++)
	{
		switch((unsigned char)cadena[i])
		{
			case 193:
			case 225:
				cad += 'a';
				break;
			case 201:
			case 233:
				cad += 'e';
			break;
			case 205:
			case 237:
				cad += 'i';
			break;
			case 211:
			case 243:
				cad += 'o';
			break;
			case 218:
			case 250:
				cad += 'u';
			break;
			case 209:
				cad += '�';
			break;
			default:
				cad += tolower(cadena[i]);
		}
	}
	return cad;
}

void Tokenizador::procesar(const string& str, list<string>& tokens, string::size_type &lastPos, const string excep)const
{
	int indice;
	int aux;
	indice = calculaIndice(lastPos, str, excep);
	//cout << str.substr(lastPos, indice-lastPos).find(delimiters) << endl;

	if(str.substr(lastPos, indice-lastPos).find('@') != string::npos)
	{//es email
		hacerPushBack(lastPos,indice,str,tokens);
		//tokens.push_back(str.substr(lastPos,indice - lastPos));
		lastPos = str.find_first_not_of(" "+delimiters, indice);
	}else// if(str.substr(lastPos, indice).find('_') != string::npos && delimiters.find('_'))
	{//no es email
		aux = calculaIndice(lastPos,str,"");
		if(str[aux] == '-'){ hacerPushBack(lastPos,indice,str,tokens); lastPos = str.find_first_not_of(" "+delimiters, indice);}
		else if (str[aux] == '.'){hacerPushBack(lastPos,indice,str,tokens); lastPos = str.find_first_not_of(" "+delimiters, indice);}
		else
		{
			hacerPushBack(lastPos,aux,str,tokens);
			lastPos = str.find_first_not_of(" "+delimiters, aux+1);
		}
		//tokens.push_back(str.substr(lastPos,indice - lastPos));
		//lastPos = str.find_first_not_of(" "+delimiters, indice);
	}
}

int Tokenizador::procesaNumeros(int last,const string& prueba,string excep,bool& aux) const
{
	for(int i = last; i < prueba.size() || i == string::npos; i++){
		//cout << "Esta es la posicion: " << i << ".Este es el valor numerico : " << (unsigned int)prueba[i] << endl;
		if(prueba[i] == ' ')//Si encuentro un blanco
		{
			if(prueba[i-1] == '.' || prueba[i-1] == ',')//Seguido de . o , devuelvo la pos anterior
				return i-1;
			else //sino la misma pos en blanco
				return i;
		}else if(excepSIGNOS.find(prueba[i]) != string::npos)//si encuentro un simbolo devuelvo hasta ahi
		{
			return i;
		}else if((unsigned int)prueba[i] == 4294967204)
		{
			//164
			//4294967204
			return i;
		}else if (delimiters.find(prueba[i]) != string::npos && (excep.find(prueba[i]) == string::npos)){//Si encuentro un delit diferente
			return i;
		}else if(prueba[i]=='.' && prueba[i+1]=='.')
		{
			return i;
		}/*else if((prueba[i]=='.'  || prueba[i]==',') && prueba.substr(i, string::npos - i).size() == 1)
		{
			return i-1;
		}*/else if(numbers.find(prueba[i]) != string::npos)
		{
			continue;
		}else if(excepNUM.find(prueba[i]) != string::npos)
		{
			continue;
		}else if (numbers.find(prueba[i]) == string::npos && ((prueba[i] >= 'a' && prueba[i]<= 'z' ) || (prueba[i]>= 'A' && prueba[i]<= 'Z' )))//es caracter
		{
			aux = false;
			if(prueba.find(',', last) != string::npos < prueba.find('.', last) != string::npos)
				return prueba.find(',', last);
			else
				return prueba.find('.', last);
		}
	}
	return string::npos;
}

int Tokenizador::calculaIndice(int last,const string& prueba,string excep) const
{
	int cont = 1;
	int p , m= 0;
	int punt = 1;
	for(int i = last; i < prueba.size() || i == string::npos; i++){
		if(prueba[i] == ' ')
		{
			if(prueba[i-1] == '.')//ALomejor arregla lo que acabe en punto
				return i-1;
			return i;
		}else

			if(excep.find(prueba[i])!=string::npos)
			{
				if(prueba[i] == '@' && delimiters.find(prueba[i]) != string::npos){
					if(cont == 1) p = i;
					if(cont > 1)return p;
					cont++;
				}
				if(prueba[i] == '.' && prueba[i+1] == '.')
					return i;

				continue;
			}
			else if(delimiters.find(prueba[i]) != string::npos)
			{
				if(prueba[i-1] == '.')
					return i-1;
				else
					return i;
			}
			/*else if(excepSIGNOS.find(prueba[i])!=string::npos){
				return i;
			}*/
	}
	return string::npos;
}

void Tokenizador::hacerPushBack(int inicio, int final, const string& cadena, list<string>& tokens)const
{
	if(pasarAminuscSinAcentos)
		tokens.push_back(pasarMinusculasNoAcentos(cadena.substr(inicio, final-inicio)));
	else
		tokens.push_back(cadena.substr(inicio, final-inicio));
}
