/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:16:00 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:16:02 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pushswap.h"

void	ps_dump(t_stream *s, t_lst *lst)
{
	t_inode *node;

	node = (t_inode *)lst->head;
	while (node != (t_inode *)lst)
	{
		ft_fprintf(s, "%d", node->val);
		node = node->next;
		if (node != (t_inode *)lst)
			ft_fprintf(s, " ");
	}
}

int		ps_issort(t_lst *lst)
{
	t_inode *node;
	t_inode *next;

	node = (t_inode *)lst->head;
	while (node != (t_inode *)lst->tail)
	{
		next = node->next;
		if (node->val > next->val)
			return (0);
		node = next;
	}
	return (1);
}

int		ps_isnsort(t_lst *lst, int n)
{
	t_inode *node;
	t_inode *next;

	node = (t_inode *)lst->head;
	while (n-- && node != (t_inode *)lst->tail)
	{
		next = node->next;
		if (node->val > next->val)
			return (0);
		node = next;
	}
	return (1);
}

int		ps_min(t_lst *lst, int n)
{
	t_inode	*node;
	t_inode	*next;
	int		min;

	min = INT_MAX;
	node = (t_inode *)lst->head;
	while (n-- && node != (t_inode *)lst->tail)
	{
		next = node->next;
		if (node->val < min)
			min = node->val;
		node = next;
	}
	return (min);
}
