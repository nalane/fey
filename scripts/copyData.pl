#!/usr/bin/perl

use strict;
use 5.010;

use File::Path qw(make_path);
use File::Spec;
use File::Copy;
use Data::Dumper qw(Dumper);

# Display usage info and die
sub usage() {
    say "Usage: ${0} sourceFolder destFolder";
    die;
}

# Copy from source folder to destination, converting shaders
sub copyDir() {
    # Get function arguments
    my $source = shift @_;
    my $dest = shift @_;

    # Exit if source is not a directory
    if (!-d $source) {
        return;
    }

    # Make sure dest directory exists
    if (!-d $dest) {
        make_path $dest;
    }

    opendir(my $dh, $source);
    my @files = readdir($dh);
    closedir($dh);
    foreach my $file (@files) {
        # Skip . and ..
        if ($file eq '.' || $file eq '..') {
            next;
        }

        my $input = File::Spec->catfile($source, $file);
        my $output = File::Spec->catfile($dest, $file);

        # Files should either be copied or translated in the case of shaders
        if (-f $input) {       
            $input = File::Spec->rel2abs($input);
            $output = File::Spec->rel2abs($output);

            # File is a shader file; translate it to SPIR-V then downgrade it to GL 4.1
            if ($input =~ m/\.(conf|vert|tesc|tese|geom|frag|comp)$/) {
                my $SPVoutput = $output . '.spv';
                my $command = "glslangValidator -V \"${input}\" -o \"${SPVoutput}\"";
                say $command;
                system($command);

                $command = "spirv-cross --version 410 \"${SPVoutput}\" --output \"${output}\"";
                say $command;
                system($command);
            }
            else {
                copy($input, $output);
            }
        }

        # Directories should be recursively copied
        else {
            &copyDir($input, $output);
        }
    }
}

# Make sure we have the right number of args
if (scalar(@ARGV) != 2) {
    &usage();
}

&copyDir(@ARGV);
