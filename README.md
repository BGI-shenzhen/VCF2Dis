# VCF2Dis
<b>VCF2Dis: an ultra-fast and efficient tool to calculate pairwise genetic distance and construct population phylogeny from VCF files</b>

###  1) Install and Parameter
------------

* 1) Install

The <b>new version</b> will be updated and maintained in <b>[hewm2008/VCF2Dis](https://github.com/hewm2008/VCF2Dis)</b>, please click below Link to download the latest version
</br><p align="center"><b>[hewm2008/VCF2Dis](https://github.com/hewm2008/VCF2Dis)</b></p>
<b> [Download](https://github.com/hewm2008/VCF2Dis/archive/v1.53.tar.gz) </b>

#### Option 1 :   Local compilation
Just `sh  make.sh`  to compile. The executable `VCF2Dis` can be found in the folder of <b>`bin/VCF2Dis`</b>
  </br> For <b>Linux /Unix </b> and <b>macOS</b>
  <pre>
        tar -zxvf  VCF2DisXXX.tar.gz            # if Link do not work ,Try <b>re-install</b> [zlib]library
        cd VCF2DisXXX;                          # [zlib] and copy them to the library Dir
        sh make.sh;                             # VCF2Dis-xx/src/include/zlib
        ./bin/VCF2Dis
  </pre>
  
**Note:** If fail to link,try to <b>re-install</b> the libraries [**_zlib_**](https://zlib.net/)
</br>**Note:**: [R](https://www.r-project.org/) with [ape](https://cran.r-project.org/web/packages/ape/index.html), [dplyr](https://cran.r-project.org/web/packages/dplyr/index.html) and  [ggtree](https://www.bioconductor.org/packages/release/bioc/html/ggtree.html) are recommended

#### Option 2: Docker container
You can use Docker to install and run VCF2Dis. Follow the steps below:
1. **Install Docker**: Ensure Docker is installed on your system. If not, you can install it by following the [Docker Official Documentation](https://docs.docker.com/get-docker/).
2. **Pull the Docker Image**: Use the following command to pull the VCF2Dis Docker image from the Alibaba Cloud Container Registry:
    ```sh
    docker pull registry.cn-shenzhen.aliyuncs.com/knight134/vcf2dis:v1.53e    ##  Docker image from the Alibaba Cloud Container Registry
    docker run -it --rm vcf2dis:v1.53e VCF2Dis                                ## After pulling the image, you can run the containe
    ```
#### Option 3: Singularity container   
1. **Install Singularity**: Ensure Singularity is installed on your system. If not, you can install it by following the [Singularity Official Documentation](https://sylabs.io/guides/3.5/user-guide/).
2. **Build the SIF File**: Use the following command to build a Singularity image file (SIF) from the Docker image:
    ```sh
    singularity build vcf2dis_1.53e.sif docker://registry.cn-shenzhen.aliyuncs.com/knight134/vcf2dis:v1.53e   # you can download follows 
    singularity exec  vcf2dis_1.53e.sif  VCF2Dis
    ```
3. **Download the SIF File**:Alternatively, you can download the built SIF file directly from the [vcf2dis_1.53e.sif](https://github.com/hewm2008/VCF2Dis/releases/download/v1.53/vcf2dis_1.53e.sif). Once downloaded, you can run it using Singularity.

* 2)  Main parameter description:
```sh
	Usage: VCF2Dis -InPut  <in.vcf>  -OutPut  <p_dis.mat>

		-InPut     <str>     Input one or muti GATK VCF genotype File
		-OutPut    <str>     OutPut Sample p-Distance matrix

		-InList    <str>     Input GATK muti-chr VCF Path List
		-SubPop    <str>     SubGroup SampleList of VCF File [ALLsample]
		-Rand      <float>   Probability (0-1] for each site to join Calculation [1]

		-help                Show more help [hewm2008 v1.53s]

 ```
For more details, please use <b>-help </b> and see the [example](https://github.com/hewm2008/VCF2Dis/blob/main/example)
```sh
		-InFormat      <str>   Input File is [VCF/FA/PHY] Format,defaut: [VCF]
		-InSampleGroup <str>   InFile of sample Group info,format(sample groupA)
		-TreeMethod    <int>   Construct Tree Method,1:NJ-tree 2:UPGMA-tree [1]
		-KeepMF                Keep the Middle File diff & Use matrix
```


### 2) Example
</br> Three examples were provided in the directory of [example/Example*](https://github.com/hewm2008/VCF2Dis/blob/main/example)

###  1) an Example of nj-tree with no boostrap
------------

* 1) To Create the p_distance matrix and construct nj-tree newick tree

```
# 1.1) To new all the sample p_distance matrix and newick tree based VCF, run VCF2Dis directly
      ./bin/VCF2Dis	-InPut	in.vcf.gz	-OutPut p_dis.mat
      #  ./bin/VCF2Dis     -InPut  in.fa.gz	-OutPut p_dis.mat -InFormat  FA

# 2.2) To new sub group sample p_distance matrix and and newick tree ; put their sample name into File sample.list
      ./bin/VCF2Dis	-InPut	chr1.vcf.gz chr2.vcf.gz	-OutPut p_dis.mat  -SubPop  sample.list
```

* 2) Simple tree visualization  (for advanced tree display and annotation please refer  to `iTOL`, `Evolview`, `MEGA`)
 </br>you will obtain the `p_dis.nwk` tree file and neighbor-joining tree in pdf format `p_dis.pdf` after VCF2Dis. 

 </br>**Note:**:if you can't get the <b>p_dis.nwk</b> tree file but had the <b>p_dis.mat</b>, here are the [3 methods to get the tree file](https://github.com/hewm2008/VCF2Dis/blob/main/OtherWay2ConstructTree.md).


###  2) an Example of nj-tree with boostrap

* 1) Running multiple times by using  a method of sampling with replacement.
Users can randomly select  a part of the sites [-Rand] and construct a new nj-tree as above, and Repeat NN times [recommand NN=100]. X=(1,2....NN);

```sh
#!/bin/bash
NN=100
if [ "$#"  -eq  1 ]; then
	NN=$1
fi
for X in $(seq 1 $NN)
do
	./bin/VCF2Dis -InPut in.vcf.gz -OutPut p_dis_${X}.mat -Rand 0.25
    # PHYLIPNEW-3.69.650/bin/fneighbor -datafile p_dis_${X}.mat -outfile tree.out1_${X}.txt -matrixtype s -treetype n -outtreefile tree.out2_${X}.tre
done
```
* 2) Merge all the nj-tree and construct  and display a boostrap nj-tree. (For advanced display tree and annotation please refer to `iTOL`, `Evolview` and `MEGA`)

```sh
#!/bin/bash
NN=100
if [ "$#"  -eq  1 ]; then
  NN=$1
fi

cat  p_*.nwk  >    alltree_merge.tre   #  cat  tree*.tre  > alltree_merge.tre
PHYLIPNEW-3.69.650/bin/fconsense   -intreefile   alltree_merge.tre  -outfile out  -treeprint Y
perl  ./bin/percentageboostrapTree.pl    alltree_merge.treefile    $NN    Final_boostrap.tre  # NN is the input number

```
How to Install PHYLIPNEW please Click on <b> [here](https://github.com/hewm2008/VCF2Dis/blob/main/Install.NJ.en.md)</b> or Click on <b>[here(Chinese)](https://github.com/hewm2008/VCF2Dis/blob/main/Install.NJ.cn.md)</b>

------------
###  4) Introduction
------------
The formula for calculating p-distance between indivisuals from VCF SNP datasets was listed below:  

                D_ij=(1/L) * [(sum(d(l)_ij))]

  </br> Where L is the length of regions where SNPs can be identified, and given the alleles at position `l` are A/C between sample `i` and sample `j`:
```
            d(l)_ij=0.0     if the genotypes of the two individuals were AA and AA;
            d(l)_ij=0.5     if the genotypes of the two individuals were AA and AC;
            d(l)_ij=0.0     if the genotypes of the two individuals were AC and AC;
            d(l)_ij=1.0     if the genotypes of the two individuals were AA and CC;
            d(l)_ij=0.0     if the genotypes of the two individuals were CC and CC;
```
To further know about the p_distance matrix based the VCF file, please refer to <b>[this website.](https://phylipweb.github.io/phylip/doc/distance.html)</b>
###  5) Results
------------
VCF2Dis have been cited in more than <b> 170 times </b> by [searching against google scholar](https://scholar.google.com.hk/scholar?hl=zh-CN&as_sdt=0%2C5&q=VCF2Dis&btnG=).
</br>Below were some NJ-tree images that I draw in the paper before.

* [50 Rices NBT](https://www.nature.com/articles/nbt.2050/figures/1)
* [31 soybeans NG](https://www.nature.com/articles/ng.715/figures/1)
</br>Display tree by MAGA after test Data VCF2Dis  -i ALL.chr*.genotypes.vcf.gz -SubPop  subsample203.list -InSampleGroup pop.info

![example1.png](https://github.com/hewm2008/VCF2Dis/blob/main/example/Example1/OUT.png)


###  6) Discussing
------------
- [:email:](https://github.com/hewm2008/VCF2Dis) hewm2008@gmail.com / hewm2008@qq.com
- join the<b><i> QQ Group : 125293663</b></i>


######################swimming in the sky and flying in the sea ########################### ##
