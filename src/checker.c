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

#include "checker.h"

static int8_t		g_comp_idx = 0;
static char			*g_comp_ops[] = {
	"sa", "sb", "ss", "pa", "pb", "ra", "rb", "rr", "rra", "rrb", "rrr",
	"dumpa", "dumpb", "exit", NULL
};

static void			ck_complete(t_sds *cmd)
{
	char	*test;
	char	*candidate;
	int8_t	match;
	int		score;
	int		max;

	if (!cmd->len || !ft_strcmp(g_comp_ops[g_comp_idx], cmd->buf))
		g_comp_idx = (int8_t)(g_comp_idx >= 13 ? 0 : g_comp_idx + 1);
	else
	{
		max = 0;
		match = -1;
		g_comp_idx = -1;
		while (g_comp_ops[++g_comp_idx] && !(score = 0))
		{
			test = g_comp_ops[g_comp_idx];
			candidate = cmd->buf - 1;
			while (*++candidate && *test)
				if (*candidate == *test)
					score += 4 - (test++ - g_comp_ops[g_comp_idx]);
			match < 0 || score > max ? (match = g_comp_idx) : 0;
		}
		g_comp_idx = match;
	}
	(void)((cmd->len = 0) || ft_sdsapd(cmd, g_comp_ops[g_comp_idx]));
}

static int			ck_exit(t_checker *ck, int ecode)
{
	if (ecode)
		ft_fprintf(g_stderr, ERR_MSG);
	rl_finalize(ck->fd);
	if (ck->tty)
		rl_histsave(".pushswapst");
	if (ck->fd != STDIN_FILENO)
		close(ck->fd);
	rl_dtor();
	if (ck->nodes)
		free(ck->nodes);
	exit(ecode);
}

static void			ck_init(t_checker *ck, int ac, char *av[])
{
	int		len;
	int		opt;
	char	*filename;

	g_optind = 1;
	filename = NULL;
	ft_bzero(ck, sizeof(t_checker));
	while ((opt = ft_getopt(ac, av, "0123456789f:")) != WUT)
		if (opt == 'f')
			filename = g_optarg;
		else if (ft_isdigit(*(av[g_optind] + 1)))
			break ;
		else
			ck_exit(ck, EXIT_FAILURE);
	if (filename && (ck->fd = open(filename, O_RDONLY, S_IRGRP | S_IRUSR)) < 0)
		exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	if (!(len = ac - g_optind))
		ck_exit(ck, EXIT_FAILURE);
	if ((ck->tty = isatty(ck->fd)))
	{
		rl_histload(".pushswapst");
		rl_complete(ck_complete);
	}
	ck->nodes = ft_calloc(len * sizeof(t_inode));
}

static int			ck_eval(t_checker *ck, char const *op)
{
	if (!ft_strcmp("sa\n", op))
		ps_operate(&ck->a, &ck->b, OP_S, LST_A);
	else if (!ft_strcmp("sb\n", op))
		ps_operate(&ck->a, &ck->b, OP_S, LST_B);
	else if (!ft_strcmp("ss\n", op))
		ps_operate(&ck->a, &ck->b, OP_S, LST_A | LST_B);
	else if (!ft_strcmp("pa\n", op))
		ps_operate(&ck->a, &ck->b, OP_P, LST_A);
	else if (!ft_strcmp("pb\n", op))
		ps_operate(&ck->a, &ck->b, OP_P, LST_B);
	else if (!ft_strcmp("ra\n", op))
		ps_operate(&ck->a, &ck->b, OP_R, LST_A);
	else if (!ft_strcmp("rb\n", op))
		ps_operate(&ck->a, &ck->b, OP_R, LST_B);
	else if (!ft_strcmp("rr\n", op))
		ps_operate(&ck->a, &ck->b, OP_R, LST_A | LST_B);
	else if (!ft_strcmp("rra\n", op))
		ps_operate(&ck->a, &ck->b, OP_RR, LST_A);
	else if (!ft_strcmp("rrb\n", op))
		ps_operate(&ck->a, &ck->b, OP_RR, LST_B);
	else if (!ft_strcmp("rrr\n", op))
		ps_operate(&ck->a, &ck->b, OP_RR, LST_A | LST_B);
	else
		return (0);
	return (1);
}

int					main(int ac, char *av[])
{
	t_checker	ck;
	char		*op;

	if (ac < 2)
		return (EXIT_FAILURE);
	ck_init(&ck, ac, av);
	ft_lstctor(&ck.a);
	ft_lstctor(&ck.b);
	ps_make(av + g_optind, ck.nodes, &ck.a);
	while (!rl_getline(ck.fd, "checker \033[36m❯\033[0m ", &op))
	{
		if (!*op || *op == '\n' || ck_eval(&ck, op))
			continue ;
		else if (!ft_strcmp("dumpa\n", op))
			ps_dump(g_stdout, &ck.a);
		else if (!ft_strcmp("dumpb\n", op))
			ps_dump(g_stdout, &ck.b);
		else if (ck.tty && ft_strcmp("exit\n", op))
			ft_dprintf(STDERR_FILENO, ERR"operation not found\n");
		else
			ck_exit(&ck, !(ck.tty && !ft_strcmp("exit\n", op)));
		ft_fflush(g_stdout);
	}
	ft_fputs(g_stdout, ck.b.len || !ps_issort(&ck.a) ? "KO\n" : "OK\n");
	return (ck_exit(&ck, EXIT_SUCCESS));
}
