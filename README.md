# VCF2Dis
<b>VCF2Dis: A new simple and efficient software to calculate p-distance matrix and tree based Variant Call Format</b>

###  1) Install and Parameter
------------

* 1) Install

The <b>new version</b> will be updated and maintained in <b>[hewm2008/VCF2Dis](https://github.com/hewm2008/VCF2Dis)</b>, please click below Link to download the latest version
</br><p align="center"><b>[hewm2008/VCF2Dis](https://github.com/hewm2008/VCF2Dis)</b></p>
<b> [Download](https://github.com/hewm2008/VCF2Dis/archive/v1.52.tar.gz) </b>

  </br> Just [make]  or [sh  make.sh ]  to compile this software.the final software can be found in the Dir <b>[bin/VCF2Dis]</b>
  </br> For <b>linux /Unix </b> and <b>macOS</b>
  <pre>
        tar -zxvf  VCF2DisXXX.tar.gz            # if Link do not work ,Try <b>re-install</b> [zlib]library
        cd VCF2DisXXX;                          # [zlib] and copy them to the library Dir
        sh make.sh;                             # VCF2Dis-xx/src/include/zlib
        ./bin/VCF2Dis
  </pre>
  
**Note:** If fail to link,try to <b>re-install</b> the libraries [**_zlib_**](https://zlib.net/)
</br>**Note:**: [R](https://www.r-project.org/) with [ape](https://cran.r-project.org/web/packages/ape/index.html), [dplyr](https://cran.r-project.org/web/packages/dplyr/index.html) and  [ggtree](https://www.bioconductor.org/packages/release/bioc/html/ggtree.html) are recommended
</br>
* 2) Parameter description:
```php
	Usage: VCF2Dis -InPut  <in.vcf>  -OutPut  <p_dis.mat>

		-InPut     <str>     Input one or muti GATK VCF genotype File
		-OutPut    <str>     OutPut Sample p-Distance matrix

		-InList    <str>     Input GATK muti-chr VCF Path List
		-SubPop    <str>     SubGroup SampleList of VCFFile [ALLsample]
		-Rand      <float>   Probability (0-1] for each site to join Calculation [1]

		-help                Show more help [hewm2008 v1.52s]

```
</br> To see more Help document, please use <b>-help </b> and see the [eaxmple](https://github.com/hewm2008/VCF2Dis/blob/main/exemple)
```php
		-InFormat      <str>   Input File is [VCF/FA/PHY] Format,defaut[VCF]
		-InSampleGroup <str>   InFile of sample Group info,format(sample groupA)
		-TreeMethod    <int>   Construct Tree Method,1:NJ-tree 2:UPGMA-tree [1]
		-KeepMF                Keep the Middle File diff & Use matrix
```


### 2) Example
</br> Three examples were provided in the directory of  [Example/example*](https://github.com/hewm2008/VCF2Dis/blob/main/exemple)

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

* 2) Visualize and beautify the it (need deal with Other software)
 </br>you will get the p_dis.nwk tree file after VCF2Dis. 
 </br>Run  MEGA     #    The [MEGA](http://www.megasoftware.net/) was used to present the phylogenetic tree based this file [p_dis.nwk]
 </br> you can see the neighbor-joining tree and save it as PDF format

if you can't get the  <b>p_dis.nwk</b>  tree file, here are the [3 methods to get the tree file](https://github.com/hewm2008/VCF2Dis/blob/main/OtherWay2ConstructTree.md).


###  2) an Example of nj-tree with boostrap

* 1) muti-run the nj-tree by using  put back sampling.
To using the the part of the sites and new the nj-tree as above. Repeat For the <b>NN</b> times. X=(1,2....NN);

```
    ./bin/VCF2Dis	-InPut	in.vcf.gz	-OutPut p_dis_X.mat    -Rand     0.25
    PHYLIPNEW-3.69.650/bin/fneighbor  -datafile p_dis_X.matrix  -outfile tree.out1_X.txt -matrixtype s -treetype n -outtreefile tree.out2_X.tre 
```
How to Install PHYLIPNEW please Click on <b> [here](https://github.com/hewm2008/VCF2Dis/blob/main/Install.NJ.en.md)</b> or Click on <b>[here(Chinese)](https://github.com/hewm2008/VCF2Dis/blob/main/Install.NJ.cn.md)</b>

* 2) merge the all the  put back sampling  NJ-tree and construct  boostrap nj-tree.

```
	cat   tree.out2_*.tre   >  ALLtree_merge.tre
	PHYLIPNEW-3.69.650/bin/fconsense   -intreefile   ALLtree_merge.tre  -outfile out  -treeprint Y
	perl  ./bin/percentageboostrapTree.pl    ALLtree_merge.treefile    NN    Final_boostrap.tre
```
* 3) construct nj-tree and present it (need deal with Other software)
```
      #    The MEGA6 (http://www.megasoftware.net/) was used to present the phylogenetic tree based this file Final_boostrap.tre] 
```
------------
###  4) Introduction
------------
To new the p_distance matrix besed the VCF file. the more infomation about the p_distance matrix, see <b>[this website.](https://phylipweb.github.io/phylip/doc/distance.html)</b>
The VCF SNPs datasets were used to calculate p-distance between individuals, according to the follow formula to operate the sample i and sample j genetic distance:

                D_ij=(1/L) * [(sum(d(l)_ij))]

  </br> Where L is the length of regions where SNPs can be identified, and given the alleles at position l are A/C:
```
            d(l)_ij=0.0     if the genotypes of the two individuals were AA and AA;
            d(l)_ij=0.5     if the genotypes of the two individuals were AA and AC;
            d(l)_ij=0.0     if the genotypes of the two individuals were AC and AC;
            d(l)_ij=1.0     if the genotypes of the two individuals were AA and CC;
            d(l)_ij=0.0     if the genotypes of the two individuals were CC and CC;
```

###  5) Results
------------
some NJ-tree images which I draw in the paper before. and VCF2Dis had was cited <b> 150 times </b> at google scholar.

* [50 Rices NBT](http://www.nature.com/nbt/journal/v30/n1/images/nbt.2050-F1.jpg)
* [31 soybeans  NG]( http://www.nature.com/ng/journal/v42/n12/images/ng.715-F1.jpg)

###  6) Discussing
------------
- [:email:](https://github.com/hewm2008/VCF2Dis) hewm2008@gmail.com / hewm2008@qq.com
- join the<b><i> QQ Group : 125293663</b></i>


######################swimming in the sky and flying in the sea ########################### ##
