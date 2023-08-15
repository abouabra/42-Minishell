/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayman <ayman@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 21:39:20 by ykhayri           #+#    #+#             */
/*   Updated: 2023/08/02 19:04:44 by ayman            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_strrevcmp(char *s1, char *s2)
{
  int i;
  int j;

  j = 0;
  i = ft_strlen(s1) - ft_strlen(s2);
  if (i < 0)
    return (1);
  while (s1[i] && s2[j] && s1[i] == s2[j]) {
    i++;
    j++;
  }
  return (s1[i] - s2[j]);
}

void double_wild(int n[5], char *av, struct dirent *read, t_chars *chars) {
  n[len] = ft_strlen(chars->arr[0]) - 1;
  if (n[len] < 1)
    n[len] = 1;
  if (av[0] == '*' && av[ft_strlen(av) - 1] != '*' &&
      !ft_strrevcmp(read->d_name, chars->arr[0]))
    chars->strings[final] =
        ft_strjoin(chars->strings[final], ft_strjoin(read->d_name, " "));
  else if (av[0] != '*' && av[ft_strlen(av) - 1] == '*' &&
           !ft_strncmp(read->d_name, chars->arr[0], n[len]))
    chars->strings[final] =
        ft_strjoin(chars->strings[final], ft_strjoin(read->d_name, " "));
  else if (av[0] == '*' && av[ft_strlen(av) - 1] == '*' &&
           ft_strnstr(read->d_name, chars->arr[0], -1))
    chars->strings[final] =
        ft_strjoin(chars->strings[final], ft_strjoin(read->d_name, " "));
}

void many_wild(int n[5], char *av, struct dirent *read, t_chars *chars) {
  n[j] = -1;
  n[valid] = 0;
  chars->strings[tmp] = read->d_name;
  while (++n[j] < n[i]) {
    if (n[j] == n[i] - 1 && av[ft_strlen(av) - 1] != '*' &&
        !ft_strrevcmp(read->d_name, chars->arr[n[j]])) {
      if (chars->strings[tmp] && chars->strings[tmp][0])
        n[valid]++;
    } else if (n[j] == 0 && av[0] != '*' &&
               !ft_strncmp(chars->strings[tmp], chars->arr[n[j]],
                           ft_strlen(chars->arr[n[j]]))) {
      n[valid]++;
      chars->strings[tmp] += ft_strlen(chars->arr[n[j]]);
    } else if (chars->strings[tmp] &&
               ((n[j] > 0 && n[j] < n[i] - 1) ||
                (av[ft_strlen(av) - 1] == '*' && n[j] == n[i] - 1) ||
                (av[0] == '*' && n[j] == 0))) {
      chars->strings[tmp] =
          ft_strnstr(chars->strings[tmp], chars->arr[n[j]], -1);
      if (chars->strings[tmp]) {
        chars->strings[tmp] += ft_strlen(chars->arr[n[j]]);
        n[valid]++;
      }
    }
  }
  if (n[valid] == n[i])
    chars->strings[final] =
        ft_strjoin(chars->strings[final], ft_strjoin(read->d_name, " "));
}

void initialize_vals(DIR **dir, t_chars *chars, int n[5], char *av) {
  char *tmp_path = getcwd(NULL, 0);
  garbage_collector(tmp_path, 0);
  *dir = opendir(tmp_path);
  chars->arr = ft_split(av, '*');
  n[valid] = 0;
  chars->strings[final] = "";
  n[k] = 0;
  n[i] = -1;
  while (chars->arr[++n[i]])
    ;
}

char *wildcard(char *av) {
  DIR *dir;
  struct dirent *read;
  t_chars chars;
  int n[5];

  initialize_vals(&dir, &chars, n, av);
  while (++n[k]) {
    read = readdir(dir);
    if (!read)
      break;
    if (n[i] == 0 && read->d_name[0] != '.')
      chars.strings[final] =
          ft_strjoin(chars.strings[final], ft_strjoin(read->d_name, " "));
    else if (n[i] == 1 && read->d_name[0] != '.')
      double_wild(n, av, read, &chars);
    else if (n[i] > 1 && read->d_name[0] != '.')
      many_wild(n, av, read, &chars);
  }
  closedir(dir);
  return (chars.strings[final]);
}
