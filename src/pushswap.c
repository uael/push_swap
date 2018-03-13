/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pushswap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:16:21 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:16:22 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pushswap.h"

#define PS_VERBOSE (1 << 0)
#define PS_COLOR (1 << 1)

void	ps_lstdump(t_lst *lst)
{
	t_inode *node;

	node = (t_inode *)lst->next;
	while (node != (t_inode *)lst)
	{
		ft_printf("%d\n", node->val);
		node = node->next;
	}
}

int	main(int ac, char *av[])
{
	int		o;
	int		i;
	uint8_t	opts;
	t_inode	*nodes;
	t_lst	a;
	t_lst	b;

	opts = 0;
	g_optind = 1;
	while ((o = ft_getopt(ac, av, "vc")) != WUT)
		if (o == 'v')
			opts |= PS_VERBOSE;
		else if (o == 'c')
			opts |= PS_COLOR;
		else
			return (EXIT_FAILURE);
	if (!(o = ac - g_optind))
		return (EXIT_FAILURE);
	nodes = ft_calloc(o * sizeof(t_inode));
	ft_lstctor(&a);
	ft_lstctor(&b);
	i = 0;
	while (g_optind < ac)
	{
		nodes[i].val = (int)ft_atoi(av[g_optind]);
		ft_lstpush(&a, (t_node *)(nodes + i));
		++i;
		++g_optind;
	}
	ps_lstdump(&a);
	free(nodes);
	return (EXIT_SUCCESS);
}
