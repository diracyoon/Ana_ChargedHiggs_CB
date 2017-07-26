AUTOMAKE_OPTIONS = foreign

AM_CXXFLAGS = -Wall -Werror -std=gnu++14 -fPIC
INCLUDES = -I`root-config --incdir`

lib_LTLIBRARIES = libAna_ChargedHiggs_CB.la

include_HEADERS = 

noinst_HEADERS = Ana_ChargedHiggs_CB_LinkDef.h

dist_data_DATA =

AM_LDFLAGS = \
        -L$(libdir)

libAna_ChargedHiggs_CB_la_LIBADD = \
        -L$(libdir) 

libAna_ChargedHiggs_CB_la_SOURCES = \
  Fitter_Performance.C \
  Ana_ChargedHiggs_CB_Dict.C

BUILT_SOURCES = \
  testexternals.C

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libAna_ChargedHiggs_CB.la

testexternals.C:
	@echo '//*** this is a generated file. Do not commit, do not edit' > $@
	@echo 'int main()' >> $@
	@echo '{' >> $@
	@echo '  return 0;' >> $@
	@echo '}' >> $@

Ana_ChargedHiggs_CB_Dict.C: \
  Const_Def.h \
  Fitter_Performance.h \
  Ana_ChargedHiggs_CB_LinkDef.h
	rootcling -f $@ -I`root-config --incdir` -c $^

pcm :
	make install
	cp Ana_ChargedHiggs_CB_Dict_rdict.pcm $(libdir)

CLEANFILES = Ana_ChargedHiggs_CB_Dict.C Ana_ChargedHiggs_CB_Dict.h Ana_ChargedHiggs_CB_Dict_rdict.pcm
