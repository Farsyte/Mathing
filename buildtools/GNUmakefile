# -*- Makefile -*-
#
# TODO: re-document this file.
# TODO: let "world" build the PDFs again.

# ================================================================
# Variables frequently modified on the "make" command line

# Optimizer Level. Defaults to "fast but may be hard to debug."
# Use "O=g" to be "debugging support but maybe not as fast."

O               := 2

Q               := @
I               := -

J               := -j

# Support "CPPFLAGS=-DNDEBUG" gutting the DEBUG macros, much
# like it guts the C library <assert.h> facility.
#
#

CPPFLAGS        := -UNDEBUG -DOPTIMIZER_LEVEL='"$O"'

# ================================================================
# Capture the name of the current makefile.
# Be sure to do this before including anything.

MAKEFILE        := ${word ${words ${MAKEFILE_LIST}},${MAKEFILE_LIST}}

# ================================================================
# Project Directory Tree Layout

INCLUDE.d       := include/
SOURCE.d        := source/
TEST.d          := test/
BENCH.d         := bench/

TOOL.d          := buildtools/
DOC.d           := doc/
WEB.d           := webtree/
TEX.d           := latex/

OUTBASE.d	:= output/
OUTPUT.d        := ${OUTBASE.d}O$O/
TESTRUN.d       := ${OUTPUT.d}test/
BENCHRUN.d      := ${OUTPUT.d}bench/
OBJECTS.d       := ${OUTPUT.d}lib/

RESBASE.d	:= result/
RESULT.d        := ${RESBASE.d}O$O/
TESTRES.d       := ${RESULT.d}test/
BENCHRES.d      := ${RESULT.d}bench/
LIBRARY.d       := ${RESULT.d}lib/

# ================================================================
# Base Names of Importance

LIBNAME         := mathing
JMA.pdf         := ${DOC.d}Just-Mathing-Around.pdf

# ================================================================
# Configuration Variables for Build Tools

CXX             := g++
XSTD            := --std=c++03
WARN            := -Wall -Wextra -Wpedantic -Weffc++

FORMAT          := clang-format
FMTFLAGS        := -i

RL              := ranlib
ARFLAGS         := -r

COV             := gcov

PRO             := gprof
PFLAGS          := -b

OFLAGS          := -O$O -g -pg --coverage
IFLAGS          := ${INCLUDE.d:%/=-I%}
CXXFLAGS        := ${XSTD} ${WARN} ${IFLAGS} ${OFLAGS}

LDFLAGS         := ${LIBRARY.d:%/=-L%}
LDLIBS          := ${LIBNAME:%=-l%}

COMPILE         := ${CXX} ${CXXFLAGS} ${CPPFLAGS} ${TARGET_ARCH}
COMPILEo        := ${COMPILE} -c
COMPILEx        := ${COMPILE} ${LDFLAGS}

DEPEND          := ${COMPILE} -MM

# ================================================================
# Derived File Names

INCLUDE.hh      := ${wildcard ${INCLUDE.d}*.hh}
LIBRARY.cc      := ${wildcard ${SOURCE.d}*.cc}
TEST.cc         := ${wildcard ${TEST.d}*.cc}
BENCH.cc        := ${wildcard ${BENCH.d}*.cc}

LIBRARY.o       := ${LIBRARY.cc:${SOURCE.d}%.cc=${OBJECTS.d}%.o}

LIBRARY.a       := ${LIBNAME:%=${LIBRARY.d}lib%.a}

TEST.exe        := ${TEST.cc:${TEST.d}%.cc=${TESTRUN.d}%}
TEST.out        := ${TEST.cc:${TEST.d}%.cc=${TESTRES.d}%.out}
TEST.pro        := ${TEST.cc:${TEST.d}%.cc=${TESTRES.d}%.pro}

BENCH.exe       := ${BENCH.cc:${BENCH.d}%.cc=${BENCHRUN.d}%}
BENCH.out       := ${BENCH.cc:${BENCH.d}%.cc=${BENCHRES.d}%.out}
BENCH.pro       := ${BENCH.cc:${BENCH.d}%.cc=${BENCHRES.d}%.pro}

INDEX.html      := ${WEB.d}index.html
REFMAN.pdf      := ${TEX.d}refman.pdf

PUBLISH         := ${JMA.pdf}

COVMARK         := ${OUTPUT.d}.coverage-ran

# ================================================================
# Macros for Command Command Prefixes and Common Commands

C               := $I $Q
M               := $Q ${MAKE} --no-print-directory -f "${MAKEFILE}"
R               := $C /bin/rm -fr
P               := $C printf
D               := $C mkdir -p
T               := $Q true

# ================================================================
# Useful Build Rules

# TARGET default -- the "build" suitable for the "edit/build/test" loop
default:        rebuild                                         ; $T

# TARGET help -- print list of common build targets and what they do
help:
	$C printf "Valid build targets:\n"
	$C grep '^# TARGET ' < "${MAKEFILE}" | \
	   sed 's/# TARGET /    make /'

# TARGET world -- build everything (*) from scratch.
# Does not build webtree or publish, for now.
# Does not build bench.out or bench.exe, for now.
world:
	$M cleanest
	$M $J test.exe bench.exe coverage
	$M test
	$M clean

# TARGET rebuild -- rebuild everything (*) that is out-of-date.
# Does not build webtree or publish, for now.
# Does not build bench.out or bench.exe, for now.
rebuild:
	$M $J test test.exe coverage
	$M clean

# TARGET webtree -- build auto-generated documentation (but not the PDFs)
webtree:        ${INDEX.html}                                   ; $T

# TARGET publish -- generate PDF files.
publish:        ${PUBLISH}                                      ; $T

# TARGET format -- re-style all the C++ source files
format:
	$Q $P "[FORMAT] using %s\n" "${FORMAT}"
	$Q ${FORMAT} ${FMTFLAGS} ${INCLUDE.hh} ${LIBRARY.cc} ${TEST.cc}

# TARGET library.o -- (re)build the object files for the library.
library.o:      ${LIBRARY.o}                                    ; $T

# TARGET library.a -- (re)build the library archive file
library.a:      ${LIBRARY.a}                                    ; $T

# TARGET test.exe  -- (re)build the test executables.
test.exe:       ${TEST.exe}                                     ; $T

# TARGET test -- (re)build the test output files.
test:           ${TEST.out}                                     ; $T

# TARGET untest -- remove test results (to force a re-test)
untest:
	$R ${TEST.out}

# TARGET coverage -- produce test coverage analysis for recent test runs
# Note that benchmarks do not count for test coverage.
coverage:       ${COVMARK}                                      ; $T

# TARGET bench.exe  -- (re)build the bench executables.
bench.exe:       ${BENCH.exe}                                   ; $T

# TARGET bench.out  -- (re)run the benchmarks.
bench.out:       ${BENCH.out}                                   ; $T

# TARGET bench -- run benchmarks that do not have updated results.
bench:
	$M $J bench.exe
	$M bench.out

# TARGET unbench -- remove bench results (to force a re-bench)
unbench:
	$R ${BENCH.out}

# TARGET clean -- tidy up the build, removing basic detritus
clean:
	$P "[CLEAN]\n"
	$R ${OBJECTS.d}*.o
	$R ${DOC.d}doxygen.log

# TARGET cleaner -- clean, then remove intermediate build products
cleaner:        clean
	$P "[CLEANER]\n"
	$R ${OUTPUT.d}

# TARGET cleanest -- remove products of "m world" for all configs
cleanest:       cleaner
	$P "[CLEANEST]\n"
	$R ${OUTBASE.d}/O*
	$R ${RESBASE.d}/O*
	$R ${INCLUDE.d}*.gcov
	$R ${SOURCE.d}*.gcov
	$R ${TEST.d}*.gcov

# TARGET unpublish -- remove generated documentation.
unpublish:
	$P "[UNPUBLISH]\n"
	$R ${DOC.d}html ${DOC.d}latex
	$R ${TEX.d} ${WEB.d}

# TARGET makevars -- print makefile variables (for debugging makefile)
makevars:       .makevars.mak
	$M dumpvars

# ================================================================
# Incorporate dynamically-generated build rules

# dependency files (which may not yet be present) in ${TESTRUN.d}.
include ${wildcard ${TESTRUN.d}*.d} /dev/null

# dependency files (which may not yet be present) in ${BENCHRUN.d}.
include ${wildcard ${BENCHRUN.d}*.d} /dev/null

# dependency files (which may not yet be present) in ${OBJECTS.d}.
include ${wildcard ${OBJECTS.d}*.d} /dev/null

# dependency files (which may not yet be present) in ${LIBRARY.d}.
include ${wildcard ${LIBRARY.d}*.d} /dev/null

# automatically generated rules (which may not yet be present)
# used by "m makevars"
include ${wildcard .*.mak} /dev/null

# ================================================================
# Building the Library

${LIBRARY.o}:     ${OBJECTS.d}%.o:           ${SOURCE.d}%.cc
	$D ${OBJECTS.d}
	$D ${LIBRARY.d}
	$P "[LIB] %s\n" "$@"
	$Q ${DEPEND} ${SOURCE.d}$*.cc | sed '1s|^$*\.o: |$@: |' > ${OBJECTS.d}$*.d
	$Q ${DEPEND} ${SOURCE.d}$*.cc | sed '1s|^$*\.o: |${LIBRARY.a}: |' > ${LIBRARY.d}lib${LIBNAME}--$*.d
	$Q ${COMPILEo} ${SOURCE.d}$*.cc -o $@

# STUPID MAKE TRICK: ${LIBRARY.a} is not dependent on ${LIBRARY.o}.
# It will be built if it does not exist, or if any ${LIBRARY.cc} are
# newer, or if a header used by ${LIBRARY.cc} when recently built has
# been updated.
#
# Removing ${LIBRARY.o} thus does not force a library rebuild.
# 
${LIBRARY.a}:     ${LIBRARY.cc}
	$M library.o
	$D ${LIBRARY.d}
	$P "[LIB] %s\n" "$@"
	$Q ${AR} ${ARFLAGS} ${LIBRARY.a} ${LIBRARY.o}
	$Q ${RL} $@

# ================================================================
# Building, Running, and Profiling Tests

${TEST.exe}:        ${TESTRUN.d}%:            ${TEST.d}%.cc ${LIBRARY.a}
	$D ${TESTRUN.d}
	$P "[COMPILE TEST] %s\n" "$*"
	$R ${TESTRUN.d}$*.{gcda,gcno}
	$R ${TESTRUN.d}$*.rd/gmon.out
	$Q ${DEPEND} ${TEST.d}$*.cc | sed '1s|^$*\.o: |${TESTRUN.d}$*: |' > ${TESTRUN.d}$*.d
	$Q ${COMPILEx} ${TEST.d}$*.cc -o ${TESTRUN.d}$* ${LDLIBS}

${TEST.out}:        ${TESTRES.d}%.out:        ${TESTRUN.d}%
	$D ${TESTRES.d}
	$D ${TESTRUN.d}$*.rd
	$P "[RUN TEST] %s\n" "$*"
	$Q ( cd "${TESTRUN.d}$*.rd" ; ../$* ) 2>&1 | tee ${TESTRES.d}$*.out
	$Q [ -e ${TESTRUN.d}$*.rd/gmon.out ]
	$P "[PROFILE TEST] %s\n" "$*"
	$Q ${PRO} ${PFLAGS} ${TESTRUN.d}$* ${TESTRUN.d}$*.rd/gmon.out > ${TESTRES.d}$*.pro

# ================================================================
# Checking Code Coverage of Tests

${COVMARK}:     ${TEST.out}
	$C printf "[COVERAGE FOR TEST] %s\n" "${TEST.exe}"
	$Q ${COV} -m ${TEST.exe} -t | buildtools/store-coverage.awk
	$C touch ${COVMARK}

# ================================================================
# Building, Running, and Profilng Benchmarks

${BENCH.exe}:   ${BENCHRUN.d}%:         ${BENCH.d}%.cc ${LIBRARY.a}
	$D ${BENCHRUN.d}
	$R ${BENCHRUN.d}$*.rd/gmon.out
	$P "[BUILD BENCHMARK] %s\n" "$*"
	$R ${BENCHRUN.d}$*.{gcda,gcno}
	$Q ${DEPEND} ${BENCH.d}$*.cc | sed '1s|^$*\.o: |${BENCHRUN.d}$*: |' > ${BENCHRUN.d}$*.d
	$Q ${COMPILEx} ${BENCH.d}$*.cc -o ${BENCHRUN.d}$* ${LDLIBS}

${BENCH.out}:   ${BENCHRES.d}%.out:     ${BENCHRUN.d}%
	$D ${BENCHRES.d}
	$D ${BENCHRUN.d}$*.rd
	$P "[RUN BENCHMARK] %s\n" "$*"
	$Q ( cd "${BENCHRUN.d}$*.rd" ; ../$* ) 2>&1 | tee ${BENCHRES.d}$*.out
	$Q [ -e ${BENCHRUN.d}$*.rd/gmon.out ]
	$P "[PROFILE BENCHMARK] %s\n" "$*"
	$Q ${PRO} ${PFLAGS} ${BENCHRUN.d}$* ${BENCHRUN.d}$*.rd/gmon.out > ${BENCHRES.d}$*.pro

# ================================================================
# Generating the Reference Manual as a set of Web Pages

${INDEX.html}:       ${INCLUDE.hh} ${LIBRARY.cc} ${TEST.cc}
	$P "[DOC] doxygen\n"
	$C doxygen ${TOOL.d}Doxyfile > ${DOC.d}doxygen.log

# ================================================================
# Generating the Reference Manual as a Single PDF

${JMA.pdf}:         ${INDEX.html}
	$P "[DOC] %s\n" "$@"
	$Q ( cd ${TEX.d}; make ) > ${TEX.d}/pdflatex.log
	$Q cp -f ${REFMAN.pdf} ${JMA.pdf}

# ================================================================
# Tools for debugging the makefile

.makevars.mak:  ${MAKEFILE} buildtools/makecheck.awk
	$C awk -f buildtools/makecheck.awk < "${MAKEFILE}" > "$@"

# ================================================================
# These target name are *not* the name of a file they build.
# It helps if GNU Make knows this.

.PHONY:         default help world rebuild bench rebench
.PHONY:         webtree publish format
.PHONY:         library.o library.a
.PHONY:         test.exe test.out untest coverage
.PHONY:         bench.exe bench.out unbench
.PHONY:         clean cleaner cleanest unpublish makevars
