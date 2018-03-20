/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps/sort.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:15:51 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:15:53 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PS_SORT_H
# define PS_SORT_H

# include <libft.h>

struct s_ps;

extern void	ps_sort(struct s_ps *ps, uint8_t stack, uint32_t n);

extern void	ps_quicksort(struct s_ps *ps, uint8_t stack, uint32_t n);

extern void	ps_selectsort(struct s_ps *ps, uint8_t stack);

extern void	ps_minisort(struct s_ps *ps, uint8_t stack, uint32_t n);

#endif
