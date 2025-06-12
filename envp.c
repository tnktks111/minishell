#include "minishell.h"

// typedef struct s_env
// {
// 	char **envp;
// 	int len;
// 	int cap;
// } t_env;

size_t ft_envplen(char **envp)
{
    size_t cnt;

    cnt = 0;
    while (envp[cnt])
        cnt++;
    return (cnt);
}

void ft_null_padding(char **envp, size_t size)
{
    size_t i;

    i = 0;
    while (i < size)
    {
        envp[i] = NULL;
        i++;
    }
}

void free_allocated_data(char **data, size_t allocated)
{
    size_t i;

    i = 0;
    while(i < allocated)
        free(data[i++]);
    free(data);
}

void free_splited_data(char **data)
{
    size_t i;

    i = 0;
    while (data[i])
        free(data[i]);
    free(data);
}

int **ft_envpcpy(char **dst, char **src)
{
    size_t i;
    i = 0;
    while (src[i])
    {
        dst[i] = ft_strdup(src[i]);
        if (!dst[i])
        {
            free_allocated_data(dst, i);
            return (EXIT_FAILURE);
        }
        i++;
    }
    return (EXIT_SUCCESS);
}

int **ft_envpdup(char **envp, t_env *env)
{
    size_t i;
    size_t len;

    len = ft_envplen(envp);
    env->envp = (char **)malloc(sizeof(char *) * (len + 1));
    i = 0;
    while (i < len)
    {
        env->envp[i] = ft_strdup(envp[i]);
        if (!env->envp[i])
        {
            free_allocated_data(env->envp, i);
            return (EXIT_FAILURE);
        }
        i++;
    }
    env->envp[i] = NULL;
    env->cap = len;
    env->len = len;
    return (EXIT_SUCCESS);
}

char **ft_add_env(t_env *env, char *key, char *val)
{
    char **new_envp;
    if (env->cap == env->len)
    {
        new_envp = (char **)malloc(sizeof(char *) * (env->cap * 2 + 1));
        ft_null_padding(new_envp, env->cap * 2);
        if (ft_envpcpy(new_envp, env->envp) == EXIT_FAILURE)
            free_splited_data(env->envp);
        free_splited_data(env->envp);
        env->cap *= 2;
    }

}

char **ft_remove_env(t_env *env, char *key)