#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOOLS 100  

typedef struct{
    int record;
    char name[50];
    int quantity;
    float cost;
}Tool;

typedef struct{
    char tanggal[12];
    char pembeli[30];
    char jenis_barang[20];
    char  barang[25];
    float cost;
    int jumlah;
    float total;
    float totalPembayaran;
    char pembatas[25];
}Beli;

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
    FILE *fp = fopen("tool.txt", "ab"); //write dengan biner
    fwrite(&tool,sizeof(Tool), 1, fp);
    fclose(fp);
}

void listTools(){
    Tool tool;
    FILE *fp = fopen("tool.txt", "rb");//read dengan biner
    if(fp==NULL){
        printf("file tool.txt tidak bisa dibuat\n");
        exit(1);
    }
    while (fread(&tool, sizeof(Tool), 1, fp)) {
        if(tool.record!=0){
        printf("Record # %d\n", tool.record);
        printf("Name: %s\n", tool.name);
        printf("Quantity: %d\n", tool.quantity);
        printf("Cost: %f\n\n", tool.cost);
        }
        printf("\n");
    }
    fclose(fp);
}

void deleteTool() {
    int record;
    printf("Enter tool record to delete: ");
    scanf("%d", &record);
    getchar();

    Tool tool;
    FILE *fp = fopen("tool.txt", "rb+");
    if(fp==NULL){
        printf("file tool.txt tidak bisa dibuat\n");
        exit(1);
    }
    while (fread(&tool, sizeof(Tool), 1, fp)) {
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
    FILE *fp = fopen("tool.txt", "rb+");
    if(fp==NULL){
        printf("file tool.txt tidak bisa dibuat\n");
        exit(1);
    }
    while (fread(&tool, sizeof(Tool), 1, fp)) {
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

void pilihtool() {
    int log=0;
    int choice;
    printf("\nMENU LIST TOOL");
    while(1){
        printf("\n1. Input tool\n");
        printf("2. List all tools\n");
        printf("3. Delete tool\n");
        printf("4. Update tool\n");
        printf("5. Back\n");
        printf("Pilihan: ");
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
                main();
            default:
                printf("Salah memasukan angka\n");
        }
    }
}

void transaksi(){
    Tool tool;
    Beli beli;
    char pembatas[]="----------------------";
    // char Nama_Pembeli[50];
    char barang[50];
    // char Jenis_Barang[20];
    // int jumlah;
    // float totalHarga;
    float totalPembayaran =0;
    char pilih;
    printf("\nTRANSAKSI\n");
    char tanggal[] = __DATE__; getchar();
    strcpy(beli.tanggal, tanggal);
    printf("Nama Pembeli : ");
    scanf("%[^\n]", beli.pembeli);
    getchar();
    do{
        int cek=0; 
        FILE *fp=fopen("tool.txt","rb+");
        do{
            printf("Jenis Barang : ");
            scanf("%s", &beli.jenis_barang);
            getchar();
            printf("Barang : ");
            scanf("%[^\n]", &barang);
            getchar();
            while (fread(&tool, sizeof(Tool), 1, fp)) {//mengulangi terus menerus
                if (strcmp(tool.name, barang)==0) {//kenapa ==0, karena jika tool.name dicompare dengan nis maka hasilnya akn 1 jika berbeda, maka dari itu jika hasilnya 0 maka baru menjalankan program dibawah
                    strcpy(beli.barang,barang);
                    printf("Harga : %.3f\n", tool.cost);
                    beli.cost = tool.cost;
                    printf("Jumlah : ");
                    scanf("%d", &beli.jumlah); getchar();
                    beli.total = beli.cost * beli.jumlah;
                    totalPembayaran += beli.total; 
                    beli.totalPembayaran = 0;
                    printf("Total : %.3f", beli.total);
                    tool.quantity -= beli.jumlah;
                    fseek(fp, -(long long)sizeof(Tool), SEEK_CUR);
                    fwrite(&tool,sizeof(Tool),1,fp);
                    cek=1;
                    fclose(fp);
                    }
                } 
            if(cek==0){
                printf("Nama Barang tidak ada di database\n");
            }
        }while(cek==0);
        printf("\nApakah ada lagi[y/n]? ");
        scanf("%c",&pilih); 
        if(pilih=='y'){
            FILE *pp=fopen("history.txt","ab");
            fwrite(&beli, sizeof(Beli), 1, pp);
            fclose(pp);
        }
        getchar();
    }while(pilih != 'n');
    strcpy(beli.pembatas,pembatas);
    beli.totalPembayaran=totalPembayaran;
    FILE *pp=fopen("history.txt","ab");
    fwrite(&beli, sizeof(Beli), 1, pp);
    fclose(pp);
    printf("Total pembayaran : %.3f", beli.totalPembayaran);
}

void history(){
    Beli beli;
    FILE *fp = fopen("history.txt", "rb");
    if(fp==NULL){
        printf("file history.txt tidak bisa dibuat\n");
        exit(1);
    }
    while (fread(&beli, sizeof(Beli), 1, fp)) {
        printf("Tanggal : %s\n", beli.tanggal);
        printf("Name    : %s\n", beli.pembeli);
        printf("Jenis   : %s\n", beli.jenis_barang);
        printf("Barang  : %s\n", beli.barang);
        printf("Harga   : %.3f\n",beli.cost);
        printf("Jumlah  : %d\n", beli.jumlah);
        printf("Total   : %.3f\n", beli.total);
        if(beli.totalPembayaran != 0){
            printf("Total Pembayaran    : %.3f\n", beli.totalPembayaran);
        }
        printf("%s\n", beli.pembatas);
    }
    fclose(fp);
}

void pilih_transaksi()
{
    int choice;
    printf("\nMENU TRANSAKSI");
    while(1){
        printf("\n1.  Transaksi Baru\n");
        printf("2.  History Transaksi\n");
        printf("3.  Kembali\n");
        printf("Pilihan : ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                transaksi();
                break;
            case 2:
                history();
                break;
            case 3:
                main();
                break;
            default:
                printf("salah memasukan angka\n");
        }
    }
}

int main(){
    int choice;
    printf("\nPT. Jaya Jaya Jaya\n");
    while(1){
        printf("1.  Transaksi\n");
        printf("2.  List Tool\n");
        printf("3.  Exit\n");
        printf("Pilihan : ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                pilih_transaksi();
                break;
            case 2:
                pilihtool();
                break;
            case 3:
                exit(0);
                break;
            default:
                printf("salah masukan angka!!!\n");
        }
    }
}