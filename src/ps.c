/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:16:00 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:16:02 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps.h"

static void	tryopen(t_ps *ps, char const *filename, int flags, int *out)
{
	int fd;

	if ((fd = open(filename, flags, 0666)) < 0)
	{
		if (ps->options & OPT_VERB)
			ft_fprintf(g_stderr, "%s: %s: %m\n", ps->program, filename);
		ps_exit(ps, EXIT_FAILURE);
	}
	*out = fd;
}

static void	dupcheck(t_ps *ps, uint32_t n)
{
	static uint32_t	words[UINT32_MAX / 16] = { 0 };
	uint32_t		bit;

	bit = words[n / 32] & (1 << n % 32);
	if (bit)
	{
		if (ps->options & OPT_VERB)
			ft_fprintf(g_stderr, "%s: %d: Duplicate value\n",
				ps->program, n);
		ps_exit(ps, EXIT_FAILURE);
	}
	words[n / 32] |= (1 << n % 32);
}

static void	makestack(t_ps *ps, char *av[], uint32_t n)
{
	t_psnode *node;

	ft_lstctor(ps->stacks + STACK_A);
	ft_lstctor(ps->stacks + STACK_B);
	ps->nodes = ft_calloc(n * sizeof(t_psnode));
	node = ps->nodes;
	while (*av)
	{
		node->val = (uint32_t)ft_atoi(*av++);
		if (errno)
		{
			if (ps->options & OPT_VERB)
				ft_fprintf(g_stderr, "%s: %s: %m\n", ps->program, av - 1);
			ps_exit(ps, EXIT_FAILURE);
		}
		dupcheck(ps, node->val);
		ft_lstpush(ps->stacks + STACK_A, (t_node *)(node++));
	}
}

void		ps_init(t_ps *ps, int ac, char *av[])
{
	int opt;

	g_optind = 1;
	ps->program = av[0];
	while ((opt = ft_getopt(ac, av, "0123456789vOUi:o:")) != WUT)
		if (opt == 'v')
			ps->options |= OPT_VERB;
		else if (opt == 'O')
			ps->options |= OPT_OPTI;
		else if (opt == 'U')
		{
			ps->options &= ~OPT_OPTI;
			ps->options |= OPT_OPLOG;
		}
		else if (opt == 'i')
			tryopen(ps, g_optarg, O_RDONLY, &ps->input);
		else if (opt == 'o')
			tryopen(ps, g_optarg, O_WRONLY, &ps->output);
		else if (ft_isdigit(*(av[g_optind] + 1)))
			break ;
		else
			ps_exit(ps, EXIT_FAILURE);
	if (!(opt = ac - g_optind))
		return ps_exit(ps, EXIT_FAILURE);
	ft_vecctor(&ps->ops, sizeof(uint8_t));
	ft_memcpy(ps->orders, (int[2]){1, -1}, 2 * sizeof(int));
	makestack(ps , av + g_optind, (uint32_t)opt);
	ps_norm(ps, STACK_A);
}

void		ps_exit(t_ps *ps, int ecode)
{
	if (ps->input != STDIN_FILENO)
		close(ps->output);
	if (ps->output != STDOUT_FILENO)
		close(ps->output);
	if (ps->nodes)
		free(ps->nodes);
	if (ps->ops.len)
		ft_vecdtor(&ps->ops, NULL);
	if (ecode)
		ft_fprintf(g_stderr, "Error\n");
	exit(ecode);
}
