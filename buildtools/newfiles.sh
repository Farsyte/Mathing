#!/bin/bash -

find * -type f -print |
    sort > .newfiles

[ ! -f .oldfiles ] ||
    comm -13 .oldfiles .newfiles |
        sed 's/^/== ADD ==\t/'

mv .newfiles .oldfiles

exit 0
