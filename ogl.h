/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ogl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/24 07:24:26 by snicolet          #+#    #+#             */
/*   Updated: 2016/07/27 19:08:19 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_H
# define OGL_H

typedef struct			s_pt_c
{
	float				x;
	float				y;
	float				z;
	unsigned int		color;
	unsigned char		tx;
	unsigned char		ty;
	unsigned char		tx_enabled;
}						t_pt_c;

#endif
