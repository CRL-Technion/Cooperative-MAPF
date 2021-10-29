###########################################################################
## Makefile generated for MATLAB file/project 'get_best_assignments'. 
## 
## Makefile     : get_best_assignments_rtw.mk
## Generated on : Mon May 31 21:55:02 2021
## MATLAB Coder version: 5.0 (R2020a)
## 
## Build Info:
## 
## Final product: ./get_best_assignments.dll
## Product type : dynamic-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# DEF_FILE                Definition file

PRODUCT_NAME              = get_best_assignments
MAKEFILE                  = get_best_assignments_rtw.mk
MATLAB_ROOT               = C:/PROGRA~1/MATLAB/R2020a
MATLAB_BIN                = C:/PROGRA~1/MATLAB/R2020a/bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)/win64
MASTER_ANCHOR_DIR         = 
START_DIR                 = C:/Users/Nir/GitRepos/CooperativeWarehouse/codegen/dll/get_best_assignments
TGT_FCN_LIB               = ISO_C++
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = .
COMPILER_COMMAND_FILE     = get_best_assignments_rtw_comp.rsp
CMD_FILE                  = get_best_assignments_rtw.rsp
DEF_FILE                  = $(PRODUCT_NAME).def
C_STANDARD_OPTS           = -fwrapv
CPP_STANDARD_OPTS         = -fwrapv

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          MinGW64 | gmake (64-bit Windows)
# Supported Version(s):    6.x
# ToolchainInfo Version:   2020a
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# C_STANDARD_OPTS
# CPP_STANDARD_OPTS
# MINGW_ROOT
# MINGW_C_STANDARD_OPTS

#-----------
# MACROS
#-----------

WARN_FLAGS            = -Wall -W -Wwrite-strings -Winline -Wstrict-prototypes -Wnested-externs -Wpointer-arith -Wcast-align
WARN_FLAGS_MAX        = $(WARN_FLAGS) -Wcast-qual -Wshadow
CPP_WARN_FLAGS        = -Wall -W -Wwrite-strings -Winline -Wpointer-arith -Wcast-align
CPP_WARN_FLAGS_MAX    = $(CPP_WARN_FLAGS) -Wcast-qual -Wshadow
MW_EXTERNLIB_DIR      = $(MATLAB_ROOT)/extern/lib/win64/mingw64
SHELL                 = %SystemRoot%/system32/cmd.exe

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = -lws2_32

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: GNU C Compiler
CC_PATH = $(MINGW_ROOT)
CC = "$(CC_PATH)/gcc"

# Linker: GNU Linker
LD_PATH = $(MINGW_ROOT)
LD = "$(LD_PATH)/g++"

# C++ Compiler: GNU C++ Compiler
CPP_PATH = $(MINGW_ROOT)
CPP = "$(CPP_PATH)/g++"

# C++ Linker: GNU C++ Linker
CPP_LD_PATH = $(MINGW_ROOT)
CPP_LD = "$(CPP_LD_PATH)/g++"

# Archiver: GNU Archiver
AR_PATH = $(MINGW_ROOT)
AR = "$(AR_PATH)/ar"

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)/mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: GMAKE Utility
MAKE_PATH = %MATLAB%/bin/win64
MAKE = "$(MAKE_PATH)/gmake"


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -g
C_OUTPUT_FLAG       = -o
LDDEBUG             = -g
OUTPUT_FLAG         = -o
CPPDEBUG            = -g
CPP_OUTPUT_FLAG     = -o
CPPLDDEBUG          = -g
OUTPUT_FLAG         = -o
ARDEBUG             =
STATICLIB_OUTPUT_FLAG =
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @move
RUN                 =

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = ruvs
CFLAGS               = -c $(MINGW_C_STANDARD_OPTS) -m64 \
                       -O3 -fno-loop-optimize -fno-aggressive-loop-optimizations
CPPFLAGS             = -c $(CPP_STANDARD_OPTS) -m64 -std=c++11 \
                       -O3 -fno-loop-optimize -fno-aggressive-loop-optimizations
CPP_LDFLAGS          = -Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)" -static -m64
CPP_SHAREDLIB_LDFLAGS  = -shared -Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)" -Wl,--no-undefined \
                         -Wl,--out-implib,$(basename $(PRODUCT)).lib
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = -Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)" -static -m64
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = -shared -Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)" -Wl,--no-undefined \
                       -Wl,--out-implib,$(basename $(PRODUCT)).lib



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = ./get_best_assignments.dll
PRODUCT_TYPE = "dynamic-library"
BUILD_TYPE = "Dynamic Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_ = -D__USE_MINGW_ANSI_STDIO=1 -DBUILDING_GET_BEST_ASSIGNMENTS -DMODEL=get_best_assignments
DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=get_best_assignments

DEFINES = $(DEFINES_) $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/rt_nonfinite.cpp $(START_DIR)/rtGetNaN.cpp $(START_DIR)/rtGetInf.cpp $(START_DIR)/get_best_assignments_rtwutil.cpp $(START_DIR)/ITaskAssignment.cpp $(START_DIR)/assignkbestsd.cpp $(START_DIR)/assignsd.cpp $(START_DIR)/sdRemoveImpossibles.cpp $(START_DIR)/bsxfun.cpp $(START_DIR)/minOrMax.cpp $(START_DIR)/assign2DRelaxed.cpp $(START_DIR)/assignAuctionWithPrice.cpp $(START_DIR)/lapRemoveImpossibles.cpp $(START_DIR)/lapAuction.cpp $(START_DIR)/unique.cpp $(START_DIR)/sortIdx.cpp $(START_DIR)/sortrows.cpp $(START_DIR)/repmat.cpp $(START_DIR)/kbestSDPartitionSolution.cpp $(START_DIR)/kbestEnforceConstraints.cpp $(START_DIR)/sort.cpp

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = rt_nonfinite.obj rtGetNaN.obj rtGetInf.obj get_best_assignments_rtwutil.obj ITaskAssignment.obj assignkbestsd.obj assignsd.obj sdRemoveImpossibles.obj bsxfun.obj minOrMax.obj assign2DRelaxed.obj assignAuctionWithPrice.obj lapRemoveImpossibles.obj lapAuction.obj unique.obj sortIdx.obj sortrows.obj repmat.obj kbestSDPartitionSolution.obj kbestEnforceConstraints.obj sort.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_ = -fvisibility=hidden
CFLAGS_BASIC = $(DEFINES) $(INCLUDES) @$(COMPILER_COMMAND_FILE)

CFLAGS += $(CFLAGS_) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_ = -fvisibility=hidden
CPPFLAGS_BASIC = $(DEFINES) $(INCLUDES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS += $(CPPFLAGS_) $(CPPFLAGS_BASIC)

#---------------------
# MEX C++ Compiler
#---------------------

MEX_CPP_Compiler_BASIC =  @$(COMPILER_COMMAND_FILE)

MEX_CPPFLAGS += $(MEX_CPP_Compiler_BASIC)

#-----------------
# MEX Compiler
#-----------------

MEX_Compiler_BASIC =  @$(COMPILER_COMMAND_FILE)

MEX_CFLAGS += $(MEX_Compiler_BASIC)

###########################################################################
## INLINED COMMANDS
###########################################################################


MINGW_C_STANDARD_OPTS = $(C_STANDARD_OPTS)


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute


all : build
	@echo "### Successfully generated all binary outputs."


build : prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


###########################################################################
## FINAL TARGET
###########################################################################

#----------------------------------------
# Create a dynamic library
#----------------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@echo "### Creating dynamic library "$(PRODUCT)" ..."
	$(CPP_LD) $(CPP_SHAREDLIB_LDFLAGS) -o $(PRODUCT) @$(CMD_FILE) $(SYSTEM_LIBS) $(TOOLCHAIN_LIBS)
	@echo "### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

%.obj : %.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : %.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : $(START_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.obj : C:/Users/Nir/GitRepos/CooperativeWarehouse/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.obj : C:/Users/Nir/GitRepos/CooperativeWarehouse/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


rt_nonfinite.obj : $(START_DIR)/rt_nonfinite.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


rtGetNaN.obj : $(START_DIR)/rtGetNaN.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


rtGetInf.obj : $(START_DIR)/rtGetInf.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


get_best_assignments_rtwutil.obj : $(START_DIR)/get_best_assignments_rtwutil.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


ITaskAssignment.obj : $(START_DIR)/ITaskAssignment.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


assignkbestsd.obj : $(START_DIR)/assignkbestsd.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


assignsd.obj : $(START_DIR)/assignsd.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


sdRemoveImpossibles.obj : $(START_DIR)/sdRemoveImpossibles.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


bsxfun.obj : $(START_DIR)/bsxfun.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


minOrMax.obj : $(START_DIR)/minOrMax.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


assign2DRelaxed.obj : $(START_DIR)/assign2DRelaxed.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


assignAuctionWithPrice.obj : $(START_DIR)/assignAuctionWithPrice.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


lapRemoveImpossibles.obj : $(START_DIR)/lapRemoveImpossibles.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


lapAuction.obj : $(START_DIR)/lapAuction.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


unique.obj : $(START_DIR)/unique.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


sortIdx.obj : $(START_DIR)/sortIdx.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


sortrows.obj : $(START_DIR)/sortrows.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


repmat.obj : $(START_DIR)/repmat.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


kbestSDPartitionSolution.obj : $(START_DIR)/kbestSDPartitionSolution.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


kbestEnforceConstraints.obj : $(START_DIR)/kbestEnforceConstraints.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


sort.obj : $(START_DIR)/sort.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(COMPILER_COMMAND_FILE) $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@echo "### PRODUCT = $(PRODUCT)"
	@echo "### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@echo "### BUILD_TYPE = $(BUILD_TYPE)"
	@echo "### INCLUDES = $(INCLUDES)"
	@echo "### DEFINES = $(DEFINES)"
	@echo "### ALL_SRCS = $(ALL_SRCS)"
	@echo "### ALL_OBJS = $(ALL_OBJS)"
	@echo "### LIBS = $(LIBS)"
	@echo "### MODELREF_LIBS = $(MODELREF_LIBS)"
	@echo "### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@echo "### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@echo "### CFLAGS = $(CFLAGS)"
	@echo "### LDFLAGS = $(LDFLAGS)"
	@echo "### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@echo "### CPPFLAGS = $(CPPFLAGS)"
	@echo "### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@echo "### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@echo "### ARFLAGS = $(ARFLAGS)"
	@echo "### MEX_CFLAGS = $(MEX_CFLAGS)"
	@echo "### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@echo "### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@echo "### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@echo "### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@echo "### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@echo "### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	$(RM) $(subst /,\,$(PRODUCT))
	$(RM) $(subst /,\,$(ALL_OBJS))
	$(ECHO) "### Deleted all derived files."


