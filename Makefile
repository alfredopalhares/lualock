PREFIX     ?= /usr/local
INSTALLDIR := $(DESTDIR)$(PREFIX)

CC	:= gcc

PKGS := clutter-gtk-1.0 lua
INCS := $(shell pkg-config --cflags $(PKGS))
LIBS := $(shell pkg-config --libs $(PKGS)) -lpam

CFLAGS	:= -g -Wall -Wextra -std=gnu99 -I. $(INCS) $(CFLAGS)
LDFLAGS	:= $(LIBS) $(LDFLAGS)

SRCS := $(wildcard *.c clib/*.c)
OBJS := $(foreach obj,$(SRCS:.c=.o),$(obj))


all: options lualock

options:
	@echo lualock build options:
	@echo "CC               = $(CC)"
	@echo "CFLAGS           = $(CFLAGS)"
	@echo "LDFLAGS          = $(LDFLAGS)"
	@echo "INSTALLDIR       = $(INSTALLDIR)"
	@echo
	@echo "build targets:"
	@echo "SRCS = $(SRCS)"
	@echo

.c.o:
	@echo $(CC) -c $< -o $@
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

lualock: $(OBJS)
	@echo $(CC) -o $@ $(OBJS)
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf lualock $(OBJS)
