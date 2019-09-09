ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

.PHONY: clean, fclean, re, open, reset, all, cleared

.SUFFIXES:

#~~~~~~~~~~~~~~~~COLORS~~~~~~~~~~~~~~

FONT_NOIR = \033[40m
BLACK = \033[30m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
PINK = \033[35m
CYAN = \033[36m
GREY = \033[37m
NORMAL = \033[0m

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

NAME = libft_malloc_$(HOSTTYPE).so

LIB_PATH = libft

VPATH = libft:	\
		src:	\
		objfiles:

OBJDIR = objfiles

CC = gcc

SRCC =	free.c malloc.c realloc.c split_block.c cat_block.c display.c		\
		get_functions.c

SRCO =	free.o malloc.o realloc.o split_block.o cat_block.o display.o		\
		get_functions.o

LIB = libft.a

CFLAGS = -Wall -Wextra -Werror

all: lib $(NAME)

#Display rules~~~~~~~~~~~~

cleared:
	@ echo "$(YELLOW)$(FONT_NOIR)$(NAME) : $(RED).o files destruction\t\t[$(GREEN)\xe2\x9c\x94$(RED)]$(NORMAL)"

full_clear:
	@ echo "$(YELLOW)$(FONT_NOIR)$(NAME) : $(RED)executable file destruction\t[$(GREEN)\xe2\x9c\x94$(RED)]$(NORMAL)"

re_init:
	@ echo "$(BLUE)$(FONT_NOIR)Reseting $(NAME) }~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~$(NORMAL)"

re_done:
	@ echo "$(BLUE)$(FONT_NOIR)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{ Reset Complete\t[$(GREEN)\xe2\x9c\x94$(BLUE)]$(NORMAL)"

#Lib rules~~~~~~~~~~~~~~~~

$(LIB):
	@ make -C $(LIB_PATH)

lib_re:
	@ make -C $(LIB_PATH) re

$(OBJDIR)/%.o: %.c
	@ mkdir -p $(OBJDIR)
	@ $(CC) $(CFLAGS) -c $<
	@ mv ./$(notdir $@) ./$(OBJDIR)/

$(NAME): $(LIB) $(OBJDIR)/$(SRCO)
	@ echo "$(PINK)$(FONT_NOIR).o successfully created\t\t\t\t\t\t[$(GREEN)\xe2\x9c\x94$(PINK)]$(NORMAL)"
	@ $(CC) $(CFLAGS) -shared -o $(NAME) $^ $(LIB_SRC)
	@ rm -f libft_malloc.so
	@ ln -s $(NAME) libft_malloc.so
	@ echo "$(PINK)$(FONT_NOIR)Compilation of $(NAME)\t\t\t[$(GREEN)\xe2\x9c\x94$(PINK)]$(NORMAL)"

clean: cleared
	@ make -C $(LIB_PATH) clean
	@ rm -f $(OBJDIR)/$(SRCO)

fclean: full_clear clean
	@ make -C $(LIB_PATH) fclean
	@ rm -f $(NAME)

re: re_init fclean lib_re $(NAME) re_done
