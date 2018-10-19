##############################################################################
#                                                                            #
#            Makefile for compiling Operating Systems Project                #
#                                                                            #
##############################################################################

#
# Macros for directories
#

# Directory for *.c and *.h files
SRC_DIR  = $(HOME_DIR)/src

# Directory for *.o files
LIB_DIR  = $(HOME_DIR)/lib.$(ARCH)

# Directory for OSP
TARGET_DIR  = $(HOME_DIR)/demo

# Compiler
C_COMP = cc

#This Makefile is executed from osp.compile after switching to LIB_DIR


#
# Make OSP
#

$(TARGET_DIR)/OSP.demo.$(ARCH) : $(LIB_DIR)/cpu.o $(LIB_DIR)/devices.o $(LIB_DIR)/devint.o $(LIB_DIR)/dialog.o $(LIB_DIR)/files.o $(LIB_DIR)/inter.o $(LIB_DIR)/iosvc.o $(LIB_DIR)/mask.o $(LIB_DIR)/memory.o $(LIB_DIR)/resources.o $(LIB_DIR)/pageint.o $(LIB_DIR)/procsvc.o $(LIB_DIR)/simcore.o $(LIB_DIR)/timeint.o $(LIB_DIR)/random.o $(LIB_DIR)/sockets.o $(LIB_DIR)/protocols.o
	$(C_COMP) $(LIB_DIR)/cpu.o $(LIB_DIR)/devices.o $(LIB_DIR)/devint.o $(LIB_DIR)/dialog.o $(LIB_DIR)/files.o $(LIB_DIR)/inter.o $(LIB_DIR)/iosvc.o $(LIB_DIR)/mask.o $(LIB_DIR)/memory.o $(LIB_DIR)/resources.o $(LIB_DIR)/pageint.o $(LIB_DIR)/procsvc.o $(LIB_DIR)/simcore.o $(LIB_DIR)/timeint.o $(LIB_DIR)/random.o $(LIB_DIR)/sockets.o $(LIB_DIR)/protocols.o -lm $(DEBUG) -o $(TARGET_DIR)/OSP.demo.$(ARCH)



#
# Compile each module
#

$(LIB_DIR)/cpu.o : $(SRC_DIR)/cpu.c $(SRC_DIR)/cpu.h $(SRC_DIR)/cpu.private.h
	$(C_COMP) $(SRC_DIR)/cpu.c -c $(DEBUG) 

$(LIB_DIR)/devices.o : $(SRC_DIR)/devices.c $(SRC_DIR)/devices.h
	$(C_COMP) $(SRC_DIR)/devices.c -c $(DEBUG) 
 
$(LIB_DIR)/devint.o : $(SRC_DIR)/devint.c $(SRC_DIR)/devint.h
	$(C_COMP) $(SRC_DIR)/devint.c -c $(DEBUG) 

$(LIB_DIR)/dialog.o : $(SRC_DIR)/dialog.c
	$(C_COMP) $(SRC_DIR)/dialog.c -c $(DEBUG) 

$(LIB_DIR)/files.o : $(SRC_DIR)/files.c $(SRC_DIR)/files.h $(SRC_DIR)/files.private.h 
	$(C_COMP) $(CCFLAGS) $(SRC_DIR)/files.c -c $(DEBUG) $(BSDFLAGS)

$(LIB_DIR)/inter.o : $(SRC_DIR)/inter.c $(SRC_DIR)/inter.h
	$(C_COMP) $(SRC_DIR)/inter.c -c $(DEBUG) 

$(LIB_DIR)/iosvc.o : $(SRC_DIR)/iosvc.c $(SRC_DIR)/iosvc.h
	$(C_COMP) $(SRC_DIR)/iosvc.c -c $(DEBUG) 

$(LIB_DIR)/mask.o : $(SRC_DIR)/mask.c $(SRC_DIR)/mask.h
	$(C_COMP) $(SRC_DIR)/mask.c -c $(DEBUG) 

$(LIB_DIR)/memory.o : $(SRC_DIR)/memory.c $(SRC_DIR)/memory.h $(SRC_DIR)/memory.private.h
	$(C_COMP) $(SRC_DIR)/memory.c -c $(DEBUG) 

$(LIB_DIR)/resources.o : $(SRC_DIR)/resources.c $(SRC_DIR)/resources.h $(SRC_DIR)/resources.private.h
	$(C_COMP) $(SRC_DIR)/resources.c -c $(DEBUG) 

$(LIB_DIR)/pageint.o : $(SRC_DIR)/pageint.c $(SRC_DIR)/pageint.h
	$(C_COMP) $(SRC_DIR)/pageint.c -c $(DEBUG) 

$(LIB_DIR)/procsvc.o : $(SRC_DIR)/procsvc.c $(SRC_DIR)/procsvc.h $(SRC_DIR)/procsvc.private.h
	$(C_COMP) $(SRC_DIR)/procsvc.c -c $(DEBUG) 

$(LIB_DIR)/simcore.o : $(SRC_DIR)/simcore.c $(SRC_DIR)/simcore.h $(SRC_DIR)/simcore.private.h 
	$(C_COMP)  $(CCFLAGS) $(SRC_DIR)/simcore.c -c $(DEBUG) $(BSDFLAGS)

$(LIB_DIR)/timeint.o : $(SRC_DIR)/timeint.c $(SRC_DIR)/timeint.h
	$(C_COMP) $(SRC_DIR)/timeint.c -c $(DEBUG) 

$(LIB_DIR)/random.o : $(SRC_DIR)/random.c
	$(C_COMP) $(SRC_DIR)/random.c -c

$(LIB_DIR)/sockets.o : $(SRC_DIR)/sockets.c $(SRC_DIR)/sockets.h $(SRC_DIR)/sockets.private.h
	$(C_COMP) $(SRC_DIR)/sockets.c -c $(DEBUG)

$(LIB_DIR)/protocols.o : $(SRC_DIR)/protocols.c $(SRC_DIR)/protocols.h $(SRC_DIR)/protocols.private.h
	$(C_COMP) $(SRC_DIR)/protocols.c -c $(DEBUG)
