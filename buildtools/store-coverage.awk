#!/usr/bin/awk -f
#
# 

function ltrim(s) { sub(/^ +/, "", s); return s }
function rtrim(s) { sub(/ +$/, "", s); return s }
function trim(s)  { return rtrim(ltrim(s)); }

function src_fini() {
    destfile = source[0];
    destfile = substr(destfile, index(destfile, ":")+1);
    destfile = destfile ".gcov"

    for (ln = 1; ln <= line_hwm; ++ln) {
        printf "%8s:%8s:%s\n", count[ln], ln, source[ln] > destfile
    }

    close(destfile)

    source[0] = "none"
    count[0] = 0
    delete source
    delete count

    line_hwm = -1
}

BEGIN {
    enable = 1;
    FS = ":"
    line_hwm = -1
}

END {
    if (line_hwm >= 0) src_fini()
}

/^----/ {
    next;
}

{
    ct = trim($1) # number, "-", or "#####" (or number followed by star?)
    if (ct ~ /.*\*$/) ct = substr(ct, 0, length(ct) - 1);
    ln = trim($2) # number
    ln = int(ln)

    rest = $0
    rest = substr(rest, index(rest, ":")+1)
    rest = substr(rest, index(rest, ":")+1)

    accept = 0
}

(ct=="-") && (ln==0) && ($3=="Source") {
    if (line_hwm >= 0) src_fini()
    ss = substr($4,0,1)
    if (ss == "/") {
        enable = 0;
    } else {
        enable = 1
        source[0] = rest
    }
    next;
}

(ln==0) { next }

enable == 0 {
    next;
}

/^ *[0-9]+:/ { accept = 1 }
/^ *[0-9]+\*:/ { accept = 1 }
/^ *#+:/ { accept = 1 }
/^ *-:/ { accept = 1 }

accept == 0 {
    next;
}

{
    if (ln > line_hwm) {
        line_hwm = ln;
        source[ln] = rest;
        count[ln] = ct;
    } else {
        if (rest == source[ln]) {
            oc = count[ln]
            if ((oc == "-") && (ct == "-")) {
                nc = "-";
            } else if ((oc == "#####") && (ct == "#####")) {
                nc = "#####";
            } else {
                nc = int(oc) + int(ct)
            }
            count[ln] = nc
        } else {
            printf "## TODO resolve source text change in line %d\n", ln
            printf "## old source %s\n", source[ln]
            printf "## new source %s\n", rest
        }
    }
}
