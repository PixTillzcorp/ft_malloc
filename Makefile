ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

.PHONY: clean, fclean, re, open, reset, all, cleared

.SUFFIXES:

# ********************************* DISPLAY ********************************** #

SRC_NBR = $(words $(SRCO))

PRCENT = $(shell echo \($(DONE) \* 100\) \/ $(SRC_NBR) | bc)

REST = $(shell echo \($(DONE) \* 100\) \% $(SRC_NBR) | bc)

PRGRSS = $(shell echo \($(PRCENT) \/ 2\) + 1 | bc)

DONE = 0

BAR = $(shell echo "\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#" | cut -c 1-$(PRGRSS))

EMPTY = $(shell echo "                                                  " | cut -c $(PRGRSS)-50)

# **************************************************************************** #

#~~~~~~~~~~~~~~~~COLORS~~~~~~~~~~~~~~

BLACK = \033[38;5;0m
RED = \033[38;5;196m
GREEN = \033[38;5;46m
YELLOW = \033[38;5;226m
ORANGE = \033[38;5;202m
BLUE = \033[38;5;18m
PINK = \033[38;5;207m
PURPLE = \033[38;5;57m
CYAN = \033[38;5;45m
GREY = \033[38;5;242m
NORMAL = \033[0m

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

NAME = libft_malloc_$(HOSTTYPE).so

LIB_PATH = libft

VPATH = libft:	\
		src:	\
		objfiles:

OBJDIR = objfiles

CC = gcc

SRCC =	free.c malloc.c realloc.c display.c	alloc.c find.c get.c erase_merge.c\
		display_ex.c calloc.c dump_hexa.c display_ex_tools.c

# SRCC =	free.c malloc.c realloc.c split_block.c cat_block.c display.c		\
# 		get_functions.c page.c

SRCO =	$(addprefix $(OBJDIR)/,	$(SRCC:.c=.o))

LIB = libft.a

CFLAGS = -Wall -Wextra -Werror

all: lib $(NAME)

#Display rules~~~~~~~~~~~~

cleared:
	@ echo "$(RED)~ Clean $(NAME) ~$(NORMAL)"

full_clear:
	@ echo "$(RED)~ Full Clean $(NAME) ~$(NORMAL)"

re_init:
	@ echo "$(YELLOW)~ Reset $(NAME) ~$(NORMAL)"

re_done:
	@ echo "$(GREEN)~ Reset Complete $(NAME) ~$(NORMAL)"

#Lib rules~~~~~~~~~~~~~~~~

lib:
	@ make -C $(LIB_PATH)

lib_re:
	@ make -C $(LIB_PATH) re

$(OBJDIR)/%.o: %.c
	@ $(eval DONE = $(shell echo $(DONE) + 1 | bc ))
	@ echo "\r \b$(PURPLE)[$(NORMAL)$(BAR)$(EMPTY)$(PURPLE)] {$(NORMAL)$(PRCENT).$(REST)$(PURPLE)} $(NORMAL)\t\c"
	@ mkdir -p $(OBJDIR)
	@ $(CC) $(CFLAGS) -c $<
	@ mv ./$(notdir $@) ./$(OBJDIR)/

$(NAME): $(LIB_PATH)/$(LIB) $(SRCO)
	@ echo "$(PURPLE)[$(GREEN)\xe2\x9c\x94$(PURPLE)] $(NAME)$(NORMAL)"
	@ $(CC) $(CFLAGS) -shared -o $(NAME) $^
	@ rm -f libft_malloc.so
	@ ln -s $(NAME) libft_malloc.so

clean: cleared
	@ make -C $(LIB_PATH) clean
	@ @ rm -f libft_malloc.so
	@ rm -f $(SRCO)

fclean: full_clear
	@ make -C $(LIB_PATH) fclean
	@ @ rm -f libft_malloc.so
	@ rm -f $(SRCO)
	@ rm -f $(NAME)

re: re_init fclean lib $(NAME) re_done
