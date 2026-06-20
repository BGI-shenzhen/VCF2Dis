#ifndef GetFig_H_
#define GetFig_H_


using namespace std;


bool fileExists(const string& filename) {
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
if (system(command.c_str())==0)
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
		system(command.c_str());
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
		system(command.c_str());

	}

	if  (fileExists(OUTpdf))
	{
	command="rm -rf "+RshellPath;
	system(command.c_str());
	}
}

return 1;
}




int GetPopTreeFig_Fun(string OutPut,  In3str1v * paraFA04)
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
"sample_groups <- tree$tip.label\n"
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
if (system(command.c_str())==0)
{
	 cout<<"Construct tree and pdf done ..."<<endl;
}
else
{
	cerr<<"\tWarning: ["+binPath+"] should install latest R package : [ape] and [ggtree] first"<<endl;
}

	if  (fileExists(OUTpdf))
	{
	command="rm -rf "+RshellPath;
	system(command.c_str());
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

// 函数用于读取样品对应 group 的关系列表
pair<map<string, string>, vector<string> > readSampleGroupList(const string& filename) {

    igzstream file (filename.c_str(),ifstream::in);
    if (file.fail()) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    map<string, string> sample_group;
    map<string, int> groups;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string sample, group;
        iss >> sample >> group;
        sample_group[sample] = group;
        groups[group]++;
    }
    file.close();

    vector<string> group_names;
    for (map<string, int>::const_iterator it = groups.begin(); it != groups.end(); ++it) {
        group_names.push_back(it->first);
    }
    sort(group_names.begin(), group_names.end());

    return make_pair(sample_group, group_names);
}

// 函数用于读取样品之间的距离矩阵
pair<vector<string>, vector<vector<double> > > readDistanceMatrix(const string& filename) {
    igzstream file (filename.c_str(),ifstream::in);
    if (file.fail()) {
        cerr << "Could not open file: " << filename << endl;
        exit(1);
    }

    string header;
    getline(file, header);

    vector<string> samples;
    vector<vector<double> > distance_matrix;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string sample;
        iss >> sample;
        samples.push_back(sample);

        vector<double> row;
        double distance;
        while (iss >> distance) {
            row.push_back(distance);
        }
        distance_matrix.push_back(row);
    }
    file.close();

    return make_pair(samples, distance_matrix);
}

// 函数用于计算 group 之间的平均距离矩阵
map<string, map<string, double> > calculateGroupDistanceMatrix(
    const map<string, string>& sample_group,
    const vector<string>& group_names,
    const vector<string>& samples,
    const vector<vector<double> >& distance_matrix
) {
    map<string, map<string, double> > group_distance_matrix;
    for (vector<string>::const_iterator it1 = group_names.begin(); it1 != group_names.end(); ++it1) {
        for (vector<string>::const_iterator it2 = group_names.begin(); it2 != group_names.end(); ++it2) {
            group_distance_matrix[*it1][*it2] = 0;
        }
    }

    for (size_t row_num = 0; row_num < samples.size(); ++row_num) {
        const string& sample1 = samples[row_num];
		 if (sample_group.find(sample1) == sample_group.end()) { continue;  }
        for (size_t col_num = 0; col_num < samples.size(); ++col_num) {
            const string& sample2 = samples[col_num];
		    if (sample_group.find(sample2) == sample_group.end()) { continue;  }
            double distance = distance_matrix[row_num][col_num];

            const string& group1 = sample_group.at(sample1);
            const string& group2 = sample_group.at(sample2);

            group_distance_matrix[group1][group2] += distance;
        }
    }

    for (vector<string>::const_iterator it1 = group_names.begin(); it1 != group_names.end(); ++it1) {
        for (vector<string>::const_iterator it2 = group_names.begin(); it2 != group_names.end(); ++it2) {
            int count = 0;
            for (map<string, string>::const_iterator pair1 = sample_group.begin(); pair1 != sample_group.end(); ++pair1) {
                if (pair1->second == *it1) {
                    for (map<string, string>::const_iterator pair2 = sample_group.begin(); pair2 != sample_group.end(); ++pair2) {
                        if (pair2->second == *it2) {
                            ++count;
                        }
                    }
                }
            }
            group_distance_matrix[*it1][*it2] /= count;
        }
    }

    return group_distance_matrix;
}

// 函数用于输出 group 之间的距离矩阵
void outputGroupDistanceMatrix(
    const vector<string>& group_names,
    const map<string, map<string, double> >& group_distance_matrix,
    const string& output_filename
) {
    ofstream file(output_filename.c_str());
    if (file.fail()) {
        cerr << "Could not open file: " << output_filename << endl;
        exit(1);
    }

    file << group_names.size() << endl;
    for (vector<string>::const_iterator it1 = group_names.begin(); it1 != group_names.end(); ++it1) {
        file.width(12);
        file << left << *it1;
        for (vector<string>::const_iterator it2 = group_names.begin(); it2 != group_names.end(); ++it2) {
            file.width(12);
            file << left << group_distance_matrix.at(*it1).at(*it2);
        }
        file << endl;
    }
    file.close();
	cout<<"P_distance of group (size number "<<group_names.size()<<") is created done ..."<<endl;
}


void calculateAndOutputGroupDistance(
    const string& sample_group_list_file,
    const string& distance_matrix_file,
    const string& output_file,
    const map<string, string>& NewName2oldName
) {
    // 读取样品组列表文件
    pair<map<string, string>, vector<string> > result1 = readSampleGroupList(sample_group_list_file);
    map<string, string> sample_group = result1.first;
    vector<string> group_names = result1.second;

    map<string, string> oldName2NewName;
    for (map<string, string>::const_iterator it = NewName2oldName.begin(); it != NewName2oldName.end(); ++it)
    {
        const string& newName = it->first;
        const string& oldName = it->second;
        oldName2NewName[oldName] = newName;
    }


    map<string, string> new_sample_group;
    for (std::map<std::string, std::string>::const_iterator it = sample_group.begin(); it != sample_group.end(); ++it) 
    {
        const std::string& oldName = it->first;
        const std::string& group = it->second;
        std::map<std::string, std::string>::iterator findIt = oldName2NewName.find(oldName);
        if (findIt != oldName2NewName.end()) 
        {
            new_sample_group[findIt->second] = group;
        } 
        else
	{
            new_sample_group[oldName] = group;
        }
    }


    pair<vector<string>, vector<vector<double> > > result2 = readDistanceMatrix(distance_matrix_file);
    vector<string> samples = result2.first;
    vector<vector<double> > distance_matrix = result2.second;
    map<string, map<string, double> > group_distance_matrix = calculateGroupDistanceMatrix(new_sample_group, group_names, samples, distance_matrix);
    outputGroupDistanceMatrix(group_names, group_distance_matrix, output_file);


}




#endif

