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

#include "pushswap.h"

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

int					main(int ac, char *av[])
{
	int		len;
	t_inode	*nodes;
	t_lst	a;
	t_lst	b;
	int		tty;
	int		opt;
	char	*ln;
	char	*filename;
	int		fd;

	if (ac < 2)
		return (EXIT_FAILURE);
	fd = STDIN_FILENO;
	filename = NULL;
	g_optind = 1;
	while ((opt = ft_getopt(ac, av, "f:")) != WUT)
		if (opt == 'f')
			filename = g_optarg;
		else
			exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	if (filename && (fd = open(filename, O_RDONLY, S_IRGRP | S_IRUSR)) < 0)
		exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	if ((tty = isatty(fd)))
	{
		if (rl_histload(".pushswapst") < 0)
		{
			ft_fputs(g_stderr, "The history file `.pushswapst' is corrupted\n");
			exit(1);
		}
		rl_complete(complete);
	}
	if (!(len = ac - g_optind))
		exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	nodes = ft_calloc(len * sizeof(t_inode));
	ft_lstctor(&a);
	ft_lstctor(&b);
	makea(av + g_optind, nodes, &a);
	while (!rl_getline(fd, "checker \033[36m❯\033[0m ", &ln))
	{
		if (!ft_strcmp("sa\n", ln))
			ps_operate(&a, &b, OP_S, LST_A);
		else if (!ft_strcmp("sb\n", ln))
			ps_operate(&a, &b, OP_S, LST_B);
		else if (!ft_strcmp("ss\n", ln))
			ps_operate(&a, &b, OP_S, LST_A | LST_B);
		else if (!ft_strcmp("pa\n", ln))
			ps_operate(&a, &b, OP_P, LST_A);
		else if (!ft_strcmp("pb\n", ln))
			ps_operate(&a, &b, OP_P, LST_B);
		else if (!ft_strcmp("ra\n", ln))
			ps_operate(&a, &b, OP_R, LST_A);
		else if (!ft_strcmp("rb\n", ln))
			ps_operate(&a, &b, OP_R, LST_B);
		else if (!ft_strcmp("rr\n", ln))
			ps_operate(&a, &b, OP_R, LST_A | LST_B);
		else if (!ft_strcmp("rra\n", ln))
			ps_operate(&a, &b, OP_RR, LST_A);
		else if (!ft_strcmp("rrb\n", ln))
			ps_operate(&a, &b, OP_RR, LST_B);
		else if (!ft_strcmp("rrr\n", ln))
			ps_operate(&a, &b, OP_RR, LST_A | LST_B);
		else if (!ft_strcmp("dumpa\n", ln))
			ps_dump(g_stdout, &a);
		else if (!ft_strcmp("dumpb\n", ln))
			ps_dump(g_stdout, &b);
		else if (tty)
			ft_dprintf(STDERR_FILENO, "operation not found\n");
		else if (*ln == '\n')
			break ;
		else
		{
			ft_dprintf(STDERR_FILENO, "%s", ln);
			exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
		}
		ft_fflush(g_stdout);
	}
	rl_finalize(fd);
	if (tty)
		rl_histsave(".pushswapst");
	if (b.len || !ps_issort(&a))
		ft_fputs(g_stdout, "KO\n");
	else
		ft_fputs(g_stdout, "OK\n");
	if (fd != STDIN_FILENO)
		close(fd);
	rl_dtor();
	free(nodes);
	return (EXIT_SUCCESS);
}
