/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_faces.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/25 17:32:33 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/01 17:06:01 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static int			valid_face(t_v3i *v, int max)
{
	int		p;

	p = 3;
	while (p--)
	{
		if ((((int *)v)[p] < 0) || (((int *)v)[p] > max))
			return (0);
	}
	return (1);
}

/*
static int			load_faces_while(char *line, t_list **plist, const int max)
{
	t_v3i	point;
	int		faces;

	faces = 0;
	if (ft_sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
}
*/

static int			load_faces_while(char *line, t_list **plist, const int max)
{
	char		**tab;
	t_v3i		point;
	size_t		size;
	int			faces;

	faces = 0;
	tab = ft_strsplit(line, ' ');
	size = ft_tabcount((void**)tab);
	if (size >= 4)
	{
		point = (t_v3i){ft_atoi(tab[1]) - 1, ft_atoi(tab[2]) - 1,
			ft_atoi(tab[3]) - 1};
		if (valid_face(&point, max))
		{
			ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
			faces = 1;
			if (size > 5)
			{
				point = (t_v3i){point.y, point.z, ft_atoi(tab[4]) - 1};
				if (valid_face(&point, max))
				{
					ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
					faces = 2;
				}
			}
		}
	}
	ft_freesplit(tab);
	return (faces);
}

static t_v3i		*load_faces_totab(t_list *lst, const size_t size)
{
	t_v3i				*allpoints;
	size_t				p;

	ft_printf("faces convert : %d\n", (int)size);
	if (!(allpoints = malloc(sizeof(t_v3i) * size)))
		return (NULL);
	p = 0;
	while (lst)
	{
		allpoints[p++] = *(t_v3i*)lst->content;
		lst = lst->next;
	}
	return (allpoints);
}

t_v3i				*load_faces(t_list *faces, const int max,
	size_t *faces_count)
{
	t_v3i	*allpoints;
	t_list	*plist;

	plist = NULL;
	*faces_count = 0;
	while (faces)
	{
		*faces_count += (size_t)load_faces_while(faces->content, &plist, max);
		faces = faces->next;
	}
	allpoints = load_faces_totab(plist, *faces_count);
	//ft_printf("faces list: %lk\n", &faces_debug, plist);
	ft_lstdel(&plist, &ft_lstpulverisator);
	return (allpoints);
}
