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
	char	*ln;

	if (!ac || !(len = ac - 1))
		return (EXIT_FAILURE);
	if ((tty = isatty(STDIN_FILENO)))
	{
		if (rl_histload(".pushswapst") < 0)
		{
			ft_fputs(g_stderr, "The history file `.pushswapst' is corrupted\n");
			exit(1);
		}
		rl_complete(complete);
	}
	nodes = ft_calloc(len * sizeof(t_inode));
	g_optind = 1;
	ft_lstctor(&a);
	ft_lstctor(&b);
	makea(av + g_optind, nodes, &a);
	while (!rl_getline(STDIN_FILENO, "checker \033[32m❯\033[0m ", &ln))
	{
		ft_dprintf(STDOUT_FILENO, ln);
	}
	rl_finalize(STDIN_FILENO);
	if (tty)
		rl_histsave(".pushswapst");
	rl_dtor();
	free(nodes);
	return (EXIT_SUCCESS);
}
