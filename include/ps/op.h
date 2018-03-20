/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps/op.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:15:51 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:15:53 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PS_OP_H
# define PS_OP_H

# include <libft.h>

struct s_ps;

typedef enum	e_op
{
	OP_S = 0,
	OP_P = 3,
	OP_R = 5,
	OP_RR = 8
}				t_op;

/*
** Swap the two first nodes of `stack`.
** @param ps    The ps env
** @param stack The id of stack to swap
*/
extern void		ps_swap(struct s_ps *ps, uint8_t stack);

/*
** Pop the first node of `stack` and push it to other.
** @param ps    The ps env
** @param stack The id of stack to pop
*/
extern void		ps_pop(struct s_ps *ps, uint8_t stack);

/*
** Rotate the nodes of `stack`, the first node is becoming the last.
** @param ps    The ps env
** @param stack The id of stack to rotate
*/
extern void		ps_rotate(struct s_ps *ps, uint8_t stack);

/*
** Reverse rotate the nodes of `stack`, the last node is becoming the first.
** @param ps    The ps env
** @param stack The id of stack to rotate
*/
extern void		ps_rrotate(struct s_ps *ps, uint8_t stack);

/*
** Get the string representation of an operation.
** @param op    The operation
 * @return      The the string representation the `op` operation
*/
extern char		*ps_opstr(uint8_t op);

#endif
