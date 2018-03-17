/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
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

static int			icmp(int const *a, int const *b)
{
	return ((*a > *b) - (*a < *b));
}


void				ps_makea(char *av[], t_inode *node, t_lst *a, int *arr)
{
	static uint32_t	set[UINT32_MAX / 32] = { 0 };
	int				*it;
	t_inode			*n;

	it = arr;
	while (*av)
	{
		*it = atoio(*av++);
		if (!bitget(set, (uint32_t)*it))
			bitset(set, (uint32_t)*it);
		else
			exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
		node->val = *it++;
		ft_lstpush(a, (t_node *)(node++));
	}
	ft_shellsort(arr, it - arr, sizeof(int), (t_ncmp *)icmp);
	while (--it >= arr)
	{
		n = node - 1;
		while (n->val != *it)
			--n;
		n->val = (int)(it - arr);
	}
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
