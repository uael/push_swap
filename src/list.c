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

static inline void	bitset(uint32_t *words, uint32_t n)
{
	words[n / (sizeof(uint32_t) * 8)] |= (1 << n % (sizeof(uint32_t) * 8));
}

static inline int	bitget(uint32_t const *words, uint32_t n)
{
	uint32_t bit;

	bit = words[n / (sizeof(uint32_t) * 8)] & (1 << n % (sizeof(uint32_t) * 8));
	return (bit != 0);
}

void				ps_make(char *av[], t_inode *node, t_lst *a)
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

void				ps_dump(t_stream *s, t_lst *lst)
{
	t_inode *node;

	node = (t_inode *)lst->next;
	while (node != (t_inode *)lst)
	{
		ft_fprintf(s, "%d\n", node->val);
		node = node->next;
	}
}

int					ps_issort(t_lst *lst)
{
	t_inode *node;
	t_inode *next;

	node = (t_inode *)lst->next;
	while (node != (t_inode *)lst->prev)
	{
		next = node->next;
		if (node->val > next->val)
			return (0);
		node = next;
	}
	return (1);
}
