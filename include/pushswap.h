/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pushswap.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:15:51 by alucas-           #+#    #+#             */
/*   Updated: 2018/03/12 18:15:53 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSHSWAP_H
# define PUSHSWAP_H

# include <libft.h>

# define LST_A (1 << 0)
# define LST_B (1 << 1)
# define LST_P (1 << 2)

# define OP_S 0
# define OP_P 1
# define OP_R 2
# define OP_RR 3

# define ERR_MSG "Error\n"
# define ERR COLOR_RED"error: "COLOR_RESET

struct s_inode;

typedef struct		s_inode
{
	struct s_inode	*prev;
	struct s_inode	*next;
	int				val;
}					t_inode;

typedef void		(t_operate)(t_lst *a, t_lst *b, uint8_t lst);

extern int			atoio(char *str);

extern void			ps_operate(t_lst *a, t_lst *b, uint8_t op, uint8_t lst);
extern void			ps_make(char *av[], t_inode *node, t_lst *a);
extern void			ps_makea(char *av[], t_inode *node, t_lst *a, int *arr);
extern void			ps_dump(t_stream *s, t_lst *lst);
extern int			ps_issort(t_lst *lst);

#endif
