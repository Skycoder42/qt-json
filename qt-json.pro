TEMPLATE = subdirs

SUBDIRS += \
    demo \
    src

demo.depends += src
