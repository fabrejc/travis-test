

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")
INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIRS})


OFBUILD_ADD_OBSERVER(export.vars.files.csv ${OFBUILD_DIST_OBSERVERS_DIR})


OFBUILD_ADD_OBSERVER(export.vars.files.kml-anim ${OFBUILD_DIST_OBSERVERS_DIR})

                      
OFBUILD_ADD_OBSERVER(export.vars.files.kml-plot ${OFBUILD_DIST_OBSERVERS_DIR})


OFBUILD_ADD_OBSERVER(export.vars.files.geovector ${OFBUILD_DIST_OBSERVERS_DIR})

                      
OFBUILD_ADD_OBSERVER(export.vars.plot.gnuplot ${OFBUILD_DIST_OBSERVERS_DIR})

                      
OFBUILD_ADD_OBSERVER(export.spatial-graph.files.dot ${OFBUILD_DIST_OBSERVERS_DIR})

                            