#!/bin/sh
exuberant-ctags -R --langmap=c:. --c++-kinds=+lpx --fields=+iaS --extra=+q `./tags-dir.sh` ./src/

