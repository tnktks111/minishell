/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:35:45 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/21 16:33:04 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void unlink_tmpfile(t_tree_node *node_simplecmd);
void unlink_all_tmpfiles(t_tree_node *node_pipeline);
bool have_quotes(char *limiter);
void remove_quotes(char **limiter);
static size_t	ft_env_count(char const *s);
static char	*env_word_splitter(char const *s);
static char	**free_and_fail(char **words, size_t allocated_count);
static char	**ft_env_split(char const *s);
void here_doc_expander(char **s, t_env *env);

void unlink_tmpfile(t_tree_node *node_simplecmd)
{
	t_redirect	*curr;

	curr = node_simplecmd->data.command.redirects;
	while (curr)
	{
		if (curr->kind == REDIR_HEREDOC)
			unlink(curr->filename);
		curr = curr->next;
	}
}

void unlink_all_tmpfiles(t_tree_node *node_pipeline)
{
	t_tree_node	*curr;

	curr = node_pipeline->left;
	while (curr->kind == NODE_PIPE)
		curr = curr->left;
	unlink_tmpfile(curr);
	curr = curr->parent;
	while (curr->kind != NODE_PIPE_LINE)
	{
		unlink_tmpfile(curr->right);
		curr = curr->parent;
	}
}

bool have_quotes(char *limiter)
{
	size_t i;

	i = 0;
	while (limiter[i])
	{
		if (limiter[i] == '\"' || limiter[i] == '\'')
			return true;
		i++;
	}
	return false;
}

void remove_quotes(char **limiter)
{
	char *newlimiter;
	size_t i;
	size_t j;
	size_t quote_cnt;

	i = 0;
	quote_cnt = 0;
	while ((*limiter)[i])
	{
		if ((*limiter)[i] == '\"' || (*limiter)[i] == '\'')
			quote_cnt++;
		i++;
	}
	newlimiter = malloc(sizeof(char) * (i - quote_cnt + 1));
	if (!newlimiter)
	{
		free(*limiter);
		*limiter = NULL;
		return ;
	}
	i = 0;
	j = 0;
	while ((*limiter)[i])
	{
		if ((*limiter)[i] != '\'' && (*limiter)[i] != '\"')
			newlimiter[j++] = (*limiter)[i];
		i++;
	}
	newlimiter[j] = 0;
	free(*limiter);
	*limiter = newlimiter;
}

static size_t	ft_env_count(char const *s)
{
	size_t	cnt;
    bool in_var;

	if (!s || !s[0])
		return (0);
    cnt = (s[0] != '$');
    in_var = (s[0] == '$');
	while (*s)
	{
        if (*s == '$')
        {
            if (*(s+1) && (ft_isdigit(*(s+1)) || *(s+1) == '?'))
            {
                s++;
                if (*(s+2) && *(s+2) == '$')
                    cnt++;
                in_var = false;

            }
            else
                in_var = true;
            cnt++;
        }
        else if (in_var == true && !(ft_isalnum(*s) || *s == '_'))
        {
            in_var = false;
            cnt++;
        }
        s++;
	}
	return (cnt);
}

static char	*env_word_splitter(char const *s)
{
	size_t	i;
	char	*res;

    if (!s || !s[0])
        return (NULL);
    i = 0;
    if (s[0] == '$')
    {
        if (s[1] && (s[1] == '?' || ft_isdigit(s[1])))
        {
            res = ft_strndup(s, 2);
            return (res);
        }
        i++;
        while(s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
            i++;
        res = ft_strndup(s, i);
        return (res);
    }
    else
    {
        while (s[i] && s[i] != '$')
            i++;
        res = ft_strndup(s, i);
        return (res);
    }
}

static char	**free_and_fail(char **words, size_t allocated_count)
{  
	size_t	i;

	i = 0;
	while (i < allocated_count)
		free(words[i++]);
	free(words);
	return (NULL);
}

static char	**ft_env_split(char const *s)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	**words;

	len = ft_env_count(s);
	words = (char **)malloc(sizeof(char *) * (len + 1));
	if (!words)
		return (NULL);
	words[len] = NULL;
	i = 0;
	j = 0;
	while (i < len)
	{
		words[i] = env_word_splitter(s + j);
		if (!words[i])
			return (free_and_fail(words, i + 1));
        j += ft_strlen(words[i]);
        i++;
	}
	
	return (words);
}

void here_doc_expander(char **s, t_env *env)
{
	char *res;
	char **tokens;
	char *newtoken;
	size_t i;

	tokens = ft_env_split(*s);
	// if (!tokens)
	// {
		
	// }
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '$')
		{
			
			
			if (tokens[i][1])
				newtoken = ft_strdup(ft_search(&tokens[i][1], env));
			else
				newtoken = ft_strdup("");
			// if (!newtoken)
			free(tokens[i]);
			tokens[i] = newtoken;
		}
		i++;
	}
	res = ft_concatenate_strarr(tokens);
	// if (!res)
	// {
		
	// }
	free_splited_data(tokens);
	free(*s);
	*s = res;
}

