#!/usr/bin/env perl
################################################################################
#
#           (c) Cambridge Silicon Radio Limited 2010
#
#           All rights reserved and confidential information of CSR
#
################################################################################

#
# Name
#       codesize - report the code size of libraries and symbols within 
#                  libraries.
#
# Synopsis
#       codesize --libpaths <path1[,path2,...]>
#                [--libs <libname1/path1,libname2/path2,...>]
#                [--listlibs] [--collapse]
#
# Options:
#
#     --libpaths: Comma-separated list of directories containing libraries to
#                 investigate. The specified directories must contain nothing 
#                 but libraries.
#     --libs: Comma-separated list of any mix of library names (with or
#             without path) and directories containing libraries (and
#             nothing but libraries). If this parameter is not specified, 
#             the code size of all libraries will be reported. If one or 
#             more libraries are specified, only the code size of
#             these libraries will be reported.
#     --listlibs: If this option is specified, all libraries found in the
#                 directories specified by --libpaths will be printed, and
#                 no code sizes will be reported.
#     --symbols: Report the code size of individual symbols.
#     --zeroinit: Include zero initialised sections.
#

use strict;
use warnings;
use File::Basename;
use Getopt::Long;

my $libpaths;
my $libs = "";
my $symbols;
my $zeroinit;
my $listlibs;

GetOptions("symbols" => \$symbols,
           "zeroinit" => \$zeroinit,
           "listlibs" => \$listlibs,
           "libpaths=s"  => \$libpaths,
           "libs=s" => \$libs);

die "Usage: " . basename($0) . " --libpaths <libpaths> [--libs <libs>] [--listlibs] [--symbols] [--zeroinit]" unless $libpaths;

# Build list of libs to investigate
my @libs;
foreach my $libpath (split(',', $libpaths))
{
    my $filesFound = 0;
    $libpath =~ s/[\/\\]$//; # remove trailing slash
    my @files = glob("$libpath/*");
    foreach my $file (@files)
    {
        if (-f $file)
        {
            $filesFound = 1;
            push(@libs, $file);
        }
    }
    if (not $filesFound == 1)
    {
        print STDERR "Warning: No libraries found at libpath \"$libpath\"!\n";
    }
}

# Build hash of libs
my %hlibs = ();
if ($libs) 
{
    foreach my $libspec (split(',', $libs))
    {
        if (-d $libspec)
        {
            $libspec =~ s/[\/\\]$//; # remove trailing slash
            my @libs = glob("$libspec/*");

            foreach my $lib (@libs)
            {
                $lib =~ s/.*[\\\/](.*)/$1/;
                $hlibs{$lib} = 1;
            }
        }
        else
        {
            $libspec =~ /.*?[\\\/]?(.*)/;
            my $lib = $1;
            $hlibs{$lib} = 1;
        }
    }
}

my $libscount = @libs;
if ($libscount == 0)
{
    print STDERR "Error: No libraries found at any of the specified libpaths!\n";
    exit(1);
}

if ($listlibs)
{
    foreach my $lib (@libs)
    {
        $lib =~ s/.*[\\\/](.*)/$1/;
        print "\t$lib\n";
    }
    exit(0);
}

sub processNmLine
{
    my ($line, $symboltypes) = @_;
    if ($line =~ /^[0-9]+ ([0-9]+) [$symboltypes] (.*)/)
    {
        my $size = int($1);
        my $name = $2;
        print("    $name: $size\n");
    }
}

# Extract information
foreach my $lib (@libs)
{    
    # Extract library name without path
    $lib =~ /.*[\\\/](.*)/;
    my $libname = $1;

    if (not $libs or $hlibs{$libname}) 
    {
        my $libSize;
        my $libTextSize = 0;
        my $libDataSize = 0;
        my $libConstdataSize = 0;
        my $libBssSize = 0;
        my @objdumpout = split('\n', `objdump -h $lib`);
        foreach my $line (@objdumpout)
        {
            if ($line =~ /^\s*[0-9] ([^\s]+)\s+([0-9a-f]+) /)
            {
                my $name = $1;
                my $size = hex($2);
                $libTextSize += $size if ($name eq ".text");
                $libDataSize += $size if ($name eq ".data");
                $libConstdataSize += $size if ($name eq ".constdata");
                $libConstdataSize += $size if ($name eq ".conststring");
                $libBssSize += $size if ($name eq ".bss");
            }
        }
        $libSize = $libTextSize + $libDataSize + $libConstdataSize;
        $libSize += $libBssSize if $zeroinit;
        print("$lib: $libSize\n");

        if ($symbols)
        {
            my @nmout = split('\n', `nm --demangle --radix=d --defined-only --print-size --size-sort $lib`);
            print("  .text: $libTextSize\n");
            foreach my $line (@nmout)
            {
                processNmLine($line, "Tt");
            }
            print("  .data: $libDataSize\n");
            foreach my $line (@nmout)
            {
                processNmLine($line, "DdGg");
            }
            print("  .constdata: $libConstdataSize\n");
            foreach my $line (@nmout)
            {
                processNmLine($line, "Rr");
            }
            if ($zeroinit) 
            {
                print("  .bss: $libBssSize\n");
                foreach my $line (@nmout)
                {
                    processNmLine($line, "BbSs");
                }
            }
        }
    }
}
