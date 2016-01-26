#-------------------------------------------------
#
# base DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = base

include("$$_PRO_FILE_PWD_/../../boxes_common.pri")

# What we are building
TEMPLATE = lib
DESTDIR = "$$_PRO_FILE_PWD_/../../../bin"
TARGET = base$${SUFFIX}

# Location of library when installed on Unix
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Our project header and source code
HEADERS +=  \
    assign.h \
    box.h \
    box_builder.h \
    box_output.h \
    convert.h \
    exception.h \
    factory_plug_in.h \
    general.h \
    initialize.h \
    mega_factory.h \
    object_pool.h \
    path.h \
    port.h \
    port_transform.h \
    port_type.h \
    product_base.h \
    product.h \
    random_generator.h \
    vector.h \
    vector_vector.h \
    vectorize.h \
    box_step.h

SOURCES +=  \
    assign.cpp \
    box.cpp \
    box_builder.cpp \
    box_output.cpp \
    convert.cpp \
    exception.cpp \
    general.cpp \
    initialize.cpp \
    mega_factory.cpp \
    object_pool.cpp \
    path.cpp \
    port.cpp \
    port_transform.cpp \
    port_type.cpp \
    product_base.cpp \
    random_generator.cpp \
    vector.cpp \
    vector_vector.cpp \
    vectorize.cpp


