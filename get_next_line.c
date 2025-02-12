/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m.chiri <m.chiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:41:19 by m.chiri           #+#    #+#             */
/*   Updated: 2025/02/12 15:34:52 by m.chiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*join_and_free(char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*new_str;

	if (s1)
		len1 = ft_strlen(s1);
	else
		len1 = 0;
	len2 = ft_strlen(s2);
	new_str = malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	if (s1)
	{
		ft_strlcpy(new_str, s1, len1 + 1);
		free(s1);
	}
	ft_strlcpy(new_str + len1, s2, len2 + 1);
	return (new_str);
}

static char	*extract_line(char **saved)
{
	char	*newline_pos;
	char	*line;
	char	*new_saved;
	size_t	line_length;	

	if (*saved == NULL)
		return (NULL);
	newline_pos = ft_strchr(*saved, '\n');
	if (newline_pos)
	{
		line_length = newline_pos - *saved + 1;
		line = malloc(line_length + 1);
		if (!line)
			return (NULL);
		ft_strlcpy(line, *saved, line_length + 1);
		new_saved = ft_strdup(newline_pos + 1);
		free(*saved);
		*saved = new_saved;
		return (line);
	}
	return (NULL);
}

static char	*handle_remaining(char **saved)
{
	char	*line;

	if (*saved == NULL || **saved == '\0')
	{
		free(*saved);
		*saved = NULL;
		return (NULL);
	}
	line = ft_strdup(*saved);
	free(*saved);
	*saved = NULL;
	return (line);
}

static int	handle_read(int fd, char **saved, char *buffer)
{
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read < 0)
	{
		free(*saved);
		*saved = NULL;
		return (-1);
	}
	buffer[bytes_read] = '\0';
	*saved = join_and_free(*saved, buffer);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*saved = NULL;
	char		buffer[BUFFER_SIZE + 1];
	char		*line;
	int			read_result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		line = extract_line(&saved);
		if (line != NULL)
			return (line);
		read_result = handle_read(fd, &saved, buffer);
		if (read_result < 0)
			return (NULL);
		if (read_result == 0)
		{
			line = handle_remaining(&saved);
			return (line);
		}
	}
}
