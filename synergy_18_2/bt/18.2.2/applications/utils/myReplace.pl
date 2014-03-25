###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
# Description: This script is meant to search and replace a particular string 
# in a file. 
###############################################################################
#use strict;
#use warnings;

#!/usr/bin/perl -w
use File::Copy;

if ($ARGV[0]){
 $file= $ARGV[0];
}
else
{
	die "no input file";
}
print "output file name: ";
chomp($outfilename = <STDIN>);
print "Search string: ";
chomp($search = <STDIN>);
print "Replacement string: ";
chomp($replace = <STDIN>);
open(INFO,"<$file") || die "cannot open $file: $!";
open(OUT, ">$outfilename") || die "cannot create $outfilename : $!";
@lines = <INFO>;
foreach $line (@lines)
{
       if($line =~ m/$search/)
       {
       	$line =~ s/$search/$replace/g; # change the lines
       	print OUT "$line";
       }
       else
       {
       	print OUT "$line";
       }

}
close(INFO);
close(OUT); 

print "\n";
print "Now let's copy the new file to the original directory with its original name\n";

print "Source Dir : ";
chomp($SD = <STDIN>);
print "Target Dir : ";
chomp($TD = <STDIN>);
print "Source file name : ";
chomp($SFN = <STDIN>);
print "Target file name : ";
chomp($TFN = <STDIN>);
opendir(SDIR, $SD);
opendir(TDIR, $TD);
if (-e $SFN)
{
unlink("$TD/$TFN");
copy($SFN, "$TD/$TFN") || die ("File cannot be copied");
}
closedir(SDIR);
closedir(TDIR);
print "OPERATION COMPLETE";

