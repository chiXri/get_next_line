/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m.chiri <m.chiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:47:10 by m.chiri           #+#    #+#             */
/*   Updated: 2024/11/04 17:58:36 by m.chiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int	ft_is_empty_file(int fd)
{
	struct stat	st;

	if (fstat(fd, &st) == -1)
	{
		perror("Error getting file status");
		return (1);
	}
	return (st.st_size == 0);
}

void	print_lines(int fd)
{
	int		call_count;
	char	*line;

	call_count = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		call_count++;
		printf("Output line %d: '%s'\n", call_count, line);
		free(line);
	}
	printf("get_next_line() was called %d times.\n", call_count);
}

int	main(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	if (ft_is_empty_file(fd))
	{
		printf("The file is empty.\n");
		close(fd);
		return (0);
	}
	print_lines(fd);
	close(fd);
	return (0);
}
