SRC_PATH = ../..
OBJ_BASE = Release

PROJECT_NAME = unreal_speccy_portable
SRCFOLDERS = . z80 tools tools/zlib tools/libpng tools/tinyxml snapshot ui platform platform/custom_ui platform/linux platform/sdl devices devices/fdd devices/input devices/sound
CXXSRCS = $(foreach dir, $(SRCFOLDERS), $(wildcard $(SRC_PATH)/$(dir)/*.cpp))
CSRCS = $(foreach dir, $(SRCFOLDERS), $(wildcard $(SRC_PATH)/$(dir)/*.c))

CXX = /opt/rs97-toolchain/bin/mipsel-linux-g++
C = /opt/rs97-toolchain/bin/mipsel-linux-gcc

DIST_PATH = $(OBJ_BASE)
OBJ_PATH = $(OBJ_BASE)
CXX_OBJS = $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(CXXSRCS))
CXX_DEPS = $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.d,$(CXXSRCS))
C_OBJS = $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o,$(CSRCS))
C_DEPS = $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.d,$(CSRCS))
BIN_TARGET = $(DIST_PATH)/$(PROJECT_NAME)

STD_OPTS  = -DGCW_ZERO -G0 -Ofast -mno-abicalls -mplt -mlong-calls -msym32 -Wall -c -fmessage-length=0 -finline-functions -fomit-frame-pointer -mhard-float -fno-builtin -fno-exceptions -fno-pic -DNDEBUG
CXXFLAGS  = $(STD_OPTS) -fno-rtti -fno-threadsafe-statics -D_POSIX -DUSE_SDL -DSDL_KEYS_DINGOO -DSDL_USE_JOYSTICK
CXXFLAGS += -fdata-sections -ffunction-sections -mno-abicalls -fno-pic -fno-pie -msym32 -mlong-calls -fno-exceptions -mno-check-zero-division

CXXFLAGS += -I/opt/rs97-toolchain/usr/mipsel-buildroot-linux-musl/sysroot/usr/include/SDL/
CFLAGS    = $(STD_OPTS)
LFLAGS    = -lSDL -Wl,--as-needed -Wl,--gc-sections -flto -s

all: build

mkdirs:
	mkdir -p $(DIST_PATH)
	mkdir -p $(foreach dir, $(SRCFOLDERS), $(OBJ_PATH)/$(dir))
	
$(CXX_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(C_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(C) $(CFLAGS) -o $@ $<

$(BIN_TARGET): $(CXX_OBJS) $(C_OBJS)
	$(CXX) $(LFLAGS) $^ -o$(BIN_TARGET)

build: mkdirs $(BIN_TARGET)

clean:
	rm -rf $(CXX_OBJS)
	rm -rf $(CXX_DEPS)
	rm -rf $(C_OBJS)
	rm -rf $(C_DEPS)
	rm -rf $(BIN_TARGET)

.PHONY: mkdirs clean build all
