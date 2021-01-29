#!/usr/bin/perl
#######################################################################
# check.pl - based on driver.pl, CS:APP Data Lab driver
#
# Copyright (c) 2004-2011, R. Bryant and D. O'Hallaron, All rights
# reserved.  May not be used, modified, or copied without permission.
#
# Note: The driver can use either btest or the BDD checker to check
# puzzles for correctness. This version of the lab uses btest, which
# has been extended to do better testing of both integer and
# floating-point puzzles.
#
#######################################################################

use strict 'vars';
use Getopt::Std;

# Set to 1 to use btest, 0 to use the BDD checker.
my $USE_BTEST = 1;

# Generic settings 
$| = 1;      # Flush stdout each time
umask(0077); # Files created by the user in tmp readable only by that user
$ENV{PATH} = "/usr/local/bin:/usr/bin:/bin";

#
# usage - print help message and terminate
#
sub usage {
    printf STDERR "$_[0]\n";
    printf STDERR "Usage: $0 [-h]\n";
    printf STDERR "Options:\n";
    printf STDERR "  -h              Print this message.\n";
    die "\n";
}

##############
# Main routine
##############
my $login = getlogin() || (getpwuid($<))[0] || "unknown";
my $tmpdir = "/var/tmp/datalab.$login.$$";
my $diemsg = "The files are in $tmpdir.";

my $driverfiles;
my $infile;
my $autograded;

my $status;
my $inpuzzles;
my $puzzlecnt;
my $line;
my $blank;
my $name;
my $c_points;
my $c_rating;
my $c_errors;
my $p_points;
my $p_rating;
my $p_errors;
my $total_c_points;
my $total_c_rating;
my $total_p_points;
my $total_p_rating;
my $tops;
my $tpoints;
my $trating;
my $foo;
my $name;
my $msg;
my $nickname;
my $autoresult;

my $bitsc_checksum;

my %puzzle_c_points;
my %puzzle_c_rating;
my %puzzle_c_errors;
my %puzzle_p_points;
my %puzzle_p_ops;
my %puzzle_p_maxops;
my %puzzle_number;


# Parse the command line arguments
no strict;
getopts('h:f:A');
if ($opt_h) {
    usage();
}

# The default input file is bits.c (change with -f)
$infile = "bits.c";
$nickname = "";

#####
# These are command line args that every driver must support
#

# Hidden flag that indicates that the driver was invoked by an autograder
if ($opt_A) {
    $autograded = $opt_A;
}

#####
# Drivers can also define an arbitary number of other command line args
#
# Optional hidden flag used by the autograder
if ($opt_f) {  
    $infile = $opt_f;
}

use strict 'vars';

################################################
# Compute the correctness and performance scores
################################################

# Make sure that an executable dlc (data lab compiler) exists
(-e "./dlc" and -x "./dlc")
    or  die "$0: ERROR: No executable dlc binary.\n";


# If using the bdd checker, then make sure it exists
if (!$USE_BTEST) {
    (-e "./bddcheck/cbit/cbit" and -x "./bddcheck/cbit/cbit")
	or  die "$0: ERROR: No executable cbit binary.\n";
}

#
# Set up the contents of the scratch directory
#
system("mkdir $tmpdir") == 0
    or die "$0: Could not make scratch directory $tmpdir.\n";

# Copy the student's work to the scratch directory
unless (system("cp $infile $tmpdir/bits.c") == 0) { 
    clean($tmpdir);
    die "$0: Could not copy file $infile to scratch directory $tmpdir.\n";
}

# Copy the various autograding files to the scratch directory
if ($USE_BTEST) {
    $driverfiles = "Makefile dlc btest.c decl.c tests.c btest.h bits.h";
    unless (system("cp -r $driverfiles $tmpdir") == 0) {
	clean($tmpdir);
	die "$0: Could not copy autogradingfiles to $tmpdir.\n";
    }
} 
else {
    $driverfiles = "dlc tests.c bddcheck";
    unless (system("cp -r $driverfiles $tmpdir") == 0) {
	clean($tmpdir);
	die "$0: Could not copy support files to $tmpdir.\n";
    }
}

# Change the current working directory to the scratch directory
unless (chdir($tmpdir)) {
    clean($tmpdir);
    die "$0: Could not change directory to $tmpdir.\n";
}

#
# Check that bits.c compiles in C89 mode. We do this first because GCC
# often gives much better error messages than DLC.
#

$bitsc_checksum = `sha1sum bits.c`;
$bitsc_checksum = substr($bitsc_checksum, 0, 40);

print "1. Checking compilation of bits.c with GCC in C89 mode.\n";
my $exitcode = system("gcc -std=c89 -pedantic -pedantic-errors -c bits.c");
if ($exitcode != 0) {
  print STDERR "$0: ERROR: bits.c did not compile correctly.\n";
  die "You will need to fix the above errors before you can continue.\n";
}

#
# Generate a zapped (for coding rules) version of bits.c. In this
# zapped version of bits.c, any functions with illegal operators are
# transformed to have empty function bodies.
#
print "\n2. Running './dlc -z' to identify coding rules violations.\n";
system("cp bits.c save-bits.c") == 0
    or die "$0: ERROR: Could not create backup copy of bits.c. $diemsg\n";
system("./dlc -z -o zap-bits.c bits.c") == 0
    or die "$0: ERROR: zapped bits.c did not compile. $diemsg\n";

#
# Run btest or BDD checker to determine correctness score
#
if ($USE_BTEST) {
    print "\n3. Compiling and running './btest -g' to determine correctness score.\n";
    system("cp zap-bits.c bits.c");

    # Compile btest
    system("make btestexplicit") == 0
	or die "$0: Could not make btest in $tmpdir. $diemsg\n";

    # Run btest
    $status = system("./btest -g > btest-zapped.out 2>&1");
    if ($status != 0) {
	die "$0: ERROR: btest check failed. $diemsg\n";
    }
}
else {
    print "\n3. Running './bddcheck/check.pl -g' to determine correctness score.\n";
    system("cp zap-bits.c bits.c");
    $status = system("./bddcheck/check.pl -g > btest-zapped.out 2>&1");
    if ($status != 0) {
	die "$0: ERROR: BDD check failed. $diemsg\n";
    }
}

#
# Run dlc to identify operator count violations.
# 
print "\n4. Running './dlc -Z' to identify operator count violations.\n";
system("./dlc -Z -o Zap-bits.c save-bits.c") == 0
    or die "$0: ERROR: dlc unable to generated Zapped bits.c file.\n";

#
# Run btest or the bdd checker to compute performance score
#
if ($USE_BTEST) {
    print "\n5. Compiling and running './btest -g -r 2' to determine performance score.\n";
    system("cp Zap-bits.c bits.c");

    # Compile btest
    system("make btestexplicit") == 0
	or die "$0: Could not make btest in $tmpdir. $diemsg\n";

    # Run btest
    $status = system("./btest -g -r 2 > btest-Zapped.out 2>&1");
    if ($status != 0) {
	die "$0: ERROR: Zapped btest failed. $diemsg\n";
    }
}
else {
    print "\n5. Running './bddcheck/check.pl -g -r 2' to determine performance score.\n";
    system("cp Zap-bits.c bits.c");
    $status = system("./bddcheck/check.pl -g -r 2 > btest-Zapped.out 2>&1");
    if ($status != 0) {
	die "$0: ERROR: Zapped bdd checker failed. $diemsg\n";
    }
}

#
# Run dlc to get the operator counts on the zapped input file
#
print "\n6. Running './dlc -e' to get operator count of each function.\n";
$status = system("./dlc -W1 -e zap-bits.c > dlc-opcount.out 2>&1");
if ($status != 0) {
    die "$0: ERROR: bits.c did not compile. $diemsg\n";
}
 
#################################################################
# Collect the correctness and performance results for each puzzle
#################################################################

#
# Collect the correctness results
#
%puzzle_c_points = (); # Correctness score computed by btest
%puzzle_c_errors = (); # Correctness error discovered by btest
%puzzle_c_rating = (); # Correctness puzzle rating (max points)
  
$inpuzzles = 0;      # Becomes true when we start reading puzzle results
$puzzlecnt = 0;      # Each puzzle gets a unique number
$total_c_points = 0;
$total_c_rating = 0; 

open(INFILE, "$tmpdir/btest-zapped.out") 
    or die "$0: ERROR: could not open input file $tmpdir/btest-zapped.out\n";

while ($line = <INFILE>) {
    chomp($line);

    # Notice that we're ready to read the puzzle scores
    if ($line =~ /^Score/) {
	$inpuzzles = 1;
	next;
    }

    # Notice that we're through reading the puzzle scores
    if ($line =~ /^Total/) {
	$inpuzzles = 0;
	next;
    }

    # Read and record a puzzle's name and score
    if ($inpuzzles) {
	($blank, $c_points, $c_rating, $c_errors, $name) = split(/\s+/, $line);
	$puzzle_c_points{$name} = $c_points;
	$puzzle_c_errors{$name} = $c_errors;
	$puzzle_c_rating{$name} = $c_rating;
	$puzzle_number{$name} = $puzzlecnt++;
	$total_c_points += $c_points;
	$total_c_rating += $c_rating;
    }

}
close(INFILE);

#
# Collect the performance results
#
%puzzle_p_points = (); # Performance points

$inpuzzles = 0;       # Becomes true when we start reading puzzle results
$total_p_points = 0;  
$total_p_rating = 0;

open(INFILE, "$tmpdir/btest-Zapped.out") 
    or die "$0: ERROR: could not open input file $tmpdir/btest-Zapped.out\n";

while ($line = <INFILE>) {
    chomp($line);

    # Notice that we're ready to read the puzzle scores
    if ($line =~ /^Score/) {
	$inpuzzles = 1;
	next;
    }

    # Notice that we're through reading the puzzle scores
    if ($line =~ /^Total/) {
	$inpuzzles = 0;
	next;
    }

    # Read and record a puzzle's name and score
    if ($inpuzzles) {
	($blank, $p_points, $p_rating, $p_errors, $name) = split(/\s+/, $line);
	$puzzle_p_points{$name} = $p_points;
	$total_p_points += $p_points;
	$total_p_rating += $p_rating;
    }
}
close(INFILE);

#
# Collect the operator counts generated by dlc
#
open(INFILE, "$tmpdir/dlc-opcount.out") 
    or die "$0: ERROR: could not open input file $tmpdir/dlc-opcount.out\n";

$tops = 0;
while ($line = <INFILE>) {
    chomp($line);

    if ($line =~ /(\d+) operators/) {
	($foo, $foo, $foo, $name, $msg) = split(/:/, $line);
	$puzzle_p_ops{$name} = $1;
	$tops += $1;
    }
}
close(INFILE);

# 
# Print a table of results sorted by puzzle number
#

print "\n";
print "------------------------- cut here ---------------------------------\n";
print "\n";

my $report = "";

$report .= sprintf("%s\t%s\n", "Correctness Results", "Perf Results");
$report .= sprintf("%s\t%s\t%s\t%s\t%s\t%s\n", "Points", "Rating", "Errors",
		   "Points", "Ops", "Puzzle");
foreach $name (sort {$puzzle_number{$a} <=> $puzzle_number{$b}} 
	       keys %puzzle_number) {
    $report .= sprintf("%d\t%d\t%d\t%d\t%d\t\%s\n", 
	   $puzzle_c_points{$name},
	   $puzzle_c_rating{$name},
	   $puzzle_c_errors{$name},
	   $puzzle_p_points{$name},
	   $puzzle_p_ops{$name},
	   $name);
}

$tpoints = $total_c_points + $total_p_points;
$trating = $total_c_rating + $total_p_rating;

$report .= sprintf "\nRaw Score = $tpoints/$trating [$total_c_points/$total_c_rating Corr + $total_p_points/$total_p_rating Perf] ($tops total operators)\n";

$report .= "\n";

$report .= "Username: $login\n";
$report .= "Host: " . `hostname -f`;
$report .= "Time: " . scalar(localtime(time())) . "\n";
$report .= "bits.c checksum: $bitsc_checksum\n";

my $scaled_tpoints = 2*$tpoints;
my $scaled_trating = 2*$trating;

$report .= "\nScaled auto-grading score = $scaled_tpoints/$scaled_trating\n";

print $report;

open(REPORT, ">report")
  or die "$0: ERROR: could not open report file $tmpdir/report\n";
print REPORT $report;
close REPORT;

open(SHA1, "sha1sum report |")
  or die "$0: ERROR: could not compute report checksum\n";
my $report_checksum = <SHA1>;
close SHA1;
$report_checksum = substr($report_checksum, 0, 40);

print "\n";
print "Report checksum: $report_checksum\n";

print "\n";
print "------------------------- cut here ---------------------------------\n";
print "\n";

print "Copy the results in between the lines above into a comment at the\n";
print "end of your submission to show that you have run this script.\n";


# Clean up and exit
clean ($tmpdir);
exit;

##################
# Helper functions
#

#
# clean - remove the scratch directory
#
sub clean {
    my $tmpdir = shift;
    system("rm -rf $tmpdir");
}

