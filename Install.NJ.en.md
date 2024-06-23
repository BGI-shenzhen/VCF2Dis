# Install PHYLIPNEW

 </br>To run <b>PHYLIPNEW-3.69.650/bin/fconsense</b> software, <b>PHYLIPNEW-3.69.660</b> must be installed first, and the steps are as follows:

###  Step 1: Download
	cd /public/software/
	wget ftp://emboss.open-bio.org/pub/EMBOSS/EMBOSS-6.6.0.tar.gz   ./
	wget ftp://emboss.open-bio.org/pub/EMBOSS/PHYLIPNEW-3.69.660.tar.gz ./
	#Download EMBOSS-6.6.0.tar.gz and PHYLIPNEW-3.69.660.tar.gz at [ftp://emboss.open-bio.org/pub/EMBOSS/]


###  Step 2: Install EMBOSS
	tar -xzvf EMBOSS-6.6.0.tar.gz  # Unzip package
	cd EMBOSS-6.6.0
	./configure --prefix=/public/software/EMBOSS-6.6.0   #[You must have read and write permissions for this Dir]
	make 

		## This software depends on some other packages. If your system does not have one, install them first. For example, Error with a report missing [libmysqlclient.so.*] library
		## then please install it, if you have root: Run:[ yum install mysql; yum install mysql-devel ] can be installed, if not, install it yourself.
		## Now others have installed mysqlclient here: [/gss1/App_dir/mariadb-10.1.26/bin/mysql] and [libmysqlclient.*] exists in [/gss1/App_dir/mariadb-10.1.26/lib/]
		## That is, you can load the environment variables directly [export LD_LIBRARY_PATH]
		## Run here: [export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/gss1/App_dir/mariadb-10.1.26/lib/]  [Writable: ~/.bashrc inside, so as not to reload every time]

	make install


### Step 3: After installing EMBOSS, install PHYLIBNEW
	tar -xzvf PHYLIPNEW-3.69.660.tar.gz   #Unzip package
	cp  -r /public/software/EMBOSS-6.6.0/lib/*        /public/software/PHYLIPNEW-3.69.650/lib/
	cp  -r /public/software/EMBOSS-6.6.0/include/*    /public/software/PHYLIPNEW-3.69.650/include/
		# After successfully installing EMBOSS, there are [include] and [lib] directories in EMBOSS-6.6.0;
		# Copy the header file and lib folder in include under EMBOSS-6.60 to PHYLIBNEW [or load some variables :export LD_LIBRARY_PATH; CFLAGS="-I$prefix/include"; LDFLAGS="-L$prefix/]
	./configure --prefix=/public/software/PHYLIPNEW-3.69.650
	make 

		##  This software depends on some other library. If the system does not have it, install them first. Let’s make an analogy here. reporting less [LibXaw.so.*] library.
		##  Then download [libXaw-1.0.13.tar.gz] in [https://www.x.org/archive/individual/lib/];
		##  tar -zxvf libXaw-1.0.13.tar.gz; cd libXaw-1.0.13 ; ./configure  --prefix=/public/software/PHYLIPNEW-3.69.650 (This directory can be used to avoid reloading environment variables)
		##  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/public/software/PHYLIPNEW-3.69.650/lib;
		##  make ;make install;

	make install


### Step 4: Run PHYLIPNEW-3.69.660/bin/fconsense 
	# After installation, the first run [PHYLIPNEW-3.69.660/bin/fconsense] will report that the [fneighbor.acd] file is missing in the EMBOSS directory.
	# Then find it in PHYLIPNEW-3.69.660 Dir [find ./ -name fneighbor.acd], normally it will under Dir [HYLIPNEW-3.69.650/emboss_acd]
	mkdir  -p /public/software/EMBOSS-6.6.0/embassy/phylipnew/ ;
	ln -s  /public/software/PHYLIPNEW-3.69.650/emboss_acd  /public/software/EMBOSS-6.6.0/embassy/phylipnew/emboss_acd
	# Create directory and link or copy


### Other Info:
  </br> If [xxx.so.\*] missing is report after running the software later, find and record the path of this [xxx.so.\*] and load it directly in \~/.bashrc, <i> export LD_LIBRARY_PATH=$export LD_LIBRARY_PATH:/Dir_Path_For_Thislibrary_located/;</i> 
  </br> such  writing info <b><i>export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/public/software/PHYLIPNEW-3.69.650/lib;</i></b> to file <b>[\~/.bashrc]</b>; Avoid reloading every time you use it


