#include <stdio.h>
#include "dirent.h"
#include <string.h>
#include "unistd.h"


void T(DIR* dir, int depth, char path[])
{
    char tmp[255];  //路径备份

    if (dir == NULL) return; //递归边界
    while (1)
    {
        dirent* de = readdir(dir);
        if (de == NULL)  //如果当前层没有文件了，就退出，循环边界
            return;
        if (strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0)
            continue;   //过滤掉当前目录和父目录

        if (de->d_type == DT_REG)  //如果是普通文件，直接输出名字
        {
            if (depth == 1)
                printf("├───");
            else
            {
                printf("│   ");
                // use corresponding tab to display hierarchy
                for (int i = 1; i < depth - 1; i++) printf("│   "); 
                printf("├───");
            }
            printf("%s\n", de->d_name);
        }
        else if (de->d_type == DT_DIR)  //如果是目录文件
        {
            if (depth == 1)
                printf("├───");
            else
            {
                printf("│   ");
                // use corresponding tab to display hierarchy
                for (int i = 1; i < depth - 1; i++) printf("│   "); 
                printf("├───");
            }
            printf("%s\n", de->d_name);

            strcpy(tmp, path);  //备份一下path，防止破坏path
            strcat(tmp, "/");
            strcat(tmp, de->d_name);   //拼接绝对路径
            DIR* p = opendir(tmp);  //打开下层目录
            T(p, depth + 1, tmp);  //递归调用，深度+1
            closedir(p); //递归结束记得关闭打开的目录
        }
    }
}

int main(int argc, char* argv[])
{
#ifdef DEBUG
    printf("debug mode is set to ON\n");
#endif
    DIR* dir;
    char path[255];
    //分为三种情况处理输入：
    if (argc == 1)  //1：如果传入参数只有一个，相当于只有命令，没有参数，即为打印当前目录
    {
        getcwd(path, sizeof(path));//用系统调用getcwd获取当前目录
        dir = opendir(path); //打开当前目录文件
        printf(".\n");//输出当前路径，即为.
    }
    else
    {
        char* p = argv[1];
        if (p[0] == '/')  //2.如果传入的目录为绝对路径（即为/开头）
        {
            dir = opendir(argv[1]);  //直接用这个绝对路径打开
            printf("%s\n", argv[1]); //输出当前绝对路径
            strcpy(path, argv[1]);  //把绝对路径备份给path，用作下面递归函数的参数
        }
        else  //3.如果传入的目录为相对路径
        {
            printf("%s\n", argv[1]);  //先输出
            getcwd(path, sizeof(path));  //获取当前绝对路径
            strcat(path, "/");     //再和输入的相对路径拼接成绝对路径
            strcat(path, argv[1]);
            dir = opendir(path);
        }
    }
    T(dir, 1, path);  //递归函数
    closedir(dir);  //关闭文件
    return 0;
}