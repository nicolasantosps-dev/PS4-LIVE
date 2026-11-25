# PS4 LIVE — Makefile inicial baseado no fluxo de samples OpenOrbis.
TITLE := PS4 LIVE
VERSION := 0.1.0
TITLE_ID := PLIV00001
CONTENT_ID := IV0000-PLIV00001_00-PS4LIVEAPP000001

TOOLCHAIN := $(OO_PS4_TOOLCHAIN)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  BIN := $(TOOLCHAIN)/bin/linux
  CC := clang
  CXX := clang++
  LD := ld.lld
else ifeq ($(UNAME_S),Darwin)
  BIN := $(TOOLCHAIN)/bin/macos
  CC := /usr/local/opt/llvm/bin/clang
  CXX := /usr/local/opt/llvm/bin/clang++
  LD := /usr/local/opt/llvm/bin/ld.lld
else
  $(error Sistema não suportado pelo Makefile inicial: $(UNAME_S))
endif

BUILD := build
OBJ := $(BUILD)/main.o
LIBS := -lc -lkernel -lc++ -lSceUserService -lSceVideoOut -lSceAudioOut -lScePad -lSceSysmodule -lSceFreeType -lSDL2
CFLAGS := --target=x86_64-pc-freebsd12-elf -fPIC -funwind-tables -c -isysroot $(TOOLCHAIN) -isystem $(TOOLCHAIN)/include
CXXFLAGS := $(CFLAGS) -isystem $(TOOLCHAIN)/include/c++/v1
LDFLAGS := -m elf_x86_64 -pie --script $(TOOLCHAIN)/link.x --eh-frame-hdr -L$(TOOLCHAIN)/lib $(LIBS) $(TOOLCHAIN)/lib/crt1.o

.PHONY: all clean check-env
all: check-env $(CONTENT_ID).pkg

check-env:
	@test -n "$(OO_PS4_TOOLCHAIN)" || (echo "Defina OO_PS4_TOOLCHAIN"; exit 1)
	@test -x "$(BIN)/create-fself" || (echo "create-fself não encontrado em $(BIN)"; exit 1)
	@test -x "$(BIN)/create-gp4" || (echo "create-gp4 não encontrado em $(BIN)"; exit 1)

$(BUILD):
	mkdir -p $(BUILD)

$(OBJ): src/main.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $<

eboot.bin: $(OBJ)
	$(LD) $(BUILD)/*.o -o $(BUILD)/ps4live.elf $(LDFLAGS)
	$(BIN)/create-fself -in=$(BUILD)/ps4live.elf -out=eboot.bin --eboot "$@" --paid 0x3800000000000011

sce_sys/param.sfo:
	$(BIN)/PkgTool.Core sfo_new $@
	$(BIN)/PkgTool.Core sfo_setentry $@ APP_TYPE --type Integer --maxsize 4 --value 1
	$(BIN)/PkgTool.Core sfo_setentry $@ APP_VER --type Utf8 --maxsize 8 --value '$(VERSION)'
	$(BIN)/PkgTool.Core sfo_setentry $@ CATEGORY --type Utf8 --maxsize 4 --value 'gd'
	$(BIN)/PkgTool.Core sfo_setentry $@ CONTENT_ID --type Utf8 --maxsize 48 --value '$(CONTENT_ID)'
	$(BIN)/PkgTool.Core sfo_setentry $@ TITLE --type Utf8 --maxsize 128 --value '$(TITLE)'
	$(BIN)/PkgTool.Core sfo_setentry $@ TITLE_ID --type Utf8 --maxsize 12 --value '$(TITLE_ID)'
	$(BIN)/PkgTool.Core sfo_setentry $@ VERSION --type Utf8 --maxsize 8 --value '$(VERSION)'

pkg.gp4: eboot.bin sce_sys/param.sfo sce_sys/icon0.png
	$(BIN)/create-gp4 -out $@ --content-id=$(CONTENT_ID) --files "$^"

$(CONTENT_ID).pkg: pkg.gp4
	$(BIN)/PkgTool.Core pkg_build $< .

clean:
	rm -rf $(BUILD) eboot.bin pkg.gp4 $(CONTENT_ID).pkg sce_sys/param.sfo
