/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_faces.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/25 17:32:33 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/25 17:35:45 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static void			load_faces_while(char *line, t_list **plist, const int max)
{
	char	**tab;
	t_v3i	point;
	size_t	size;

	tab = ft_strsplit(line, ' ');
	size = ft_tabcount((void**)tab);
	if (size >= 4)
	{
		point = (t_v3i){ft_atoi(tab[1]), ft_atoi(tab[2]), ft_atoi(tab[3])};
		if (((point.x >= 0) && (point.x < max)) &&
				((point.y >= 0) && (point.y < max)) &&
				((point.z >= 0) && (point.z < max)))
			ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
		if (size > 5)
		{
			point = (t_v3i){ft_atoi(tab[2]), ft_atoi(tab[3]),
				ft_atoi(tab[4])};
			ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
		}
	}
	ft_freesplit(tab);
}

t_v3i				*load_faces(t_list *faces, const int max)
{
	t_v3i	*allpoints;
	t_list	*plist;

	plist = NULL;
	while (faces)
	{
		load_faces_while(faces->content, &plist, max);
		faces = faces->next;
	}
	//allpoints = load_faces_tab(plist);
	allpoints = NULL;
	ft_printf("list: %lk\n", &faces_debug, plist);
	ft_lstdel(&plist, &ft_lstpulverisator);
	return (allpoints);
}
