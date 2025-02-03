#ifndef GetFig_H_
#define GetFig_H_


using namespace std;


bool fileExists(const std::string& filename) {
    return (access(filename.c_str(), F_OK) != -1);
}


int GetFig_Fun(string OutPut,  In3str1v * paraFA04)
{
	if(!(paraFA04->OUTRtree))
	{
	   return 1;
	}
	char   buf[2048]={'\0'};
	string cc="which  Rscript   2> /dev/null ";
	memset( buf, '\0', sizeof(buf) );
	FILE   *stream ;
	stream=popen(cc.c_str(),"r");
	fread( buf, sizeof(char), sizeof(buf), stream);
	string binPath=buf;
	binPath=binPath.substr(0,binPath.length()-1);

	if (binPath == "")
	{
		cout <<"\twarning: can't find the [Rscript ] in your $PATH ; no tree and pdf Out"<<endl;
		cout <<"\tRscript\tvistreecode.r\t"<<OutPut<<endl;
		cout <<"\tor see more at\n\t\t[ https://github.com/hewm2008/VCF2Dis/blob/main/OtherWay2ConstructTree.md ]\t"<<OutPut<<endl;
	}
	else
	{

		string OUT=OutPut;
		string ext =OUT.substr(OUT.rfind('.') ==string::npos ? OUT.length() : OUT.rfind('.') + 1);

		if ( ext == "mat" || ext == "Mat" || ext == "MAT" )
		{
			OUT=OutPut.substr(0,OutPut.length()-4);
		}
		else if ( ext == "gz" )
		{
			OUT=OutPut.substr(0,OutPut.length()-3);
		}

		string  RshellPath=OUT+".tmp.r";
		string  OUTnwk=OUT+".nwk";
		string  OUTpdf=OUT+".pdf";
		string LimitNum=Int2Str(paraFA04->LimShowNameNum);
		ofstream  RR (RshellPath.c_str());


		if((!RR.good()))
		{
			cerr << "open OUT File error: "<<RshellPath<<endl;
			return  0;
		}


		string PopInfo="\n";
		if (!(paraFA04->PopInfo).empty())
		{
			PopInfo="pop_info <- read.table(\""+(paraFA04->PopInfo)+"\", header = FALSE, stringsAsFactors = FALSE, col.names = c(\"label\", \"group\"))\n"
		"match_indices <- match(tree$tip.label, pop_info$label)\n"
		"sample_groups[!is.na(match_indices)] <- pop_info$group[match_indices[!is.na(match_indices)]]\n"
		;
		}

		string TreeMethod="\n";
		if ((paraFA04->Tree)==2)
		{
			TreeMethod="tree <-  as.phylo(hclust(dist_matrix, method = \"average\")) \n" ;
		}
		else
		{
			TreeMethod="tree <- nj(dist_matrix)\n";
		}


string  Rshell="#!/usr/bin/Rscript\n"
"\n"
"args <- commandArgs(trailingOnly = TRUE)\n"
"InMatFile <-args[1]\n"
"\n"
"\n"
"if (length(args) != 1) \n"
"{\n"
"   cat(\"\\t\\tUsage: Rscript vistreecode.r  p_dis.mat \\n\")\n"
"   cat(\"\\t\\tPlease provide the input matrix file p_dis.mat \\n\")\n"
"   q(\"no\")\n"
"}\n"
"\n"
"if (!file.exists(InMatFile))\n"
"{\n"
"   cat(\"\\t\\tInPut p_dis.mat is no exists,please check\\n\")\n"
"   q(\"no\")\n"
"}\n"

"#install and load APE package\n"
"if(!require(ape)) install.packages(\"ape\")\n"
"library(ape)\n"
"\n"
"# read data from file \n"
"matrix_data <- read.table(InMatFile, header=F, row.names=1,skip=1)\n"
"dist_matrix <- as.dist(as.matrix(matrix_data))\n"
"\n"
"# Construct a tree using UPGMA/NJ method\n"
+TreeMethod+
"# print tree\n"
"#pdf(\"treeA.pdf\") ;print(tree); dev.off();\n"
"\n"
"# save the tree to Newick format file\n"
"write.tree(tree, file=\""+OUTnwk+"\")\n"
"\n"
"if (!require(\"BiocManager\", quietly = TRUE))  install.packages(\"BiocManager\")\n"
"if(!require(ggtree))  BiocManager::install(\"ggtree\")	\n"
"library(ggtree)\n"
"if(!require(dplyr))  BiocManager::install(\"dplyr\") \n"
"sample_groups <- rep(\"UnKnow\", length(tree$tip.label)) \n"
+PopInfo+
"group_info <- data.frame(label = tree$tip.label, group = sample_groups)\n"
"tree<-full_join(tree,group_info,by=\"label\")\n"
"tree@extraInfo$group[is.na(tree@extraInfo$group)]<-\"ZZBranches\"\n"
"num_tips <- length(tree@phylo$tip.label)\n"
"if ( num_tips  < "+LimitNum+") {\n"
" g <- ggtree(tree, layout = \"equal_angle\", aes(label = label,color = group)) + geom_tiplab()  \n"
"} else {\n"
" g <- ggtree(tree,layout=\"equal_angle\",aes(color = group))\n"
"}\n"
"# save tree pdf\n"
"ggsave(\""+OUTpdf+"\", g)\n"
"\n"
"\n";


	RR<<Rshell<<endl;
	RR.close();

		string	command=binPath+"\t"+RshellPath+"\t"+OutPut+" 2> /dev/null > /dev/null ";
if (std::system(command.c_str())==0)
{
	 cout<<"Construct tree and pdf done ..."<<endl;
}
else
{
	cerr<<"\tWarning: ["+binPath+"] should install latest R package : [ape] and [ggtree] first"<<endl;
}


	string  sedshell=paraFA04->InStr2+".tmp.sed.sh";
    if ( (fileExists(OUTnwk))  && (fileExists(sedshell)) )
	{
	 	cout<<"Re-Construct tree back by raw name ..."<<endl;
		string  OUTnwk2=OUT+"_2.nwk";
		command="cp\t"+OUTnwk+"\t"+OUTnwk2+"; sh "+ sedshell+" "+ OUTnwk2+" ;  rm  "+ sedshell  ;
		std::system(command.c_str());
string  Rshell="#!/usr/bin/Rscript\n"
"if (!require(\"BiocManager\", quietly = TRUE))  install.packages(\"BiocManager\")\n"
"if(!require(ggtree))  BiocManager::install(\"ggtree\")	\n"
"library(ggtree)\n"
"if(!require(dplyr))  BiocManager::install(\"dplyr\") \n"
"tree <- read.tree(\""+OUTnwk2+"\")\n"
"sample_groups <- rep(\"UnKnow\", length(tree$tip.label)) \n"
+PopInfo+
"group_info <- data.frame(label = tree$tip.label, group = sample_groups)\n"
"tree<-full_join(tree,group_info,by=\"label\")\n"
"tree@extraInfo$group[is.na(tree@extraInfo$group)]<-\"ZZBranches\"\n"
"num_tips <- length(tree@phylo$tip.label)\n"
"if ( num_tips  < "+LimitNum+") {\n"
" g <- ggtree(tree, layout = \"equal_angle\", aes(label = label,color = group)) + geom_tiplab()  \n"
"} else {\n"
" g <- ggtree(tree,layout=\"equal_angle\",aes(color = group))\n"
"}\n"
"# save tree pdf\n"
"ggsave(\""+OUTpdf+"\", g)\n"
"\n"
"\n";

		ofstream  RR (RshellPath.c_str());
		RR<<Rshell<<endl;
		RR.close();

		string	command=binPath+"\t"+RshellPath+" 2> /dev/null > /dev/null ";
		std::system(command.c_str());

	}

	if  (fileExists(OUTpdf))
	{
	command="rm -rf "+RshellPath;
	std::system(command.c_str());
	}
}

return 1;
}



/*//
3 30
Taxon1     ACCGTTTCCACAGCATTATGG	GCTCGATGA
Taxon2     CACTTCACAAATCAATATTGA	GCTCGATGA
Taxon3     TAAGGTATTGGGCTTGGTTCG	GCTCGATGA
//
*/


int singlePhy2Fa(string III ,string OOO) 
{
	
	igzstream inputFile (III.c_str(),ifstream::in);
	ofstream  outputFile(OOO.c_str());

	if (!inputFile || !outputFile) 
	{
		cerr << "Error opening file" << endl;
		return -1;
	}

	string line;
	getline(inputFile, line);
	// Read Phylip file
	while (getline(inputFile, line)) {
		if (!line.empty())
		{
			size_t pos = line.find_first_of(" \t");
			 string name = line.substr(0, pos);
			 string sequence = line.substr(pos + 1);
			 sequence=replace_all(sequence," ","");
			 outputFile << ">" << name << "\n" << sequence << endl;
		}
	}

	inputFile.close();
	outputFile.close();
	return 0;
}

#endif
