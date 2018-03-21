/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:16:00 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:16:02 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <librl.h>

#include "ps.h"

static int	evalr(t_ps *ps, char *cmd)
{
	if (!ft_strcmp("ra", cmd))
		ps_rotate(ps, STACK_A);
	else if (!ft_strcmp("rb", cmd))
		ps_rotate(ps, STACK_B);
	else if (!ft_strcmp("rr", cmd))
	{
		ps_rotate(ps, STACK_A);
		ps_rotate(ps, STACK_B);
	}
	else if (!ft_strcmp("rra", cmd))
		ps_rrotate(ps, STACK_A);
	else if (!ft_strcmp("rrb", cmd))
		ps_rrotate(ps, STACK_B);
	else if (!ft_strcmp("rrr", cmd))
	{
		ps_rrotate(ps, STACK_A);
		ps_rrotate(ps, STACK_B);
	}
	else
		return (0);
	return (1);
}

static int	eval(t_ps *ps, char *cmd)
{
	size_t	len;

	if ((len = ft_strlen(cmd)) < 2 || len > 4)
		return (0);
	if (cmd[len - 1] == '\n')
		cmd[--len] = '\0';
	if (!ft_strcmp("sa", cmd))
		ps_swap(ps, STACK_A);
	else if (!ft_strcmp("sb", cmd))
		ps_swap(ps, STACK_B);
	else if (!ft_strcmp("ss", cmd))
	{
		ps_swap(ps, STACK_A);
		ps_swap(ps, STACK_B);
	}
	else if (!ft_strcmp("pa", cmd))
		ps_pop(ps, STACK_B);
	else if (!ft_strcmp("pb", cmd))
		ps_pop(ps, STACK_A);
	else
		return (evalr(ps, cmd));
	return (1);
}

static void	savest(void)
{
	rl_dtor();
	rl_histsave(".pushswapst");
}

static void	init(t_ps *ps, int ac, char *av[], int *tty)
{
	ft_bzero(ps, sizeof(t_ps));
	ps->input = STDIN_FILENO;
	ps->output = STDOUT_FILENO;
	ps_init(ps, ac, av);
	ps->options &= ~OPT_STEP;
	if ((*tty = isatty(ps->input)))
	{
		rl_histload(".pushswapst");
		atexit(savest);
	}
}

int			main(int ac, char *av[])
{
	t_ps	ps;
	char	*op;
	int		tty;

	if (ac < 2)
		return (EXIT_SUCCESS);
	init(&ps, ac, av, &tty);
	while (!rl_getline(ps.input, "checker \033[36m❯\033[0m ", &op))
		if (!*op || *op == '\n' || eval(&ps, op))
			continue ;
		else if (!ft_strcmp("dumpa\n", op))
			ps_dump(&ps, STACK_A);
		else if (!ft_strcmp("dumpb\n", op))
			ps_dump(&ps, STACK_B);
		else if (!ft_strcmp("exit\n", op))
			break ;
		else if (tty)
			continue ;
	ft_dprintf(ps.output, ps.stacks[STACK_B].len ||
		!ps_issort(&ps, STACK_A, -1) ? "KO\n" : "OK\n");
	if (ps.options & OPT_VERB)
		ps_dump(&ps, STACK_BOTH);
	ps_exit(&ps, EXIT_SUCCESS);
}
