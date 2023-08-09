#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-mouse_dongle.mk)" "nbproject/Makefile-local-mouse_dongle.mk"
include nbproject/Makefile-local-mouse_dongle.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=mouse_dongle
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/config/mouse_dongle/interrupts.c ../src/config/mouse_dongle/stdio/xc32_monitor.c ../src/main.c ../src/config/mouse_dongle/exceptions.c ../src/config/mouse_dongle/peripheral/spi/spi_slave/plib_spi2_slave.c ../src/config/mouse_dongle/peripheral/clk/plib_clk.c ../src/config/mouse_dongle/initialization.c ../src/config/mouse_dongle/peripheral/evic/plib_evic.c ../src/config/mouse_dongle/peripheral/gpio/plib_gpio.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/848713354/interrupts.o ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/848713354/exceptions.o ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o ${OBJECTDIR}/_ext/1654279220/plib_clk.o ${OBJECTDIR}/_ext/848713354/initialization.o ${OBJECTDIR}/_ext/256882503/plib_evic.o ${OBJECTDIR}/_ext/256828675/plib_gpio.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/848713354/interrupts.o.d ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/848713354/exceptions.o.d ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o.d ${OBJECTDIR}/_ext/1654279220/plib_clk.o.d ${OBJECTDIR}/_ext/848713354/initialization.o.d ${OBJECTDIR}/_ext/256882503/plib_evic.o.d ${OBJECTDIR}/_ext/256828675/plib_gpio.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/848713354/interrupts.o ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/848713354/exceptions.o ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o ${OBJECTDIR}/_ext/1654279220/plib_clk.o ${OBJECTDIR}/_ext/848713354/initialization.o ${OBJECTDIR}/_ext/256882503/plib_evic.o ${OBJECTDIR}/_ext/256828675/plib_gpio.o

# Source Files
SOURCEFILES=../src/config/mouse_dongle/interrupts.c ../src/config/mouse_dongle/stdio/xc32_monitor.c ../src/main.c ../src/config/mouse_dongle/exceptions.c ../src/config/mouse_dongle/peripheral/spi/spi_slave/plib_spi2_slave.c ../src/config/mouse_dongle/peripheral/clk/plib_clk.c ../src/config/mouse_dongle/initialization.c ../src/config/mouse_dongle/peripheral/evic/plib_evic.c ../src/config/mouse_dongle/peripheral/gpio/plib_gpio.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-mouse_dongle.mk ${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ0512EFE064
MP_LINKER_FILE_OPTION=,--script="..\src\config\mouse_dongle\p32MZ0512EFE064.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/848713354/interrupts.o: ../src/config/mouse_dongle/interrupts.c  .generated_files/flags/mouse_dongle/db2cc419ef738e9a012802da1a101ec09f49bd6e .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/848713354" 
	@${RM} ${OBJECTDIR}/_ext/848713354/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/848713354/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/848713354/interrupts.o.d" -o ${OBJECTDIR}/_ext/848713354/interrupts.o ../src/config/mouse_dongle/interrupts.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1663542588/xc32_monitor.o: ../src/config/mouse_dongle/stdio/xc32_monitor.c  .generated_files/flags/mouse_dongle/4260e2879d7d792982138ae2b3fee8132612fd66 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1663542588" 
	@${RM} ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1663542588/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o ../src/config/mouse_dongle/stdio/xc32_monitor.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/mouse_dongle/209e9434326d5f06bc73d1e6d5785008087e43b9 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/848713354/exceptions.o: ../src/config/mouse_dongle/exceptions.c  .generated_files/flags/mouse_dongle/6c59ad95417b555277f8a169c1ae15f8c3fb8d68 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/848713354" 
	@${RM} ${OBJECTDIR}/_ext/848713354/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/848713354/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/848713354/exceptions.o.d" -o ${OBJECTDIR}/_ext/848713354/exceptions.o ../src/config/mouse_dongle/exceptions.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o: ../src/config/mouse_dongle/peripheral/spi/spi_slave/plib_spi2_slave.c  .generated_files/flags/mouse_dongle/97f4f9e6feeaf1f134194b1ba686081022c5f168 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1399043725" 
	@${RM} ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o.d 
	@${RM} ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o.d" -o ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o ../src/config/mouse_dongle/peripheral/spi/spi_slave/plib_spi2_slave.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1654279220/plib_clk.o: ../src/config/mouse_dongle/peripheral/clk/plib_clk.c  .generated_files/flags/mouse_dongle/815bf0a86e2ac83eaca14db857ba6aa95abe1b9b .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1654279220" 
	@${RM} ${OBJECTDIR}/_ext/1654279220/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1654279220/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1654279220/plib_clk.o.d" -o ${OBJECTDIR}/_ext/1654279220/plib_clk.o ../src/config/mouse_dongle/peripheral/clk/plib_clk.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/848713354/initialization.o: ../src/config/mouse_dongle/initialization.c  .generated_files/flags/mouse_dongle/39e09fe9eb55fde60682f89633b18e3e7e81b662 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/848713354" 
	@${RM} ${OBJECTDIR}/_ext/848713354/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/848713354/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/848713354/initialization.o.d" -o ${OBJECTDIR}/_ext/848713354/initialization.o ../src/config/mouse_dongle/initialization.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/256882503/plib_evic.o: ../src/config/mouse_dongle/peripheral/evic/plib_evic.c  .generated_files/flags/mouse_dongle/a5f58351802af44c63ed878885bd763772af60f9 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/256882503" 
	@${RM} ${OBJECTDIR}/_ext/256882503/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/256882503/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/256882503/plib_evic.o.d" -o ${OBJECTDIR}/_ext/256882503/plib_evic.o ../src/config/mouse_dongle/peripheral/evic/plib_evic.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/256828675/plib_gpio.o: ../src/config/mouse_dongle/peripheral/gpio/plib_gpio.c  .generated_files/flags/mouse_dongle/f259a480e434a6236838e03e2e5b864ccac915a3 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/256828675" 
	@${RM} ${OBJECTDIR}/_ext/256828675/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/256828675/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/256828675/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/256828675/plib_gpio.o ../src/config/mouse_dongle/peripheral/gpio/plib_gpio.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/848713354/interrupts.o: ../src/config/mouse_dongle/interrupts.c  .generated_files/flags/mouse_dongle/61974ee1526ce35d2275e6dab2168c23a84f8364 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/848713354" 
	@${RM} ${OBJECTDIR}/_ext/848713354/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/848713354/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/848713354/interrupts.o.d" -o ${OBJECTDIR}/_ext/848713354/interrupts.o ../src/config/mouse_dongle/interrupts.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1663542588/xc32_monitor.o: ../src/config/mouse_dongle/stdio/xc32_monitor.c  .generated_files/flags/mouse_dongle/94843646b1fa2acbf9902f22ba61b07eb25008ec .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1663542588" 
	@${RM} ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1663542588/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1663542588/xc32_monitor.o ../src/config/mouse_dongle/stdio/xc32_monitor.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/mouse_dongle/8f5c60524743b226395253437ff832a850775bed .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/848713354/exceptions.o: ../src/config/mouse_dongle/exceptions.c  .generated_files/flags/mouse_dongle/345ea76dc3adf591816bd3a7d230e7af98aba658 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/848713354" 
	@${RM} ${OBJECTDIR}/_ext/848713354/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/848713354/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/848713354/exceptions.o.d" -o ${OBJECTDIR}/_ext/848713354/exceptions.o ../src/config/mouse_dongle/exceptions.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o: ../src/config/mouse_dongle/peripheral/spi/spi_slave/plib_spi2_slave.c  .generated_files/flags/mouse_dongle/6322890eff799e7112c18bb940f2a54ee74041b3 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1399043725" 
	@${RM} ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o.d 
	@${RM} ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o.d" -o ${OBJECTDIR}/_ext/1399043725/plib_spi2_slave.o ../src/config/mouse_dongle/peripheral/spi/spi_slave/plib_spi2_slave.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1654279220/plib_clk.o: ../src/config/mouse_dongle/peripheral/clk/plib_clk.c  .generated_files/flags/mouse_dongle/804e6ba51245fb4dc97b3cd365abc83e4db70e26 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1654279220" 
	@${RM} ${OBJECTDIR}/_ext/1654279220/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1654279220/plib_clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1654279220/plib_clk.o.d" -o ${OBJECTDIR}/_ext/1654279220/plib_clk.o ../src/config/mouse_dongle/peripheral/clk/plib_clk.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/848713354/initialization.o: ../src/config/mouse_dongle/initialization.c  .generated_files/flags/mouse_dongle/fbb1ad27b9f283bff83e3a16d65b95d47bc5ca36 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/848713354" 
	@${RM} ${OBJECTDIR}/_ext/848713354/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/848713354/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/848713354/initialization.o.d" -o ${OBJECTDIR}/_ext/848713354/initialization.o ../src/config/mouse_dongle/initialization.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/256882503/plib_evic.o: ../src/config/mouse_dongle/peripheral/evic/plib_evic.c  .generated_files/flags/mouse_dongle/d466078037a5f05f44883654be5d125578474092 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/256882503" 
	@${RM} ${OBJECTDIR}/_ext/256882503/plib_evic.o.d 
	@${RM} ${OBJECTDIR}/_ext/256882503/plib_evic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/256882503/plib_evic.o.d" -o ${OBJECTDIR}/_ext/256882503/plib_evic.o ../src/config/mouse_dongle/peripheral/evic/plib_evic.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/256828675/plib_gpio.o: ../src/config/mouse_dongle/peripheral/gpio/plib_gpio.c  .generated_files/flags/mouse_dongle/7a39b87b85c49f5b466164c8b598e2e3df3a92b4 .generated_files/flags/mouse_dongle/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/256828675" 
	@${RM} ${OBJECTDIR}/_ext/256828675/plib_gpio.o.d 
	@${RM} ${OBJECTDIR}/_ext/256828675/plib_gpio.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/mouse_dongle" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/256828675/plib_gpio.o.d" -o ${OBJECTDIR}/_ext/256828675/plib_gpio.o ../src/config/mouse_dongle/peripheral/gpio/plib_gpio.c    -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/config/mouse_dongle/p32MZ0512EFE064.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/config/mouse_dongle/p32MZ0512EFE064.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_mouse_dongle=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/mouse_dongle.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
