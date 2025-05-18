#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include "customs.h"
#include "monitor_state.h"
#include <pthread.h>

#define CMD_FILE ".monitor_cmd"

#define PIPE_CHUNK 10


int pipefd[2]; // pipefd[0] = read end, pipefd[1] = write end
pthread_t monitor_reader_thread;

typedef struct {
    char hunt_id[DIRNAMESIZE];
    int read_fd;
    pid_t child_pid;
} ScorePipe;

typedef struct {
    const char *name;
    void (*handler)(const char *cmd);
    bool requires_monitor;
} command_t;

void print_prompt() {
    const char *prompt = "> ";
    safe_print(prompt);
}

void write_command(const char *cmd) {
    int fd = open(CMD_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        abandonCSTM();
    }
    if (write(fd, cmd, strlen(cmd)) == -1 || write(fd, "\n", 1) == -1) {
        close(fd);
        abandonCSTM();
    }
    if (close(fd) == -1) {
        safe_print("Failed to close command file.\n");
    }
}

void *monitor_output_reader(void *arg) {
    char buffer[1024];
    ssize_t n;

    while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        safe_print(buffer); 
    }

    return NULL;
}

void handle_help(const char *cmd){
    const char *help_message = 
        "Available commands:\n"
        "  start_monitor: Start the monitor process\n"
        "  list_hunts: List all hunts and their treasures\n"
        "  list_treasures <hunt_id>: List all treasures in a specific hunt\n"
        "  view_treasure <hunt_id> <treasure_id>: View details of a specific treasure\n"
        "  calculate_score: Calculate scores for all hunts\n"
        "  stop_monitor: Stop the monitor process\n"
        "  exit: Exit the program\n";
    safe_print(help_message);
}

void handle_start_monitor(const char *cmd){
    if (monitor_ex->state == RUNNING){
        safe_print("Monitor is already running.\n");
        return;
    }

    if (pipe(pipefd) == -1){
        abandonCSTM();
    }

    monitor_ex->pid = fork();
    if (monitor_ex->pid == -1){
        abandonCSTM();
    }
    else if (monitor_ex->pid == 0){
        // In child (monitor)
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        dup2(pipefd[1], STDERR_FILENO); // Optional: redirect stderr too
        close(pipefd[1]); // Done with original fd

        char *args[] = {"./monitor", NULL};
        execv("./monitor", args);
        //if we got here, execv failed
        abandonCSTM(); 
    }
    else {
        // In parent (hub)
        close(pipefd[1]); // Close write end (only monitor uses it)

        pthread_create(&monitor_reader_thread, NULL, monitor_output_reader, NULL);

        monitor_ex->state = RUNNING;
        safe_print("Monitor started successfully.\n");
    }
}

void handle_monitor_command(const char *cmd){
    write_command(cmd);
    if (kill(monitor_ex->pid, SIGUSR1) == -1){
        abandonCSTM();
    }
}

void handle_stop_monitor(const char *cmd){
    monitor_ex->state = SHUTTING_DOWN;
    if (kill(monitor_ex->pid, SIGTERM) == -1){
        safe_print("Failed to send SIGTERM to monitor.\n");
        if (kill(monitor_ex->pid, SIGKILL) == -1){
            abandonCSTM();
        }
    }
    close(pipefd[0]); // Triggers EOF in the reader thread
    // Wait for pipe to close and reader to exit
    pthread_detach(monitor_reader_thread);
}

void handle_calculate_score(const char *cmd) {
    DIR *d = opendir(".");
    if (!d) {
        safe_print("Failed to open current directory.\n");
        return;
    }

    struct dirent *entry;
    ScorePipe *pipes = NULL;
    int count = 0;
    int capacity = 0;

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type != DT_DIR) continue;
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".git") == 0) continue;

        // Check if the .dat file exists
        char dat_path[PATH_MAX];
        snprintf(dat_path, sizeof(dat_path), "%s/%s.dat", entry->d_name, entry->d_name);
        if (access(dat_path, F_OK) != 0) continue;

        // Grow array if needed
        if (count == capacity) {
            capacity += PIPE_CHUNK;
            pipes = realloc(pipes, capacity * sizeof(ScorePipe));
            if (!pipes) {
                safe_print("Memory allocation failed.\n");
                closedir(d);
                return;
            }
        }

        int pipefd[2];
        if (pipe(pipefd) == -1) {
            safe_print("Failed to create pipe.\n");
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            safe_print("Fork failed.\n");
            close(pipefd[0]);
            close(pipefd[1]);
            continue;
        } else if (pid == 0) {
            // Child
            close(pipefd[0]); // close read end
            dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
            close(pipefd[1]);

            execlp("./score_calculator", "score_calculator", entry->d_name, NULL);
            // If exec fails
            abandonCSTM();
        } else {
            // Parent
            close(pipefd[1]); // close write end

            strncpy(pipes[count].hunt_id, entry->d_name, DIRNAMESIZE - 1);
            pipes[count].hunt_id[DIRNAMESIZE - 1] = '\0';
            pipes[count].read_fd = pipefd[0];
            pipes[count].child_pid = pid;
            count++;
        }
    }

    closedir(d);

    // Read and print output from each in order
    for (int i = 0; i < count; ++i) {
        waitpid(pipes[i].child_pid, NULL, 0);

        char buffer[1024];
        ssize_t n;
        while ((n = read(pipes[i].read_fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, n);
        }

        close(pipes[i].read_fd);
    }

    free(pipes);
}

void handle_exit(const char *cmd){
    exit(EXIT_SUCCESS);
}

void handle_sigchld(int sig){
    int status;
    pid_t pid = wait(&status);
    if (pid == -1){
        ; //ignoring cause there's lots of children
    }
    else if (pid == monitor_ex->pid){
        safe_print("Monitor process has exited.\n");
        monitor_ex->state = OFFLINE;
        monitor_ex->pid = 0;
    }
}

command_t commands[] = {
    {"help", handle_help, false},
    {"start_monitor", handle_start_monitor, false},
    {"list_hunts", handle_monitor_command, true},
    {"list_treasures", handle_monitor_command, true},
    {"view_treasure", handle_monitor_command, true},
    {"calculate_score", handle_calculate_score, false},
    {"stop_monitor", handle_stop_monitor, true},
    {"exit", handle_exit, false},
    {NULL, NULL, false} // Sentinel to mark end of array
};

int main(){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGUSR1,&sa,NULL) == -1){
        safe_print(strerror(errno));
    }

    sa.sa_handler = handle_sigchld;
    if (sigaction(SIGCHLD,&sa,NULL) == -1){
        safe_print(strerror(errno));
    }

    char input[256];
    while (1) {
        print_prompt();
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read == -1){
            if (errno == EINTR) {
                continue; // Interrupted by signal, retry read
            }
            else{
                abandonCSTM();
            }
        }
        if (bytes_read == 0) break;

        if (monitor_ex->state == SHUTTING_DOWN) {
            safe_print("Monitor is shutting down. Please wait...\n");
            continue;
        }

        input[bytes_read] = '\0';
        input[strcspn(input, "\n")] = 0;
        trim_string(input);
        if (strlen(input) == 0) continue;

        char command[256];
        char *iterator = command;
        strncpy(command, input, sizeof(command) - 1);
        while (!isspace(*iterator) && *iterator != '\0') {
            iterator++;
        }
        *iterator = '\0'; // Null-terminate the command string

        if (strcmp(input,"help") == 0){
            handle_help(input);
            continue;
        }

        command_t *cmd = commands;
        bool command_found = false;
        
        while (cmd->name != NULL) {
            if (strncmp(command, cmd->name, strlen(cmd->name)) == 0) {
                command_found = true;

                if (cmd->requires_monitor && monitor_ex->state == OFFLINE) {
                    const char *msg = "Monitor is not running.\n";
                    safe_print(msg);
                } 
                else if (strcmp(cmd->name, "exit") == 0 && monitor_ex->state == RUNNING) {
                    const char *msg = "Monitor still running. Stop it before exiting.\n";
                    safe_print(msg);
                }
                else {
                    cmd->handler(input);
                }
            }
            cmd++;
        }

        if (!command_found) {
            const char *msg = "Unknown command. Type 'help' for more information\n";
            safe_print(msg);
        }
    }

    return 0;
}