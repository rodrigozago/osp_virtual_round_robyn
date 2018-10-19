##############################################################################
#                                                                            #
#            Makefile for compiling administrative part of OSP 		     #
#                                                                            #
##############################################################################

#
# Macros for directories
#

# Directory for gen.c , run_osp.c, and submit.c
SRC_DIR  = $(HOME_DIR)/admin

# Directory for *.o files
LIB_DIR  = $(HOME_DIR)/admin_lib

# Directory for executables: gen, run_osp, and submit
TARGET_DIR  = $(HOME_DIR)/bin

C_COMP = cc



#
# Make OSP
#

$(TARGET_DIR)/dummy : $(LIB_DIR)/run_osp.$(ARCH).o $(LIB_DIR)/submit.$(ARCH).o $(LIB_DIR)/gen.$(ARCH).o $(LIB_DIR)/index.$(ARCH).o
#	$(C_COMP) $(LIB_DIR)/check_date.$(ARCH).o $(LIB_DIR)/index.$(ARCH).o -o $(TARGET_DIR)/check_date.$(ARCH)
	$(C_COMP) $(LIB_DIR)/run_osp.$(ARCH).o $(LIB_DIR)/index.$(ARCH).o -o $(TARGET_DIR)/run_osp.$(ARCH)
	$(C_COMP) $(LIB_DIR)/submit.$(ARCH).o $(LIB_DIR)/index.$(ARCH).o -o $(TARGET_DIR)/submit.$(ARCH)
	$(C_COMP) $(LIB_DIR)/gen.$(ARCH).o $(LIB_DIR)/index.$(ARCH).o -o $(TARGET_DIR)/gen.$(ARCH)


#
# Compile each program
#
 
#$(LIB_DIR)/check_date.$(ARCH).o : $(SRC_DIR)/check_date.c
#	$(C_COMP) $(SRC_DIR)/check_date.c -c 
#	/bin/mv check_date.o  check_date.$(ARCH).o

$(LIB_DIR)/run_osp.$(ARCH).o : $(SRC_DIR)/run_osp.c
	$(C_COMP)  $(CCFLAGS) $(SRC_DIR)/run_osp.c -c $(BSDFLAGS)
	/bin/mv run_osp.o  run_osp.$(ARCH).o
 
$(LIB_DIR)/submit.$(ARCH).o : $(SRC_DIR)/submit.c
	$(C_COMP) $(SRC_DIR)/submit.c -c 
	/bin/mv submit.o  submit.$(ARCH).o

$(LIB_DIR)/gen.$(ARCH).o : $(SRC_DIR)/gen.c
	$(C_COMP) $(SRC_DIR)/gen.c -c 
	/bin/mv gen.o  gen.$(ARCH).o

$(LIB_DIR)/index.$(ARCH).o : $(SRC_DIR)/index.c
	$(C_COMP) $(SRC_DIR)/index.c -c 
	/bin/mv index.o  index.$(ARCH).o
