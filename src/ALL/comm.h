#ifndef comm_H_
#define comm_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <zlib.h>
#include "../gzstream/gzstream.h"

typedef long long llong ;
using namespace std;

inline void  LogLackArg( string  flag )
{
	cerr << "\t\tLack Argument for [ -"<<flag<<" ]"<<endl;    
}



string &  replace_all(string &  str,const  string &  old_Avalue,const string &  new_Avalue)
{
	while(true)   {
		string::size_type  pos(0);
		if(   (pos=str.find(old_Avalue))!=string::npos   )
			str.replace(pos,old_Avalue.length(),new_Avalue);
		else   break;
	}
	return   str;
}


void split(const string& str,vector<string>& tokens,  const string& delimiters = " ")
{
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos     = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
}




inline string Int2Str (llong  A )
{
	stringstream   sstrm ;
	sstrm  <<  A ;
	return  sstrm.str();
}


int ReadList (string soaplist  ,   vector <string> & Soap_AStat  )
{
	igzstream LIST (soaplist.c_str(),ifstream::in); // igzstream
	int soapfilecout=0 ;
	if (!LIST.good())
	{
		cerr << "open List error: "<<soaplist<<endl;
		return  soapfilecout ;
	}
	while(!LIST.eof())
	{
		string  line ;
		getline(LIST,line);
		if (line.length()<=0)  { continue  ; }
		Soap_AStat.push_back(line);
		soapfilecout++;
	}
	LIST.close();
	return  soapfilecout ;
}




#endif // comm_H_  ;


