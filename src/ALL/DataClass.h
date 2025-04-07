
////////////////////////swimming in the sea & flying in the sky //////////////////


/*
 * DataClass.h
 *
 *  Created on: 2011-11-21
 *      Author: hewm@genomics.org.cn
 */

#ifndef DataClass_H_
#define DataClass_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


///////////////// q_seq  for site ////////


class In3str1v {
	public:
		string InStr1 ;
		string InStr2 ;
		vector <string> InVec ;
		string InStr3 ;
		string PopInfo ;
		int Threads ;
		bool  TF ;
		bool  OUTRtree ;
		int   InFormat ;
		double Rand;
		int Seed ;
		int Tree ;
		int LimShowNameNum; //Limit the number of names displayed
		In3str1v()
		{
			InStr1="";
			InStr2="";
			InStr3="";
			PopInfo="";
			Threads=1;
			TF=false ;			
			OUTRtree=true;
			InFormat=1;
			Rand=1.0;
			Seed=0;
			Tree=1;
			LimShowNameNum=50;
		}
};


////////swimming in the sky and flying in the sea *///////////



#endif /* DataClass_H_ */

//////////////// swimming in the sky and flying in the sea ////////////////
