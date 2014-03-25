###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #3 $
###############################################################################
use strict;
use warnings;
 
open (PSR_FILE, $ARGV[0]) or die "Unable to open $ARGV[0]...";
my ($outFileName);
$outFileName = $ARGV[0];
$outFileName =~ s/^(.*).psr/$1.c/ or die "File must be of type .psr";
open (C_FILE, ">$outFileName") or die "Unable to open output file $outFileName";
print "Converting $ARGV[0] to $outFileName\n";
my ($nameTag);
$nameTag = $ARGV[0];
$nameTag =~ s/^.*[\/\\](.*).psr/$1/;
$nameTag =~ s/-/_/;
$nameTag =~ s/\./_/;

print C_FILE "/*****************************************************************************\n";
print C_FILE "\n";
print C_FILE "        Copyright Cambridge Silicon Radio Limited and its licensors 2011.\n";
print C_FILE "        All rights reserved.\n";
print C_FILE "\n";
print C_FILE "*****************************************************************************/
\n";
print C_FILE "/* Note: this is an auto-generated file. */\n";
print C_FILE "\n";
print C_FILE "#include \"csr_synergy.h\"\n";
print C_FILE "#include \"csr_types.h\"\n";
print C_FILE "\n";
print C_FILE "const CsrCharString bootstrap_patch_string_$nameTag\[\] = \\\n";
 
while (<PSR_FILE>){
    my($line) = $_; 
    chomp($line); 

    # Remove blank lines:
    $line =~ s/^\s*$//;

    # Remove comment lines:
    $line =~ s/^#.*$//;
    $line =~ s/^\/.*$//;

    # Kill windows line endings
    $line =~ s/\r//;

    # Kill superflous spaces
    $line =~ s/ +/ /;
    $line =~ s/ *&/&/;
    $line =~ s/& */&/;
    $line =~ s/ *=/=/;
    $line =~ s/= */=/;

    # Convert to correct comments and add \ at end of line
    $line =~ s/^\/\/(.*)/\/\*$1\*\/\\/; 

    #(Wrap ps keys in "<KeyValue>\n"\ )
    $line =~ s/^(&.*)/"$1\\n"\\/; 
           
    # Print the line to the screen and add a newline 
    if( not $line =~ /^$/)
    {
        print C_FILE "$line\n"; 
    }
}
print C_FILE ";\n\n";
close C_FILE;
close PSR_FILE;
