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

static int	getmin(t_lst *a)
{
	t_inode	*min_node;
	t_inode	*node;
	int		index;
	int		min;

	min_node = (t_inode *)a->head;
	node = min_node->next;
	min = 0;
	index = 1;
	while (node != (t_inode *)a)
	{
		if (node->val < min_node->val)
		{
			min_node = node;
			min = index;
		}
		node = node->next;
		index++;
	}
	return (min);
}

static int	getmax(t_lst *a)
{
	t_inode	*max_node;
	t_inode	*node;
	int		index;
	int		max;

	max_node = (t_inode *)a->head;
	node = max_node->next;
	max = 0;
	index = 1;
	while (node != (t_inode *)a)
	{
		if (node->val > max_node->val)
		{
			max_node = node;
			max = index;
		}
		node = node->next;
		index++;
	}
	return (max);
}

static void		ps_moveto(t_lst *lst, int idx, uint8_t af)
{
	if (idx == 1)
		ps_operate(lst, lst, OP_S, (uint8_t)(af | LST_P));
	else if (idx > 1)
	{
		if (idx <= (int)(lst->len / 2))
		{
			while (idx--)
				ps_operate(lst, lst, OP_R, (uint8_t)(af | LST_P));
		}
		else
		{
			while (lst->len - idx)
			{
				ps_operate(lst, lst, OP_RR, (uint8_t)(af | LST_P));
				++idx;
			}
		}
	}
}

static void		dsort(t_lst *lst, uint8_t flag)
{
	if (ps_issort(lst))
		return ;
	ps_operate(lst, lst, OP_S, (uint8_t)(flag | LST_P));
}

static void		tinysort(t_lst *lst, uint8_t flag)
{
	t_inode	*head;
	int     first;
	int     second;
	int     third;

	if (ps_issort(lst))
		return ;
	head = (t_inode *)lst->head;
	first = head->val;
	second = head->next->val;
	third = head->next->next->val;
	if (first > second && second < third && first < third)
		ps_operate(lst, lst, OP_S, (uint8_t)(flag | LST_P));
	else if (first < second && third < first)
		ps_operate(lst, lst, OP_RR, (uint8_t)(flag | LST_P));
	else if (first > second && second < third)
		ps_operate(lst, lst, OP_R, (uint8_t)(flag | LST_P));
	else if (first > second && second > third)
		ps_operate(lst, lst, OP_S, (uint8_t)(flag | LST_P));
	else if (first > second && second < third)
		ps_operate(lst, lst, OP_RR, (uint8_t)(flag | LST_P));
	else if (first < second && second > third && first < third)
		ps_operate(lst, lst, OP_RR, (uint8_t)(flag | LST_P));
	tinysort(lst, flag);
}

void			selectionsort(t_lst *a, t_lst *b)
{
	int min;
	int i;

	i = 0;
	while (a->len > 3)
	{
		min = getmin(a);
		ps_moveto(a, min, LST_A);
		if (ps_issort(a))
			break ;
		ps_operate(a, b, OP_P, LST_B | LST_P);
		++i;
	}
	if (a->len == 3)
		tinysort(a, LST_A);
	else
		dsort(a, LST_A);
	while (i--)
		ps_operate(a, b, OP_P, LST_A | LST_P);
}

void			selectionsortb(t_lst *a, t_lst *b)
{
	int max;

	while (b->len)
	{
		max = getmax(b);
		if (max == 0)
			ps_operate(a, b, OP_P, LST_A | LST_P);
		else
		{
			ps_moveto(b, max, LST_B);
			if (ps_issort(b))
				break ;
			ps_operate(a, b, OP_P, LST_A | LST_P);
		}
	}
}

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
	if (low || (int)(a->len + b->len - 1) != high)
		while (--r >= 0)
			ps_operate(a, b, OP_RR, (uint8_t)(LST_A | LST_P));

	if (a->len <= 45)
		selectionsort(a, b);
	if (b->len <= 45)
		selectionsortb(a, b);

	// Push low values to A
	while (b->len > 0)
		ps_operate(a, b, OP_P, LST_A | LST_P);
	return (pi);
}

static void quicksort(t_lst *a, t_lst *b, int low, int high)
{
	int		pi;
	int		n;

	if ((n = high - low) <= 0 || ps_isnsort(a, n))
		return ;
	if (n == 1)
		ps_operate(a, b, OP_S, LST_A | LST_P);
	else if (n == 2)
	{
		t_inode	*head;
		int     first;
		int     second;
		int     third;

		while (!ps_isnsort(a, n))
		{
			head = (t_inode *)a->head;
			first = head->val;
			second = head->next->val;
			if (first > second)
				ps_operate(a, b, OP_S, LST_A | LST_P);
			head = (t_inode *)a->head;
			second = head->next->val;
			third = head->next->next->val;
			if (second > third)
			{
				ps_operate(a, b, OP_R, LST_A | LST_P);
				ps_operate(a, b, OP_S, LST_A | LST_P);
				ps_operate(a, b, OP_RR, LST_A | LST_P);
			}
		}
	}
	else
	{
		pi = partition(a, b, low, high);
		quicksort(a, b, low, pi);
		while (a->len && ((t_inode *)a->head)->val <= pi)
			ps_operate(a, b, OP_R, LST_A | LST_P);
		quicksort(a, b, pi + 1, high);
	}
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

	/*ft_fwrite(g_stdout, "a[", sizeof(char), 2);
	ps_dump(g_stdout, &a);
	ft_fwrite(g_stdout, "] b[", sizeof(char), 4);
	ps_dump(g_stdout, &b);
	ft_fwrite(g_stdout, "]\n", sizeof(char), 2);
	ft_fflush(g_stdout);*/

	if (a.len < 3)
		dsort(&a, LST_A);
	else if (a.len == 3)
		tinysort(&a, LST_A);
	else if (a.len <= 45)
		selectionsort(&a, &b);
	else
		quicksort(&a, &b, 0, (int)a.len - 1);
	while (((t_inode *)a.head)->val)
		ps_operate(&a, &b, OP_R, LST_A | LST_P);
	o = ps_issort(&a) ? 0 : 1;

	/*ft_fwrite(g_stdout, "a[", sizeof(char), 2);
	ps_dump(g_stdout, &a);
	ft_fwrite(g_stdout, "] b[", sizeof(char), 4);
	ps_dump(g_stdout, &b);
	ft_fwrite(g_stdout, "]\n", sizeof(char), 2);
	ft_fflush(g_stdout);*/

	free(nodes);
	free(arr);
	return (o);
}

