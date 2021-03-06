FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)  


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}
                    "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src/tests")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DisplayHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" "-h")
SET_TESTS_PROPERTIES(integration-DisplayHelp 
                     PROPERTIES PASS_REGULAR_EXPRESSION "openfluid \\[<command>\\] \\[<options>\\] \\[<args>\\]")  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DisplayVersion 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "--version")
  SET_TESTS_PROPERTIES(integration-DisplayVersion PROPERTIES PASS_REGULAR_EXPRESSION ${OPENFLUID_VERSION_FULL})  


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ShowPaths 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                       "show-paths"
                   )  
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "#1 foobar")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "#1 barfoo")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OFBUILD_TESTS_USERDATA_DIR}")
SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OFBUILD_TESTS_TEMP_DIR}")
IF(OFBUILD_ENABLE_MARKET)
  SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES PASS_REGULAR_EXPRESSION "${OPENFLUID_MARKETBAGDIR}")  
ELSE()
  SET_TESTS_PROPERTIES(integration-ShowPaths PROPERTIES FAIL_REGULAR_EXPRESSION "${OPENFLUID_MARKETBAGDIR}")
ENDIF()



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ShowPathsNoEnvVars
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                       "show-paths"
                       "-p" "foobar"
                       "-n" "barfoo")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES ENVIRONMENT "")                  
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES PASS_REGULAR_EXPRESSION "#1 foobar")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES PASS_REGULAR_EXPRESSION "#1 barfoo")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES FAIL_REGULAR_EXPRESSION "${OFBUILD_TESTS_USERDATA_DIR}")
SET_TESTS_PROPERTIES(integration-ShowPathsNoEnvVars PROPERTIES FAIL_REGULAR_EXPRESSION "${OFBUILD_TESTS_TEMP_DIR}")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsList 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "simulators" "--list"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsReport 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" "simulators"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ObserversList 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "observers" "--list"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-ObserversList PROPERTIES PASS_REGULAR_EXPRESSION "tests.empty")  
                         
 
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-ObserversReport
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "observers"
                           "-n" "${OFBUILD_TESTS_BINARY_DIR}")  
SET_TESTS_PROPERTIES(integration-ObserversReport PROPERTIES PASS_REGULAR_EXPRESSION "empty observer for tests")
  

###########################################################################

  
IF(OFBUILD_CUSTOM_CMAKE_VERSION VERSION_EQUAL "2.8.0" OR OFBUILD_CUSTOM_CMAKE_VERSION VERSION_GREATER "2.8.0") 
  OPENFLUID_ADD_TEST(NAME integration-SimulatorsReportEnvVar 
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                              "report" 
                              "simulators")
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar 
                       PROPERTIES ENVIRONMENT "OPENFLUID_SIMS_PATH=${OFBUILD_TESTS_BINARY_DIR};OPENFLUID_TEMP_PATH=${OFBUILD_TESTS_TEMP_DIR};OPENFLUID_USERDATA_PATH=${OFBUILD_TESTS_USERDATA_DIR}")                           
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
  SET_TESTS_PROPERTIES(integration-SimulatorsReportEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")    
ENDIF()



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsReportNoEnvVar 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" 
                           "simulators")                           
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
SET_TESTS_PROPERTIES(integration-SimulatorsReportNoEnvVar PROPERTIES WILL_FAIL TRUE)        


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-SimulatorsMatchReport 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report" 
                           "simulators" "tests.primitives.*"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-SimulatorsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.use")  
SET_TESTS_PROPERTIES(integration-SimulatorsMatchReport PROPERTIES PASS_REGULAR_EXPRESSION "tests.primitives.variables.prod")
                           

###########################################################################

OPENFLUID_ADD_TEST(NAME integration-SimulatorsXMLReport 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "report"
                           "-x"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_TESTS_PROPERTIES(integration-SimulatorsXMLReport PROPERTIES WILL_FAIL TRUE)        
                              
  
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputDoesNotExist 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${CMAKE_BINARY_DIR}/path/does/not/exist")                           
SET_TESTS_PROPERTIES(integration-InputDoesNotExist PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputWithNoModel
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.NoModel"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoModel"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoModel"
                   )               
SET_TESTS_PROPERTIES(integration-InputWithNoModel PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-InputWithNoRun
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.NoRun"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NoRun"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")                           
SET_TESTS_PROPERTIES(integration-InputWithNoRun PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingSimulator
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingSim"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingSim"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingSimulator PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingUnitsClass
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingClass"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingClass"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingUnitsClass PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingVariable
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingVar"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingVar"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingVariable PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-MissingAttributes
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.MissingAttributes"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MissingAttributes"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-MissingAttributes PROPERTIES WILL_FAIL TRUE)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-DuplicateVariable
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.DuplicateVar"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.DuplicateVar"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")               
SET_TESTS_PROPERTIES(integration-DuplicateVariable PROPERTIES WILL_FAIL TRUE)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Verbose
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Verbose"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-v")
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Quiet
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Quiet"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-q")


###########################################################################


FILE(COPY "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.PRJ.Primitives" 
       DESTINATION "${OFBUILD_TESTS_OUTPUT_DATA_DIR}"
       PATTERN ".svn" EXCLUDE)

OPENFLUID_ADD_TEST(NAME integration-Project
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"  
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.PRJ.Primitives"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-c")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Profiling
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.Coupling"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.Profiling"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                           "-k")


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterSingle
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterSingle"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterRunSingle"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_PROPERTY(TEST integration-FluidXWriterSingle APPEND PROPERTY DEPENDS unit-api-FluidXDescriptor_TEST)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-FluidXWriterMany
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                            "run"
                            "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterMany"
                            "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.FluidXWriterRunMany"
                            "-p" "${OFBUILD_TESTS_BINARY_DIR}")
SET_PROPERTY(TEST integration-FluidXWriterMany APPEND PROPERTY DEPENDS unit-api-FluidXDescriptor_TEST)
                                 

###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID")
  
OPENFLUID_ADD_TEST(NAME integration-CMakeModuleCheck
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID" 
                           "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                           "${CMAKE_CURRENT_SOURCE_DIR}/FindOpenFLUID"
                           "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                           "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                           "-DTEST_SEARCH_PATHS1=${CMAKE_BINARY_DIR}/src"
                           "-DTEST_SEARCH_PATHS2=${CMAKE_SOURCE_DIR}/src"
                   PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/FindOpenFLUID")                                                                                          


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake")
  
OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorConfigure
                   COMMAND "${CMAKE_COMMAND}"
                             "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake" 
                             "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                             "${CMAKE_CURRENT_SOURCE_DIR}/tests.sim.cmake"
                             "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                             "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                             "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                             "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                             "-DSIM_SIM2DOC_MODE=off"
                   PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake")                                                                                          


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorBuild
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake" 
                           "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
                           "--target" "install"
                           "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorConfigure)                                                            


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorTest
                   COMMAND "${CMAKE_COMMAND}"
                           "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake" 
                           "${CMAKE_CTEST_COMMAND}" "-V")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorTest APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.simui.cmake"
                              "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleSimulatorParamsUIBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.simui.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleSimulatorParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorParamsUIConfigure)


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obs.cmake"
                              "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleObserverBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverConfigure)                                                            


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverTest
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake" 
                              "${CMAKE_CTEST_COMMAND}" "-V")
SET_PROPERTY(TEST integration-CMakeModuleObserverTest APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)                                                            


###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.obsui.cmake"
                              "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DOBS_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake")                              
                              

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleObserverParamsUIBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obsui.cmake"
                              "--target" "install"
                              "--clean-first")
SET_PROPERTY(TEST integration-CMakeModuleObserverParamsUIBuild APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIConfigure)                                                            



###########################################################################


IF(OFBUILD_ENABLE_APP_BUILDER AND OFBUILD_ENABLE_GUI)

FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextConfigure
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake" 
                              "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tests.bext.cmake"
                              "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                              "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                              "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"                              
                              "-DBEXT_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                    PRE_TEST EMPTY_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")         
                                         
ENDIF()


###########################################################################


IF(OFBUILD_ENABLE_APP_BUILDER AND OFBUILD_ENABLE_GUI)

OPENFLUID_ADD_TEST(NAME integration-CMakeModuleBuilderextBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake" 
                              "${CMAKE_COMMAND}" "--build" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.bext.cmake")
SET_PROPERTY(TEST integration-CMakeModuleBuilderextBuild APPEND PROPERTY DEPENDS integration-CMakeModuleBuilderextConfigure)                                                            

ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-CMakeModuleRun
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "run"
                           "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.CMakeModule"
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModule" 
#                           "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.sim.cmake"
#                           "-n" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.obs.cmake"
                    PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.CMakeModule")
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleSimulatorBuild)
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverBuild)
SET_PROPERTY(TEST integration-CMakeModuleRun APPEND PROPERTY DEPENDS integration-CMakeModuleObserverParamsUIBuild)
                                                             

###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocHelp 
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "buddy" 
                             "--buddy-help" 
                             "sim2doc"
                    )
ENDIF()


###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2Doc
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "buddy" 
                             "sim2doc"
                             "--options=inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc"                              
                    )
ENDIF()  


###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDF
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                                "buddy" 
                                "sim2doc"
                                "-o" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF,tplfile=${CMAKE_SOURCE_DIR}/${OPENFLUID_SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF"           
                    )
ENDIF()


###########################################################################


IF(NOT APPLE)
  OPENFLUID_ADD_TEST(NAME integration-BuddySim2DocPDFHTML
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                             "buddy" 
                             "sim2doc"
                             "-o" "inputcpp=${CMAKE_SOURCE_DIR}/src/tests/simulators/tests.sim2doc/Sim2DocSim.cpp,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF-HTML,tplfile=${CMAKE_SOURCE_DIR}/${OPENFLUID_SHARE_COMMON_INSTALL_PATH}/sim2doc_tpl.tex,pdf=1,html=1"
                     PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/sim2doc-PDF-HTML"           
                    )
ENDIF()


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesHelp 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "buddy"
                           "examples"
                           "--buddy-help" 
                  )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesInstallAll
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy" "examples"
                              "-o" "selection=*,sourcedir=${OFBUILD_SOURCE_EXAMPLES_PROJECTS_DIR},installdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects,force=1"
                   PRE_TEST REMOVE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy"                              
                              )
  

###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyExamplesRunPrimitives 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run" 
                           "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives"                                                   
                    POST_TEST CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives/OUT/openfluid-messages.log"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives/OUT/fullA_unitsA1_var1.csv"
                              CHECK_FILE_EXIST "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/examplesbuddy/projects/Primitives/OUT/fullB_unitsB2_var5.csv"
                   )
SET_PROPERTY(TEST integration-BuddyExamplesRunPrimitives APPEND PROPERTY DEPENDS integration-BuddyExamplesInstallAll)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "buddy"
                           "--buddy-help" "newsim"
                  )



###########################################################################


FILE(MAKE_DIRECTORY "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim")
CONFIGURE_FILE("${OFBUILD_TESTS_INPUT_MISCDATA_DIR}/tests.buddy.newsim/CMakeLists.txt.in" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim/CMakeLists.txt" @ONLY)
    
OPENFLUID_ADD_TEST(NAME integration-BuddyNewSim
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy"
                              "newsim"
                              "-o" "simid=tests.buddy.newsim,cppclass=NewSimulatorUsingBuddy,outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim,authorname=John Doe,authoremail=doe@foo.bar.org"
                             )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimConfigure
                   COMMAND "${CMAKE_COMMAND}"
                            "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim" "${CMAKE_COMMAND}" "-G" "${CMAKE_GENERATOR}"
                            "-DOpenFLUIDHelpers_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                            "-DOpenFLUID_DIR=${OFBUILD_DIST_CMAKE_MODULES_DIR}"
                            "-DOpenFLUID_ROOT_DIR=${OFBUILD_DIST_DIR}"
                            "-DOpenFLUID_EXTRA_SEARCH_PATHS=${CMAKE_BINARY_DIR}/src"
                            "-DSIM_INCLUDE_DIRS=${CMAKE_SOURCE_DIR}/src"
                            "-DSIM_SIM2DOC_MODE=off" 
                            ".")
SET_PROPERTY(TEST integration-BuddyNewSimConfigure APPEND PROPERTY DEPENDS integration-BuddyNewSim)



###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimBuild
                   COMMAND "${CMAKE_COMMAND}"
                              "-E" "chdir" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim" "${CMAKE_BUILD_TOOL}"
                              )
SET_PROPERTY(TEST integration-BuddyNewSimBuild APPEND PROPERTY DEPENDS integration-BuddyNewSimConfigure)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimCheck
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                         "report" "simulators"
                         "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim")
SET_TESTS_PROPERTIES(integration-BuddyNewSimCheck PROPERTIES PASS_REGULAR_EXPRESSION "tests.buddy.newsim"
                                                                PASS_REGULAR_EXPRESSION "John Doe"
                                                                PASS_REGULAR_EXPRESSION "doe@foo.bar.org") 
SET_PROPERTY(TEST integration-BuddyNewSimCheck APPEND PROPERTY DEPENDS integration-BuddyNewSimBuild)                                                                 
                       


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewSimRun
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                                       "run"
                                       "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.BuddyNewSim"
                                       "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.BuddyNewSim" 
                                       "-p" "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/tests.buddy.newsim"
                                       "-v")
SET_PROPERTY(TEST integration-BuddyNewSimRun APPEND PROPERTY DEPENDS integration-BuddyNewSimBuild)


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewDataHelp
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy"
                              "--buddy-help" "newdata"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewData
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                              "buddy" "newdata"
                              "--options=outputdir=${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.IN.NewData"
                              )


###########################################################################


OPENFLUID_ADD_TEST(NAME integration-BuddyNewDataCheck
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}"
                           "run"
                                       "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.IN.NewData"
                                       "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.NewData"
                                       "-p" "${OFBUILD_TESTS_BINARY_DIR}"
                              )
SET_PROPERTY(TEST integration-BuddyNewDataCheck APPEND PROPERTY DEPENDS integration-BuddyNewData)


###########################################################################


IF(OFBUILD_ENABLE_APP_MINIMAL)
  OPENFLUID_ADD_TEST(NAME integration-MinimalRun
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                  "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.PrimitivesHopla"
                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalRun"
                                  "${OFBUILD_TESTS_BINARY_DIR}"
                              )
ENDIF()  

###########################################################################


IF (OFBUILD_ENABLE_APP_MINIMAL)
  OPENFLUID_ADD_TEST(NAME integration-MinimalFailsArgs
                     COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                  "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.VariableTimeProd"
                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalFailsArgs"
                              )
  SET_TESTS_PROPERTIES(integration-MinimalFailsArgs PROPERTIES WILL_FAIL TRUE)
ENDIF()

###########################################################################


IF(OFBUILD_ENABLE_APP_MINIMAL)
  OPENFLUID_ADD_TEST(NAME integration-MinimalFailsInput
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/openfluid-minimal"
                                  "${OFBUILD_TESTS_INPUT_DATASETS_DIR}/OPENFLUID.IN.doesnotexist"
                                  "${OFBUILD_TESTS_OUTPUT_DATA_DIR}/OPENFLUID.OUT.MinimalFailsInput"
                                  "${OFBUILD_TESTS_BINARY_DIR}"
                              )                              
  SET_TESTS_PROPERTIES(integration-MinimalFailsInput PROPERTIES WILL_FAIL TRUE)
ENDIF() 
 
###########################################################################


OPENFLUID_ADD_TEST(NAME integration-Debug 
                   COMMAND "${OFBUILD_DIST_BIN_DIR}/${OPENFLUID_CMD_APP}" 
                           "report" "simulators" "--list"
                           "-p" "${OFBUILD_TESTS_BINARY_DIR}")
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES PASS_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ELSE(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  SET_TESTS_PROPERTIES(integration-Debug PROPERTIES FAIL_REGULAR_EXPRESSION "OpenFLUID debugging mode is enabled")
ENDIF() 
  
  