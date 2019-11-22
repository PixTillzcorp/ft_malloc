/******************************************************************************/
/*____________________________________________________________________________*/
/*_____/\/\/\/\/\___/\/\____________/\/\/\/\/\/\_/\/\__/\/\__/\/\_____________*/
/*____/\/\____/\/\_______/\/\__/\/\____/\/\___________/\/\__/\/\__/\/\/\/\/\__*/
/*___/\/\/\/\/\___/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\______/\/\_____*/
/*__/\/\_________/\/\____/\/\/\______/\/\_____/\/\__/\/\__/\/\____/\/\________*/
/*_/\/\_________/\/\__/\/\__/\/\____/\/\_____/\/\__/\/\__/\/\__/\/\/\/\/\_____*/
/*____________________________________________________________________________*/
/*                                                                            */
/*----- Date ----------------{ 2019-09-10 16:14:54 }--------------------------*/
/*----- Author --------------{ PixTillz }-------------------------------------*/
/*----- Last Modified by ----{ hippolyteeinfalt }-----------------------------*/
/*----- Last Modified time --{ 2019-10-12 18:46:46 }--------------------------*/
/******************************************************************************/

#include "./includes/malloc.h"
#include <stdlib.h>
#include <sys/resource.h>

#define LEN_TAB 40000
#define CMD_SIZE 500

static void print_rlimit()
{
    struct rlimit data;

    getrlimit(RLIMIT_DATA, &data);
    printf("CURRENT RLIMIT [%ju]\n", data.rlim_cur + 100);
    printf("MAXIMUM RLIMIT [%ju]\n", data.rlim_max + 100);
}

int main() {
    size_t size = 0;
    size_t power_rand = 10;
    int exit = 1;
    int i = 0;
    int index = 0;
    char cmd[CMD_SIZE];
    char *tmp[LEN_TAB];
    void *last = NULL;

    while (exit) {
        ft_putstr("-> ");
        scanf("%s", cmd);
        i = 0;
        if (!strcmp(cmd, "show"))
            show_alloc_mem();
        else if (!strcmp(cmd, "show_ex"))
            show_alloc_mem_ex();
        else if (!strcmp(cmd, "add")) {
            ft_putstr("Number:");
            scanf("%s", cmd);
            ft_putstr("allocation of ");
            put_size(size);
            ft_putendl(" octet(s)");
            if (index)
                index++;
            while (i < atoi(cmd)) {
                tmp[index + i] = (char *)malloc(sizeof(char) * size);
                i++;
            }
            index += i - 1;
            last = tmp[index];
        }
        else if (!strcmp(cmd, "free")) {
            ft_putstr("Which:");
            scanf("%s", cmd);
            if (!strcmp(cmd, "all")) {
                while (i <= index)
                    free(tmp[i++]);
            }
            else if (!strcmp(cmd, "last")) {
                if (last) {
                    free(last);
                    last = NULL;
                }
                else
                    ft_putstr("No last malloc to free.\n");
            }
            else if (!strcmp(cmd, "random") && power_rand) {
                srand(time(NULL));
                while (i < index) {
                    if (!(rand() % power_rand))
                        free(tmp[i]);
                    i++;
                }
            }
        }
        else if (!strcmp(cmd, "dump_hexa")) {
            ft_putstr("Which:");
            scanf("%s", cmd);
            if (!strcmp(cmd, "all")) {
                while (i <= index)
                    dump_hexa(tmp[i++]);
            }
            else if (!strcmp(cmd, "last")) {
                if (last)
                    dump_hexa(last);
                else
                    ft_putstr("No last malloc to dump.\n");
            }
            else if (!strcmp(cmd, "random") && power_rand) {
                srand(time(NULL));
                while (i < index) {
                    if (!(rand() % power_rand))
                        dump_hexa(tmp[i]);
                    i++;
                }
            }
        }
        else if (!strcmp(cmd, "set")) {
            ft_putstr("Malloc size:");
            scanf("%s", cmd);
            size = atoll(cmd);
        }
        else if (!strcmp(cmd, "rlimit"))
            print_rlimit();
        else if (!strcmp(cmd, "write")) {
            ft_putstr("string:");
            ft_bzero(cmd, CMD_SIZE);
            scanf("%s", cmd);
            if (last)
                ft_memcpy(last, cmd, strlen(cmd));
            else
                ft_putstr("No last malloc to write in.\n");
        }
        else if (!strcmp(cmd, "calloc")) {
            ft_putstr("Number:");
            scanf("%s", cmd);
            index++;
            tmp[index] = calloc(atoi(cmd), size);
        }
        else if (!strcmp(cmd, "set_rand")) {
            ft_putstr("Power rand:");
            scanf("%s", cmd);
            power_rand = atoi(cmd);
        }
        else if (!strcmp(cmd, "exit")) {
            exit = 0;
        }
    }
    return (0);
}

//int main(int argc, char **argv)
//{
//	char **addr1;
//	char *addr2;
//	char *addr3;
//	int		i;
//
//	i = 0;
//	if (!(addr1 = (char **)malloc(sizeof(char *) * 100)))
//		return (1);
//	if (!(addr2 = (char *)malloc(sizeof(char) * 10)))
//		return (1);
//	if (!(addr3 = (char *)malloc(sizeof(char) * 10)))
//		return (1);
//	show_alloc_mem();
////	addr2 = "Hippolyte";
////	addr3 = "Alexandre";
////	ft_putendl(addr3);
//	// show_alloc_mem();
//	// if (!(addr2 = (char *)malloc(sizeof(char) * 128)))
//	// 	return (1);
//	// show_alloc_mem();
//	// if (!(addr3 = (char *)malloc(sizeof(char) * 128)))
//	// 	return (1);
//	while (i < LEN_TAB)
//		addr1[i++] = (char *)malloc(sizeof(char) * 128);
////    addr1[i++] = (char *)malloc(sizeof(char) * 50);
//	show_alloc_mem();
//	// free(tabl[4]);
//	// free(tabl[3]);
//	// free(tabl[2]);
//	// free(tabl[1]);
//	// free(tabl[0]);
//	// free(tabl);
//	// show_alloc_mem();
//	// free(addr2);
//	// show_alloc_mem();
//	// if (!(addr2 = (char *)malloc(sizeof(char) * 50)))
//	// 	return (1);
//	// show_alloc_mem();
//	// free(addr1[1]);
//	// free(addr3);
//	// show_alloc_mem();
//	return (0);
//}


// int main()
// {
// 	int i;
// 	char *addr;

// 	i = 0;
// 	while (i < 50) {
// 		// if (i == 2)
// 		// 	while (1);
// 		addr = (char *)malloc(arc4random() % 4096);
// 		addr[0] = 42;
// 		i++;
// 	}
// 	show_alloc_mem();
// 	return (0);
// }
