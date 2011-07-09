#!/bin/sh
GCCVER=`gcc -dumpversion`
DIRS="/usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/x86_64-pc-linux-gnu/bits \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/x86_64-pc-linux-gnu/32/bits \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4 \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/tr1_impl \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/tr1 \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4 \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/parallel \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/ext \
      /usr/lib/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/bits \
      /usr/lib64/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4 \
      /usr/lib64/gcc/x86_64-pc-linux-gnu/${GCCVER}/include/g++-v4/debug"
for x in `grep CXXFLAGS Makefile`
do
	if test `echo $x | grep "^-I"`
	then
		DIRS="$DIRS `echo $x | sed "s/-I//g"`"
	fi
done
echo $DIRS

