ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

#===== Build options.
# Override on the make command line, e.g. "make SHOW_ENV=1", or from the
# "make arguments" field of a CLion build configuration. Command line
# variables propagate to the recursive sub-makes automatically.
#
#   SHOW_ENV   0 (default) to leave the environment out, 1 to dump it.
#
SHOW_ENV ?= 0
ifneq ($(SHOW_ENV),0)
CCFLAGS += -DSHOW_ENV
endif

# Make compares file timestamps, so changing an option above would not by
# itself rebuild anything. Record the option values in a stamp file in the
# variant directory and make every object depend on it. The file is rewritten
# only when the values actually change, so a repeated build stays a no-op
# while flipping an option forces a recompile.
#
# To add an option, add it to the list below as well.
BUILD_OPTIONS := SHOW_ENV=$(SHOW_ENV)
OPTIONS_STAMP := .build-options
$(shell [ "$$(cat $(OPTIONS_STAMP) 2>/dev/null)" = '$(BUILD_OPTIONS)' ] || echo '$(BUILD_OPTIONS)' > $(OPTIONS_STAMP))

# EXTRA_CCDEPS is a prerequisite of the %.o:%.c rules in qrules.mk, so it has
# to be set before qtargets.mk pulls those rules in.
EXTRA_CCDEPS += $(OPTIONS_STAMP)
EXTRA_CLEAN  += $(OPTIONS_STAMP)

# qmacros.mk is what defines VARIANT_LIST, so pull it in before testing the
# variant below. qrules.mk guards its own include of qmacros.mk on VARIANT_TAG,
# so including it here does not cause a double include.
include $(MKFILES_ROOT)/qmacros.mk

# The stock rules give the "g" variant debug info (VFLAG_g=-g in qmacros.mk) and
# drop -DNDEBUG, but they leave the optimisation level alone, so a -g build
# would still be built with -O2. Turn optimisation off so the debug variants
# single-step cleanly.
ifeq ($(filter g, $(VARIANT_LIST)),g)
OPTIMIZE_TYPE=NONE
endif

include $(MKFILES_ROOT)/qtargets.mk
