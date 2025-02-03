
<b>VCF2Dis: an ultra-fast and efficient tool to calculate pairwise genetic distance and construct population phylogeny from VCF files</b>

###  INSTALL
We provide three alternative options to install `VCF2Dis`

#### Option 1 :   Local compilation
------------
Just `sh  make.sh`  to compile `VCF2Dis`. The executable `VCF2Dis` can be found in the folder of <b>`bin/VCF2Dis`</b>
  </br> For <b>Linux /Unix </b> and <b>macOS</b>
  <pre>
        tar -zxvf  VCF2DisXXX.tar.gz            # if Link do not work ,Try <b>re-install</b> [zlib]library
        cd VCF2DisXXX;                          # [zlib] and copy them to the library Dir
        sh make.sh;                             # VCF2Dis-xx/src/include/zlib
        ./bin/VCF2Dis
  </pre>
**Note:** If fail to link,try to <b>re-install</b> the libraries [**_zlib_**](https://zlib.net/)
</br>**Note:**: [R](https://www.r-project.org/) with [ape](https://cran.r-project.org/web/packages/ape/index.html), [dplyr](https://cran.r-project.org/web/packages/dplyr/index.html) and  [ggtree](https://www.bioconductor.org/packages/release/bioc/html/ggtree.html) packages are recommended


#### Option 2: Docker container
You can use Docker to install and run VCF2Dis, following the steps listed below:
1. **Install Docker**: Ensure Docker is installed on your system. If not, please install it by following the [Docker Official Documentation](https://docs.docker.com/get-docker/).
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
3. **Download the SIF File**:Alternatively, you can download the SIF file directly from the github: [vcf2dis_1.53e.sif](https://github.com/hewm2008/VCF2Dis/releases/download/v1.53/vcf2dis_1.53e.sif). Once downloaded, you can run it using Singularity.

### Contact
------------
- [:email:](https://github.com/hewm2008/VCF2Dis) hewm2008@gmail.com / hewm2008@qq.com
- join the<b><i> QQ Group : 125293663</b></i>


######################swimming in the sky and flying in the sea ########################### ##

