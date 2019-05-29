#ifndef VCF2DiffMatrix_H_
#define VCF2DiffMatrix_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iomanip>
#include "./ALL/comm.h"
#include "./ALL/DataClass.h"
#include <cstdlib>
#include "./gzstream/gzstream.C"
#include <zlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;
typedef long long  llong ;

void  VCF2DiffMatrixSNP_help()
{
	cout <<""
		"\n"
		"\tUsage: VCF2Dis -InPut  <in.vcf>  -OutPut  <p_dis.mat>\n"
		"\n"
		"\t\t-InPut     <str>     Input one or muti GATK VCF genotype File\n"
		"\t\t-OutPut    <str>     OutPut Sample p-Distance matrix\n"
		"\n"
		"\t\t-InList    <str>     Input GATK muti-chr VCF Path List\n"
		"\t\t-SubPop    <str>     SubGroup SampleList of VCFFile [ALLsample]\n"
		"\t\t-Rand      <float>   Probability (0-1] for each site to join Calculation [1]\n"
		"\t\t-KeepMF              Keep the Middle File diff & Use matrix\n"
		"\n"
		"\t\t-help                Show more help [hewm2008 v1.36]\n"
		"\n";
}

void  VCF2Dis_More_help()
{
	cout <<""
		"\n"
		"\tUsage: VCF2Dis -i <in.vcf>  -o  <p_dis.mat>\n"
		"\n"
		"\t\t  #1.0) Parameters can used as short letter\n"
		"\t\t          Such as : [-i] short for [-InPut], [-o] for [-OutPut],[-s] for [-SubPop], [-k] for [-KeepMF]\n"
		"\n"
		"\t\t  #2.1) To new all the sample p_distance matrix based VCF, run VCF2Dis directly\n"
		"\t\t           ./bin/VCF2Dis	-i	in.vcf.gz  -o p_dis.mat\n"
		"\n"
		"\t\t  #2.2) To new sub group sample p_distance matrix ; Put their sample name into File sample.list\n"
		"\t\t         ./bin/VCF2Dis	-InPut	chr1.vcf.gz chr2.vcf.gz  -OutPut p_dis.mat  -SubPop  sample.list\n"
		"\n"
		"\t\t  #3.0) Default use all site to join the Calculation. To run the bootstrap tree , can run muti-time with using part of site, Para [-Rand]\n"
		"\t\t         ./bin/VCF2Dis	-InPut	in.vcf.gz  -OutPut p_dis.mat   -Rand  0.25 \n"

		"\t\t  #4.0) see the web readme for more. hewm2008@gmail.com / hewm2008@qq.com /join the QQ Group : 125293663\n"
		"\n";
}


int VCF2DiffMatrix_help01(int argc, char **argv , In3str1v * paraFA04 )
{
	if (argc < 2 ) {VCF2DiffMatrixSNP_help();return 0;}
	int file_count=0;

	for(int i = 1; i < argc ; i++)
	{
		if(argv[i][0] != '-')
		{
			cerr << "command option error! please check." << endl;
			return 0;
		}
		string flag=argv[i] ;
		flag=replace_all(flag,"-","");

		if (flag  == "InPut" || flag == "i"   || flag == "I")
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			string A=argv[i];
			(paraFA04->InVec).push_back(A);
			file_count++;
			bool RunT=true;
			while(RunT)
			{
				if ( (i+1) < argc  && (argv[i+1][0]!='-'))
				{
					i++;
					A=argv[i];
					(paraFA04->InVec).push_back(A);
					file_count++;
				}
				else
				{
					RunT=false;
				}
			}
		}
		else if (flag  ==  "OutPut" || flag == "o"  || flag == "O" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->InStr2=argv[i];
		}
		else if (flag  ==  "InList" || flag == "L"  || flag == "l" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			string A=argv[i];
			file_count+=(ReadList (A ,(paraFA04->InVec)));
		}
		else if (flag  ==  "SubPop" || flag == "s"   || flag == "S")
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->InStr3=argv[i];
		}
		else if (flag  ==  "Rand" || flag == "r"  || flag == "R" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->Rand=atof(argv[i]);
			if (  (paraFA04->Rand)>0  &&  (paraFA04->Rand)<=1.0) {} else
			{
				cerr  <<"Para [-Rand] ratio should be in the Number region (0,1]\n";
				return 0;
			}
		}

		else if (flag == "help" || flag == "h"  || flag == "H")
		{
			VCF2Dis_More_help();return 0;
		}
		else if (flag == "KeepMF" || flag == "k"   || flag == "K")
		{
			paraFA04->TF=true ;
		}
		else
		{
			cerr << "UnKnow argument -"<<flag<<endl;
			return 0;
		}
	}

	if  (  file_count<1 )
	{
		cerr<< "In File empty [-InPut]  [-InList], must set one of them "<<endl ;
		return 0;
	}

	if  ((paraFA04->InStr2).empty()  )
	{
		cerr<< "lack argument for [-OutPut] the must"<<endl ;
		return 0;
	}
	return 1 ;
}



int SameVCFHeader ( vector <string>  Files )
{
	int file_count=Files.size();
	string  FirstHeader="";
	int ALLsame=1;
	for ( int ii=0 ; ii<file_count ; ii++)
	{
		igzstream INVCFA ((Files[ii]).c_str(),ifstream::in);
		if (INVCFA.fail())
		{
			cerr << "open SNP VCF File error: "<<(Files[ii])<<endl;
			return 0;
		}
		int Count =0 ; 
		while( (!INVCFA.eof())  && Count <88888 )
		{
			string  line ;
			getline(INVCFA,line);
			Count++;
			if (line.length()<=0 )  { continue  ; }
			else if (line[0] == '#' && line[1] == '#')  { continue ;}
			else if( line[0] == '#' && line[1] != '#')
			{
				vector <string> Vsample;
				split(line,Vsample," \t");
				if  ( Vsample[0]  == "#CHROM")  
				{
					if (FirstHeader =="")
					{
						FirstHeader=line;
					}
					else
					{
						if  (FirstHeader  != line )
						{
							cerr << "This VCF File header diff with before"<<(Files[ii])<<endl;
							cerr << FirstHeader<<"\n"<<line<<endl;
							ALLsame=0;
							return 0;
						}
					}
				}
				break;
			}
			else if ( line[0] != '#' && line[1] != '#' )
			{
				cerr<<"wrong Line : "<<line<<endl;
				cerr<<"VCF Header same thing wrong, can find sample info before site info"<<endl;
				cerr<<"VCF Header sample info Flag : [  #CHROM  ] "<<endl;
				return  0;
				break;
			}
		}
		INVCFA.close();
	}

	return  ALLsame ;
}



int main(int argc, char *argv[])
{
	In3str1v *paraFA04 = new In3str1v;
	if ((VCF2DiffMatrix_help01(argc, argv, paraFA04)==0))
	{
		delete paraFA04 ;
		return 0 ;
	}


	int file_count=(paraFA04->InVec).size();
	if (file_count> 1 )
	{
		if ((SameVCFHeader(paraFA04->InVec)) ==0)
		{
			delete paraFA04 ;
			return 0 ;
		}
	}

	ofstream  OUT ((paraFA04->InStr2).c_str());

	if((!OUT.good()))
	{
		cerr << "open OUT File error: "<<(paraFA04->InStr2)<<endl;
		delete  paraFA04 ; return  0;
	}

	igzstream SNP (((paraFA04->InVec)[0]).c_str(),ifstream::in);
	if (SNP.fail())
	{
		cerr << "open SNP File error: "<<(paraFA04->InVec)[0]<<endl;
		delete  paraFA04 ; return  0;
	}

	map <int , string > SampleID;
	int sampleNumber=0;
	map <string ,int>  ::iterator  key_it ;

	map <string ,int >  SubVetor;
	vector<string> Vsample;

	if (!(paraFA04->InStr3).empty())
	{
		igzstream SampleList ((paraFA04->InStr3).c_str(),ifstream::in);
		if (SampleList.fail())
		{
			cerr << "open Sub Group IN File error: "<<(paraFA04->InStr3)<<endl;
			return  0;
		}

		while(!SampleList.eof())
		{
			string  line ;
			getline(SampleList,line);
			if (line.length()<=0 || line[0] == '#' )  { continue  ; }
			vector<string> inf ;
			split(line,inf," \t");
			int A=inf.size();
			for(int ii=0 ; ii<A ; ii++)
			{
				key_it=SubVetor.find(inf[ii]);
				if (key_it==SubVetor.end())
				{
					SubVetor.insert(map <string ,int> ::value_type(inf[ii],0));
				}
				else
				{
					//			(it->second)++;
				}
			}
		}
		SampleList.close();
	}

	vector <int> SampleSite;

	while(!SNP.eof())
	{
		string  line ;
		getline(SNP,line);
		if (line.length()<=0 )  { continue  ; }
		else if (line[0] == '#' && line[1] == '#')  { continue ;}
		else if( line[0] == '#' && line[1] != '#')
		{
			split(line,Vsample," \t");
			if  ( Vsample[0]  != "#CHROM")  
			{
				continue ;
			}
			int A=Vsample.size();

			if (!(paraFA04->InStr3).empty())
			{
				for(int ii=9 ; ii< A ; ii++)
				{
					key_it=SubVetor.find(Vsample[ii]);
					if (key_it!=SubVetor.end())
					{
						SampleSite.push_back(ii);
						(key_it->second)++;
					}
				}
			}
			else
			{
				for(int ii=9 ; ii< A ; ii++)
				{
					SampleSite.push_back(ii);
					sampleNumber++;
				}
			}
			break ;
		}
		else if ( line[0] != '#' && line[1] != '#' )
		{
			cerr<<"wrong Line : "<<line<<endl;
			cerr<<"VCF Header same thing wrong, can find sample info before site info"<<endl;
			cerr<<"VCF Header sample info Flag : [  #CHROM  ] "<<endl;
			return  0;
			break;
		}
	}


	for(key_it=SubVetor.begin(); key_it!=SubVetor.end() ;  key_it++)
	{
		if ((key_it->second)==0)
		{
			cerr<<"warning : Sample [ "<<(key_it->first)<<" ] can't be found in the VCF Header\n";
		}
		else if  ((key_it->second)>1)
		{
			cerr<<"warning : Sample [ "<<(key_it->first)<<" ] can be found [Repeat] in the VCF Header\n";
		}
	}

	sampleNumber=SampleSite.size();
	if  (sampleNumber<6)
	{
		cerr<<"warning : Sample Number [ "<<sampleNumber<<" ] is too samll "<<endl;
		if  (sampleNumber<3)
		{
			cerr<<"It can't work since the sample Number too samll  "<<endl;
			delete paraFA04 ;
			return 0;
		}
	}




	for(int ii=0; ii<sampleNumber ; ii++)
	{
		int AA=(Vsample[SampleSite[ii]]).length();
		if  (AA>12)
		{
			cerr<<"warning : Sample name too long [ "<<(Vsample[SampleSite[ii]])<<" ] length is "<<AA<<" biger (12byte)"<<endl;
			(Vsample[SampleSite[ii]])=(Vsample[SampleSite[ii]]).substr(0,12);
		}
	}

	cout<<"Total Sample Number to construct p-distance matrix is [ "<<sampleNumber<<" ]\nStart To Cal ..."<<endl;


	map <string  ,int > Sum;
	map <string  ,int > Diff;

	int **SumData;
	int **DiffData;

	SumData = new int*[sampleNumber];
	DiffData = new int*[sampleNumber];
	for(int j=0;j<sampleNumber;j++)
	{
		SumData[j] = new int[sampleNumber];
		DiffData[j] = new int[sampleNumber];
	}

	for (int i=0;i<sampleNumber;i++)
	{
		for (int j=0;j<sampleNumber;j++)
		{
			SumData[i][j]=0; 
			DiffData[i][j]=0; 
		}
	}


	int VecSizeNum=Vsample.size()+4;
	vector<string> inf(VecSizeNum);
	vector<string> Ctmp (20)  ;
	vector<string> Btmp (20)  ;

	if ( ( (paraFA04->Rand)>0)  &&  ((paraFA04->Rand)<1.0) )
	{

		short int Y=(short int)(paraFA04->Rand*100)-1;
		short int X; 
		srand((unsigned)time(NULL));
		string  line ;
		while(!SNP.eof())
		{			
			line.clear();
			getline(SNP,line);
			if (line.length()<=0 )  { continue  ; }
			X=(rand()%100);
			if (X>Y) {continue ;}

			inf.clear();
			split(line,inf," \t");
		
			for(int jj=0; jj<sampleNumber ; jj++)
			{
				Btmp.clear();
				split(inf[SampleSite[jj]], Btmp,":");
				if  ( (Btmp[0] =="./.")  ||  (Btmp[0] ==".|." ) ||  (Btmp[0] ==".") )
				{
					continue ;
				}

				for(int kk=jj+1; kk<sampleNumber ; kk++)
				{
					Ctmp.clear();
					split(inf[SampleSite[kk]], Ctmp,":");
					if  ( (Ctmp[0] =="./.")  ||  (Ctmp[0] ==".|." ) ||  (Ctmp[0] ==".") )
					{
						continue ;
					}

					if (Btmp[0][0] != Ctmp[0][0] )
					{
						DiffData[jj][kk]++;
					}
					if (Btmp[0][2] != Ctmp[0][2] )
					{
						DiffData[jj][kk]++;
					}

					SumData[jj][kk]+=2;
	
				}
			}
		}







	}
	else
	{
		string  line ;
		while(!SNP.eof())
		{
			line.clear();
			getline(SNP,line);
			if (line.length()<=0 )  { continue  ; }

			inf.clear();
			split(line,inf," \t");

			for(int jj=0; jj<sampleNumber ; jj++)
			{
				Btmp.clear();
				split(inf[SampleSite[jj]], Btmp,":");
				if  ( (Btmp[0] =="./.")  ||  (Btmp[0] ==".|." ) ||  (Btmp[0] ==".") )
				{
					continue ;
				}
				for(int kk=jj+1; kk<sampleNumber ; kk++)
				{
					Ctmp.clear();
					split(inf[SampleSite[kk]], Ctmp,":");
					if  ( (Ctmp[0] =="./.")  ||  (Ctmp[0] ==".|." ) ||  (Ctmp[0] ==".") )
					{
						continue ;
					}

					if (Btmp[0][0] != Ctmp[0][0] )
					{
						DiffData[jj][kk]++;
					}
					if (Btmp[0][2] != Ctmp[0][2] )
					{
						DiffData[jj][kk]++;
					}

					SumData[jj][kk]+=2;

				}
			}

		}
	}




	SNP.close();




	if  ( file_count  > 1 )
	{
		for (int This =1 ; This< file_count ;  This ++ )
		{
			igzstream INVCFB (((paraFA04->InVec)[This]).c_str(),ifstream::in);
			if (INVCFB.fail())
			{
				cerr << "open SNP VCF File error: "<<((paraFA04->InVec)[This])<<endl;
				return 0;
			}

			string  line ;
			while(!INVCFB.eof())
			{
				line.clear();
				getline(INVCFB,line);
				if (line.length()<=0 )  { continue  ; }
				else if (line[0] == '#' && line[1] == '#')  { continue ;}
				else if( line[0] == '#' && line[1] != '#')
				{
					break ;
				}
				else if ( line[0] != '#' && line[1] != '#' )
				{
					cerr<<"wrong Line : "<<line<<endl;
					cerr<<"VCF Header same thing wrong, can find sample info before site info"<<endl;
					cerr<<"VCF Header sample info Flag : [  #CHROM  ] "<<endl;
					return  0;
					break;
				}
			}


			if ( ( (paraFA04->Rand)>0)  &&  ((paraFA04->Rand)<1.0) )
			{

				short int Y=(short int)(paraFA04->Rand*100)-1;
				short int X; 
				string  line ;
				while(!INVCFB.eof())
				{
					line.clear();
					getline(INVCFB,line);
					if (line.length()<=0 )  { continue  ; }
					X=(rand()%100);
					if (X>Y) {continue ;}

					inf.clear();
					split(line,inf," \t");
	

					for(int jj=0; jj<sampleNumber ; jj++)
					{
						Btmp.clear();
						split(inf[SampleSite[jj]], Btmp,":");
						if  ( (Btmp[0] =="./.")  ||  (Btmp[0] ==".|." ) ||  (Btmp[0] ==".") )
						{
							continue ;
						}
						for(int kk=jj+1; kk<sampleNumber ; kk++)
						{
							Ctmp.clear();
							split(inf[SampleSite[kk]], Ctmp,":");
							if  ( (Ctmp[0] =="./.")  ||  (Ctmp[0] ==".|." ) ||  (Ctmp[0] ==".") )
							{
								continue ;
							}

							if (Btmp[0][0] != Ctmp[0][0] )
							{
								DiffData[jj][kk]++;
							}
							if (Btmp[0][2] != Ctmp[0][2] )
							{
								DiffData[jj][kk]++;
							}

							SumData[jj][kk]+=2;

						}
					}

				}
			}

			else
			{
				while(!INVCFB.eof())
				{
					string  line ;
					getline(INVCFB,line);
					if (line.length()<=0 )  { continue  ; }


					inf.clear();
					split(line,inf," \t");

					for(int jj=0; jj<sampleNumber ; jj++)
					{
						Btmp.clear();
						split(inf[SampleSite[jj]], Btmp,":");
						if  ( (Btmp[0] =="./.")  ||  (Btmp[0] ==".|." ) ||  (Btmp[0] ==".") )
						{
							continue ;
						}
						for(int kk=jj+1; kk<sampleNumber ; kk++)
						{
							Ctmp.clear();
							split(inf[SampleSite[kk]], Ctmp,":");
							if  ( (Ctmp[0] =="./.")  ||  (Ctmp[0] ==".|." ) ||  (Ctmp[0] ==".") )
							{
								continue ;
							}
							if (Btmp[0][0] != Ctmp[0][0] )
							{
								DiffData[jj][kk]++;

							}
							if (Btmp[0][2] != Ctmp[0][2] )
							{
								DiffData[jj][kk]++;
							}

							SumData[jj][kk]+=2;

						}
					}

				}
			}


			INVCFB.close();


		}
	}


	cout<<"Start To Create P_distance ..."<<endl;

	OUT<<setw(5)<<sampleNumber<<endl;

	for (int jj=0 ; jj< sampleNumber ;jj++ )
	{
		OUT<<setiosflags(ios::left)<<setw(12)<<Vsample[SampleSite[jj]];
		for (int kk=0 ; kk< sampleNumber ;kk++)
		{
			if  (jj==kk)
			{
				OUT<<setiosflags(ios::fixed)<<setprecision(6)<<"\t"<<0.0;
				continue ;
			}
			else if  (jj>kk)
			{
				SumData[jj][kk]=SumData[kk][jj];
				DiffData[jj][kk]=DiffData[kk][jj];
			}
			int Count=SumData[jj][kk];
			int Diff_here=DiffData[jj][kk];

			double p_dis=Diff_here*1.0/Count;

			OUT<<setiosflags(ios::fixed)<<setprecision(6)<<"\t"<<p_dis;
		}
		OUT<<endl;
	}
	OUT.close();



	if (paraFA04->TF)
	{
		string Ain=(paraFA04->InStr2)+".Use";
		string BinA=(paraFA04->InStr2)+".Diff";

		ofstream  OUTUSE (Ain.c_str());
		ofstream  OUTDiff (BinA.c_str());

		OUTUSE<<setw(5)<<sampleNumber<<endl;
		OUTDiff<<setw(5)<<sampleNumber<<endl;

		for (int jj=0 ; jj< sampleNumber ;jj++ )
		{
			OUTUSE<<setiosflags(ios::left)<<setw(12)<<Vsample[SampleSite[jj]];
			OUTDiff<<setiosflags(ios::left)<<setw(12)<<Vsample[SampleSite[jj]];
			for (int kk=0 ; kk< sampleNumber ;kk++)
			{
				if  (jj==kk)
				{
					OUTUSE<<"\t"<<0;
					OUTDiff<<"\t"<<0;
					continue ;
				}

				double Count=SumData[jj][kk]*0.5;
				double Diff_here=DiffData[jj][kk]*0.5;


				OUTUSE<<setiosflags(ios::fixed)<<setprecision(1)<<"\t"<<Count;
				OUTDiff<<setiosflags(ios::fixed)<<setprecision(1)<<"\t"<<Diff_here;
			}
			OUTUSE<<endl;
			OUTDiff<<endl;
		}
		OUTUSE.close();
		OUTDiff.close();


	}


	for (int i=0;i<sampleNumber;i++)
	{
		delete[] SumData[i];
		delete[] DiffData[i];

	}                     
	delete[] SumData; 
	delete[] DiffData; 


	cout<<"P_distance is created done ..."<<endl;
	delete paraFA04 ;
	return 0;
}
#endif // VCF2DiffMatrix_H_  //
///////// swimming in the sky and flying in the sea ////////////






