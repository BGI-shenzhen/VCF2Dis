#!/usr/bin/perl -w
use strict;
#explanation:this program is edited to
#edit by hewm;   Sun Jun 23 11:30:50 CST 2024
#Version 1.0    hewm@genomics.org.cn

die  "Version 1.0\t2024-06-23;\nUsage: $0 <Distance.out.mdist.id><Distance.out.mdist><new.mat>\n" unless (@ARGV ==3);

#############Befor  Start  , open the files ####################

open (IA,"$ARGV[0]") || die "input file can't open $!";
open (IB,"$ARGV[1]") || die "input file can't open $!";
open (OA,">$ARGV[2]") || die "output file can't open $!" ;

my $num=`wc  -l  $ARGV[1] | awk '{print \$1}'`;  chomp $num;
print  OA "  $num\n";
################ Do what you want to do #######################

while(<IA>)
{
	chomp ;
	my @inf=split ;
	$_=<IB>;
	print OA $inf[0],"\t$_";
}


close IA;
close OA ;

######################swimming in the sky and flying in the sea ##########################
