#!/usr/bin/perl -w
use strict;
#explanation:this program is edited to 
#edit by hewm;   Wed Feb 20 11:02:07 HKT 2019
#Version 1.0    hewm@genomics.org.cn 

die  "Version 1.0\t2019-02-20;\nUsage: $0 <merge.tre><RepeatTime><boostrap.tre>\n" unless (@ARGV ==3);

#############Befor  Start  , open the files ####################

open (IA,"$ARGV[0]") || die "input file can't open $!";
my $TotalRepeat=$ARGV[1];
open (OA,">$ARGV[2]") || die "output file can't open $!" ;

################ Do what you want to do #######################
$/=";";

	while(<IA>) 
	{
		$_=~s/\n//g;
		next if ($_ eq "");
		my $Start=0;
		my $Now=$Start;
		my $Ttue=1;
		my $Str=$_ ;

		while($Ttue==1)
		{
			$Now=index($Str,":",$Start);
			if  ($Now==-1)
			{
				$Ttue=0;
			}
			else
			{
				my $Length=$Now-$Start;
				my $AAA=substr($Str,$Start,$Length);
					$Start=$Now+1;
				my $NowA=index($Str,",",$Start);
				my $NowB=index($Str,")",$Start);
				if  ($NowA!=-1  &&  $NowB!=-1)
				{
					if  ($NowA>$NowB)
					{
						$Now=$NowB;
					}
					else
					{
						$Now=$NowA;
					}
				}
				elsif  ($NowA==-1  &&  $NowB==-1)
				{
					print "bad Format,some thing wrong!!!\n";
				}
				elsif  ($NowA==-1  &&  $NowB!=-1)
				{
					$Now=$NowB;
				}
				elsif   ($NowA!=-1  &&  $NowB==-1)
				{
					$Now=$NowA;					
				}
					$Length=$Now-$Start;
				my $BBB=substr($Str,$Start,$Length);
				   $BBB=sprintf ("%.1f",$BBB*100.0/$TotalRepeat);
				   $Start=$Now;
				 print OA "$AAA:$BBB"
			}
		}
		my $Length=length($Str);
		my $BBB=substr($Str,$Start,$Length);
		 print OA  "$BBB\n";
	}
$/="\n";
close IA;
close OA ;

######################swimming in the sky and flying in the sea ###########################
