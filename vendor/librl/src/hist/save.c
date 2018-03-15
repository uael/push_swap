/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist/save.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 09:52:30 by alucas-           #+#    #+#             */
/*   Updated: 2017/12/11 13:31:59 by alucas-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hist.h"

static char		g_buf[FT_PAGE_SIZE];

inline int		rl_histsave(char const *filename)
{
	t_sds		line;
	t_stream	*out;
	char		*beg;
	char		*eol;
	uint8_t		i;

	if (!(out = ft_fopen(filename, O_WRONLY | O_TRUNC | O_CREAT,
		g_buf, FT_PAGE_SIZE)))
		return (ft_throw(WUT, FT_DBG));
	i = 0;
	ft_sdsctor(&line);
	while (rl_histcpy(i++, &line))
	{
		beg = line.buf;
		while ((eol = ft_strchr(beg, '\n')))
		{
			ft_fwrite(out, beg, 1, eol - beg);
			ft_fputc(out, '\n');
			beg = eol + 1;
		}
		ft_fprintf(out, "%s\n\n", beg);
	}
	ft_sdsdtor(&line);
	return (ft_fclose(out));
}
