
## 安装 PHYLIPNEW 记要

要运行 PHYLIPNEW-3.69.650/bin/fconsense 须先安装<b> PHYLIPNEW-3.69.660 </b>，其步骤如下：

### 第一步：下载
	cd /public/software/
        wget ftp://emboss.open-bio.org/pub/EMBOSS/EMBOSS-6.6.0.tar.gz   ./
        wget ftp://emboss.open-bio.org/pub/EMBOSS/PHYLIPNEW-3.69.660.tar.gz ./
	#在  ftp://emboss.open-bio.org/pub/EMBOSS/   里面下载好  EMBOSS-6.6.0.tar.gz  和 PHYLIPNEW-3.69.660.tar.gz


### 第二步：安装EMBOSS ;
	tar -xzvf EMBOSS-6.6.0.tar.gz   # 解压
	cd  EMBOSS-6.6.0/    
	./configure --prefix=/public/software/EMBOSS-6.6.0   # [prefix后面接的目录必须是你自己有权限的目录]
	make 

		##   其中这个软件依赖一些其他包，若系统没有就先安装之，在这打个比如，如报了少  缺libmysqlclient.so.*的库的错
		##   那么请安装之，若有root:  运行： yum install mysql   ;  yum install mysql-devel即可以安装，若没有到自行安装
		##   现别人已经安装好在这了： /gss1/App_dir/mariadb-10.1.26/bin/mysql  在 /gss1/App_dir/mariadb-10.1.26/lib/里面存在
		##   libmysqlclient.*的，即可以直接加载环境变量就行   export LD_LIBRARY_PATH ;
		##   在这运行：  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/gss1/App_dir/mariadb-10.1.26/lib/  [可写入： ~/.bashrc 里面，免得每次都要重新加载]

	make install  

### 第三步：安装好EMBOSS以后，安装PHYLIBNEW

	tar -xzvf PHYLIPNEW-3.69.660.tar.gz   # 解压 
	cp  -r /public/software/EMBOSS-6.6.0/lib/*        /public/software/PHYLIPNEW-3.69.650/lib/
	cp  -r /public/software/EMBOSS-6.6.0/include/*    /public/software/PHYLIPNEW-3.69.650/include/
	   # 在安装好EMBOSS后 即在 EMBOSS-6.6.0  里面有include 和lib 目录;
	   # 将EMBOSS-6.60下include里的头文件和lib文件夹copy到 PHYLIBNEW下 [or 加载一些变量 export LD_LIBRARY_PATH ; CFLAGS="-I$prefix/include" ;  LDFLAGS="-L$prefix/ ]
 	./configure --prefix=/public/software/PHYLIPNEW-3.69.650  
	make 

		##  其中这个软件依赖一些其他包，若系统没有就先安装之，在这打个比例  报了少 LibXaw.soXX的库
		##  在 https://www.x.org/archive/individual/lib/ 下载里面的 	libXaw-1.0.13.tar.gz ;
		##  #解压 libXaw-1.0.13.tar.gz，./configure  --prefix=/public/software/PHYLIPNEW-3.69.650 (可以这个目录，免得又重新加载环境变量就行 
		##  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/public/software/PHYLIPNEW-3.69.650/lib; 
		##  make  ;make install;

	make install

### 第四步： 安装后好在 PHYLIPNEW-3.69.660/bin/fconsense 
	 #但运行会报  少了在EMBOSS目录少了  fneighbor.acd 的文件
  	 # 那么在 PHYLIPNEW-3.69.660 找之(find ./ -name fneighbor.acd),正常当在HYLIPNEW-3.69.650/emboss_acd下
	mkdir  -p /public/software/EMBOSS-6.6.0/embassy/phylipnew/ ;
	ln -s  /public/software/PHYLIPNEW-3.69.650/emboss_acd  /public/software/EMBOSS-6.6.0/embassy/phylipnew/emboss_acd
	 # 创建目录 并链接  or 可以copy 

### 其它信息
 若后面再运行报少了xxx.so\.*的话  把这个xxx.so.\*的路径找到并记录下来,直接在 \~/.bashrc加载之，export LD_LIBRARY_PATH=$export LD_LIBRARY_PATH:库所在路径
 如把 <b><i>export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/public/software/PHYLIPNEW-3.69.650/lib;  </i></b>写进  <b>\~/.bashrc</b>
 免得每次要用都重新加载之

