# VCF2Dis
<b>VCF2Dis: A new simple and efficient software to calculate p-distance matrix and tree based Variant Call Format</b>


  </br> if you can't get the <b>p_dis.nwk</b> tree file but had the p-distance matrix [p_dis.mat], here are the 3 Other ways to get the tree file.
</br> 
</br> 


#### Method 1 : online WebSite
   Choose one of **A/B**
   </br> **A.** Upload the web [fneighbor(http://emboss.toulouse.inra.fr/cgi-bin/emboss/fneighbor?_pref_hide_optional=1)](http://emboss.toulouse.inra.fr/cgi-bin/emboss/fneighbor?_pref_hide_optional=1) ，the Click the **Run fneighbor** bottom . then you can get the output file **datafile.treefile**
   </br> **B.**  Upload the <b>p_dis.mat</b> to the website [fastme (http://www.atgc-montpellier.fr/fastme/)](http://www.atgc-montpellier.fr/fastme/), select <b>Data Type</b> to the  <b>Distance matrix </b>,Click the bottom twist <b>execute & email results</b>. you will get the <b>p_dis_mat_fastme-tree.nwk</b> , and Email not mandatory;
   
   Run  MEGA        #    The MEGA (http://www.megasoftware.net/) was used to present the phylogenetic tree based this file [p_dis_mat_fastme-tree.nwk]
 
#### Method 2 : PHYLIPNEW
   Use the PHYLIPNEW to construct nj-tree
</br>How to Install PHYLIPNEW please Click on <b> [here](https://github.com/hewm2008/VCF2Dis/blob/main/Install.NJ.en.md)</b> or Click on <b>[here(Chinese)](https://github.com/hewm2008/VCF2Dis/blob/main/Install.NJ.cn.md)</b>
```
      #    3.1 Run  PHYLIP
      #   After p_distance done , software PHYLIPNEW 3.69 (http://evolution.genetics.washington.edu/phylip.html) ,with neighbor-joining method can was used to construct the phylogenetic tree on the basis of this  p_distance matrix;
       
           PHYLIPNEW-3.69.650/bin/fneighbor  -datafile p_dis.matrix  -outfile tree.out1.txt -matrixtype s -treetype n -outtreefile tree.out2.tre

      #    3.2 Run  MEGA
      #    The MEGA6 (http://www.megasoftware.net/) was used to present the phylogenetic tree based this file [tree.out2.tre]
```

#### Method 3  : R script with ape
```
      #    3.1 Run  R Rscript
		Rscript  exemple1/vistreecode.r    p_dis.mat
      #    3.2 Run  MEGA
      #    The MEGA6 (http://www.megasoftware.net/) was used to present the phylogenetic tree based this file [tree.out2.tre]
```

* 4) you can see the neighbor-joining tree and save it as PDF format

######################swimming in the sky and flying in the sea ########################### ##
