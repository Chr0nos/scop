/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/25 17:25:00 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/25 17:37:08 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void				vertex_debug(t_printf *pf)
{
	const t_v3f		*vertex = (t_v3f*)pf->raw_value;
	char			buff[100];
	int				len;

	len = ft_snprintf(buff, 100, "x: %f y: %f z: %f",
		(double)vertex->x, (double)vertex->y, (double)vertex->z);
	ft_printf_append(pf, buff, (size_t)len);
}

void				faces_debug(t_printf *pf)
{
	const t_list		*lst = (t_list *)pf->raw_value;
	t_v3i				*faces;
	char				space[4][13];
	int					len[3];
	int					face_id;

	face_id = 0;
	while (lst)
	{
		faces = lst->content;
		len[0] = ft_itobuff(space[0], faces->x, 10, "0123456789");
		len[1] = ft_itobuff(space[1], faces->y, 10, "0123456789");
		len[2] = ft_itobuff(space[2], faces->z, 10, "0123456789");
		ft_printf_append(pf, "[", 1);
		ft_printf_append(pf, space[3], (size_t)ft_itobuff(space[3],
			face_id++, 10, "0123456789"));
		ft_printf_append(pf, "]{", 2);
		ft_printf_append(pf, space[0], (size_t)len[0]);
		ft_printf_append(pf, " ", 1);
		ft_printf_append(pf, space[1], (size_t)len[1]);
		ft_printf_append(pf, " ", 1);
		ft_printf_append(pf, space[2], (size_t)len[2]);
		ft_printf_append(pf, "}", 1);
		lst = lst->next;
		if (lst)
			ft_printf_append(pf, ", ", 2);
	}
}
