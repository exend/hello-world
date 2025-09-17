ifeq ($(OS),Windows_NT)
	EXT :=
	MKDIR := md
	RMDIR := rmdir /s /q
else
	ifeq ($(shell uname -s),Linux)
		EXT := .exe
	else # UNIX
		EXT :=
	endif
	MKDIR := mkdir -p
	RMDIR := rm -rf
endif

CC := clang
CFLAGS := -target x86_64-w64-mingw32

LD := lld-link
LDFLAGS := /entry:_efi_entry /subsystem:efi_application

TARGET := BOOTx64

SRCDIR := src
BUILDDIR := build
TESTDIR := tests
OBJDIR := $(BUILDDIR)/obj
BINDIR := $(BUILDDIR)/bin

SRCS := $(wildcard $(SRCDIR)/*.s)
OBJS := $(patsubst %.s,$(OBJDIR)/%.o,$(notdir $(SRCS)))
TESTHEADERS := $(wildcard $(TESTDIR)/*.h) $(foreach subdir,$(TESTDIR)/*,$(wildcard $(subdir)/*.h))
TESTSRCS := $(wildcard $(TESTDIR)/*.c) $(foreach subdir,$(TESTDIR)/*,$(wildcard $(subdir)/*.c))
TESTOBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(TESTSRCS)))

.PHONY: all clean test

all: $(BINDIR)/$(TARGET).efi

clean:
	$(RMDIR) "$(BUILDDIR)"

$(BINDIR)/$(TARGET).efi: $(OBJS) | $(BINDIR)
	$(LD)$(EXT) $(LDFLAGS) $(OBJS) /out:$@

$(OBJDIR)/%.o: $(SRCDIR)/%.s | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	$(MKDIR) "$(OBJDIR)"
	@echo $(filter %.c,$(TESTSRCS))

$(BINDIR):
	$(MKDIR) "$(BINDIR)"

test: $(BINDIR)/test.exe

$(BINDIR)/test.exe: $(OBJS) $(TESTOBJS) | $(BINDIR)
	$(LD)$(EXT) $(OBJS) $(TESTOBJS) /out:$@

$(OBJDIR)/%.o: $(TESTDIR)/%.c $(TESTHEADERS) | $(OBJDIR)
	clang-cl /D_CRT_SECURE_NO_WARNINGS -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/*/%.c $(TESTHEADERS) | $(OBJDIR)
	clang-cl /D_CRT_SECURE_NO_WARNINGS -c $< -o $@
