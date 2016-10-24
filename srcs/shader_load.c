/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_load.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 17:40:39 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/22 18:24:47 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ocl.h"
#include <fcntl.h>

int		shaders_load(const char fragment, const char *vertex)
{
	char	shader[4096];
	int		len;
	int		fd;

	if ((fd = open(fragment, O_RDONLY)) < 0)
		return (-1);
	len = (int)read(fd, shader, 4096);
	close(fd);

	if ((fd = open(vertex, O_RDONLY)) < 0)
		return (-1);
	len = (int)read(fd, shader, 4096);
	close(fd);
	return (0);
}
