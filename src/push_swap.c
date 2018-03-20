/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:16:00 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:16:02 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ps.h"

static void		tinysort(t_ps *ps, uint8_t stack, uint32_t n)
{
	t_psnode	*head;
	int64_t		first;
	int64_t		second;

	if (n != 2)
		return ;
	if (!(head = ps_head(ps, stack)))
		return ;
	first = (int64_t)head->val * ps->orders[stack];
	second = (int64_t)head->next->val * ps->orders[stack];
	if (first > second)
		ps_swap(ps, stack);
}

static void		minirelsort(t_ps *ps, uint8_t stack)
{
	t_psnode	*head;
	int64_t		first;
	int64_t		second;
	int64_t		third;

	if (!(head = ps_head(ps, stack)))
		return ;
	first = (int64_t)head->val * ps->orders[stack];
	second = (int64_t)head->next->val * ps->orders[stack];
	third = (int64_t)head->next->next->val * ps->orders[stack];
	if (first > second && second < third && first < third)
		ps_swap(ps, stack);
	else if (first < second && third < first)
		ps_rrotate(ps, stack);
	else if (first > second && second < third)
		ps_rotate(ps, stack);
	else if (first > second && second > third)
		ps_swap(ps, stack);
	else if (first > second && second < third)
		ps_rrotate(ps, stack);
	else if (first < second && second > third && first < third)
		ps_rrotate(ps, stack);
	else
		return ;
	minirelsort(ps, stack);
}

static void		minisort(t_ps *ps, uint8_t stack, uint32_t n)
{
	t_psnode	*head;
	int64_t		second;
	int64_t		third;

	if (n != 3)
		return (tinysort(ps, stack, n));
	if (ps->stacks[stack].len == 3)
		return (minirelsort(ps, stack));
	tinysort(ps, stack, 2);
	if (!(head = ps_head(ps, stack)))
		return ;
	second = head->next->val * ps->orders[stack];
	third = head->next->next->val * ps->orders[stack];
	if (second > third)
	{
		ps_rotate(ps, stack);
		ps_swap(ps, stack);
		ps_rrotate(ps, stack);
	}
	tinysort(ps, stack, 2);
}

static int		keeprotate(t_ps *ps, uint8_t stack, uint32_t n, int64_t pivot)
{
	t_psnode *node;

	if (!(node = ps_head(ps, stack)))
		return (0);
	while (n--)
	{
		if (((int64_t)node->val * ps->orders[stack]) < pivot)
			return (1);
		node = node->next;
	}
	return (0);
}

static int64_t	getpivot(t_ps *ps, uint8_t stack, uint32_t n)
{
	t_psnode	*node;
	int64_t		min;
	int64_t		max;
	int64_t		pivot;

	if (!(node = ps_head(ps, stack)))
		return (0);
	max = 0;
	min = UINT32_MAX;
	while (n-- && node != (t_psnode *)(ps->stacks + stack))
	{
		if (node->val > max)
			max = node->val;
		if (node->val < min)
			min = node->val;
		node = node->next;
	}
	pivot = (int64_t)(((max - min) / 2) + min) * ps->orders[stack];
	return (pivot + 1);
}

static void		largesort(t_ps *ps, uint8_t stack, uint32_t n)
{
	uint32_t	push;
	uint32_t	rotate;
	int64_t		pivot;
	uint32_t	i;

	push = 0;
	rotate = 0;
	if (n <= 3)
		return (minisort(ps, stack, n));
	i = 0;
	if (!(pivot = getpivot(ps, stack, n)))
		return ;
	while (keeprotate(ps, stack, n, pivot) && i++ < n)
		if (((int64_t)ps_head(ps, stack)->val * ps->orders[stack]) < pivot)
		{
			ps_pop(ps, stack);
			++push;
		}
		else
		{
			ps_rotate(ps, stack);
			++rotate;
		}
	if (stack)
		largesort(ps, (uint8_t)(stack ^ 1), push);
	if (rotate > ps->stacks[stack].len / 2 && ps->stacks[stack].len > 3)
		while (rotate++ < ps->stacks[stack].len)
			ps_rotate(ps, stack);
	else if (ps->stacks[stack].len > 3)
		while (rotate--)
			ps_rrotate(ps, stack);
	largesort(ps, stack, n - push);
	if (!stack)
		largesort(ps, (uint8_t)(stack ^ 1), push);
	while (push--)
		ps_pop(ps, (uint8_t)(stack ^ 1));
}

int				main(int ac, char *av[])
{
	t_ps ps;

	ft_bzero(&ps, sizeof(t_ps));
	ps.input = STDIN_FILENO;
	ps.output = STDOUT_FILENO;
	ps.options |= OPT_OPTI;
	ps_init(&ps, ac, av);
	if (ps.options & OPT_VERB)
		ps_dump(&ps, STACK_A);
	largesort(&ps, STACK_A, (uint32_t)ps.stacks[STACK_A].len);
	if (ps.options & OPT_VERB)
		ps_dump(&ps, STACK_A);
	ps_exit(&ps, EXIT_SUCCESS);
}
