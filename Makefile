# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmeier <jmeier@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/14 16:30:04 by jmeier            #+#    #+#              #
#    Updated: 2019/10/09 00:21:42 by jmeier           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

FUNC = main \
read_line autocomplete cursor input_util expand\
lexer_parser lex_main lex_check lex_check2 operator t_ast t_ast_util\
command pipe redirect heredoc aggregate error_handler thing\
b_cd b_echo b_exit b_env b_setenv b_unsetenv \
util t_line t_hist path util2 \

VPATH = src:src/builtin:src/input:src/lexerParser:src/exec:src/helper
C_SRC = $(addsuffix, $(FUNC))

O_LOC = obj
O_SRC = $(addsuffix .o, $(addprefix $(O_LOC)/, $(FUNC)))

LIB_LOC = ftprintf
LIB_NAM = libftprintf.a
LIB_SRC = $(addprefix $(LIB_LOC)/, $(LIB_NAM))

H_LOC = -I inc/ -I ftprintf/inc -I ftprintf/libft/inc
HEADERS = inc/*.h

all: $(NAME)

$(NAME): $(LIB_SRC) $(O_SRC)
	@/bin/echo -n "無駄"
	@gcc -Wall -Werror -Wextra $^ -o $(NAME)
	@/bin/echo -n "ァァ！」"
	@echo "\nそして、時は動き出す。"

$(O_LOC):
	@mkdir -p $(O_LOC)

$(O_LOC)/%.o: %.c | $(O_LOC)
	@/bin/echo -n "無駄"
	@gcc -Wall -Werror -Wextra $(H_LOC) -o $@ -c $<
	@/bin/echo -n "無駄"

$(LIB_SRC): force
	@make -C $(LIB_LOC)
	@echo
	@/bin/echo -n "「無駄だ！無駄"

test: $(LIB_SRC) $(O_SRC)
	@/bin/echo -n "無駄"
	@gcc -Wall -Werror -Wextra -g -fsanitize=address $^ -o $(NAME)
	@/bin/echo -n "ァァ！」"
	@echo "\nそして、時は動き出す。"

force:
	@true

clean:
	@echo "ザ・ワールド！"
	@/bin/rm -f $(O_SRC)
	@rm -rf $(O_LOC);

fclean: clean
	@/bin/rm -f $(NAME)
	@make fclean -C $(LIB_LOC)
	@echo "時よ止まれ！"

re: fclean all

.PHONY: all clean fclean re
