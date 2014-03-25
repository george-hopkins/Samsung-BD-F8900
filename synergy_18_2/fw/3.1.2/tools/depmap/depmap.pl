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
#       depmap - map dependencies of/on/between libraries.
#
# Synopsis
#       depmap --libpaths <path1[,path2,...]>
#              [--maplibs <libname1/path1,libname2/path2,...>]
#              [--listlibs] [--collapse] [--noforward] [--noreverse]
#              [--noundefined] [--dot]
#
# Options:
#
#     --libpaths: Comma-separated list of directories containing libraries to
#                 investigate. The specified directories must contain nothing 
#                 but libraries.
#     --maplibs: Comma-separated list of any mix of library names (with or
#                without path) and directories containing libraries (and
#                nothing but libraries). If this parameter is not specified, 
#                the dependencies of all libraries will be mapped. If one or 
#                more libraries are specified, only the dependencies on/of 
#                these libraries will be mapped.
#     --listlibs: If this option is specified, all libraries found in the
#                 directories specified by --libpaths will be printed, and
#                 no dependency mapping will be conducted.
#     --collapse: Collapse individual symbol dependencies into library
#                 dependencies. This is recommended when using --dot.
#     --noforward: Disregard forward dependencies (from maplibs to others)
#     --noreverse: Disregard reverse dependencies (to maplibs from others)
#     --noundefined: Disregard dependencies on symbols that are not defined in
#                    any library.
#     --dot: Output in dot format suitable for Graphviz (dot or neato).
#

use strict;
use warnings;
use File::Basename;
use Getopt::Long;

my $libpaths;
my $maplibs;
my $dot;
my $rev = 1;
my $forw = 1;
my $collapse;
my $undefined = 1;
my $listlibs;

GetOptions("dot" => \$dot,
           "reverse!" => \$rev,
           "forward!" => \$forw,
           "collapse" => \$collapse,
           "undefined!" => \$undefined,
           "listlibs" => \$listlibs,
           "libpaths=s"  => \$libpaths,
           "maplibs=s" => \$maplibs);

die "Usage: " . basename($0) . " --libpaths <libpaths> [--maplibs <maplibs>] [--listlibs] [--dot] [--collapse] [--noreverse] [--noforward] [--noundefined]" unless $libpaths;

my %symbols = (); # Look up where a symbol is defined (symbol => libname)
my %refs = (); # Look up the undefined (0) and defined (1) references of a specific libname (libname => symbol => 0/1)
my %mapped = (); # Keep track of dependencies (no duplicates)

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

# Extract information
foreach my $lib (@libs)
{
    my %librefs = ();
    my @nmout = split('\n', `nm --demangle --no-sort $lib`);

    # Extract library name without path
    $lib =~ /.*[\\\/](.*)/;
    my $libname = $1;

    foreach my $line (@nmout)
    {
        if ($line =~ /^.* [TDBGR] (.*)/) # Match symbols defined inside the library
        {
            my $symbol = $1;
            if ($symbols{$symbol})
            {
                $symbols{$symbol} = $symbols{$symbol} . "," . $libname;
            }
            else
            {
                $symbols{$symbol} = $libname;
            }
            $librefs{$symbol} = 1; # Tag as intra-lib reference (the library defines the symbol itself)
        }
        if ($line =~ /^.* U (.*)/) # Match undefined symbols
        {
            my $symbol = $1;
            if (not $line =~ /^.* U _(.*)/) # Disregard system references (assumed to start with _)
            {
                if (not $librefs{$symbol})
                {
                    $librefs{$symbol} = 0;
                }
            }
        }
    }

    $refs{$libname} = \%librefs;
}

# Store a single dependency in %mapped hash (no duplicates)
sub mapDep
{
    my ($src, $dst, $where) = @_;
    my $dep;

    if ($where)
    {
        $dep = "$src -> $dst ($where)";
    }
    else
    {
        $dep = "$src -> $dst";
    }
    $mapped{$dep} = 1;
}

# Build all dependencies
foreach my $maplib (keys(%refs))
{
    my %librefs = %{$refs{$maplib}};

    foreach my $symbol (keys(%librefs))
    {
        if ($librefs{$symbol} == 0) # Ignore intra-lib references
        {
            my $where = $symbols{$symbol};
            if (not $where)
            {
                mapDep($maplib, $symbol);
            }
            else
            {
                if ($collapse)
                {
                    foreach my $deflib (split(',', $where))
                    {
                        mapDep($maplib, $deflib);
                    }
                }
                else
                {
                    mapDep($maplib, $symbol, $where);
                }
            }
        }
    }
}

# Build hash of maplibs
my %hmaplibs = ();
foreach my $maplibspec (split(',', $maplibs))
{
    if (-d $maplibspec)
    {
        $maplibspec =~ s/[\/\\]$//; # remove trailing slash
        my @maplibs = glob("$maplibspec/*");

        foreach my $maplib (@maplibs)
        {
            $maplib =~ s/.*[\\\/](.*)/$1/;
            if ($refs{$maplib})
            {
                $hmaplibs{$maplib} = 1;
            }
            else
            {
                print STDERR "Warning: Library \"$maplib\" not found!\n";
            }
        }
    }
    else
    {
        $maplibspec =~ /.*?[\\\/]?(.*)/;
        my $maplib = $1;
        if ($refs{$maplib})
        {
            $hmaplibs{$maplib} = 1;
        }
        else
        {
            print STDERR "Warning: Library \"$maplibspec\" not found!\n";
        }
    }
}

sub inMapLibs
{
    my ($libs) = @_;
    return 1 if not $maplibs;
    foreach my $lib (split(',', $libs))
    {
        if ($hmaplibs{$lib})
        {
            return 1;
        }
    }
    return 0;
}

if ($dot)
{
    print "digraph \"source tree\"\n{\n";
    print "\toverlap=scale;\n";
    print "\tsize=\"8,8\";\n";
    print "\tratio=\"fill\";\n";
    print "\tfontsize=\"28\";\n";
    print "\tfontname=\"Helvetica\";\n";
}

foreach my $dep (sort(keys(%mapped)))
{
    my @matchouter = $dep =~ /^(.*) -> (.*)$/;
    my @matchinner = $matchouter[1] =~ /^(.*) \((.*)\)$/;

    if (($forw and inMapLibs($matchouter[0])) or # Forward dependencies of form "lib -> ..."
        ($rev and inMapLibs($matchouter[1])) or # Reverse dependencies of form "... -> lib"
        ($rev and $matchinner[1] and inMapLibs($matchinner[1]))) # Reverse dependencies of form "... -> symbol (lib1,lib2,...)"
    {
        if ($undefined or $matchinner[1] or $refs{$matchouter[1]}) # Ignore undefined symbols of form "... -> symbol"
        {
            if ($dot)
            {
                $dep =~ s/^(.*) -> (.*)$/\"$1\" -> \"$2\"/;
            }
            print "\t$dep\n";
        }
    }
}

print "}\n" if $dot;
