LIBS += -lGL -lGLEW -lglfw
SOURCES += \
    main.cpp \
    common/shader.cpp \
    common/objloader.cpp \
    common/controls.cpp \
    common/sphere.cpp \
    common/texture.cpp \
    model.cpp

HEADERS += \
    common/shader.hpp \
    common/objloader.hpp \
    common/controls.hpp \
    common/sphere.h \
    common/texture.hpp \
    model.h \
    md2_utils.h

OTHER_FILES += \
    fragmentshader \
    vertexshader

DESTDIR = $$PWD
