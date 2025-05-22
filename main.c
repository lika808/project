#include <stdio.h>
#include "filesystem.h"

int main() {
    init_filesystem("disk.myfs");

    int choice;
    char filename[50];
    char data[256];

    while (1) {
        printf("\n1. Create file\n2. Write to file\n3. Read file\n4. Delete file\n5. List files\n6. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("Enter filename: ");
                scanf("%s", filename);
                create_file(filename);
                break;
            case 2:
                printf("Enter filename: ");
                scanf("%s", filename);
                printf("Enter data: ");
                scanf(" %[^\n]", data);
                write_file(filename, data);
                break;
            case 3:
                printf("Enter filename: ");
                scanf("%s", filename);
                read_file(filename);
                break;
            case 4:
                printf("Enter filename: ");
                scanf("%s", filename);
                delete_file(filename);
                break;
            case 5:
                list_files();
                break;
            case 6:
                close_filesystem();
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
}
