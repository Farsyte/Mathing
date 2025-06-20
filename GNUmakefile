# -*- Makefile -*-
#
# This Makefile is specific to building a project where all of the
# real meat is in header files, and where each source (source/*.cc)
# compiles to a binary that can be run as part of the build.
#
# Debug and Release build products are kept in separate subdirectories.
#
# Debug products are compiled with "gprof" support, including coverage
# analysis.

Q		= @
I		= -
C		= $I $Q

M		= $Q ${MAKE} --no-print-directory
R		= $C /bin/rm -f

SRCDIR		= source/
INCDIR		= include/
DBGDIR		= debug/
RELDIR		= release/

CXX		= g++
XSTD		= --std=c++03
WEFF		= -Weffc++
WARN		= -Wall -Wextra -Wpedantic ${WEFF}

COV		= gcov
PRO		= gprof

CXXFLAGS	= $(XSTD) $(WARN) $(INCDIR:%=-I%)

# Debug build is set up for debugging, profiling, test coverage,
# and whatever optimizations can be done in that context.
DBG_FLAGS	= -g -Og -pg --coverage

# Release build is compiled for speed, even at the cost of losing
# strict standards compliant.
REL_FLAGS	= -Ofast

DBGDEP		= $C ${CXX} ${CXXFLAGS} ${CPPFLAGS} ${DBG_FLAGS} ${TARGET_ARCH} -MM
DBGCXX		= ${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} ${DBG_FLAGS} ${TARGET_ARCH}

RELDEP		= $C ${CXX} ${CXXFLAGS} ${CPPFLAGS} ${REL_FLAGS} ${TARGET_ARCH} -MM
RELCXX		= ${CXX} ${CXXFLAGS} ${CPPFLAGS} ${LDFLAGS} ${REL_FLAGS} ${TARGET_ARCH}

SRCS		= ${wildcard ${SRCDIR}*.cc}

DBINS		= ${SRCS:${SRCDIR}%.cc=${DBGDIR}%}
DOUTS		= ${DBINS:${DBGDIR}%=${DBGDIR}%.out}
DDEPS		= ${DBINS:${DBGDIR}%=${DBGDIR}%.d}
DGCDAS		= ${DBINS:${DBGDIR}%=${DBGDIR}%.gcda}
DGCNOS		= ${DBINS:${DBGDIR}%=${DBGDIR}%.gcno}
DPROS		= ${DBINS:${DBGDIR}%=${DBGDIR}%.pro}
DCOVS		= ${SRCS:%.cc=%.cov}

RBINS		= ${SRCS:${SRCDIR}%.cc=${RELDIR}%}
RDEPS		= ${RBINS:${RELDIR}%=${RELDIR}%.d}
ROUTS		= ${RBINS:${RELDIR}%=${RELDIR}%.out}
RGCDAS		= ${RBINS:${RELDIR}%=${RELDIR}%.gcda}
RGCNOS		= ${RBINS:${RELDIR}%=${RELDIR}%.gcno}

# TARGET help -- print help text (must be the first target in the file)
help:
	$C printf "Valid build targets:\n"
	$C grep '^# TARGET ' < GNUmakefile | sed 's/# TARGET /    make /'
	$Q false

# TARGET all_dbg -- build all debug binaries
all_dbg:	$(DBINS)

# TARGET all_rel -- build all release binaries
all_rel:	$(RBINS)

# TARGET run_dbg -- run all debug binaries
run_dbg:	$(DOUTS)

# TARGET pro_dbg -- generate profile reports
pro_dbg:	$(DPROS)

# TARGET pro_dbg -- generate profile reports
cov_dbg:	$(DCOVS)

# TARGET run_rel -- run all release binaries
run_rel:	$(ROUTS)

# TARGET clean -- remove build-generated files
clean:
	$R $(DBINS) $(DOUTS) $(DERRS) $(DGCDAS) $(DGCNOS) $(DDEPS) $(DPROS) $(DCOVS)
	$R $(RBINS) $(ROUTS) $(RERRS) $(RGCDAS) $(RGCNOS) $(RDEPS)
	$R gmon.out *.gcov

# TARGET world -- rebuild everything
world:
	$M clean
	$M all_dbg
	$M run_dbg
	$M pro_dbg
	$M cov_dbg
	$M all_rel
	$M run_rel

include ${wildcard ${DBGDIR}*.d} ${wildcard ${RELDIR}*.d} /dev/null

${DBGDIR}%:	${SRCDIR}%.cc
	${DBGDEP} $< | sed '1s|^$*\.o: |$@: |' > $@.d
	${DBGCXX} $^ ${LOADLIBES} ${LDLIBS} -o $@

${DBGDIR}%.out:	${DBGDIR}%
	${DBGDIR}$* > "$@" 2>&1

${DBGDIR}%.pro:	${DBGDIR}%.out
	${PRO} ${DBGDIR}$* > "$@"

${SRCDIR}%.cov:	${DBGDIR}%.out
	${COV} ${DBGDIR}$* -t | bin/covtrim.awk > "$@"

${RELDIR}%:	${SRCDIR}%.cc
	${RELDEP} $< | sed '1s|^$*\.o: |$@: |' > $@.d
	${RELCXX} $^ ${LOADLIBES} ${LDLIBS} -o $@

${RELDIR}%.out:	${RELDIR}%
	${RELDIR}$* > "$@" 2>&1

# TARGET check -- print a bunch of variables for debugging the GNUmakefile
check:
	$C printf "%-32s := %s\n" "Q" "${Q}"
	$C printf "%-32s := %s\n" "I" "${I}"
	$C printf "%-32s := %s\n" "C" "${C}"
	$C printf "%-32s := %s\n" "M" "${M}"
	$C printf "%-32s := %s\n" "R" "${R}"
	$C printf "%-32s := %s\n" "SRCDIR" "${SRCDIR}"
	$C printf "%-32s := %s\n" "INCDIR" "${INCDIR}"
	$C printf "%-32s := %s\n" "DBGDIR" "${DBGDIR}"
	$C printf "%-32s := %s\n" "RELDIR" "${RELDIR}"
	$C printf "%-32s := %s\n" "CXX" "${CXX}"
	$C printf "%-32s := %s\n" "XSTD" "${XSTD}"
	$C printf "%-32s := %s\n" "WEFF" "${WEFF}"
	$C printf "%-32s := %s\n" "WARN" "${WARN}"
	$C printf "%-32s := %s\n" "CXXFLAGS" "${CXXFLAGS}"
	$C printf "%-32s := %s\n" "DBG" "${DBG}"
	$C printf "%-32s := %s\n" "DBGOPT" "${DBGOPT}"
	$C printf "%-32s := %s\n" "PROF" "${PROF}"
	$C printf "%-32s := %s\n" "COV" "${COV}"
	$C printf "%-32s := %s\n" "OPT" "${OPT}"
	$C printf "%-32s := %s\n" "DBG_FLAGS" "${DBG_FLAGS}"
	$C printf "%-32s := %s\n" "REL_FLAGS" "${REL_FLAGS}"
	$C printf "%-32s := %s\n" "DBGDEP" "${DBGDEP}"
	$C printf "%-32s := %s\n" "DBGCXX" "${DBGCXX}"
	$C printf "%-32s := %s\n" "RELDEP" "${RELDEP}"
	$C printf "%-32s := %s\n" "RELCXX" "${RELCXX}"
	$C printf "%-32s := %s\n" "SRCS" "${SRCS}"
	$C printf "%-32s := %s\n" "DBINS" "${DBINS}"
	$C printf "%-32s := %s\n" "DOUTS" "${DOUTS}"
	$C printf "%-32s := %s\n" "DDEPS" "${DDEPS}"
	$C printf "%-32s := %s\n" "DERRS" "${DERRS}"
	$C printf "%-32s := %s\n" "DGCDAS" "${DGCDAS}"
	$C printf "%-32s := %s\n" "DGCNOS" "${DGCNOS}"
	$C printf "%-32s := %s\n" "RBINS" "${RBINS}"
	$C printf "%-32s := %s\n" "RDEPS" "${RDEPS}"
	$C printf "%-32s := %s\n" "ROUTS" "${ROUTS}"
	$C printf "%-32s := %s\n" "RERRS" "${RERRS}"
	$C printf "%-32s := %s\n" "RGCDAS" "${RGCDAS}"
	$C printf "%-32s := %s\n" "RGCNOS" "${RGCNOS}"
