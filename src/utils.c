/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:16:00 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:16:02 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pushswap.h"

inline int	atoio(char *str)
{
	int		neg;
	long	ret;

	neg = 0;
	ret = 0;
	if (!ft_isdigit(*str) && *str != '-' && *str != '+')
		exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	if (*str == '-')
		neg = ++str > 0;
	else if (*str == '+')
		++str;
	while (*str)
		if (!ft_isdigit(*str) ||
			(ret = ret * 10 + *str++ - '0') > (neg ? 1L + INT_MAX : INT_MAX))
			exit(ft_fprintf(g_stderr, ERR_MSG) - sizeof(ERR_MSG) + 1);
	return ((int)ret * (neg ? -1 : 1));
}
