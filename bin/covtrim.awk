#!/usr/bin/awk -f
BEGIN {
    enable = 1
}
/0:Source:/ {
    enable = 1
}
/0:Source:\// {
    enable = 0
}
{
    if (enable)
        print;
}
