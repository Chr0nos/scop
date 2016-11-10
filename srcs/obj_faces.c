/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_faces.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/25 17:32:33 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/10 12:33:50 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static int			valid_face(t_v3i *v, int max)
{
	int		p;

	p = 3;
	while (p--)
		if ((((int *)v)[p] < 0) || (((int *)v)[p] > max))
			return (0);
	return (1);
}

/*
** loads a standard face, by standard i mean the format provided by 42
** so: > f 4 5 6 7
** or >  f 4 5 6
** return the number of faces parsed, can be : 0 1 2
*/

static int			load_faces_std(const char *line, t_list **plist,
	const int max)
{
	t_v3i	point;
	int		extra;
	int		ret;

	if ((ret = ft_sscanf(line, "f %d %d %d %d", &point.x, &point.y, &point.z,
		&extra)) == 4)
	{
		point = (t_v3i){point.x - 1, point.y - 1, point.z - 1};
		if (!valid_face(&point, max))
			return (0);
		ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
		point = (t_v3i){point.y, point.z, extra - 1};
		ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
		return (2);
	}
	else if (ret == 3)
	{
		point = (t_v3i){point.x - 1, point.y - 1, point.z - 1};
		ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
		return  (1);
	}
	return (0);
}

static int			load_faces_while(char *line, t_list **plist, const int max)
{
	t_v3i	point;
	int		ret;

	ret = load_faces_std(line, plist, max);
	if (ret > 0)
		return (ret);
	if (((ret = ft_sscanf(line, "f %d/%^d %d/%^d %d/%^d", &point.x, &point.y,
		&point.z)) == 6) ||
		((ret = ft_sscanf(line, "f %d/%^d/%^d %d/%^d/%^d %d/%^d/%^d", &point.x,
		&point.y, &point.z)) == 9))
	{
		point = (t_v3i){point.x - 1, point.y - 1, point.z - 1};
		ft_lstadd(plist, ft_lstnew(&point, sizeof(t_v3i)));
		return (1);
	}
	ft_printf("face error: %d\n", ret);
	return (0);
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
