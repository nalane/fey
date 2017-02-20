#!/usr/bin/perl

use strict;
use 5.010;

use File::Basename;

sub usage() {
    say "Usage: ${0} [-o | -s] item_name";
    die;
}

sub processTemplate() {
    my $template = shift @_;
    my $dest = shift @_;
    my $name = shift @_;

    open(my $templateFH, "<", $template) || die $!;
    open(my $destFH, ">", $dest) || die $!;
    while (my $line = <$templateFH>) {
	$line =~ s/\@name\@/${name}/g;
	print $destFH $line;
    }

    close $templateFH;
    close $destFH;
}

my $path = dirname($0);
my $templatesPath = $path . "/templates/";
my $includePath = $path . "/../inc/controllers";
my $sourcePath = $path . "/../src/controllers";

if (scalar(@ARGV) != 2) {
    &usage();
}

my $type = $ARGV[0];
if ($type ne '-o' && $type ne '-s') {
    &usage();
}

my $item_name = $ARGV[1];
my $destinationCPP = $sourcePath . "/" . $item_name . ".cpp";
my $destinationHPP = $includePath . "/" . $item_name . ".hpp";

my $sourceCPP = $templatesPath;
my $sourceHPP = $templatesPath;

if ($type eq '-o') {
    $sourceCPP .= "/object.cpp";
    $sourceHPP .= "/object.hpp";
}
else {
    $sourceCPP .= "/scene.cpp";
    $sourceHPP .= "/scene.hpp";
}

&processTemplate($sourceCPP, $destinationCPP, $item_name);
&processTemplate($sourceHPP, $destinationHPP, $item_name);
