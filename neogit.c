#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define MAX_FILEPATH_LENGTH 10000
#define MAX_LINE_LENGTH 10000
#define dbg(x) printf("%s\n", x);
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

int add_to_staging(char *filepath);
int apply_reg();
void path_to_name(char *);
char name[MAX_LINE_LENGTH];
char email[MAX_LINE_LENGTH];
char neogitpath[MAX_FILEPATH_LENGTH];
char file_name[120];
int rev_flag = 0;
int exist = 0;
char curr_dir[MAX_FILEPATH_LENGTH];
char stagingpath[MAX_FILEPATH_LENGTH];
char trackedpath[MAX_FILEPATH_LENGTH];
char fixed_address[MAX_FILEPATH_LENGTH];
char w_branch[MAX_FILEPATH_LENGTH];
char branchname[MAX_FILEPATH_LENGTH];
char clean_path[MAX_FILEPATH_LENGTH];
char last_edit_time[40];
int check_repo_exist()
{
    // char curr_dir[10000];
    getcwd(curr_dir, sizeof(curr_dir));
    // printf("%s", curr_dir);
    struct dirent *entry;
    char temp_dir_name[10000];
    getcwd(temp_dir_name, sizeof(temp_dir_name));
    while ((strcmp(temp_dir_name, "/")) != 0)
    // dbg("n");
    {
        DIR *dir = opendir(".");
        while ((entry = readdir(dir)) != NULL)
        {
            if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
            {
                chdir(".neogit");
                getcwd(neogitpath, MAX_FILEPATH_LENGTH);
                // printf("neogit repo already exists!");
                exist = 1;
                chdir(curr_dir);
                closedir(dir);
                return 0;
            }
        }
        chdir("..");
        getcwd(temp_dir_name, sizeof(temp_dir_name));
        // printf("%s\n", temp_dir_name);
    }
    return 1;
}
void stagepath_builder()
{
    strcpy(stagingpath, neogitpath);
    strcat(stagingpath, "/staging.txt");
}
void fix_address(char *realpath)
{
    char realpathcopy[MAX_FILEPATH_LENGTH];
    strcpy(realpathcopy, realpath);
    path_to_name(realpathcopy);
    char *ptr = strstr(realpath, ".neogit");
    if (ptr != NULL)
    {
        *(ptr + 0) = '\0';
        strcpy(clean_path, realpath);
        strcat(clean_path, file_name);
        printf("%s", clean_path);
    }
    else
    {
        strcpy(clean_path, realpath);
    }
    // strcpy
}
void w_branch_find()
{
    chdir(neogitpath);
    FILE *read = fopen("w_branch.txt", "r");
    fscanf(read, "%s", w_branch);
    fclose(read);
}
void branchname_find()
{
    chdir(neogitpath);
    FILE *read = fopen("branch_name.txt", "r");
    fscanf(read, "%s", branchname);
    fclose(read);
}
int run_config(int argc, char *argv[])
{
    if (strcmp(argv[2], "-global") == 0)
    {
        if (strstr(argv[3], "alias.") != NULL)
        {
            chdir("/");
            FILE *file;
            if ((file = fopen("alias.txt", "a")) == NULL)
            {
                system("sudo touch alias.txt");
                system("sudo chmod 777 alias.txt");
                file = fopen("alias.txt", "a");
            }
            int len = strlen(argv[3]);
            char niknama[MAX_LINE_LENGTH];
            strncpy(niknama, argv[3] + 6, len - 5);
            char command_check[MAX_LINE_LENGTH];
            strcpy(command_check, "neogit ");
            strcat(command_check, argv[4]);
            chdir("/");
            if (access("gitfolder", F_OK) == -1)
            {

                system("sudo mkdir gitfolder");
                system("sudo chmod 777 gitfolder");
                chdir("gitfolder");
                mkdir(".neogit", 0755);
                // chdir(".neogit");
            }
            chdir("gitfolder");

            int result = system(command_check);
            chdir(curr_dir);

            if (result)
            {
                printf("invalid command!\n");
                return 1;
            }
            fprintf(file, "%s   %s\n", niknama, argv[4]);
            chdir(curr_dir);
            fclose(file);
            return 0;
        }
        if (strlen(argv[4]) == 0)
        {
            printf("faild to register!\n");
            return 0;
        }
        if (argv[3][5] == 'n')
        {
            chdir("/");
            system("sudo touch name.txt");
            system("sudo chmod 777 name.txt");
            FILE *file = fopen("name.txt", "w");
            if (file == NULL)
            {

                return 1;
            }
            fprintf(file, "%s", argv[4]);
            fclose(file);
            chdir(curr_dir);
        }
        else if (argv[3][5] == 'e')
        {
            chdir("/");
            system("sudo touch mail.txt");
            system("sudo chmod 777 mail.txt");
            FILE *file = fopen("mail.txt", "w");
            if (file == NULL)
            {

                return 1;
            }
            fprintf(file, "%s", argv[4]);
            fclose(file);
        }
        chdir(curr_dir);
        return 0;
    }

    else
    {

        chdir(neogitpath);
        if (strstr(argv[2], "alias.") != NULL)
        {
            FILE *file = fopen("alias.txt", "a");
            int len = strlen(argv[2]);
            char niknama[MAX_LINE_LENGTH];
            chdir("gitfolder");
            char command_check[MAX_LINE_LENGTH];
            strcpy(command_check, "neogit ");
            strcat(command_check, argv[3]);

            int result = system(command_check);
            chdir(curr_dir);

            if (result)
            {
                printf("invalid command!\n");
                return 1;
            }
            strncpy(niknama, argv[2] + 6, len - 5);
            fprintf(file, "%s   %s\n", niknama, argv[3]);
            chdir(curr_dir);
            fclose(file);
            return 0;
        }
        if (strlen(argv[3]) == 0)
        {
            printf("faild to register!\n");
            return 0;
        }
        if (argv[2][5] == 'n')
        {
            FILE *file = fopen("name.txt", "w");
            fprintf(file, "%s", argv[3]);
            fclose(file);
        }

        else if (argv[2][5] == 'e')
        {
            FILE *file = fopen("mail.txt", "w");
            fprintf(file, "%s", argv[3]);
            fclose(file);
        }
        chdir(curr_dir);
        return 0;
    }
}
int trackedpath_build()
{
    strcpy(trackedpath, neogitpath);
    strcat(trackedpath, "/");
    strcat(trackedpath, "tracks.txt");
}
int run_init()
{
    // char curr_dir[10000];
    getcwd(curr_dir, sizeof(curr_dir));
    // printf("%s", curr_dir);
    struct dirent *entry;
    char temp_dir_name[10000];
    getcwd(temp_dir_name, sizeof(temp_dir_name));
    while ((strcmp(temp_dir_name, "/")) != 0)
    // dbg("n");
    {
        DIR *dir = opendir(".");
        while ((entry = readdir(dir)) != NULL)
        {
            if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
            {
                chdir(".neogit");
                getcwd(neogitpath, MAX_FILEPATH_LENGTH);
                printf("neogit repo already exists!\n");
                exist = 1;
                chdir(curr_dir);
                closedir(dir);
                return 0;
            }
        }
        chdir("..");
        getcwd(temp_dir_name, sizeof(temp_dir_name));
        // printf("%s\n", temp_dir_name);
    }
    chdir(curr_dir);
    mkdir(".neogit", 0755);

    chdir(".neogit");
    // system("touch stage_cpy.txt");
    mkdir("all_files", 0755);
    FILE *mk = fopen("tracks.txt", "w");
    fclose(mk);
    // getcwd(neogitpath, MAX_FILEPATH_LENGTH);
    FILE *stage_making = fopen("staging.txt", "w");
    fclose(stage_making);
    // printf("ll");
    mkdir("branch_commits", 0755);
    mkdir("curr_branch_files", 0755);
    FILE *id = fopen("globalid.txt", "w");
    fprintf(id, "0");
    id = fopen("branch_name.txt", "w");
    fprintf(id, "master");
    id = fopen("w_branch.txt", "w");
    fprintf(id, "master");
    mkdir("stage_steps", 0755);

    chdir("stage_steps");
    FILE *step = fopen("count.txt", "w");
    fprintf(step, "0");
    fclose(step);
    chdir("..");
    mkdir("files", 0755);
    mkdir("last_commit", 0755);
    chdir("last_commit");
    FILE *writting = fopen("last_ID.txt", "w");
    fprintf(writting, "0");
    writting = fopen("curr_ID.txt", "w");
    fprintf(writting, "0");
    writting = fopen("branch.txt", "w");
    fprintf(writting, "master");
    writting = fopen("msg.txt", "w");
    writting = fopen("state.txt", "w");
    fclose(writting);
    writting = fopen("name.txt", "w");
    writting = fopen("mail.txt", "w");
    fclose(writting);
    chdir("..");
    mkdir("all_commits", 0755);
    mkdir("staging_area", 0755);
    chdir(curr_dir);
    printf("neogit repo initialized successfully!\n");
    return 0;
}
int run_makebranch()
{
    getcwd(curr_dir, sizeof(curr_dir));
    // printf("%s", curr_dir);
    struct dirent *entry;
    char temp_dir_name[10000];
    getcwd(temp_dir_name, sizeof(temp_dir_name));
    // while ((strcmp(temp_dir_name, "/")) != 0)
    // // dbg("n");
    // {
    //     DIR *dir = opendir(".");
    //     while ((entry = readdir(dir)) != NULL)
    //     {
    //         if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
    //         {
    //             chdir(".neogit");
    //             getcwd(neogitpath, MAX_FILEPATH_LENGTH);
    //             printf("neogit repo already exists!\n");
    //             exist = 1;
    //             chdir(curr_dir);
    //             closedir(dir);
    //             return 0;
    //         }
    //     }
    //     chdir("..");
    //     getcwd(temp_dir_name, sizeof(temp_dir_name));
    //     // printf("%s\n", temp_dir_name);
    // }
    chdir(curr_dir);
    mkdir(".neogit", 0755);
    chdir(".neogit");
    mkdir("all_files", 0755);
    // system("touch stage_cpy.txt");

    FILE *mk = fopen("tracks.txt", "w");
    fclose(mk);
    // getcwd(neogitpath, MAX_FILEPATH_LENGTH);
    FILE *stage_making = fopen("staging.txt", "w");
    fclose(stage_making);
    // printf("ll");
    mkdir("branch_commits", 0755);
    mkdir("curr_branch_files", 0755);
    FILE *id = fopen("globalid.txt", "w");
    fprintf(id, "0");
    id = fopen("branch_name.txt", "w");
    fprintf(id, "master");
    id = fopen("w_branch.txt", "w");
    fprintf(id, "master");
    mkdir("stage_steps", 0755);

    chdir("stage_steps");
    FILE *step = fopen("count.txt", "w");
    fprintf(step, "0");
    fclose(step);
    chdir("..");
    mkdir("files", 0755);
    mkdir("last_commit", 0755);
    chdir("last_commit");
    FILE *writting = fopen("last_ID.txt", "w");
    fprintf(writting, "0");
    writting = fopen("curr_ID.txt", "w");
    fprintf(writting, "0");
    writting = fopen("branch.txt", "w");
    fprintf(writting, "master");
    writting = fopen("msg.txt", "w");
    writting = fopen("state.txt", "w");
    fclose(writting);
    writting = fopen("name.txt", "w");
    writting = fopen("mail.txt", "w");
    fclose(writting);
    chdir("..");
    mkdir("all_commits", 0755);
    mkdir("staging_area", 0755);
    chdir(curr_dir);
    // printf("branch initialized successfully!\n");
    return 0;
}
int check_file_exist(int argc, char filename[])
{
    DIR *dir;
    struct dirent *entry;
    char temp_dir_name[MAX_FILEPATH_LENGTH];
    getcwd(curr_dir, MAX_FILEPATH_LENGTH);
    strcpy(temp_dir_name, temp_dir_name);
    while (!strcmp(temp_dir_name, "/"))
    {
        dir = opendir(".");
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, filename) == 0)
            {
                closedir(dir);
                chdir(curr_dir);
                return 0;
            }
        }
        chdir("..");
        getcwd(temp_dir_name, MAX_FILEPATH_LENGTH);
    }
    closedir(dir);
    chdir(curr_dir);
    return 1;
}

int run_add(int argc, char *const argv[])
{
    char filepath[MAX_FILEPATH_LENGTH];
    int i = 2;
    chdir(neogitpath);
    chdir("stage_steps");
    int step;
    FILE *stepp = fopen("count.txt", "r");
    fscanf(stepp, "%d", &step);
    fclose(stepp);
    stepp = fopen("count.txt", "w");
    step++;
    fprintf(stepp, "%d", step);
    fclose(stepp);
    char count[10];
    sprintf(count, "%d", step);
    stepp = fopen(count, "w");

    fclose(stepp);
    chdir("..");
    // getcwd(curr_dir, MAX_FILEPATH_LENGTH);
    //  printf("%s\n", cwd);
    if (strcmp(argv[i], "-f") == 0)
    {
        i++;
    }
    while (1)
    {
        int p = 1;
        strcpy(filepath, curr_dir);
        int length = strlen(filepath);
        filepath[length] = '/';
        filepath[length + 1] = 0;
        // printf("%s\n", filepath);
        // printf("%d",argc);
        if (argv[i] == NULL)
            break;
        strcat(filepath, argv[i]);
        if (access(filepath, F_OK) == -1)
        {
            printf("file or directory not found!\n");
            p = 0;
        }
        else
        {

            struct stat path_stat;
            stat(filepath, &path_stat);

            char temp_file[MAX_FILEPATH_LENGTH];
            // printf("%c", entry->d_name);

            if (S_ISDIR(path_stat.st_mode))
            {
                DIR *dir = opendir(filepath);
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL)
                {
                    if (entry->d_type != DT_DIR)
                    {
                        strcpy(temp_file, filepath);
                        strcat(temp_file, "/");
                        strcat(temp_file, entry->d_name);
                        add_to_staging(temp_file);
                        add_to_tracks(temp_file);
                        copy_file(temp_file);
                        chdir(neogitpath);
                        chdir("files");
                        // mkdir(path_to_name(filepath), 0755);
                        chdir(curr_dir);
                        p = 0;
                    }
                }
            }
        }
        if (p)
        {
            add_to_staging(filepath);

            add_to_tracks(filepath);
            copy_file(filepath);
            chdir(neogitpath);
            chdir("files");
            path_to_name(filepath);
            mkdir(file_name, 0755);
            chdir(curr_dir);
        }
        i++;
    }

    return 0;
}
void path_to_name(char path[])
{
    char *token = strtok(path, "/");
    while (token != NULL)
    {
        strcpy(file_name, token);
        token = strtok(NULL, "/");
    }
}
void address_to_time(char *realpath)
{
    struct stat attr;
    stat(realpath, &attr);
    dbg("dfl");
    strcpy(last_edit_time, ctime(&attr.st_ctime));
}

int copy_file(char *filepath)
{
    char command[MAX_LINE_LENGTH];
    strcpy(command, "cp ");
    strcat(command, filepath);
    strcat(command, " ");
    strcat(command, neogitpath);
    strcat(command, "/staging_area");
    system(command);
    return 0;
}
int add_to_tracks(char *filepath)
{
    FILE *trk = fopen(trackedpath, "r");
    char line[MAX_LINE_LENGTH];
    while (fscanf(trk, "%s\n", line) != EOF)
    {
        if (strcmp(line, filepath) == 0)
        {
            fclose(trk);
            return 0;
        }
    }
    trk = fopen(trackedpath, "a");
    fprintf(trk, "%s\n", filepath);
    return 0;
}

int add_to_staging(char *filepath)
{

    fix_address(filepath);

    strcpy(filepath, clean_path);

    char time[100];
    FILE *fil = fopen(filepath, "r");
    struct stat status;
    fil = fstat(fileno(fil), &status);
    strcpy(time, ctime(&status.st_ctime));
    // chdir(neogitpath);
    // chdir("stage_steps");
    // char step[10];
    // FILE *stepp = fopen("count.txt", "r");
    // fscanf(stepp, "%s", step);
    // fclose(stepp);
    // stepp = fopen(step, "a");
    // //printf("%s", time);
    // chdir("..");

    // FILE *track = fopen(trackedpath, "a");
    //  if(access(neogitpath, F_OK) == -1){
    //      chdir(neogitpath);
    //      mkdir("staging",0755 );
    //      FILE *file = fopen("staging", "w");
    //      if(file == NULL)printf("null");
    //      fprintf(file, "%s\n", filepath);
    //      fclose(file);
    //      chdir(curr_dir);
    //      return 0;

    FILE *file = fopen(stagingpath, "r+");

    char line[MAX_LINE_LENGTH];
    char temp[100];
    while (fscanf(file, "%s    ", line) != EOF)
    {
        int length = strlen(line);
        if (line[length - 1] == 10)
        {
            line[length - 1] = 0;
        }
        if (strcmp(filepath, line) == 0)
        {

            fprintf(file, "%s", time);
            chdir(curr_dir);
            return 0;
        }
        fscanf(file, "%s\n", line);
    }
    // chdir(neogitpath);
    FILE *write;
    write = fopen(stagingpath, "a");
    fprintf(write, "%s    %s", filepath, time);
    chdir("stage_steps");
    char step[10];
    FILE *stepp = fopen("count.txt", "r");
    fscanf(stepp, "%s", step);
    fclose(stepp);
    stepp = fopen(step, "a");
    // printf("%s", time);
    chdir("..");
    fprintf(stepp, "%s\n", filepath);

    // fprintf(track, "%s");
    fclose(write);
    fclose(stepp);
    chdir(curr_dir);
    return 0;
}

int dep_add(int dep, int x)
{
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            printf("%d %s\n", x, entry->d_name);
        }
    }
}
int apply_reg()
{
    // changes working directory
    chdir(neogitpath);
    if (access("name.txt", F_OK) == -1 || access("mail.txt", F_OK) == -1)
    {
        if (access("/name.txt", F_OK) == -1 || access("/mail.txt", F_OK) == -1)
        {
            printf("tell me who you are.\n");
            return 1;
        }
        else
        {
            char command[MAX_LINE_LENGTH];
            strcpy(command, "cp ");
            strcat(command, "/name.txt /mail.txt ");
            strcat(command, neogitpath);
            system(command);
            strcat(command, "/last_commit");
            system(command);
            return 0;
        }
    }

    char command[MAX_LINE_LENGTH];
    strcpy(command, "cp name.txt mail.txt last_commit");
    system(command);
    return 0;
}
int date_to_sec(char *str_time)
{
    struct tm tm_time;
    time_t time;

    memset(&tm_time, 0, sizeof(struct tm));
    strptime(str_time, "%a %b %d %H:%M:%S %Y", &tm_time);
    time = mktime(&tm_time);

    return time;
}
int date_compare(char *time0, char *time1)
{
    int zer = strlen(time0);
    int one = strlen(time1);
    if (time0[zer - 1] == 10)
        time0[zer - 1] = 0;
    if (time1[one - 1] == 10)
        time1[one - 1] = 0;
    if (date_to_sec(time0) > date_to_sec(time1))
    {
        return 1;
    }
    else

        return 0;
    {
        dbg("nim");
    }
}

int file_counter()
{
    int num = 0;
    FILE *count = fopen(stagingpath, "r");
    char c[10000];
    while (fgets(c, 10000, count) != NULL)
    {
        num++;
    }
    fclose(count);
    return num;
}

int run_commit(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("invali command!\n");
        return 1;
    }
    if (strlen(argv[3]) == 0 || strlen(argv[3]) > 72)
    {
        printf("declare a valid commit messeage!\n");
        return 1;
    }
    // chdir(neogitpath);
    // if(access("state", F_OK) != -1){
    //     printf("you are not allowed to commit\n");
    //     return 0;
    // }
    if (apply_reg())
        return 0;
    // check stage exist
    FILE *stage = fopen(stagingpath, "r");
    char tmp[MAX_LINE_LENGTH];
    if (fscanf(stage, "%s    ", tmp) == EOF)
    {
        printf("you have no staged files!\n");
        fclose(stage);
        return 0;
    }
    fclose(stage);
    // end of checking
    int last_commit_ID = lst_commit_ID_read();

    chdir(neogitpath);
    if (access("commit_per.txt", F_OK) != -1)
    {
        stage = fopen("commit_per.txt", "r");
        int n;
        fscanf(stage, "%d", &n);
        fclose(stage);
        if (!n)
        {
            printf("you are not able to commit here\n");
            return 0;
        }
    }
    chdir("last_commit");
    FILE *write = fopen("last_ID.txt", "w");

    fprintf(write, "%d", last_commit_ID);
    fclose(write);
    write = fopen("num_of_files.txt", "w");
    int y = file_counter();
    fprintf(write, "%d", y);
    fclose(write);
    write = fopen("branch.txt", "w");
    fprintf(write, "%s", branchname);
    write = fopen("msg.txt", "w");
    fprintf(write, "%s", argv[3]);
    fclose(write);
    FILE *read = fopen(stagingpath, "r");
    write = fopen("state.txt", "a");
    while (fscanf(read, "%s    ", tmp) != EOF)
    {
        if (update(tmp, last_commit_ID))
        {

            fprintf(write, "%s    %d\n", tmp, last_commit_ID);
        }
        for (int i = 0; i < 24; i++)
        {
            char c = fgetc(read);
        }
    }
    fclose(read);
    fclose(write);
    write = fopen(stagingpath, "w");
    fclose(write);
    chdir(neogitpath);
    chdir("all_commits");
    sprintf(tmp, "%d", last_commit_ID);
    mkdir(tmp, 0755);
    chdir("..");
    FILE *bname = fopen("branch_name.txt", "r");
    char branchname[1000];
    fscanf(bname, "%s", branchname);
    bname = fopen("last_commit/branch.txt", "w");
    fprintf(bname, "%s", branchname);
    fclose(bname);
    char command[120];

    if (strcmp(branchname, "master"))
    {
        chdir(neogitpath);
        strcat(command, "cp -a last_commit/ ../../../all_commits/");
        strcat(command, tmp);
        strcat(command, "/");
        system(command);
        chdir(curr_dir);
    }
    chdir(neogitpath);
    strcpy(command, "cp -a last_commit/. all_commits/");
    strcat(command, tmp);
    strcat(command, "/");
    system(command);
    chdir(neogitpath);
    strcpy(command, "cp -a staging_area/. curr_branch_files/");
    system(command);
    copy_file_to_directory(last_commit_ID);
    strcpy(command, "cp -a last_commit/. branch_commits/");
    strcat(command, tmp);
    system(command);
    return 0;
    // mkdir()
}
void copy_file_to_directory(int last_commit_ID)
{
    chdir(neogitpath);
    DIR *file = opendir("staging_area");
    struct dirent *entry;
    int number_of_files = 0;
    while ((entry = readdir(file)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
            continue;
        char command[MAX_FILEPATH_LENGTH * 2];
        char filenumname[10];
        sprintf(filenumname, "%d", last_commit_ID);
        number_of_files++;
        strcpy(command, "cp staging_area/");
        strcat(command, entry->d_name);
        strcat(command, " files/");
        strcat(command, entry->d_name);
        strcat(command, "/");
        strcat(command, filenumname);
        system(command);
        if (strcmp(branchname, "master"))
        {

            strcpy(command, "cp staging_area/");
            strcat(command, entry->d_name);
            strcat(command, " ../../../all_files");
            system(command);
        }

        strcpy(command, "cp staging_area/");
        strcat(command, entry->d_name);
        strcat(command, " all_files");
        system(command);
    }
    closedir(file);
    system("rm -r staging_area");
    mkdir("staging_area", 0755);
}
int lst_commit_ID_read()
{
    if (strcmp(branchname, "master") == 0)
    {
        chdir(neogitpath);

        FILE *read = fopen("globalid.txt", "r");
        int n;
        fscanf(read, "%d", &n);
        n++;

        // read = fopen("last_ID", "w");
        // fprintf(read, "%d", n);
        fclose(read);
        read = fopen("globalid.txt", "w");
        fprintf(read, "%d", n);
        fclose(read);
        chdir(curr_dir);
        return n;
    }
    else
    {
        int n;
        chdir(curr_dir);
        chdir("..");
        chdir("..");
        // chdir("..");
        FILE *glblid = fopen("globalid.txt", "r");
        fscanf(glblid, "%d", &n);
        fclose(glblid);
        n++;
        glblid = fopen("globalid.txt", "w");
        fprintf(glblid, "%d", n);
        fclose(glblid);
        return n;
    }
}
int lastidread()
{
    if (strcmp(branchname, "master") == 0)
    {
        chdir(neogitpath);

        FILE *read = fopen("globalid.txt", "r");
        int n;
        fscanf(read, "%d", &n);
        n;

        // read = fopen("last_ID", "w");
        // fprintf(read, "%d", n);
        fclose(read);
        chdir(curr_dir);
        return n;
    }
    else
    {
        int n;
        chdir(curr_dir);
        chdir("..");
        chdir("..");
        // chdir("..");
        FILE *glblid = fopen("globalid.txt", "r");
        fscanf(glblid, "%d", &n);
        fclose(glblid);
        n;
        return n;
    }
}
int update(char *path, int last_commit_ID)
{
    FILE *file = fopen("state.txt", "r+");
    char line[MAX_LINE_LENGTH];
    int n;
    while (fscanf(file, "%s    ", line) != EOF)
    {
        if (strcmp(line, path) == 0)
        {
            fprintf(file, "%d\n", last_commit_ID);
            fclose(file);
            return 0;
        }
        fscanf(file, "%d\n", &n);
    }
    fclose(file);
    return 1;
}
int is_staged(char *path)
{
    chdir(neogitpath);
    FILE *file = fopen("staging.txt", "r");
    char line[MAX_LINE_LENGTH];
    while (fscanf(file, "%s    ", line) != EOF)
    {
        if (strcmp(line, path) == 0)
        {
            chdir(curr_dir);
            fclose(file);
            return 0;
        }
        fscanf(file, "%s\n", line);
    }
    chdir(curr_dir);
    fclose(file);
    return 1;
}
int run_reset(char realpath[])
{
    struct stat type;

    stat(realpath, &type);
    if (S_ISDIR(type.st_mode))
    {
        DIR *dir = opendir(realpath);
        struct dirent *entry;
        char file[10000];
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
            {
                strcpy(file, realpath);
                strcat(file, "/");
                strcat(file, entry->d_name);
                run_reset(file);
            }
            else if (entry->d_type != DT_DIR)
            {
                // printf("%s", entry->d_name);
                // dbg("dd");
                strcpy(file, realpath);
                strcat(file, "/");
                strcat(file, entry->d_name);
                // printf("%s", file);
                run_reset(file);
            }
        }

        return 0;
    }
    chdir(neogitpath);
    char filepath[MAX_FILEPATH_LENGTH];
    char time[MAX_LINE_LENGTH];
    // char realpath[MAX_FILEPATH_LENGTH];

    FILE *read;
    read = fopen("staging.txt", "r");
    FILE *write;
    write = fopen("stg_cpy.txt", "w");
    while (fscanf(read, "%s   ", filepath) != EOF)
    {
        fgets(time, MAX_LINE_LENGTH, read);
        if (strlen(time) > 0 && time[strlen(time) - 1] == 10)
        {
            time[strlen(time) - 1] = 0;
        }
        if (strcmp(filepath, realpath) == 0)
        {
            continue;
        }
        else
        {
            fprintf(write, "%s    %s\n", filepath, time);
        }
    }
    fclose(write);
    fclose(read);
    system("cp stg_cpy.txt staging.txt");
    path_to_name(realpath);
    // printf("%s", file_name);
    chdir("staging_area");
    char command[MAX_FILEPATH_LENGTH];
    strcpy(command, "rm ");
    strcat(command, file_name);
    // printf("%s", command);
    system(command);
    chdir("..");
    return 0;
}
int run_reset_undo()
{
    chdir(neogitpath);
    chdir("stage_steps");
    char step[10];
    char realpath[MAX_FILEPATH_LENGTH];
    FILE *read = fopen("count.txt", "r");
    fscanf(read, "%s", step);
    fclose(read);
    read = fopen(step, "r");

    while (fscanf(read, "%s\n", realpath) != EOF)
    {
        run_reset(realpath);
    }
    chdir("stage_steps");
    fclose(read);
    read = fopen("count.txt", "r");
    int num;
    // dbg("lll");
    fscanf(read, "%d", &num);
    num--;
    fclose(read);
    read = fopen("count.txt", "w");

    fprintf(read, "%d", num);
    fclose(read);
    char command[MAX_LINE_LENGTH];
    strcpy(command, "rm ");
    strcat(command, step);
    system(command);
    return 0;
}

int branch_initialize(char *branch_name)
{
    chdir(neogitpath);
    chdir("branches");
    chdir(branch_name);
    chdir(".neogit");
    FILE *write = fopen("branch_name.txt", "w");
    fprintf(write, "%s", branch_name);
    fclose(write);
    system("cp -a ../../../last_commit/. last_commit/");
    system("cp -a ../../../curr_branch_files/. curr_branch_files/");
    system("cp -a ../../../files/. files/");
    // chdir("last_commit");
    // write = fopen("branch.txt", "w");
    // fprintf(write, "%s", branch_name);
}
void commitjastprint(int ID)
{
    char c[100];
    sprintf(c, "%d", ID);
    char realadress[1000];
    strcpy(realadress, neogitpath);
    strcat(realadress, "/all_commits/");
    strcat(realadress, c);
    address_to_time(realadress);
    chdir(c);
    // chdir(c);
    FILE *file;

    file = fopen("last_ID.txt", "r");
    fgets(c, 1000, file);
    printf("ID:                        %s\n", c);
    fclose(file);

    printf("time of commit:       %s", last_edit_time);
    file = fopen("branch.txt", "r");
    fgets(c, 1000, file);
    printf("branch:                   %s\n", c);
    fclose(file);

    file = fopen("name.txt", "r");
    fgets(c, 1000, file);
    printf("author:                 %s\n", c);
    fclose(file);

    file = fopen("num_of_files.txt", "r");
    fgets(c, 1000, file);
    printf("commited files:            %s\n", c);
    fclose(file);

    file = fopen("msg.txt", "r");
    fgets(c, 1000, file);
    printf("commit message:       %s\n", c);
    fclose(file);
    printf("\n\n");
    chdir("..");
}
void commit_inf_print(int ID, char mode[], int size)
{
    chdir(neogitpath);
    chdir("all_commits");
    char c[1000];
    if (strcmp("before", mode) == 0)
    {
        for (int i = ID; i > 0; i--)
        {
            commitjastprint(i);
        }
    }
    if (strcmp("n_last", mode) == 0)
    {
        for (int i = ID; i > ID - size; i--)
        {
            commitjastprint(i);
        }
    }
    if (strcmp("after", mode) == 0)
    {
        int comm_count = lastidread();
        printf("<%d>", comm_count);
        printf("this id %d", ID);
        dbg("id");
        int j = comm_count;
        while (j >= ID)
        {
            commitjastprint(j);
        }
    }
}
int run_log(int argc, char *argv[])
{
    if (argv[2] == NULL)
    {
        int com_count = lastidread();
        commit_inf_print(com_count, "before", 0);
        return 0;
    }
    if (strcmp(argv[2], "-n") == 0)
    {
        int com_count = lastidread();
        int n;
        sscanf(argv[3], "%d", &n);
        commit_inf_print(com_count, "n_last", n);
    }
    if (strcmp(argv[2], "-branch") == 0)
    {
        int com_count = lastidread();
        chdir(neogitpath);
        chdir("all_commits");
        char x[100];
        int t = com_count;
        for (t; t > 0; t--)
        {
            sprintf(x, "%d", t);
            chdir(x);
            FILE *file = fopen("branch.txt", "r");
            fscanf(file, "%s", x);
            // printf("%s", x);
            fclose(file);
            // printf("%s", x);
            if (strcmp(x, argv[3]) == 0)
            {
                commitjastprint(t);
            }
        }
    }
    if (strcmp(argv[2], "author"))
    {
        int com_count = lastidread();
        chdir(neogitpath);
        chdir("all_commits");
        char x[100];
        int t = com_count;
        for (t; t > 0; t--)
        {
            sprintf(x, "%d", t);
            chdir(x);
            FILE *file = fopen("name.txt", "r");
            fscanf(file, "%s", x);
            // printf("%s", x);
            fclose(file);
            // printf("%s", x);
            if (strcmp(x, argv[3]) == 0)
            {
                commitjastprint(t);
            }
        }
    }

    if (strcmp(argv[2], "-since") == 0)
    {
        // commit_inf_print(0, "after", 0);
        int com_count = lastidread();
        chdir(neogitpath);
        chdir("all_commits");
        char x[100];
        for (int i = com_count; i > 0; i--)
        {
            sprintf(x, "%d", i);
            char a[MAX_FILEPATH_LENGTH];
            strcpy(a, neogitpath);
            strcat(a, "/all_commits/");
            strcat(a, x);

            address_to_time(a);
            if (date_compare(last_edit_time, argv[3]))
            {
                commitjastprint(i);
            }
        }
    }
    if (strcmp(argv[2], "-before") == 0)
    {
        int com_count = lastidread();
        chdir(neogitpath);
        chdir("all_commits");
        char x[100];
        for (int i = com_count; i > 0; i--)
        {
            sprintf(x, "%d", i);
            char a[MAX_FILEPATH_LENGTH];
            strcpy(a, neogitpath);
            strcat(a, "/all_commits/");
            strcat(a, x);
            address_to_time(a);
            if (!date_compare(last_edit_time, argv[3]))
            {
                commitjastprint(i);
            }
        }
    }

    if (strcmp(argv[2], "-search") == 0)
    {
        int com_count = lastidread();
        chdir(neogitpath);
        chdir("all_commits");
        char c[100];
        for (int i = com_count; i > 0; i--)
        {
            sprintf(c, "%d", i);
            chdir(c);
            FILE *file = fopen("msg.txt", "r");
            fgets(c, 100, file);
            fclose(file);
            chdir("..");
            for (int j = 3; j < argc; j++)
            {
                if (strstr(c, argv[j]) != NULL)
                {
                    commitjastprint(i);
                    break;
                }
            }
        }
    }
}

int digit(char *a)
{
    int i = 0;
    int t = 0;
    for (i; a[i] != '\0'; i++)
    {
        if (isdigit(a[i]))
            t++;
    }
    if (t == i)
        return 1;
    else
        return 0;
}
int exist_branch(char *bname)
{
    chdir(neogitpath);
    DIR *dir = opendir("branches");
    // if(dir == NULL){
    //     printf("ldjs");
    // }
    // dbg("fjkdl");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // printf("%s", entry->d_name);
        if (strcmp(bname, entry->d_name) == 0)
        {
            return 0;
        }
    }
    return 1;
}

int empty_stage()
{
    FILE *stage = fopen(stagingpath, "r");
    char c[MAX_LINE_LENGTH];
    if (fscanf(stage, "%s    ", c) == EOF)
    {
        fclose(stage);
        return 0;
    }
    return 1;
}

int run_checkout(int argc, char *argv[])
{
    if (empty_stage())
    {
        printf("you have changes that has not been commited!\n");
        return 0;
    }
    if (strcmp(argv[2], "HEAD") == 0)
    {
        char comm[1000];
        strcpy(comm, "neogit checkout ");
        strcat(comm, w_branch);
        system(comm);
        return 0;
    }
    else if (strstr(argv[2], "HEAD-") != NULL)
    {
        int idd;
        sscanf(argv[2], "HEAD-%d", &idd);
        char comm[1000];
        int comm_count = lastidread();
        printf("%d", comm_count);
        sprintf(comm, "neogit revert -n %d", comm_count - idd);
        system(comm);
        chdir(neogitpath);
        FILE *file = fopen("curr_comm.txt", "w");
        fprintf(file, "%d", comm_count - idd);
        fclose(file);
        return 0;
    }
    if (!digit(argv[2]))
    {
        if (strcmp("master", argv[2]) == 0)
        {
            chdir(neogitpath);
            if (access("state", F_OK) != -1)
            {

                system("rm -r state");
            }
            mkdir("state", 0755);
            FILE *file;
            file = fopen("w_branch.txt", "w");
            fprintf(file, "%s", argv[2]);
            fclose(file);
            file = fopen("commit_per.txt", "w");
            fprintf(file, "%d", 1);
            fclose(file);
            system("cp -a curr_branch_files/* state/");
            return 0;
        }
        if (exist_branch(argv[2]))
        {
            printf("no such branch\n");
            return 0;
        }
        chdir(neogitpath);
        chdir("branches");
        chdir(argv[2]);
        chdir(".neogit");
        system("cp -a curr_branch_files/. ../../../state/");
        chdir(neogitpath);
        FILE *file;
        file = fopen("w_branch.txt", "w");
        fprintf(file, "%s", argv[2]);
        fclose(file);
        file = fopen("commit_per.txt", "w");
        fprintf(file, "%d", 1);
        fclose(file);
        return 0;
    }
    else
    {
        chdir(neogitpath);
        if (access("state", F_OK) != -1)
        {
            // dbg("nima");
            system("rm -r state");
        }
        char a[1000];
        mkdir("state", 0755);
        FILE *brnch;

        brnch = fopen("commit_per.txt", "w");
        fprintf(brnch, "0");
        fclose(brnch);
        brnch = fopen("curr_comm.txt", "w");
        fprintf(brnch, "%s", argv[2]);
        fclose(brnch);
        chdir("all_commits");
        chdir(argv[2]);

        brnch = fopen("branch.txt", "r");
        char brname[1000];
        fscanf(brnch, "%s", brname);
        fclose(brnch);
        if (strcmp(brname, "master"))
        {
            int lst = lastidread();
            char last[10];
            int y;
            sscanf(argv[2], "%d", &y);
            sprintf(last, "%d", lst);
            chdir(neogitpath);
            chdir("branches");
            chdir(brname);
            chdir(".neogit");
            DIR *dir = opendir("files");
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL)
            {
                if (!(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")))
                    continue;
                chdir("files");
                chdir(entry->d_name);
                int i = lst;
                char x[1000];
                while (i > 0)
                {
                    sprintf(x, "%d", i);
                    if (access(x, F_OK) != -1 && i <= y)
                        break;
                    i--;
                }
                if (i == 0)
                {
                    chdir("..");
                    chdir("..");
                    continue;
                }

                sprintf(x, "cp %d ../../../../../state/%s", i, entry->d_name);
                system(x);
                chdir("..");
                chdir("..");
            }
        }
        else
        {
            int lst = lastidread();
            char last[10];
            int y;
            sscanf(argv[2], "%d", &y);
            sprintf(last, "%d", lst);
            chdir(neogitpath);
            DIR *dir = opendir("files");
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL)
            {
                if (!(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")))
                    continue;
                chdir("files");
                chdir(entry->d_name);
                int i = lst;
                char x[1000];
                while (i > 0)
                {
                    sprintf(x, "%d", i);
                    if (access(x, F_OK) != -1 && i <= y)
                        break;
                    i--;
                }
                if (i == 0)
                {
                    chdir("..");
                    chdir("..");
                    continue;
                }
                sprintf(x, "cp %d ../../state/%s", i, entry->d_name);
                system(x);
                chdir("..");
                chdir("..");
            }
        }
    }
}

int list_of_files(char *find)
{
    DIR *dif = opendir(curr_dir);
    struct dirent *entr;
    int i = 0;
    while ((entr = readdir(dif)) != NULL)
    {
        if (strcmp(find, entr->d_name) == 0)
        {

            return 0;
        }
        i++;
    }
    return 1;
    // for(int t = 0;t<=i; t++){
    //   printf("%s\n", cwd_files[t]);
    // }
}

// int in_dir(char cwd_files[200][100],char* find){
//     for(int t = 0;t<=200; t++){
//         if(cwd_files[t] == NULL)break;
//        printf("%s\n", cwd_files[t]);
//    }
//     for(int i = 0; i<200; i++){

//     //printf("%s   n%s\n", cwd_files[i], find);
//         if(strcmp(find, cwd_files[i]) == 0){
//             //printf("%s", cwd_files);
//             return 0;
//         }
//         if(cwd_files[i] == NULL)return 1;
//     }
// }
int is_same(FILE *org, FILE *git)
{
    char orgline[MAX_LINE_LENGTH];
    char gitline[MAX_LINE_LENGTH];
    while (1)
    {

        if (fgets(orgline, MAX_LINE_LENGTH, org) == 0 & fgets(gitline, MAX_LINE_LENGTH, git) == 0)
        {
            return 0;
        }

        if (strcmp(orgline, gitline))
        {
            return 1;
        }
    }
}
int run_status()
{
    // list_of_files(cwd_files);
    char cwd_files[200][100];
    if (!strcmp(w_branch, "master"))
    {
        chdir(neogitpath);
        DIR *dir = opendir("curr_branch_files");
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (!((strcmp(entry->d_name, ".")) && (strcmp(entry->d_name, ".."))))
            {
                continue;
            }

            if (list_of_files(entry->d_name))
            {
                printf("%s       D\n", entry->d_name);
                continue;
            }

            else
            {
                chdir(neogitpath);
                chdir("curr_branch_files");
                FILE *git = fopen(entry->d_name, "r");
                chdir(curr_dir);
                FILE *org = fopen(entry->d_name, "r");
                char filepath[1000];
                strcpy(filepath, curr_dir);
                strcat(filepath, "/");
                strcat(filepath, entry->d_name);

                if (is_same(org, git))
                {

                    if (is_staged(filepath))
                    {
                        printf("%s      -M\n", entry->d_name);
                        continue;
                    }
                    else
                    {
                        printf("%s      +M\n", entry->d_name);
                        continue;
                    }
                }
                else
                {
                    if (!is_same(org, git))
                    {
                        if (is_staged(filepath))
                        {
                            printf("%s      -A\n", entry->d_name);
                            continue;
                        }
                        else
                        {
                            printf("%s      +A\n", entry->d_name);
                            continue;
                        }
                    }
                }
                int com_count = lastidread();
                char c[10];
                sprintf(c, "%d", com_count);
                chdir(neogitpath);
                chdir("files");
                chdir(entry->d_name);
                char mim[89];
                getcwd(mim, 89);
                // printf("%s", mim);
                if (access(c, F_OK) == -1)
                {

                    continue;
                }
            }
        }
    }
}

int which_branch()
{
    int id;
    chdir(neogitpath);
    FILE *file = fopen("commit_per.txt", "r");

    fscanf(file, "%d", &id);

    fclose(file);
    if (id)
    {
        return lastidread();
    }
    else
    {
        file = fopen("curr_comm.txt", "r");
        fscanf(file, "%d", &id);
        fclose(file);
        // printf("%d", id);
        return id;
    }
}

void bubble_sort(char array[100][100], int size)
{
    int i, j;
    char temp[100];

    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (strcmp(array[j], array[j + 1]) > 0)
            {
                // Swap array[j] and array[j+1]
                strcpy(temp, array[j]);
                strcpy(array[j], array[j + 1]);
                strcpy(array[j + 1], temp);
            }
        }
    }
}

int run_tag(int argc, char *argv[])
{
    chdir(neogitpath);
    if (access("tags", F_OK) == -1)
    {
        mkdir("tags", 0755);
    }
    if (argc == 2)
    {
        chdir("tags");
        DIR *dir = opendir(".");
        struct dirent *entry;
        int i = 0;
        char file_name[100][100];
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, "..") && strcmp(entry->d_name, "."))
            {
                // printf("%s\n", entry->d_name);
                strcpy(file_name[i], entry->d_name);
                i++;
            }
        }
        bubble_sort(file_name, i);
        for (int j = 0; j < i; j++)
        {
            printf("%s\n", file_name[j]);
        }
        return 0;
    }
    if (strcmp(argv[2], "show") == 0)
    {
        chdir(neogitpath);
        chdir("tags");
        chdir(argv[3]);
        printf("tag name:       %s\n", argv[3]);
        FILE *file = fopen("tag_ID.txt", "r");
        char cc[100];
        fscanf(file, "%s", cc);
        fclose(file);
        printf("commit_ID       %s\n", cc);
        file = fopen("name.txt", "r");
        fscanf(file, "%s", cc);
        printf("user:       %s\n", cc);
        fclose(file);
        file = fopen("msg.txt", "r");
        if (file != NULL)
        {
            fscanf(file, "%s", cc);
            printf("user:       %s\n", cc);
            fclose(file);
        }
        // getcwd(cc, 100);
        // strcat(cc, "/");
        // strcat(cc, argv[3]);
        // printf("%s", cc);
        // address_to_time(cc);
        struct stat attr;
        stat("tag_ID.txt", &attr);

        // // printf("Last modified: %s", ctime(&attr.st_ctime));

        printf("time:       %s\n", ctime(&attr.st_ctime));

        return 0;
    }
    chdir(neogitpath);
    if (access("name.txt", F_OK) == -1 || access("mail.txt", F_OK) == -1)
    {
        if (access("/name.txt", F_OK) == -1 || access("/mail.txt", F_OK) == -1)
        {
            printf("tell me who you are.\n");
            return 1;
        }
    }
    // printf("%d", which_branch());
    chdir("tags");
    if ((access(argv[3], F_OK) != -1) && (strcmp(argv[argc - 1], "-f") != 0))
    {
        printf("there is a tag with same name!\n");
        return 0;
    }
    // for -f
    if (strcmp(argv[argc - 1], "-f") == 0)
    {
        // remove("nima");
        char comm[1000];
        if (access(argv[3], F_OK) != -1)
        {
            strcpy(comm, "rm -r ");
            strcat(comm, argv[3]);
            system(comm);
        }
        // mkdir(argv[3], 0755);
        strcpy(comm, "neogit tag -a");
        for (int i = 3; i < argc - 1; i++)
        {
            strcat(comm, " ");
            strcat(comm, argv[i]);
        }
        // printf("%s", comm);
        system(comm);
    }
    else
    {
        // for no -f
        FILE *write;
        int id;
        char msg[1000];
        char command[1000];
        if (argc == 4)
        {
            // no msg
            // no id
            sprintf(command, "neogit tag -a %s -c %d", argv[3], which_branch());
            system(command);
            return 0;
        }
        else if ((strcmp(argv[4], "-c") == 0))
        {
            // no msg
            // yes id

            sscanf(argv[5], "%d", &id);
            chdir(neogitpath);
            chdir("tags");
            mkdir(argv[3], 0755);
            chdir(argv[3]);
            system("cp ../../name.txt name.txt");
            system("cp ../../mail.txt maile.txt");
            write = fopen("tag_ID.txt", "w");
            fprintf(write, "%d", id);
            fclose(write);
            write = fopen("tag_name.txt", "w");
            fprintf(write, "%s", argv[3]);
            fclose(write);
            return 0;
        }
        else if ((strcmp(argv[4], "-m") == 0) && (argc < 8))
        {
            // dbg("djlf");
            // y msg
            // n id
            sprintf(command, "neogit tag -a %s -m %s -c %d", argv[3], argv[5], which_branch());
            // printf("%s", command);
            system(command);
            return 0;
        }
        else if (strcmp(argv[6], "-c") == 0)
        {
            // y msg
            // y id
            sscanf(argv[7], "%d", &id);
            chdir(neogitpath);
            chdir("tags");
            mkdir(argv[3], 0755);
            chdir(argv[3]);
            system("cp ../../name.txt name.txt");
            system("cp ../../mail.txt maile.txt");
            strcpy(msg, argv[5]);
            write = fopen("tag_ID.txt", "w");
            fprintf(write, "%d", id);
            fclose(write);
            write = fopen("tag_name.txt", "w");
            fprintf(write, "%s", argv[3]);
            fclose(write);
            write = fopen("msg.txt", "w");
            fprintf(write, "%s", argv[5]);
            fclose(write);
            return 0;
        }
    }
}

int is_whitespace_line(const char *line)
{
    while (*line != '\0')
    {
        if (!isspace((unsigned char)*line))
        {
            return 0; // Non-whitespace character found
        }
        line++;
    }
    return 1; // Only whitespace characters found
}
int run_diffi(int argc, char *argv[])
{
    //printf("df");
    char command[1000];
    char c[100];
    chdir(neogitpath);
    if (strcmp(argv[2], "-f") == 0)
    {
        if (access("diffi", F_OK) != -1)
        {
            system("rm -r diffi");
            system("rm -r diffi2");
        }
        mkdir("diffi", 0755);
        mkdir("diffi2", 0755);
        if (access("diffi_cp", F_OK) != -1)
        {
            system("rm -r diffi_cp");
            system("rm -r diffi_cp2");
        }
        mkdir("diffi_cp", 0755);
        mkdir("diffi_cp2", 0755);
        chdir(curr_dir);
        strcpy(c, argv[3]);
        path_to_name(c);
        sprintf(command, "cp %s .neogit/diffi/%s", argv[3], file_name);
        system(command);
        sprintf(command, "cp %s .neogit/diffi_cp/%s", argv[3], file_name);
        system(command);
        strcpy(c, argv[4]);
        path_to_name(c);
        sprintf(command, "cp %s .neogit/diffi2/%s", argv[4], file_name);
        system(command);
        sprintf(command, "cp %s .neogit/diffi_cp2/%s", argv[4], file_name);
        system(command);

        chdir(neogitpath);
        int bg1, end1, bg2, end2;
        int flag = 0;
        if (argc > 5)
        {
            sscanf(argv[6], "%d-%d", &bg1, &end1);
            sscanf(argv[8], "%d-%d", &bg2, &end2);
            flag = 1;
            // printf("%d", bg1)
        }
        // first file
        FILE *read;
        FILE *write;
        chdir("diffi");
        read = fopen(argv[3], "r");
        chdir("../diffi_cp");
        write = fopen(argv[3], "w");
        char line[1000];
        for (int i = 0; i < 10000; i++)
        {
            if (flag)
            {
                if (i + 1 < bg1)
                {
                    continue;
                }
                if (i + 1 > end1)
                {
                    break;
                }
            }
            if (fgets(line, 1000, read) == NULL)
            {
                break;
            }
            if (!is_whitespace_line(line))
            {
                fprintf(write, "%s", line);
            }
        }

        fclose(write);
        fclose(read);
        chdir(neogitpath);
        chdir("diffi2");
        read = fopen(argv[4], "r");
        chdir("../diffi_cp2");
        write = fopen(argv[4], "w");
        for (int i = 0; i < 10000; i++)
        {
            if (flag)
            {
                if (i + 1 < bg2)
                {
                    continue;
                }
                if (i + 1 > end2)
                {
                    break;
                }
            }
            if (fgets(line, 1000, read) == NULL)
            {
                break;
            }

            if (!is_whitespace_line(line))
            {
                fprintf(write, "%s", line);
            }
        }
        chdir(neogitpath);
        chdir("diffi_cp");
        fclose(write);
        read = fopen(argv[3], "r");
        FILE *read2;
        chdir("..");
        chdir("diffi_cp2");
        read2 = fopen(argv[4], "r");
        char line2[10000];
        //fgets(line2, 10000, read2);
        
        // printf("%s", line2);
        for (int i = 0; i < 10000; i++)
        {
            if ((fgets(line, 1000, read) == NULL))
            {
            
                break;
            }
                if((fgets(line2, 1000, read2)) == NULL){
                    
                    break;
                }
            if (strcmp(line2, line))
            {
                //printf("nnnn%smmmm",line2);
                printf(">>>>>"COLOR_GREEN"\n%s - %d\n%s\n"COLOR_BLUE"\n%s - %d\n%s\n"COLOR_RESET"<<<<<",argv[3], i+1, line, argv[4], i+1, line2);
            }
        }
        return 0;
    }
    else if(strcmp(argv[2], "-c") == 0){
        char command[100];
        strcpy(command, "neogit checkout ");
        strcat(command, argv[3]);
        system(command);
        chdir(neogitpath);
        if(access("state1", F_OK)!=-1){
            system("rm -r state1");
            system("rm -r state2");
        }
        if(access("diffi", F_OK)!=-1){
            system("rm -r diffi");
            system("rm -r diffi2");
            system("rm -r diffi_cp");
            system("rm -r diffi_cp2");
        }
        mkdir("diffi", 0755);
        mkdir("diffi2", 0755);
        mkdir("diffi_cp", 0755);
        mkdir("diffi_cp2", 0755);
        system("cp -a state/ state1/");
        strcpy(command, "neogit checkout ");
        strcat(command, argv[4]);
        //dbg("df");
        system(command);
        system("cp -a state/ state2/");
        system("neogit checkout HEAD");
        char first_list[100][100];
        char second_list[100][100];
        DIR *dir = opendir("state1");
        struct dirent *entry;
        int i = 0;
        while((entry= readdir(dir))!=NULL){
            if(strcmp(entry->d_name, ".")&&strcmp(entry->d_name, "..") ){
                strcpy(first_list[i], entry->d_name);
                i++;
            }
        }
        closedir(dir);
        int j = 0;
        dir = opendir("state2");
        while((entry= readdir(dir))!=NULL){
            if(strcmp(entry->d_name, ".")&&strcmp(entry->d_name, "..") ){
                strcpy(second_list[j], entry->d_name);
                j++;
            }
        }
        char diffis[100][100];
        int y = 0;
        printf("files that are not in the second commit_id:\n");
        for(int t=0; t<i; t++){
            int flag = 1;
            for(int x = 0; x<j; x++){
                if(strcmp(first_list[t], second_list[x]) == 0){
                    strcpy(diffis[y], first_list[t]);
                    //printf("%s", first_list[t]);
                    flag = 0;
                    y++;
                }
            }
                if(flag)printf("%s\n", first_list[t]);

        }
        printf("files that are not in the first commit_id:\n");
        for(int t = 0;t<j; t++){
            int flag = 1;
            for(int x = 0; x<i;x++){
                if(strcmp(second_list[t], first_list[x]) == 0){
                    flag = 0;
                }
            }
            if(flag)printf("%s\n", second_list[t]);
        }
        chdir(neogitpath);
        for(int i = 0; i<y;i++){
         sprintf(command, "cp state1/%s diffi/%s", diffis[i], diffis[i]);
         //printf("%s", command);
         system(command); 
         sprintf(command, "cp state1/%s diffi_cp/%s", diffis[i], diffis[i]);
         system(command);   
        sprintf(command, "cp state2/%s diffi2/%s", diffis[i], diffis[i]);
         system(command); 
         sprintf(command, "cp state2/%s diffi_cp2/%s", diffis[i], diffis[i]);
         system(command);  
        FILE *read;
        FILE *write;
        chdir("diffi");
        read = fopen(diffis[i], "r");
        chdir("../diffi_cp");
        write = fopen(diffis[i], "w");
        char line[1000];
        for (int i = 0; i < 10000; i++)
        {
            // if (flag)
            // {
            //     if (i + 1 < bg1)
            //     {
            //         continue;
            //     }
            //     if (i + 1 > end1)
            //     {
            //         break;
            //     }
            // }
            if (fgets(line, 1000, read) == NULL)
            {
                break;
            }
            if (!is_whitespace_line(line))
            {
                fprintf(write, "%s", line);
            }
        }

        fclose(write);
        fclose(read);
        chdir(neogitpath);
        chdir("diffi2");
        read = fopen(diffis[i], "r");
        chdir("../diffi_cp2");
        write = fopen(diffis[i], "w");
        for (int i = 0; i < 10000; i++)
        {
            // if (flag)
            // {
            //     if (i + 1 < bg2)
            //     {
            //         continue;
            //     }
            //     if (i + 1 > end2)
            //     {
            //         break;
            //     }
           // }
            if (fgets(line, 1000, read) == NULL)
            {
                break;
            }

            if (!is_whitespace_line(line))
            {
                fprintf(write, "%s", line);
            }
        }
        
        chdir(neogitpath);
        chdir("diffi_cp");
        fclose(write);
        fclose(read);
        // FILE* read;
        // char line[1000];
        read = fopen(diffis[i], "r");
        FILE *read2;
        chdir("..");
        chdir("diffi_cp2");
        read2 = fopen(diffis[i], "r");
        char line2[10000];
        //fgets(line2, 10000, read2);
        
        // printf("%s", line2);
        for (int i = 0; i < 10000; i++)
        {
            if ((fgets(line, 1000, read) == NULL))
            {
            
                break;
            }
                if((fgets(line2, 1000, read2)) == NULL){
                    
                    break;
                }
            if (strcmp(line2, line))
            {
                //printf("nnnn%smmmm",line2);
                printf(">>>>>"COLOR_GREEN"\n%s - %d\n%s\n"COLOR_BLUE"\n%s - %d\n%s\n"COLOR_RESET"<<<<<",diffis[i], i+1, line, diffis[i], i+1, line2);
            }
        }
        

        }
        
    }
    return 0;
}
int setmsg(){

}

int run_merge(int argc, char*argv[]){
    char command[100];
    char neoname[100];
    strcpy(neoname, argv[3]);
    strcat(neoname, "_");
    strcat(neoname, argv[4]);
        printf("%s", neoname);
    //dbg("dd");
    sprintf(command, "neogit branch %s_%s", argv[3], argv[4]);
    chdir(curr_dir);
    system(command);
    char first_list[100][100];
    char second_list[100][100];
    DIR *dir;
    struct dirent *entry;
    if(strcmp("master", argv[3]) == 0 || strcmp("master", argv[4]) == 0){
        chdir(neogitpath);
        chdir("branches");
        mkdir("master", 0755);
        chdir("master");
        mkdir(".neogit", 0755);
        chdir(".neogit");
        //mkdir("state", 0755);
        chdir(neogitpath);
        system("cp -a state/. branches/master/.neogit/curr_branch_files");
        //chdir(curr_dir);

    }
        chdir(neogitpath);
        chdir("branches");
        chdir(argv[3]);
        chdir(".neogit");
        int i = 0;
        dir = opendir("curr_branch_files");
        while((entry = readdir(dir))!=NULL){
            if(strcmp("..", entry->d_name)&&strcmp(".", entry->d_name)){
                strcpy(first_list[i], entry->d_name);
                i++;
                //printf("%s\n", entry->d_name);
            }
            
        }
        closedir(dir);
        chdir("..");
        chdir("..");
        //chdir
        chdir(argv[4]);
        chdir(".neogit");
        dir = opendir("curr_branch_files");
        chdir("curr_branch_files");
        if(dir == NULL)printf("s");
        int j = 0;
        while((entry = readdir(dir))!=NULL){
        //dbg("d");
            if(strcmp("..", entry->d_name)&&strcmp(".", entry->d_name)){
                strcpy(second_list[j], entry->d_name);
                j++;
                //printf("%s\n", entry->d_name);
            }
            
        }
        char diffis[100][100];
        int y = 0;
        for(int t=0; t<i; t++){
            int flag = 1;
            for(int x = 0; x<j; x++){
                if(strcmp(first_list[t], second_list[x]) == 0){
                    strcpy(diffis[y], first_list[t]);
                    //printf("%s", first_list[t]);
                    flag = 0;
                    y++;
                }
                if(flag){
                    sprintf(command, "cp %s ../../../%s/.neogit/curr_branch_files/", first_list[t], neoname);
                    system(command);
                }
            }
            for(int t = 0;t<j; t++){
            int flag = 1;
            for(int x = 0; x<i;x++){
                if(strcmp(second_list[t], first_list[x]) == 0){
                    flag = 0;
                }
            }
            if(flag){
                sprintf(command, "cp %s ../../../%s/.neogit/curr_branch_files/", second_list[t], neoname);
                system(command);
            }
        }
        chdir(neogitpath);
        if(access("diffi", F_OK) == -1){
            mkdir("diffi", 0777);
            mkdir("diffi_cp", 0777);
             mkdir("diffi", 0777);
            mkdir("diffi_cp", 0777);
                    }
        chdir("branches");
        for(int i = 0; i<y;i++){
         sprintf(command, "cp -a %s/.neogit/curr_branch_files/%s ../../../../diffi",argv[3] ,diffis[i]);
         //printf("%s", command);
         system(command); 
         sprintf(command, "cp %s/.neogit/curr_branch_files/%s ../../../../diffi_cp",argv[3] ,diffis[i], diffis[i]);
         system(command);   
         sprintf(command, "cp %s/.neogit/curr_branch_files/%s ../../../../diffi2",argv[4] ,diffis[i], diffis[i]);
         system(command); 
          sprintf(command, "cp %s/.neogit/curr_branch_files/%s ../../../../diffi_cp2",argv[4] ,diffis[i], diffis[i]);
         system(command);  
        FILE *read;
        FILE *write;
        chdir("diffi");
        read = fopen(diffis[i], "r");
        chdir("../diffi_cp");
        write = fopen(diffis[i], "w");
        char line[1000];
        for (int i = 0; i < 10000; i++)
        {
            // if (flag)
            // {
            //     if (i + 1 < bg1)
            //     {
            //         continue;
            //     }
            //     if (i + 1 > end1)
            //     {
            //         break;
            //     }
            // }
            if (fgets(line, 1000, read) == NULL)
            {
                break;
            }
            if (!is_whitespace_line(line))
            {
                fprintf(write, "%s", line);
            }
        }

        fclose(write);
        fclose(read);
        chdir(neogitpath);
        chdir("diffi2");
        read = fopen(diffis[i], "r");
        chdir("../diffi_cp2");
        write = fopen(diffis[i], "w");
        for (int i = 0; i < 10000; i++)
        {
            // if (flag)
            // {
            //     if (i + 1 < bg2)
            //     {
            //         continue;
            //     }
            //     if (i + 1 > end2)
            //     {
            //         break;
            //     }
           // }
            if (fgets(line, 1000, read) == NULL)
            {
                break;
            }

            if (!is_whitespace_line(line))
            {
                fprintf(write, "%s", line);
            }
        }
        
        chdir(neogitpath);
        chdir("diffi_cp");
        fclose(write);
        fclose(read);
        // FILE* read;
        // char line[1000];
        read = fopen(diffis[i], "r");
        FILE *read2;
        chdir("..");
        chdir("diffi_cp2");
        read2 = fopen(diffis[i], "r");
        char line2[10000];
        //fgets(line2, 10000, read2);
        
        // printf("%s", line2);
        for (int i = 0; i < 10000; i++)
        {
            if ((fgets(line, 1000, read) == NULL))
            {
            
                break;
            }
                if((fgets(line2, 1000, read2)) == NULL){
                    
                    break;
                }
            if (strcmp(line2, line))
            {
                //printf("nnnn%smmmm",line2);
                printf(">>>>>"COLOR_GREEN"\n%s - %d\n%s\n"COLOR_BLUE"\n%s - %d\n%s\n"COLOR_RESET"<<<<<",diffis[i], i+1, line, diffis[i], i+1, line2);
            }
        }
        printf("choose in files");
        if(1){
            system("cp %s ../../%s/.neogit/curr_branch_files", diffis[i], neoname);
        }

        }
        return 0;

        }
        
}
int main(int argc, char *const argv[])
{
    // printf("%d", argc);
    if (argc < 2)
    {
        perror("too few argumants");
        return 1;
    }

    getcwd(curr_dir, MAX_FILEPATH_LENGTH);

    if (strcmp(argv[1], "init") == 0)
    {
        return run_init();
        // printf("%d", check_repo_exist());
    }
    if (strcmp(argv[1], "makebranch") == 0)
    {
        return run_makebranch();
    }
    if (check_repo_exist())
    {
        // dbg("ll")
        printf("create a neogit repo!\n");
        return 0;
    }
    stagepath_builder();
    trackedpath_build();
    w_branch_find();
    branchname_find();
    chdir(curr_dir);
    if (strcmp(argv[1], "add") == 0)
    {
        if (strcmp(w_branch, "master") == 0)
        {
            return run_add(argc, argv);
        }

        chdir(curr_dir);
        char cmd[1000];
        strcpy(cmd, "cp ");
        strcat(cmd, argv[2]);
        strcat(cmd, " ");
        strcat(cmd, neogitpath);
        strcat(cmd, "/branches/");
        strcat(cmd, w_branch);
        system(cmd);
        chdir(neogitpath);
        chdir("branches");
        chdir(w_branch);
        strcpy(cmd, "neogit add ");
        strcat(cmd, argv[2]);
        system(cmd);
    }
    if (strcmp(argv[1], "branch") == 0)
    {
        chdir(neogitpath);
        if (access("branches", F_OK) == -1)
        {
            mkdir("branches", 0755);
            chdir("branches");
            mkdir(argv[2], 0755);
            chdir(argv[2]);
            system("neogit makebranch");
            printf("branch created\n");
        }
        else
        {
            chdir("branches");
            if (access(argv[2], F_OK) != -1)
            {
                printf("branch with the same name exists!\n");
                return 0;
            }
            else
            {
                mkdir(argv[2], 0755);
                chdir(argv[2]);
                system("neogit makebranch");
                printf("branch created\n");
            }
        }
        return branch_initialize(argv[2]);
    }
    if (!strcmp(argv[1], "log"))
    {
        return run_log(argc, argv);
    }
    if(strcmp("merge", argv[1])  == 0){
        return run_merge(argc, argv);
    }
    if (strcmp(argv[1], "commit") == 0)
    {
        if (strcmp(w_branch, "master"))
        {

            chdir(neogitpath);
            chdir("branches");
            chdir(w_branch);
            char cmd[MAX_FILEPATH_LENGTH];
            int j = 1;
            strcpy(cmd, argv[0]);
            while (argv[j] != NULL)
            {
                strcat(cmd, " ");
                strcat(cmd, argv[j]);
                j++;
            }
            return system(cmd);
        }
        else
        {

            return run_commit(argc, argv);
        }
    }
    if (strcmp(argv[1], "config") == 0)
    {
        return run_config(argc, argv);
    }
    if (strcmp(argv[1], "revert") == 0)
    {
        // dbg("dbg");
        rev_flag = 1;
        if (strcmp(argv[2], "-n") && (strstr(argv[2], "-m") == NULL))
        {
            char comm[100];
            strcpy(comm, "neogit checkout ");
            strcat(comm, argv[argc - 1]);
            system(comm);
            chdir(neogitpath);
            FILE *add = fopen("commit_per.txt", "w");
            fprintf(add, "1");
            fclose(add);
            system("neogit add state");

            // system
            // system(cc);
            // chdir(neogitpath);
            // chdir("state");
            // DIR* dir = opendir(".");
            // struct dirent *add;
            chdir("all_commits");

            chdir(argv[argc - 1]);
            char msg[100];
            FILE *readmsg = fopen("msg.txt", "r");
            fgets(msg, 100, readmsg);
            chdir("../..");
            printf("%s", msg);
            strcpy(comm, "neogit commit -m ");
            strcat(comm, msg);
            system(comm);
        }
        else if (strstr(argv[2], "-m") != 0 && strstr(argv[argc - 1], "HEAD") == 0)
        {
            char msg[100];
            int ln = strlen(argv[3]);
            argv[3][ln - 1] = '\0';
            // printf("%d", ln);
            strcpy(msg, argv[3]);
            // printf("%s", msg);
            char comm[100];
            strcpy(comm, "neogit checkout ");
            strcat(comm, argv[argc - 1]);
            system(comm);
            chdir(neogitpath);
            FILE *add = fopen("commit_per.txt", "w");
            fprintf(add, "1");
            fclose(add);
            system("neogit add state");
            chdir("all_commits");

            chdir(argv[argc - 1]);

            // FILE* readmsg = fopen("msg.txt", "r");
            // fgets(msg, 100, readmsg);
            chdir("../..");
            // printf("%s", msg);
            strcpy(comm, "neogit commit -m ");
            strcat(comm, msg);
            system(comm);
        }
        else if (strcmp("-n", argv[2]) == 0)
        {
            char comm[1000];
            strcpy(comm, "neogit checkout ");
            if (argv[3] != NULL)
            {
                strcat(comm, argv[argc - 1]);
                system(comm);
                return 0;
            }
            else
            {

                strcat(comm, w_branch);
                system(comm);
                return 0;
            }
        }
        else if (strstr(argv[argc - 1], "HEAD") != 0)
        {
            int idd;
            sscanf(argv[argc - 1], "HEAD-%d", &idd);
            // printf("%d", idd);
            char msg[100];
            int ln = strlen(argv[3]);
            argv[3][ln - 1] = '\0';
            // printf("%d", ln);
            strcpy(msg, argv[3]);
            // printf("%s", msg);
            int comm_count = lastidread();
            char comm[1000];
            sprintf(comm, "neogit revert [-m %s] %d", msg, comm_count - idd);
            system(comm);
        }
    }
    if (strcmp(argv[1], "diffi") == 0)
    {
        return run_diffi(argc, argv);
    }
    if (strcmp(argv[1], "checkout") == 0)
    {
        return run_checkout(argc, argv);
    }
    if (strcmp(argv[1], "tag") == 0)
    {
        return run_tag(argc, argv);
    }
    if (strcmp(argv[1], "reset") == 0)
    {
        if (strcmp(argv[2], "-undo") == 0)
        {
            return run_reset_undo();
        }
        else
        {
            char realpath[MAX_FILEPATH_LENGTH];
            getcwd(realpath, MAX_FILEPATH_LENGTH);
            strcat(realpath, "/");
            strcat(realpath, argv[2]);
            return run_reset(realpath);
        }
    }
    if (strcmp(argv[1], "status") == 0)
    {

        return run_status();
    }
    int tmp = 1;
    char aliaspath[1000];
    strcpy(aliaspath, neogitpath);
    strcat(aliaspath, "/alias.txt");
    FILE *file = fopen(aliaspath, "r");
    if (file == NULL)
    {
        tmp -= 1;
    }
    char nik[MAX_LINE_LENGTH];
    char neo[MAX_LINE_LENGTH];
    // printf("%d", tmp);
if(strcmp(argv[2], "-s") == 0){
    return setmsg();
}
    if (tmp)
    {

        while ((fscanf(file, "%s    ", nik) != EOF))
        {
            int i = 0;
            fgets(neo, MAX_LINE_LENGTH, file);
            if (strcmp(argv[1], nik) == 0)
            {
                char command_build[MAX_LINE_LENGTH];
                strcpy(command_build, "neogit ");

                int len = strlen(neo);
                if (len > 0 && neo[len - 1] == '\n')
                {
                    neo[len - 1] = '\0';
                }
                strcat(command_build, neo);
                while (argv[i] != NULL)
                {
                    strcat(command_build, " ");
                    strcat(command_build, argv[i]);
                    i++;
                }
                system(command_build);
                if (system)
                    return 1;
                else
                    return 0;
            }
        }
    }

    file = fopen("/alias.txt", "r");
    if (file == NULL)
    {
        return 1;
    }
    while ((fscanf(file, "%s    ", nik) != EOF))
    {
        int i = 2;
        fgets(neo, MAX_LINE_LENGTH, file);
        if (strcmp(argv[1], nik) == 0)
        {
            char command_build[MAX_LINE_LENGTH];
            strcpy(command_build, "neogit ");
            int len = strlen(neo);
            if (len > 0 && neo[len - 1] == '\n')
            {
                neo[len - 1] = '\0';
            }
            strcat(command_build, neo);

            while (argv[i] != NULL)
            {
                strcat(command_build, " ");
                strcat(command_build, argv[i]);
                i++;
            }
            // printf("%s \n", command_build);
            system(command_build);
            if (system)
                return 1;
            else
                return 0;
        }
    }
    return 1;
}