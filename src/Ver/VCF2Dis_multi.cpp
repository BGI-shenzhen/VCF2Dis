#ifndef VCF2DiffMatrix_H_
#define VCF2DiffMatrix_H_
#include <zlib.h>
#include <omp.h>
#include <stdio.h>
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
#include <math.h>
#include <sys/time.h>
#include <algorithm>
#include "./ALL/kseq.h"
#include "GetFig.h"
#define Block  32
KSEQ_INIT(gzFile, gzread)
	typedef long long  llong ;
	using namespace std;


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
		"\t\t-Threads   <int>     Threads to use [auto]\n"
		"\t\t-Rand      <float>   Probability (0-1] for each site to join Calculation [1]\n"
		"\n"
		"\t\t-help                Show more help [hewm2008 v1.54m]\n"
		"\tCitation doi:     https://doi.org/10.1093/gigascience/giaf032\n"
                
		"\n";
}

void  VCF2Dis_More_help()
{
	cout <<""
		"\n\n"
		"\tUsage: VCF2Dis -i <in.vcf>  -o  <p_dis.mat>\n"

		"\t\t  -InFormat      <str>   Input File is [VCF/FA] Format,defaut[VCF]\n"
		"\t\t  -InSampleGroup <str>   InFile of sample Group info,format(sample groupA)\n"
		"\t\t  -TreeMethod    <int>   Construct Tree Method,1:NJ-tree 2:UPGMA-tree [1]\n"
		"\t\t  -KeepMF                Keep the Middle File diff & Use matrix\n"
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
		"\t\t  #5.0) Citation : https://doi.org/10.1093/gigascience/giaf032\n"
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
		else if (flag  ==  "InSampleGroup" || flag == "InPop"   || flag == "InPopInfo")
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->PopInfo=argv[i];
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
		else if (flag  ==  "Tree" ||  flag  ==  "TreeMethod" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->Tree=atoi(argv[i]);
			if (  ((paraFA04->Tree)<1 )  ||  ((paraFA04->Tree)>2) )
			{
				cerr  <<"Para [-TreeMethod] ratio should be in int 1/2\n";
				return 0;
			}
		}
		else if (flag  == "InFormat" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			string VCF=argv[i];

			if (  (VCF == "VCF")  || (VCF == "vcf")   )
			{
				paraFA04->InFormat=1;
			}
			else if  (  (VCF == "FA")  || (VCF == "fa")   )
			{
				paraFA04->InFormat=2;
			}
			else
			{
				paraFA04->InFormat=3;
			}
		}
		else if (flag  ==  "Seed" ||  flag  ==  "seed" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->Seed=atoi(argv[i]);
			if (  (paraFA04->Seed)<1 )
			{
				cerr  <<"Para [-Seed] ratio should be in int >=1 \n";
				return 0;
			}
		}
		else if (flag  ==  "Seed" ||  flag  ==  "seed" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->Seed=atoi(argv[i]);
			if (  (paraFA04->Seed)<1 )
			{
				cerr  <<"Para [-Seed] ratio should be in int >=1 \n";
				return 0;
			}
		}
		else if (flag  ==  "LimitNum" ||  flag  ==  "LimShowNameNum" )
		{
			if(i + 1 == argc) {LogLackArg(flag); return 0;}
			i++;
			paraFA04->LimShowNameNum=atoi(argv[i]);
			if (  (paraFA04->LimShowNameNum)<1 )
			{
				cerr  <<"Para [-LimitNum] ratio should be in int >=1 \n";
				return 0;
			}
		}
		else if (flag == "Threads"  ||  flag == "threads"  ||  flag == "Thread" )
		{
			if(i + 1== argc) {LogLackArg(flag);return 0;}
			i++;
			paraFA04->Threads=atoi(argv[i]);
			if ((paraFA04->Threads)<1)
			{
				(paraFA04->Threads)=1;
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
		else if (flag == "NoOUTtree")
		{
			paraFA04->OUTRtree=false;
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
				cerr<<"VCF Header something wrong, can't find sample info before site info"<<endl;
				cerr<<"VCF Header sample info Flag : [  #CHROM  ] "<<endl;
				return  0;
				break;
			}
		}
		INVCFA.close();
	}

	return  ALLsame ;
}


int VCF2DisMain(In3str1v *paraFA04)
{
	int file_count=(paraFA04->InVec).size();

	if (file_count> 1 )
	{
		if ((SameVCFHeader(paraFA04->InVec)) ==0)
		{
			return 0 ;
		}
	}

	ofstream  OUT ((paraFA04->InStr2).c_str());

	if((!OUT.good()))
	{
		cerr << "open OUT File error: "<<(paraFA04->InStr2)<<endl;
		return  0;
	}

	igzstream SNP (((paraFA04->InVec)[0]).c_str(),ifstream::in);
	if (SNP.fail())
	{
		cerr << "open SNP File error: "<<(paraFA04->InVec)[0]<<endl;
		return  0;
	}

	//	map <int , string > SampleID;
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
			if (!line.empty() && line[line.size() - 1] == '\r')  { line.erase(line.size() - 1);}
			if (line.length()<=0)  { continue  ; }
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
			cerr<<"VCF Header something wrong, can't find sample info before site info"<<endl;
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
			return 0;
		}
	}


	map <string ,string > TFSameName;

	for ( int ii=0; ii<sampleNumber ; ii++ )
	{
		int AA=(Vsample[SampleSite[ii]]).length();
		if  (AA>10)
		{
			int BB=AA;
			AA=AA-10;
			string NewName=(Vsample[SampleSite[ii]]).substr(AA,10);
			for (int attempt = 0; attempt < 29 ; ++attempt)
			{
				int siteAA=(rand() % (10));
				int siteBB=(rand() % (10));
				int type = rand() % 3;
				switch(type)
				{
					case 0:  NewName[siteAA]= 'a' + rand() % 26; NewName[siteBB]= 'a' + rand() % 26; break;
					case 1:  NewName[siteAA]= 'A' + rand() % 26; NewName[siteBB]= 'A' + rand() % 26; break;
					case 2:  NewName[siteAA]= '0' + rand() % 10; NewName[siteBB]= '0' + rand() % 10; break;
					default: NewName[siteAA]++;  NewName[siteBB]++;
				}

				if  (TFSameName.find(NewName)==TFSameName.end())
				{
					attempt=30;
				}
			}
			cerr<<"warning : Sample name too long [ "<<(Vsample[SampleSite[ii]])<<" ] length is "<<BB<<" biger (10byte),new Name is "<<NewName<<endl;
			TFSameName[NewName]=(Vsample[SampleSite[ii]]);
			(Vsample[SampleSite[ii]])=NewName;
		}
	}


	if ((paraFA04->Threads) <2)
	{
		(paraFA04->Threads)=2;
		if  (sampleNumber<100)
		{
			(paraFA04->Threads)=2;
		}
		else if  (sampleNumber<500)
		{
			 (paraFA04->Threads)=4;
		}
		else if  (sampleNumber<1000)
		{
			(paraFA04->Threads)=8;
		}
		else if  (sampleNumber<1500)
		{
			(paraFA04->Threads)=16;
		}
		else if  (sampleNumber>1500)
		{
			 (paraFA04->Threads)=32;
		}
		cout<<"auto use the Threads number is "<<(paraFA04->Threads)<<endl;
	}

	cout<<"Total Sample Number to construct p-distance matrix is [ "<<sampleNumber<<" ]\nStart To Cal ..."<<endl;

	if (!TFSameName.empty())
	{
		string  outsed=paraFA04->InStr2+".tmp.sed.sh";
		std::ofstream outFile(outsed.c_str());
		//		for (const std::pair<const std::string, std::string>& pair : TFSameName)
		//		{
		//			outFile << "sed  \'s/"<< pair.first<<":/"<<pair.second<<":/g\' $1 > $1.tmp ; mv $1.tmp  $1 ;" <<endl;
		//	}

		for (std::map<std::string, std::string>::iterator it = TFSameName.begin(); it != TFSameName.end(); ++it)
		{
			outFile << "sed  \'s/"<< it->first<<":/"<<it->second<<":/g\' $1 > $1.tmp ; mv $1.tmp  $1 ;" <<endl;
		}
		outFile.close();
	}


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

	int VecSizeNum=Vsample.size();
	//vector<string> inf(VecSizeNum);
	//	std::vector <const char*>   tokens
	std::vector <const char*>  VetLine [Block];
	string lineMM[Block];
	for(int i = 0; i < Block ; i++)
	{
		VetLine[i].resize(VecSizeNum);
		//new std::vector <const char*> [VecSizeNum];
		for(int j = 0; j < Block ; j++)
		{
			VetLine[j].push_back("NULL");
		}
	}
	//	vector<string> Ctmp (20)  ;
	//	vector<string> Btmp (20)  ;

	int BlockReadCount=0;

	if ( ( (paraFA04->Rand)>0)  &&  ((paraFA04->Rand)<1.0) )
	{
		short int Y=(short int)(paraFA04->Rand*100)-1;
		short int X;
		struct timeval tv; 
		gettimeofday(&tv, NULL);
		X=(rand()%100); X=(rand()%100);  // waste time
		gettimeofday(&tv, NULL);
		srand(tv.tv_sec + tv.tv_usec+((unsigned)time(NULL)));
		if ( paraFA04->Seed > 0)
		{
			srand(paraFA04->Seed);
		}
		//srand((unsigned)time(NULL));
		while(!SNP.eof())
		{
			lineMM[BlockReadCount].clear();
			getline(SNP,lineMM[BlockReadCount]);
			if (lineMM[BlockReadCount].length()<=0 )  { continue  ; }

			X=(rand()%100);
			if (X>Y) {continue ;}

			//split(line,inf," \t");

			split2(lineMM[BlockReadCount],VetLine[BlockReadCount],VecSizeNum);


			BlockReadCount++;

			if (BlockReadCount!=Block  &&  (!SNP.eof()))
			{
				continue;
			}
omp_set_num_threads(paraFA04->Threads);
#pragma omp parallel for schedule (dynamic)
			for(int jj=0; jj<sampleNumber ; jj++)
			{

				for(int kk=jj+1; kk<sampleNumber ; kk++)
				{
					for (int cc=0; cc<BlockReadCount ; cc++)
					{

						if  ( (VetLine[cc][SampleSite[jj]])[0] == '.')
						{
							continue ;
						}
						if  ( VetLine[cc][SampleSite[kk]][0] =='.') 
						{
							continue ;
						}
						if  (VetLine[cc][SampleSite[jj]][0]   != VetLine[cc][SampleSite[kk]][0])
						{
							DiffData[jj][kk]++;
						}

						if  (VetLine[cc][SampleSite[jj]][2]   != VetLine[cc][SampleSite[kk]][2])
						{
							DiffData[jj][kk]++;
						}

						SumData[jj][kk]+=2;
					}
				}
			}

			BlockReadCount=0;
		}





	}
	else
	{
		while(!SNP.eof())
		{
			lineMM[BlockReadCount].clear();
			getline(SNP,lineMM[BlockReadCount]);
			if (lineMM[BlockReadCount].length()<=0 )  { continue  ; }
			split2(lineMM[BlockReadCount],VetLine[BlockReadCount],VecSizeNum);

			//			std::istringstream iss(line);	for (int kkk=0 ; kkk<VecSizeNum ; kkk++ ){iss>>VetLine[BlockReadCount][kkk];}
			BlockReadCount++;
			if (BlockReadCount!=Block  &&  (!SNP.eof()))
			{
				continue;
			}
omp_set_num_threads(paraFA04->Threads);
#pragma omp parallel for schedule (dynamic)
			for(int jj=0; jj<sampleNumber ; jj++)
			{
				for(int kk=jj+1; kk<sampleNumber ; kk++)
				{
					for (int cc=0; cc<BlockReadCount ; cc++)
					{
						if  ( (VetLine[cc][SampleSite[jj]])[0] == '.')
						{
							continue ;
						}
						if  (VetLine[cc][SampleSite[kk]][0] =='.') 
						{
							continue ;
						}
						if  (VetLine[cc][SampleSite[jj]][0]   != VetLine[cc][SampleSite[kk]][0])
						{
							DiffData[jj][kk]++;
						}

						if  (VetLine[cc][SampleSite[jj]][2]   != VetLine[cc][SampleSite[kk]][2])
						{
							DiffData[jj][kk]++;
						}

						SumData[jj][kk]+=2;
					}
				}
			}

			BlockReadCount=0;

		}
	}




	SNP.close();


	if  ( file_count  > 1 )
	{
		for (int This =1 ; This< file_count ;  This ++ )
		{
			igzstream SNP (((paraFA04->InVec)[This]).c_str(),ifstream::in);
			if (SNP.fail())
			{
				cerr << "open SNP VCF File error: "<<((paraFA04->InVec)[This])<<endl;
				return 0;
			}

			while(!SNP.eof())
			{
				string  line ;
				getline(SNP,line);
				if (line.length()<=0 )  { continue  ; }
				else if (line[0] == '#' && line[1] == '#')  { continue ;}
				else if( line[0] == '#' && line[1] != '#')
				{
					break ;
				}
				else if ( line[0] != '#' && line[1] != '#' )
				{
					cerr<<"wrong Line : "<<line<<endl;
					cerr<<"VCF Header something wrong, can't find sample info before site info"<<endl;
					cerr<<"VCF Header sample info Flag : [  #CHROM  ] "<<endl;
					return  0;
					break;
				}
			}


			if ( ( (paraFA04->Rand)>0)  &&  ((paraFA04->Rand)<1.0) )
			{

				short int Y=(short int)(paraFA04->Rand*100)-1;
				short int X; 
				struct timeval tv; 
				gettimeofday(&tv, NULL);
				X=(rand()%100); X=(rand()%100);  // waste time
				gettimeofday(&tv, NULL);
				srand(tv.tv_sec + tv.tv_usec+((unsigned)time(NULL)));
				if ( paraFA04->Seed > 0 )
				{
					srand(paraFA04->Seed);
				}
				while(!SNP.eof())
				{

					lineMM[BlockReadCount].clear();
					getline(SNP,lineMM[BlockReadCount]);
					if (lineMM[BlockReadCount].length()<=0 )  { continue  ; }



					X=(rand()%100);
					if (X>Y) {continue ;}
					split2(lineMM[BlockReadCount],VetLine[BlockReadCount],VecSizeNum);
					//					std::istringstream iss(line);for (int kkk=0 ; kkk<VecSizeNum ; kkk++ ){ iss>>VetLine[BlockReadCount][kkk];}


					BlockReadCount++;

					if (BlockReadCount!=Block  &&  (!SNP.eof()))
					{
						continue;
					}
omp_set_num_threads(paraFA04->Threads);
#pragma omp parallel for schedule (dynamic)
					for(int jj=0; jj<sampleNumber ; jj++)
					{
						for(int kk=jj+1; kk<sampleNumber ; kk++)
						{
							for (int cc=0; cc<BlockReadCount ; cc++)
							{

								if  ( (VetLine[cc][SampleSite[jj]])[0] == '.')
								{
									continue ;
								}
								if  ( VetLine[cc][SampleSite[kk]][0] =='.') 
								{
									continue ;
								}
								if  (VetLine[cc][SampleSite[jj]][0]   != VetLine[cc][SampleSite[kk]][0])
								{
									DiffData[jj][kk]++;
								}

								if  (VetLine[cc][SampleSite[jj]][2]   != VetLine[cc][SampleSite[kk]][2])
								{
									DiffData[jj][kk]++;
								}

								SumData[jj][kk]+=2;
							}
						}
					}

					BlockReadCount=0;



				}
			}

			else
			{
				while(!SNP.eof())
				{
					lineMM[BlockReadCount].clear();
					getline(SNP,lineMM[BlockReadCount]);
					if (lineMM[BlockReadCount].length()<=0 )  { continue  ; }
					split2(lineMM[BlockReadCount],VetLine[BlockReadCount],VecSizeNum);


					BlockReadCount++;

					if (BlockReadCount!=Block  &&  (!SNP.eof()))
					{
						continue;
					}
omp_set_num_threads(paraFA04->Threads);
#pragma omp parallel for schedule (dynamic)
					for(int jj=0; jj<sampleNumber ; jj++)
					{
						for(int kk=jj+1; kk<sampleNumber ; kk++)
						{
							for (int cc=0; cc<BlockReadCount ; cc++)
							{

								if  ( (VetLine[cc][SampleSite[jj]])[0] == '.')
								{
									continue ;
								}
								if  ( VetLine[cc][SampleSite[kk]][0] =='.') 
								{
									continue ;
								}
								if  (VetLine[cc][SampleSite[jj]][0]   != VetLine[cc][SampleSite[kk]][0])
								{
									DiffData[jj][kk]++;
								}

								if  (VetLine[cc][SampleSite[jj]][2]   != VetLine[cc][SampleSite[kk]][2])
								{
									DiffData[jj][kk]++;
								}

								SumData[jj][kk]+=2;
							}
						}
					}

					BlockReadCount=0;




				}
			}
			SNP.close();


		}
	}




	if (BlockReadCount>0)
	{
omp_set_num_threads(paraFA04->Threads);
#pragma omp parallel for schedule (dynamic)
		for(int jj=0; jj<sampleNumber ; jj++)
		{
			for(int kk=jj+1; kk<sampleNumber ; kk++)
			{
				for (int cc=0; cc<BlockReadCount ; cc++)
				{

					if  ( (VetLine[cc][SampleSite[jj]])[0] == '.')
					{
						continue ;
					}
					if  ( VetLine[cc][SampleSite[kk]][0] =='.') 
					{
						continue ;
					}
					if  (VetLine[cc][SampleSite[jj]][0]   != VetLine[cc][SampleSite[kk]][0])
					{
						DiffData[jj][kk]++;
					}

					if  (VetLine[cc][SampleSite[jj]][2]   != VetLine[cc][SampleSite[kk]][2])
					{
						DiffData[jj][kk]++;
					}
					SumData[jj][kk]+=2;
				}
			}
		}

		BlockReadCount=0;




	}



	for(int i = 0; i < Block ; i++)
	{
		//delete [] VetLine[i] ;
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
	return 0;
}





int Fa2DisMain(In3str1v *paraFA04)
{

	ofstream  OUT ((paraFA04->InStr2).c_str());
	if((!OUT.good()))
	{
		cerr << "open OUT File error: "<<(paraFA04->InStr2)<<endl;
		return  0;
	}

	map <string,int > SubVetor;
	bool RunALL=true;
	map <string ,int>  ::iterator  key_it ;
	if (!(paraFA04->InStr3).empty())
	{
		igzstream SampleList ((paraFA04->InStr3).c_str(),ifstream::in);
		if (SampleList.fail())
		{
			cerr << "open Sub Group IN File error: "<<(paraFA04->InStr3)<<endl;
			return  0;
		}
		RunALL=false;
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
					SubVetor.insert(map <string ,int> ::value_type(inf[ii],-1));
				}
				else
				{
				}
			}
		}
		SampleList.close();
	}


	int file_count=(paraFA04->InVec).size();
	vector <string> InSeq ;
	int FlagCount=0;

	int l;
	llong SeqLength=0;



	gzFile fp;
	kseq_t *seq;

	for (int ii=0; ii<file_count ; ii++ )
	{
		fp = gzopen(((paraFA04->InVec)[ii]).c_str(),"r");
		seq = kseq_init(fp);

		while ((l = kseq_read(seq)) >= 0)
		{
			string chr=(seq->name.s);
			key_it=SubVetor.find(chr);
			if (RunALL)
			{
				if  (key_it==SubVetor.end())
				{
					SubVetor.insert(map <string ,int> ::value_type(chr,FlagCount));
				}
				else
				{
					cerr<<"Sample\t[ "<<chr<<" ] can be found [Repeat] in the Fa Files,please check it ..."<<endl;
					return 0;
				}
			}
			else
			{
				if  (key_it==SubVetor.end())
				{
					continue;
				}
				else
				{
					if (key_it->second==-1)
					{
						key_it->second=FlagCount;
					}
					else
					{
						cerr<<"Sample\t[ "<<chr<<" ] can be found [Repeat] in the Fa Files,please check it ..."<<endl;
						return 0;
					}
				}
			}


			llong seq_length = (seq->seq.l);
			if (SeqLength==0){SeqLength=seq_length;}
			else if (SeqLength!=seq_length)
			{
				cerr<<"Sample\t[ "<<chr<<" ] seq length "<<seq_length<<" is diff with the others Sample before :" <<SeqLength<<" "<<endl;
				return 0;
			}
			string RefSeq=(seq->seq.s);
			transform(RefSeq.begin(),RefSeq.end(),RefSeq.begin(),::toupper);
			InSeq.push_back(RefSeq); FlagCount++;
		}
	}

	kseq_destroy(seq);
	gzclose(fp);

	int sampleNumber=0;
	map <int,string>  SampleName;
	for(key_it=SubVetor.begin(); key_it!=SubVetor.end() ;  key_it++)
	{
		if ((key_it->second)==-1)
		{
			cerr<<"warning : Sample [ "<<(key_it->first)<<" ] can't be found in the Fa Files\n";
		}
		else
		{
			sampleNumber++;
			int AA=(key_it->first).length();
			string ChrName=(key_it->first);
			if  (AA>10)
			{
				cerr<<"warning : Sample name too long [ "<<(key_it->first)<<" ] length is "<<AA<<" biger (10byte)"<<endl;
				ChrName=(key_it->first).substr(0,10);
			}
			SampleName[key_it->second]=ChrName;
		}
	}

	if  (sampleNumber<5)
	{
		cerr<<"warning : Sample Number [ "<<sampleNumber<<" ] is too samll "<<endl;
		if  (sampleNumber<3)
		{
			cerr<<"It can't work since the sample Number too samll  "<<endl;
			return 0;
		}
	}

	if  (FlagCount!=sampleNumber)
	{
		cerr<<"warning : Sample Number [ "<<sampleNumber<<" is diff SeqNumber " <<FlagCount<<endl;
		cerr<<"It can't work"<<endl;
		return 0;
	}

	cout<<"Total Sample Number to construct p-distance matrix is [ "<<sampleNumber<<" ]\nStart To Cal ..."<<endl;

	if ((paraFA04->Threads) <2)
	{
		(paraFA04->Threads)=2;
		if  (sampleNumber<100)
		{
			(paraFA04->Threads)=2;
		}
		else if  (sampleNumber<500)
		{
			 (paraFA04->Threads)=4;
		}
		else if  (sampleNumber<1000)
		{
			(paraFA04->Threads)=8;
		}
		else if  (sampleNumber<1500)
		{
			(paraFA04->Threads)=16;
		}
		else if  (sampleNumber>1500)
		{
			 (paraFA04->Threads)=32;
		}
		cout<<"auto use the Threads number is "<<(paraFA04->Threads)<<endl;
	}




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

	bool Ascii[256] = {false};
	for (int cc=0 ; cc<256 ; cc++) {Ascii[cc]=false;}
	Ascii['A']=true;	Ascii['T']=true;	Ascii['G']=true;	Ascii['C']=true;


	if ( ( (paraFA04->Rand)>0)  &&  ((paraFA04->Rand)<1.0) )
	{

		short int Y=(short int)(paraFA04->Rand*100)-1;
		short int X;
		struct timeval tv; 
		gettimeofday(&tv, NULL);
		X=(rand()%100); X=(rand()%100);  // waste time
		gettimeofday(&tv, NULL);
		srand(tv.tv_sec + tv.tv_usec+((unsigned)time(NULL)));
		if ( paraFA04->Seed > 0 )
		{
			srand(paraFA04->Seed);
		}

		X=(rand()%100);
		bool *RandAyy ;
		RandAyy = new bool [SeqLength];
		for ( int  site=0 ; site<SeqLength ; site++)
		{
			X=(rand()%100);
			if (X>Y) {RandAyy[site] = true;}
			else
			{
				RandAyy[site] = false ;
			}
		}
omp_set_num_threads(paraFA04->Threads);
#pragma omp parallel for schedule (dynamic)
		for(int Sampleii=0; Sampleii<sampleNumber ; Sampleii++)
		{
			int Samplejj=Sampleii+1;
			for( ; Samplejj<sampleNumber ; Samplejj++)
			{
				for ( int  site=0 ; site<SeqLength ; site++)
				{
					if (RandAyy[site]) { continue ;}
					if  (InSeq[Sampleii][site] == 'N'  || InSeq[Sampleii][site] == '-' || InSeq[Sampleii][site] == '.')
					{
						continue ;
					}					
					if  (InSeq[Samplejj][site] == 'N'  || InSeq[Samplejj][site] == '-' || InSeq[Samplejj][site] == '.')
					{
						continue ;
					}
					if (InSeq[Sampleii][site] != InSeq[Samplejj][site])
					{
						if (Ascii[InSeq[Sampleii][site]]  &&   Ascii[InSeq[Samplejj][site]])
						{
							DiffData[Sampleii][Samplejj]+=2;
						}
						else
						{
							DiffData[Sampleii][Samplejj]+=1;
						}
					}
					SumData[Sampleii][Samplejj]+=2;
				}
			}
		}

		delete [] RandAyy ;

	}
	else
	{

omp_set_num_threads(paraFA04->Threads);
#pragma omp parallel for schedule (dynamic)
		for(int Sampleii=0; Sampleii<sampleNumber ; Sampleii++)
		{
			int Samplejj=Sampleii+1;
			for( ; Samplejj<sampleNumber ; Samplejj++)
			{
				for ( int  site=0 ; site<SeqLength ; site++)
				{

					if  (InSeq[Sampleii][site] == 'N'  || InSeq[Sampleii][site] == '-' || InSeq[Sampleii][site] == '.')
					{
						continue ;
					}					
					if  (InSeq[Samplejj][site] == 'N'  || InSeq[Samplejj][site] == '-' || InSeq[Samplejj][site] == '.')
					{
						continue ;
					}
					if (InSeq[Sampleii][site] != InSeq[Samplejj][site])
					{
						if (Ascii[InSeq[Sampleii][site]]  &&   Ascii[InSeq[Samplejj][site]])
						{
							DiffData[Sampleii][Samplejj]+=2;
						}
						else
						{
							DiffData[Sampleii][Samplejj]+=1;
						}
					}
					SumData[Sampleii][Samplejj]+=2;
				}
			}
		}








	}





	cout<<"Start To Create P_distance ..."<<endl;

	OUT<<setw(5)<<sampleNumber<<endl;

	for (int jj=0 ; jj< sampleNumber ;jj++ )
	{
		OUT<<setiosflags(ios::left)<<setw(12)<<SampleName[jj];
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
			OUTUSE<<setiosflags(ios::left)<<setw(12)<<SampleName[jj];
			OUTDiff<<setiosflags(ios::left)<<setw(12)<<SampleName[jj];
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
	return 0;
}





int main(int argc, char *argv[])
{
	In3str1v *paraFA04 = new In3str1v;
	if ((VCF2DiffMatrix_help01(argc, argv, paraFA04)==0))
	{
		delete paraFA04;
		return 0;
	}

	if (( paraFA04->InFormat)==1)
	{
		VCF2DisMain(paraFA04);
	}
	else if ((paraFA04->InFormat)==2)
	{
		Fa2DisMain(paraFA04);
	}
	else
	{
		int filenum=(paraFA04->InVec).size();
		if (filenum>1)
		{
			cerr<<"phy format only one input file,pls check it\n";
			delete paraFA04;
			return 0;
		}
		string III=(paraFA04->InVec)[0];
		string OOO=paraFA04->InStr2 + ".tmp.fa";
		singlePhy2Fa(III ,OOO);
		(paraFA04->InVec)[0]=OOO;
		Fa2DisMain(paraFA04);
		III="rm -rf "+OOO;
		std::system(III.c_str());
	}



	GetFig_Fun(paraFA04->InStr2,paraFA04);

	delete paraFA04;
	return 0;
}


#endif // VCF2DiffMatrix_H_  //
///////// swimming in the sky and flying in the sea ////////////




