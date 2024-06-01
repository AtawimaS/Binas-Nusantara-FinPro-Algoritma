#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOOLS 100  

typedef struct {
    int record;
    char name[50];
    int quantity;
    float cost;
} Tool;

void inputTool() {
    Tool tool;
    printf("Enter record: ");
    scanf("%d", &tool.record);
    getchar();
    printf("Enter tool name: ");
    scanf("%[^\n]", tool.name);
    getchar();
    printf("Enter quantity: ");
    scanf("%d", &tool.quantity);
    getchar();
    printf("Enter cost: ");
    scanf("%f", &tool.cost);
    getchar();
    FILE *fp = fopen("hardware.txt", "ab"); //write dengan biner
    fwrite(&tool,sizeof(Tool), 1, fp);
    fclose(fp);
}

void listTools(){
    Tool tool;
    FILE *fp = fopen("hardware.txt", "rb");//read dengan biner
    if(fp==NULL){
        printf("file hardware.txt tidak bisa dibuat\n");
        exit(1);
    }
    while (fread(&tool, sizeof(Tool), 1, fp)) {
        if(tool.record!=0){
        printf("Record # %d\n", tool.record);
        printf("Name: %s\n", tool.name);
        printf("Quantity: %d\n", tool.quantity);
        printf("Cost: %f\n\n", tool.cost);
        }
    }
    fclose(fp);
}
void deleteTool() {
    int record;
    printf("Enter tool record to delete: ");
    scanf("%d", &record);
    getchar();

    Tool tool;
    FILE *fp = fopen("hardware.txt", "rb+");
    if(fp==NULL){
        printf("file hardware.txt tidak bisa dibuat\n");
        exit(1);
    }
    while (fread(&tool, sizeof(Tool), 1, fp) == 1) {
        if (tool.record == record) {
            tool.record = 0;
            strcpy(tool.name, "");
            tool.quantity = 0;
            tool.cost = 0.0;
            fseek(fp, -(long long)sizeof(Tool), SEEK_CUR);
            fwrite(&tool, sizeof(Tool), 1, fp);
            printf("Tool deleted\n");
            break;
        }
    }
    fclose(fp);
}

void updateTool() {
    int record;
    printf("Enter tool ID to update: ");
    scanf("%d", &record);
    getchar();

    Tool tool;
    FILE *fp = fopen("hardware.txt", "rb+");
    if(fp==NULL){
        printf("file hardware.txt tidak bisa dibuat\n");
        exit(1);
    }
    while (fread(&tool, sizeof(Tool), 1, fp) == 1) {
        if (tool.record == record) {
            printf("Enter new name: ");
            scanf("%[^\n]", tool.name);
            getchar();
            printf("Enter new quantity: ");
            scanf("%d", &tool.quantity);
            getchar();
            printf("Enter new cost: ");
            scanf("%f", &tool.cost);
            getchar();
            fseek(fp, -(long long)sizeof(Tool), SEEK_CUR);
            fwrite(&tool, sizeof(Tool), 1, fp);
            printf("Tool updated\n");
            break;
        }
    }
    fclose(fp);
}
int main() {
    int choice;
    while (1) {
        printf("1. Input tool\n");
        printf("2. List all tools\n");
        printf("3. Delete tool\n");
        printf("4. Update tool\n");
        printf("5. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
		printf("\n");
        switch (choice) {
            case 1:
                inputTool();
                break;
            case 2:
                listTools();
                break;
            case 3:
                deleteTool();
                break;
            case 4:
                updateTool();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid\n");
        }
    }
    return 0;
}
