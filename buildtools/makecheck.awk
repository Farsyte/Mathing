#!/usr/bin/awk -f
#
# Read a GNUmakefile from standard input, and generate
# a fragment of a makefile on the output that prints
# the values of all variables set in the input.

BEGIN {
    printf "dumpvars::\n"

    q = "\""
    n = "\\n"
    d = "$"
}
/^#/ { next }
/^\t/ { next }
/=/ {
    v = $1
    if (saw[v] != "") {
        printf "\t# multiple entries for %s\n", v
        next
    }
    saw[v] = "y";
    vv = "$(" $1 ")"
    f1 = "%-14s"
    f2 = q "%s" q
    printf "\t-@ printf \"\t%s  = %s\\n\" \"%s\" \"%s\"\n", f1, f2, v, vv
}
