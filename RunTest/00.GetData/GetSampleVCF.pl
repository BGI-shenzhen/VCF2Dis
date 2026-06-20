#!/usr/bin/perl -w
use strict;
#explanation:this program is edited to 
#edit by HeWeiMing;   Thu Jan 14 22:26:57 CST 2010
#Version 1.0    hewm@genomics.org.cn 

die  "Version 1.0\t2010-01-14;\nUsage: $0 <biallelic.snp.gz><ALL.chr1.vcf,gz><SampleNumber>\n" unless (@ARGV ==3);

#############Befor  Start  , open the files ####################

open IA,"gzip -cd $ARGV[0] | "  || die "input file can't open $!" ;
#open OA,">$ARGV[2]" || die "output file can't open $!" ;
my $InFile=$ARGV[1];

if  ($InFile =~s/\.gz$/\.gz/)
{
	open IB,"gzip -cd  $InFile | "  || die "input file can't open $!" ;
}
else
{
	open IB,"$InFile"  || die "input file can't open $!" ;
}

################ Do what you want to do #######################
my %hash=();
while(<IA>) 
{ 
	chomp ; 
	my @inf=split ;
	$hash{$inf[1]}=1;
}
close IA;
my $StartSample=9;
my $End=$StartSample+$ARGV[2];
while(<IB>)
{
	chomp ;
	if ($_=~s/##/##/)
	{
		print $_ ,"\n";
		next ;
	}
	my @inf=split ;
	if( exists  $hash{$inf[1]})
	{
		$_=join("\t",@inf[0..$End]);
		print $_ ,"\n";
	}
}
close IB ;

######################swimming in the sky and flying in the sea ############################
