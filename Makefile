ifeq ($(OS),Windows_NT)
	MKDIR = if not exist "$(1)" md "$(1)"
	RMDIR := rmdir /s /q
else
	MKDIR = mkdir -p "$(1)"
	RMDIR := rm -rf
endif

TARGET := BOOTx64

INCDIR := include
SRCDIR := src
BUILDDIR := build
TESTDIR := tests
OBJDIR := $(BUILDDIR)/obj
BINDIR := $(BUILDDIR)/bin

SRCS := $(wildcard $(SRCDIR)/*.s)
OBJS := $(patsubst %.s,$(OBJDIR)/%.o,$(notdir $(SRCS)))
TESTHEADERS := $(wildcard $(TESTDIR)/*.h) $(foreach subdir,$(TESTDIR)/*,$(wildcard $(subdir)/*.h))
TESTSRCS := $(wildcard $(TESTDIR)/*.c) $(foreach subdir,$(TESTDIR)/*,$(wildcard $(subdir)/*.c))
TESTOBJS := $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(TESTSRCS))

CC := clang
CL := clang-cl
MINGWFLAGS := -target x86_64-w64-mingw32
CFLAGS := /I$(INCDIR) /D_CRT_SECURE_NO_WARNINGS

LD := lld-link
LDFLAGS := /entry:_efi_entry /subsystem:efi_application

.PHONY: all clean test

all: $(BINDIR)/$(TARGET).efi

$(BINDIR)/$(TARGET).efi: $(OBJS)
	@$(call MKDIR,$(dir $@))
	$(LD) $(LDFLAGS) $(OBJS) /out:$@

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	@$(call MKDIR,$(dir $@))
	$(CC) $(MINGWFLAGS) -c $< -o $@

test: $(BINDIR)/test.exe

$(BINDIR)/test.exe: $(OBJS) $(TESTOBJS)
	@$(call MKDIR,$(dir $@))
	$(LD) $(OBJS) $(TESTOBJS) /out:$@

$(OBJDIR)/%.o: $(TESTDIR)/%.c $(TESTHEADERS)
	@$(call MKDIR,$(dir $@))
	$(CL) $(CFLAGS) -c $< -o $@

clean:
	$(RMDIR) "$(BUILDDIR)"
