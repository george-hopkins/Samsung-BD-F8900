FW_ROOT := ../../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/lowlevel/sdio
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_sdio)
LIB_PATH := $(BSP_LIB)

# These source files can only compile on VC9 and VC10
ifneq ($(filter $(COMPILER),VC9 VC10),)
SRC := \
    libsdio_async.c \
    libsdio_csr.c \
    libsdio.c \
    list.c
endif

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
