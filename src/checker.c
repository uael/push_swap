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

#define ERR_MSG "Error\n"

inline static int	atoio(char *str)
{
	int		neg;
	long	ret;

	neg = 0;
	ret = 0;
	if (!ft_isdigit(*str) && *str != '-' && *str != '+')
		exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	if (*str == '-')
		neg = ++str > 0;
	else if (*str == '+')
		++str;
	while (*str)
		if (!ft_isdigit(*str) ||
			(ret = ret * 10 + *str++ - '0') > (neg ? 1L + INT_MAX : INT_MAX))
			exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	return ((int)ret * (neg ? -1 : 1));
}

inline static void	bitset(uint32_t *words, uint32_t n)
{
	words[n / (sizeof(uint32_t) * 8)] |= (1 << n % (sizeof(uint32_t) * 8));
}

inline static int	bitget(uint32_t const *words, uint32_t n)
{
	uint32_t bit;

	bit = words[n / (sizeof(uint32_t) * 8)] & (1 << n % (sizeof(uint32_t) * 8));
	return (bit != 0);
}

static void			makea(char *av[], t_inode *node, t_lst *a)
{
	static uint32_t	set[UINT32_MAX / 32] = { 0 };

	while (*av)
	{
		node->val = atoio(*av++);
		if (!bitget(set, (uint32_t)node->val))
			bitset(set, (uint32_t)node->val);
		else
			exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
		ft_lstpush(a, (t_node *)(node++));
	}
}

static void			complete(t_sds *cmd)
{
	(void)cmd;
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
		rl_complete(complete);
	}
	ck->nodes = ft_calloc(len * sizeof(t_inode));
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
	makea(av + g_optind, ck.nodes, &ck.a);
	while (!rl_getline(ck.fd, "checker \033[36m❯\033[0m ", &op))
	{
		if (!ft_strcmp("sa\n", op))
			ps_operate(&ck.a, &ck.b, OP_S, LST_A);
		else if (!ft_strcmp("sb\n", op))
			ps_operate(&ck.a, &ck.b, OP_S, LST_B);
		else if (!ft_strcmp("ss\n", op))
			ps_operate(&ck.a, &ck.b, OP_S, LST_A | LST_B);
		else if (!ft_strcmp("pa\n", op))
			ps_operate(&ck.a, &ck.b, OP_P, LST_A);
		else if (!ft_strcmp("pb\n", op))
			ps_operate(&ck.a, &ck.b, OP_P, LST_B);
		else if (!ft_strcmp("ra\n", op))
			ps_operate(&ck.a, &ck.b, OP_R, LST_A);
		else if (!ft_strcmp("rb\n", op))
			ps_operate(&ck.a, &ck.b, OP_R, LST_B);
		else if (!ft_strcmp("rr\n", op))
			ps_operate(&ck.a, &ck.b, OP_R, LST_A | LST_B);
		else if (!ft_strcmp("rra\n", op))
			ps_operate(&ck.a, &ck.b, OP_RR, LST_A);
		else if (!ft_strcmp("rrb\n", op))
			ps_operate(&ck.a, &ck.b, OP_RR, LST_B);
		else if (!ft_strcmp("rrr\n", op))
			ps_operate(&ck.a, &ck.b, OP_RR, LST_A | LST_B);
		else if (!ft_strcmp("dumpa\n", op))
			ps_dump(g_stdout, &ck.a);
		else if (!ft_strcmp("dumpb\n", op))
			ps_dump(g_stdout, &ck.b);
		else if (ck.tty)
			ft_dprintf(STDERR_FILENO, "operation not found\n");
		else if (*op == '\n')
			break ;
		else
		{
			ft_dprintf(STDERR_FILENO, "%s", op);
			exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
		}
		ft_fflush(g_stdout);
	}
	ft_fputs(g_stdout, ck.b.len || !ps_issort(&ck.a) ? "KO\n" : "OK\n");
	return (ck_exit(&ck, EXIT_SUCCESS));
}
