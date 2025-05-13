#!/usr/bin/perl -w
use strict;
#explanation:this program is edited to
#edit by hewm;   Sun Jun 23 10:30:34 CST 2024
#Version 1.0    hewm@genomics.org.cn

die  "Version 1.0\t2024-06-23;\nUsage: $0 <In.vcf.gz><Out.vcf>\n" unless (@ARGV ==2);

#############Befor  Start  , open the files ####################

my $InFile=$ARGV[0];
if  ($InFile =~s/\.gz$/\.gz/)
{
	open IA,"gzip -cd  $InFile | "  || die "input file can't open $!" ;
}
else
{
	open IA,"$InFile"  || die "input file can't open $!" ;
}


open (OA,">$ARGV[1]") || die "output file can't open $!" ;

################ Do what you want to do #######################

while(<IA>)
{
	chomp ;
	if  ($_=~s/#/#/g)
	{
		print OA $_,"\n";
		next ;
	}
	my @inf=split ;
	if ( (length($inf[3])==1)  &&  (length($inf[4])==1) )
	{
		print OA$_,"\n";
	}
}


close IA;
close OA ;

######################swimming in the sky and flying in the sea ##########################
