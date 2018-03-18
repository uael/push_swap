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

#include <stdio.h>
#include "pushswap.h"

#define PS_VERBOSE (1 << 0)
#define PS_COLOR (1 << 1)

static int	partition(t_lst *a, t_lst *b, int low, int high)
{
	int r;
	int pi;

	pi = (high - low) / 2 + low;
	r = 0;
	while ((int)b->len <= pi - low)
	{
		if (a->len && ((t_inode *)a->head)->val <= pi)
			ps_operate(a, b, OP_P, LST_B | LST_P);
		else
		{
			ps_operate(a, b, OP_R, LST_A | LST_P);
			++r;
		}
	}

	// Reset order
	while (--r >= 0)
		ps_operate(a, b, OP_RR, LST_A | LST_P);

	// Push low values to A
	while (b->len)
		ps_operate(a, b, OP_P, LST_A | LST_P);
	return (pi);
}

static void sort(t_lst *a, t_lst *b, int low, int high)
{
	int pi;
	int n;

	if ((n = high - low) <= 0 || ps_isnsort(a, n))
		return ;
	pi = partition(a, b, low, high);
	sort(a, b, low, pi);
	while (a->len && ((t_inode *)a->head)->val <= pi)
		ps_operate(a, b, OP_R, LST_A | LST_P);
	sort(a, b, pi + 1, high);
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
	sort(&a, &b, 0, (int)a.len - 1);
	while (((t_inode *)a.head)->val)
		ps_operate(&a, &b, OP_R, LST_A | LST_P);
	o = ps_issort(&a) ? 0 : 1;
	free(nodes);
	free(arr);
	return (o);
}

