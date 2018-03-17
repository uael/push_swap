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

static inline int	swap(t_lst *a, t_lst *b)
{
	t_inode *ahead;
	t_inode *bhead;
	uint8_t flag;

	flag = 0;
	ahead = (t_inode *)a->head;
	bhead = (t_inode *)b->head;
	if (a->len > 1 && ahead->next->val < ahead->val)
		flag |= LST_A;
	if (b->len > 1 && bhead->next->val < bhead->val)
		flag |= LST_B;
	if (flag)
	{
		ps_operate(a, b, OP_S, (uint8_t)(flag | LST_P));
		return (1);
	}
	return (0);
}

static void sort(t_lst *a, t_lst *b)
{
	(void)a;
	(void)b;
}

int	main(int ac, char *av[])
{
	int		o;
	uint8_t	opts;
	t_inode	*nodes;
	int		*arr;
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
	ft_lstctor(&a);
	ft_lstctor(&b);
	nodes = ft_calloc(o * sizeof(t_inode));
	arr = ft_malloc(o * sizeof(int));
	ps_makea(av + g_optind, nodes, &a, arr);
	sort(&a, &b);
	ps_dump(g_stdout, &a);
	free(nodes);
	free(arr);
	return (EXIT_SUCCESS);
}

