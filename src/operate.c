/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:16:00 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:16:02 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pushswap.h"

static void	operates(t_lst *a, t_lst *b, uint8_t lst)
{
	if ((lst & LST_A))
	{
		if (a && a->len > 1)
			ft_nodenswp(a->next);
		else
			lst &= ~LST_A;
	}
	if ((lst & LST_B))
	{
		if (b && b->len > 1)
			ft_nodenswp(b->next);
		else
			lst &= ~LST_B;
	}
	if (!(lst & LST_P))
		return ;
	if ((lst & LST_A) && (lst & LST_B))
		ft_fwrite(g_stdout, "ss\n", 1, 3);
	else if ((lst & LST_A))
		ft_fwrite(g_stdout, "sa\n", 1, 3);
	else if ((lst & LST_B))
		ft_fwrite(g_stdout, "sb\n", 1, 3);
}

static void	operatep(t_lst *a, t_lst *b, uint8_t lst)
{
	t_node *node;

	if ((lst & LST_A))
	{
		if (b && b->len)
		{
			node = b->next;
			--b->len;
			ft_nodeulink(node, node->prev, node->next);
			ft_lstusht(a, node);
			(lst & LST_P) ? ft_fwrite(g_stdout, "pa\n", 1, 3) : 0;
		}
	}
	else if ((lst & LST_B))
	{
		if (a && a->len)
		{
			node = a->next;
			--a->len;
			ft_nodeulink(node, node->prev, node->next);
			ft_lstusht(b, node);
			(lst & LST_P) ? ft_fwrite(g_stdout, "pb\n", 1, 3) : 0;
		}
	}
}

static void	operater(t_lst *a, t_lst *b, uint8_t lst)
{
	if ((lst & LST_A))
	{
		if (a && a->len > 1)
			ft_nodenswp((t_node *)a);
		else
			lst &= ~LST_A;
	}
	if ((lst & LST_B))
	{
		if (b && b->len > 1)
			ft_nodenswp((t_node *)b);
		else
			lst &= ~LST_B;
	}
	if (!(lst & LST_P))
		return ;
	if ((lst & LST_A) && (lst & LST_B))
		ft_fwrite(g_stdout, "rr\n", 1, 3);
	else if ((lst & LST_A))
		ft_fwrite(g_stdout, "ra\n", 1, 3);
	else if ((lst & LST_B))
		ft_fwrite(g_stdout, "rb\n", 1, 3);
}

static void	operaterr(t_lst *a, t_lst *b, uint8_t lst)
{
	if ((lst & LST_A))
	{
		if (a && a->len > 1)
			ft_nodenswp(a->prev);
		else
			lst &= ~LST_A;
	}
	if ((lst & LST_B))
	{
		if (b && b->len > 1)
			ft_nodenswp(b->prev);
		else
			lst &= ~LST_B;
	}
	if (!(lst & LST_P))
		return ;
	if ((lst & LST_A) && (lst & LST_B))
		ft_fwrite(g_stdout, "rrr\n", 1, 4);
	else if ((lst & LST_A))
		ft_fwrite(g_stdout, "rra\n", 1, 4);
	else if ((lst & LST_B))
		ft_fwrite(g_stdout, "rrb\n", 1, 4);
}

void		ps_operate(t_lst *a, t_lst *b, uint8_t op, uint8_t lst)
{
	static t_operate *operate[] = {
		[OP_S] = operates,
		[OP_P] = operatep,
		[OP_R] = operater,
		[OP_RR] = operaterr
	};

	return (operate[op](a, b, lst));
}