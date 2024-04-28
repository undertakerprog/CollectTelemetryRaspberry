#include "send_data.h"

void send_file_to_computer() {
    const char* username = "undertaken";
    const char* hostname = "172.20.10.3";
    const char* local_file_path = "/home/undertaker/project/build/debug/sensor_data.json";
    const char* remote_directory_path = "/home/undertaken/C++/project/";
    const char* password = "9234";

    char command[256];
    snprintf(command, sizeof(command), "sshpass -p '%s' scp %s %s@%s:%s", password, local_file_path, username, hostname, remote_directory_path);

    int status = system(command);

    if (status == -1) {
        fprintf(stderr, "Error executing scp command\n");
    } else if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        if (exit_status != 0) {
            fprintf(stderr, "scp command failed with exit status %d\n", exit_status);
        } else {
            printf("File sent successfully\n");
        }
    } else {
        fprintf(stderr, "scp command terminated abnormally\n");
    }
}
