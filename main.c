#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <alg.h>
#include <time.h>


#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_ACCOUNTS 100
#define MAX_LEN 2048
#define MAX_ITEMS 100
#define rnd rand()%100

bool Loggedin = false;

typedef struct AC {

    int userId;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char role[20];
    int isActive;
    int Money;

} Account;

int EXPTable[20] = {0,300,700,1000,1100,1200,1300,1400,1500,1600,1700,
                    1800,1900,2000,2100,2200,2300};

typedef struct Monsters{
    int EntryId,
    Level,
    Diff,
    Health,
    Min_DMG,
    Max_DMG,
    Lucky,
    Armor,
    Money_Drop;
    char Name[100];


}MonsterInfo;

typedef struct CU {

    int userId;
    char username[MAX_USERNAME_LENGTH];
    char role[20];
    int Money;
    int Min_dmg;
    int Max_dmg;
    int Health;
    int Luck;
    int Level;
    int GMode;
    int CurExp;
    int Armor;


} Current_User_Information;

bool NewSession = true;
bool upLevel = false;

typedef struct Shop{
    char ItemName[MAX_USERNAME_LENGTH];
    int Price;
    int ItemId;

} ShopItems;

typedef enum LogStatus{
    InvalidAccount,
    LoggedIn,
    BannedAccount
}Loginstatus;

int rf2(int low,int high){
    return  (rand()%(high-low+1))+low;
}
void ListAllMonsters(){

    MonsterInfo Monsters[1000];
    FILE *Monsters_Data = fopen("monsters.txt","r");
    if(Monsters_Data==NULL){
        printf("Error opening file.\n");
        return;
    }

    int counter=0;
    while (fscanf(Monsters_Data, "%*s %*s %d %*s %s %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s",
                  &Monsters[counter].EntryId,Monsters[counter].Name,&Monsters[counter].Level,
                  &Monsters[counter].Diff, &Monsters[counter].Health,
                  &Monsters[counter].Min_DMG,&Monsters[counter].Max_DMG ,
                  &Monsters[counter].Lucky,&Monsters[counter].Armor,
                  &Monsters[counter].Money_Drop)== 10) {

        counter++;
    }

    for (int i = 0; i < counter; ++i) {
        printf("EntryId: %d\nName: %s\nLevel: %d\nDiff: %d\nHealth: %d\nMin_DMG: %d\nMax_DMG: %d\nlucky: %d\nArmor: %d\nMoney_Drop: %d\n\n",
               Monsters[i].EntryId,
               Monsters[i].Name,
               Monsters[i].Level,
               Monsters[i].Diff,
               Monsters[i].Health,
               Monsters[i].Min_DMG,
               Monsters[i].Max_DMG,
               Monsters[i].Lucky,
               Monsters[i].Armor,
               Monsters[i].Money_Drop);
    }

    fclose(Monsters_Data);
}



int GetMonsters(MonsterInfo *Monster_List){
    int count=0;
    MonsterInfo Monsters_List[20];
    FILE *Monsters_Data = fopen("monsters.txt","r");
    if(Monsters_Data==NULL){
        printf("Error opening file.\n");
        return 0;
    }


    while (fscanf(Monsters_Data, "%*s %*s %d %*s %s %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s",
                  &Monster_List[count].EntryId,Monster_List[count].Name,&Monster_List[count].Level,
                  &Monster_List[count].Diff, &Monster_List[count].Health,
                  &Monster_List[count].Min_DMG,&Monster_List[count].Max_DMG ,
                  &Monster_List[count].Lucky,&Monster_List[count].Armor,
                  &Monster_List[count].Money_Drop)== 10) {
        count++;
    }


    fclose(Monsters_Data);

    return count;
}

void Register_New_Monster(MonsterInfo *NewEntry){

    FILE *Monster_Data = fopen("monsters.txt","a");
    if(Monster_Data==NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    fprintf(Monster_Data, "\n#Moster\nEntryId: %d\nName: %s\nLevel: %d\nDiff: %d\nHealth: %d\nMin_dmg: %d\nMax_dmg: %d\nLuck: %d\nArmor: %d\nMoney_drop: %d\n#EndMoster",
            NewEntry->EntryId,NewEntry->Name,NewEntry->Level,NewEntry->Diff,NewEntry->Health,NewEntry->Min_DMG,NewEntry->Max_DMG,NewEntry->Lucky,NewEntry->Armor,NewEntry->Money_Drop);
    fclose(Monster_Data);

    return;
}

void ListAllUsers(){
    Account acc[MAX_ACCOUNTS];
    FILE *accounts_data = fopen("accounts.txt","r");
    if(accounts_data==NULL){
        printf("Error opening file.\n");
        return;
    }

    int counter=0;
    while (fscanf(accounts_data, "%d %s %s %s %d %*d",
                  &acc[counter].userId, acc[counter].username,
                  acc[counter].password, acc[counter].role,
                  &acc[counter].isActive) == 5) {

                counter++;
    }

    for (int i = 0; i < counter; ++i) {
        printf("Id: %d\nUsername:%s\nPassword:%s\nRole:%s\nActive:%d\n\n",
               acc[i].userId,
               acc[i].username,
               acc[i].password,
               acc[i].role,
               acc[i].isActive);
    }

    fclose(accounts_data);
}

int getLastUserId() {
    FILE *accounts_data = fopen("accounts.txt", "r");
    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    int lastUserId = 0;
    Account account;

    while (fscanf(accounts_data, "%d %*s %*s %*s %*d %*d", &account.userId) == 1) {
        if (account.userId > lastUserId)
            lastUserId = account.userId;
    }

    fclose(accounts_data);

    return lastUserId;
}
int getLastMonsterId() {
    FILE *monster_data = fopen("monsters.txt", "r");
    if (monster_data == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    int lastMonsterId = 0;
    MonsterInfo New_Creature;

    while (fscanf(monster_data, "%*s %*s %d %*s %*s %*s %*d %*s %*d %*s %*d %*s %*d %*s %*d %*s %*d %*s %*d %*s %*d %*s", &New_Creature.EntryId) == 1) {

        if (New_Creature.EntryId > lastMonsterId)
            lastMonsterId = New_Creature.EntryId;
    }

    fclose(monster_data);

    return lastMonsterId;
}

bool check_exist(char* username){
    FILE* accounts_data = fopen("accounts.txt", "r");
    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    char storedUsername[MAX_USERNAME_LENGTH];
    while (fscanf(accounts_data, "%s", storedUsername) == 1) {
        if (strcmp(storedUsername, username) == 0) {
            fclose(accounts_data);
            return true;
        }
    }

    fclose(accounts_data);
    return false;
}

void registerAccount(Account account) {
    FILE *accounts_data = fopen("accounts.txt", "a");
    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(accounts_data, "%d %s %s %s %d %d\n", account.userId, account.username, account.password, account.role, account.isActive,50);
    fclose(accounts_data);

    printf("\033[1;32mRegistration successful.\033[1;0m\n\n");
    return;
}


Loginstatus loginAccount(char* username, char* password, Current_User_Information* currentUser) {
    FILE *accounts_data = fopen("accounts.txt", "r");
    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return InvalidAccount;
    }

    Account account;


    while (fscanf(accounts_data, "%d %s %s %s %d %d",
                  &account.userId, account.username,
                  account.password, account.role,
                  &account.isActive,&currentUser->Money) == 6) {

        if (strcmp(account.username, username) == 0 &&
        strcmp(account.password, password) == 0&&
        account.isActive==1) {
            fclose(accounts_data);
            currentUser->userId = account.userId;
            strcpy(currentUser->username, account.username);
            strcpy(currentUser->role, account.role);
            return LoggedIn;
        }
        else if(account.isActive==0
        && (strcmp(account.username,username)==0))
        {
            return BannedAccount;
        }
        account.isActive=50;
    }

    fclose(accounts_data);
    return InvalidAccount;
}
void SetNewCharacterDefaultValues(int Id){

    FILE *new_chr_data = fopen("characters.txt","a");
    if(new_chr_data==NULL)
    {
        printf("Error opening file.\n");
        return;
    }


    int min=5,max=10,Level=1,health=80,Luck=5,exp=0,armor=1;
    fprintf(new_chr_data, "\n#Chr\nId: %d\nLevel: %d\nEXP: %d\nMin_dmg: %d\nMax_dmg: %d\nHealth: %d\nLuck: %d\nArmor: %d\n#EndChr",Id,Level,exp,min,max,health,Luck,armor);
    fclose(new_chr_data);

    return;
}
void SetCharacterInfo(Current_User_Information *currentUser) {
    FILE *chr_data = fopen("characters.txt", "r");
    if (chr_data == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int id, min, max, health, luck,level,exp,armor;
    while (fscanf(chr_data, "%*s %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s",
                  &id, &level,
                  &exp,
                  &min,&max,
                  &health,&luck,&armor) == 8) {

        if (id == currentUser->userId) {
            fclose(chr_data);
            currentUser->Level = level;
            currentUser->CurExp=exp;
            currentUser->Min_dmg = min;
            currentUser->Max_dmg = max;
            currentUser->Health = health;
            currentUser->Luck = luck;
            if(NewSession)currentUser->GMode=0;
            else currentUser->GMode=currentUser->GMode;
            currentUser->Armor=armor;
        }
    }

        fclose(chr_data);
        return;


}

void BanAccountById(int Id,int ban_unban,int CurrentId){
    int banned = 0;
    char BannedName[MAX_USERNAME_LENGTH];
    FILE *accounts_data = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt","w");
    char Lines[MAX_LEN];
    Account account;
    int TempLastId=0,LastId=getLastUserId();


    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return;
    }
    if(!temp){
        printf("Error opening file.\n");
        fclose(accounts_data);
        return;
    }


    while (fscanf(accounts_data, "%d %s %s %s %d %d",
                  &account.userId, account.username,
                  account.password, account.role,
                  &account.isActive,&account.Money) == 6) {
        TempLastId++;
        if (account.userId == Id) {
            if(ban_unban==1)
                account.isActive=0,banned=1;
            else if(ban_unban==2)
                account.isActive=1,banned=2;



            strcpy(BannedName,account.username);


        }

        fprintf(temp, "%d %s %s %s %d %d\n", account.userId, account.username, account.password, account.role, account.isActive,account.Money);

        if(TempLastId==LastId)
            break;

    }





    fclose(accounts_data);
    fclose(temp);
    if(banned==1) {
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
        printf("\nPlayer: %s With Id: %d\nHas Been Banned Successfully.\n\n1. BackToMain\n",BannedName,Id);
        int back=0;
        do{
            if(CurrentId==Id)
                printf("WOW YOU BANNED YOURSELF...EXITINGNOW!\n");
            printf("Enter:");
            scanf("%d",&back);
            if(back==1) {
                system("cls");
                if(CurrentId==Id) exit(0);
                break;
            }
            if(CurrentId==Id) exit(0);
        }while(1);
    }
    else if(banned==2){
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");
        printf("\nPlayer: %s With Id: %d\nHas Been UnBanned Successfully.\n\n1. BackToMain\n",BannedName,Id);
        int back=0;
        do{
            printf("Enter:");
            scanf("%d",&back);
            if(back==1) {
                system("cls");
                break;
            }
        }while(1);
    }
    else {
        printf("\nUserId is incorrect or there is an error occurred\n"
               "Please try again later.\n\n");
        remove("temp.txt");
        return;
    }


}
void BanAccountByName(char *Name,int ban_unban){
    printf("\033[1;31mThis Function is Under maintenance and we are working to fix it as soon as possible\nSorry For That :(\n\n\n\033[1;36m1. Back\n\nEnter:");
    int back=0;
    do{
        scanf("%d",&back);
        if(back==1) {
            system("cls");
            break;
        }
    }while(1);

    return;
}
void EnableDisableGodMod(Current_User_Information *currentuser){
    if(currentuser->GMode == 1)
        currentuser->GMode=0;
    else
        currentuser->GMode=1;
}

int StoreShopItems(ShopItems *Items){
    int Items_Count=0;
    FILE* items_data = fopen("ItemsList.txt","r");
    if(items_data==NULL)
    {
        printf("Error opening the file.");
        return -1;
    }

    int itemid,price;
    char name[MAX_USERNAME_LENGTH];
    while(fscanf(items_data,"%*s %*s %d %*s %s %*s %d %*s",
                  &itemid,
                  name,
                  &price)==3){
        strcpy(Items[Items_Count].ItemName,name);
        Items[Items_Count].ItemId=itemid;
        Items[Items_Count].Price=price;
        Items_Count++;
    }

    fclose(items_data);
    return Items_Count;
}
void UpdateUserInfo(Current_User_Information *curentuser,int Status,int amount,int lvl,int ex){

    FILE *chr_data = fopen("characters.txt", "r");
    int TempLastId=0,LastId=getLastUserId();
    if (chr_data == NULL) {
        printf("Error opening file.\n");
        return;
    }
    FILE *temp = fopen("temp1.txt","w");
    if(!temp){
        printf("Error opening file.\n");
        fclose(chr_data);
        return;
    }
    int id;
    while (fscanf(chr_data, "%*s %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s",
                  &id, &curentuser->Level,
                  &curentuser->CurExp,
                  &curentuser->Min_dmg,&curentuser->Max_dmg,
                  &curentuser->Health,&curentuser->Luck,&curentuser->Armor) == 8) {

        TempLastId++;
        if(id==curentuser->userId)
        {
            float NEW_VALUE;
            int last;
            curentuser->Level=lvl;
            curentuser->CurExp+=ex;
            if(curentuser->CurExp >= EXPTable[curentuser->Level]) {
                curentuser->Level++;
                curentuser->CurExp=0;
                upLevel=true;
            }
            switch (Status) {
                case 0:
                    break;
                case 1://MINDMG
                if(upLevel)
                    curentuser->Min_dmg+=amount;
                else
                   curentuser->Min_dmg+=5*amount;
                    break;
                case 2://MAXDMG
                    if(upLevel)
                        curentuser->Max_dmg+=amount;
                    else
                        curentuser->Max_dmg+=7*amount;
                    break;
                case 3://HEALTH
                if(upLevel)
                    curentuser->Health+=amount;
                else
                    curentuser->Health+=10*amount;
                    break;
                case 4://LUCK
                if(upLevel)
                    curentuser->Luck+=amount;
                else
                    curentuser->Luck+=3*amount;
                    break;
                case 5://ARMOR
                if(upLevel)
                    curentuser->Armor+=amount;
                else
                    curentuser->Armor+=2*amount;
                    break;
                default://ERROR = MINDMG
                    NEW_VALUE = (float)curentuser->Min_dmg * (float)pow(1.02, amount);
                    last = NEW_VALUE;
                    curentuser->Min_dmg=last;
                    break;
            }
        }
        fprintf(temp , "#Chr\nId: %d\nLevel: %d\nEXP: %d\nMin_dmg: %d\nMax_dmg: %d\nHealth: %d\nLuck: %d\nArmor: %d\n#EndChr\n",
                id,
                curentuser->Level,
                curentuser->CurExp,
                curentuser->Min_dmg,
                curentuser->Max_dmg,
                curentuser->Health,
                curentuser->Luck,
                curentuser->Armor);

        if(TempLastId==LastId)
            break;
    }
    fclose(chr_data);
    fclose(temp);
    remove("characters.txt");
    rename("temp1.txt","characters.txt");
    return;

}
int BuyItem(Current_User_Information *currentUser,int price,int item_id,int amount){
    int result=0;

    FILE *accounts_data = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt","w");
    Account account;
    int TempLastId=0,LastId=getLastUserId();


    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return 0;
    }
    if(!temp){
        printf("Error opening file.\n");
        fclose(accounts_data);
        return 0;
    }

    Current_User_Information InformationsUser;
    InformationsUser.userId=currentUser->userId;
    InformationsUser.Level=currentUser->Level;
    InformationsUser.CurExp=currentUser->CurExp;
    InformationsUser.Min_dmg=currentUser->Min_dmg;
    InformationsUser.Max_dmg=currentUser->Max_dmg;
    InformationsUser.Luck=currentUser->Luck;
    InformationsUser.Health=currentUser->Health;
    InformationsUser.Armor=currentUser->Armor;
    while (fscanf(accounts_data, "%d %s %s %s %d %d",
                  &account.userId, account.username,
                  account.password, account.role,
                  &account.isActive,&account.Money) == 6) {
        TempLastId++;
        if (account.userId == currentUser->userId) {
            account.Money-=price;
            currentUser->Money-=price;
            result=1;
            UpdateUserInfo(&InformationsUser,item_id,amount,currentUser->Level,currentUser->CurExp);
        }

        fprintf(temp, "%d %s %s %s %d %d\n", account.userId, account.username, account.password, account.role, account.isActive,account.Money);

        if(TempLastId==LastId)
            break;

    }


    fclose(accounts_data);
    fclose(temp);
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
    return result;
}
void DoShopping(Current_User_Information *currentuser){
    int choice,sub_choice;
    int itemid,amount;
    bool done = false;

        do{
            system("cls");
            printf("\t\t\t\t\033[0m-->\033[1;32mWELCOME TO Shop Portal\033[0m<--\033[1;36m\t\t\t\tCurrentMoney: %d\n\n\n",currentuser->Money);
            ShopItems Shop[MAX_ITEMS];
            int Itemscount = StoreShopItems(Shop);
            printf("\033[1;36mAll Available Items\n\n\n\033[1;33m");
            int Edits[6]={5,7,10,3,2};
            for (int i = 1; i <= Itemscount; i++) {
                printf("%d. ItemName: %s ( %d Points )\tPrice: %d\n\n",i,Shop[i-1].ItemName,Edits[i-1],Shop[i-1].Price);
            }

            printf("\033[1;36m\n1. BuyItem\n2. Back\n\nEnter:");
            scanf("%d",&choice);

            switch (choice) {
                case 1:
                    printf("Enter ItemId:");
                    scanf("%d",&itemid);
                    if(itemid>Itemscount){
                        printf("\nItem NotFound!\n\n(AnyNumber) Back\nEnter:");
                        scanf("%d",&sub_choice);
                        continue;
                    }
                    printf("Enter Item Amount:");
                    scanf("%d",&amount);

                    if((Shop[itemid-1].Price*amount)>(currentuser->Money))
                    {
                        printf("You Don't have enough Money!\n\n(AnyNumber) Back\nEnter:");
                        scanf("%d",&sub_choice);
                    }

                    else
                    {
                        printf("\nYou Sure You Want To Buy \" %s \" ?\nTotal Price: %d\nYour Money Will Be: %d\n\n1. Yes\n2. Cancel\n\nEnter:",
                               Shop[itemid - 1].ItemName,
                               (amount * Shop[itemid - 1].Price),
                               ((currentuser->Money) - (Shop[itemid - 1].Price * amount)));

                        scanf("%d", &sub_choice);

                        if (sub_choice == 1) {

                            int Result = BuyItem(currentuser, (Shop[itemid - 1].Price * amount), itemid,amount);
                            if (Result == 1) {
                                printf("\nYou have Successfully bought the item!\n\n1. BackToShopping\n2. Back To Main Menu\n\nEnter:");
                                SetCharacterInfo(currentuser);
                            }
                            else if (Result == 0)
                                printf("An Unknown Error Has Occurred.\nPlease Report it to the Admin and we will try to fix it as soon as possible.\n1. BackToShopping\n2. Back To Main Menu\nEnter:");
                            scanf("%d", &sub_choice);
                            while (!done) {
                                if (sub_choice == 1)
                                    break;
                                else if (sub_choice == 2)
                                    done = true;
                                else
                                    printf("Invalid Choice.\nPlease Try again.\n");

                            }

                        }
                        else
                            printf("Cancelled\n");
                    }
                    break;
                case 2:
                    done = true;
                    break;
                default:
                    printf("Invalid Choice.\n");
                    break;
            }
        } while (!done);
    system("cls");
    return;
}

void Edit_Money(int Id,int NewValue){
    FILE *accounts_data = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt","w");
    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int Temp=0,Last=getLastUserId();
    Account account;
    int sum;
    while (fscanf(accounts_data, "%d %s %s %s %d %d",&account.userId,account.username,account.password,account.role,&account.isActive,&account.Money)==6) {
        Temp++;
        if(Id==account.userId){
            account.Money+=NewValue;
        }
        fprintf(temp,"%d %s %s %s %d %d\n",account.userId,account.username,account.password,account.role,account.isActive,account.Money);

        if(Temp==Last)
            break;
    }
    fclose(accounts_data);
    fclose(temp);
    remove("accounts.txt");
    rename("temp.txt","accounts.txt");


}

void SwitchRole(int Id,char* NEWROLE){
    FILE *accounts_data = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt","w");
    if (accounts_data == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int Temp=0,Last=getLastUserId();
    Account account;
    int sum;
    while (fscanf(accounts_data, "%d %s %s %s %d %d",&account.userId,account.username,account.password,account.role,&account.isActive,&account.Money)==6) {
        Temp++;
        if(Id==account.userId){
            strcpy(account.role,NEWROLE);
        }
        fprintf(temp,"%d %s %s %s %d %d\n",account.userId,account.username,account.password,account.role,account.isActive,account.Money);

        if(Temp==Last)
            break;
    }
    fclose(accounts_data);
    fclose(temp);
    remove("accounts.txt");
    rename("temp.txt","accounts.txt");

}
bool field=false;
bool fighting=false;

void EnterField(Current_User_Information *currentuser){

    do {
        system("cls");
        printf("\t\t\t\t\033[0m-->\033[1;32mBeginner Field Area\033[0m<--\n\n\n");
        printf("\033[1;36mThere Is Some Monsters Around You!\n\n\033[1;33m");
        MonsterInfo Monsters[100], current_monsters[6];
        int MonsterCount = GetMonsters(Monsters);
        int numberOfMonsters = rf2(1, 4);
        int temp = numberOfMonsters, Num = 0;
        while (temp--) {
            int level;
            if (currentuser->Level <= 2)
                level = rf2(1, currentuser->Level + 2);
            else
                level = rf2(currentuser->Level - 2, currentuser->Level + 2);

            current_monsters[Num] = Monsters[level - 1];
            Num++;
        }
        for (int i = 1; i <= numberOfMonsters; ++i) {
            printf("%d. Name: %s\nLevel: %d\nHealth: %d\n\n",
                   i,
                   current_monsters[i-1].Name,
                   current_monsters[i-1].Level,
                   current_monsters[i-1].Health);
        }

        int choice,sub_choice;
        printf("1. Back To Country\n"
               "2. Attack A Monster\n"
               "3. MoveForward\n\nEnter:");
        scanf("%d",&choice);
        switch (choice) {
            case 1:
                system("cls");
                field=false;
                break;
            case 2:
                system("cls");
                printf("\033[1;0m\t\t\t\t-->\033[1;32mAttacking Monster Portal\033[1;0m<--\033[1;36m\n\n\n");
                for (int i = 1; i <= numberOfMonsters; ++i) {
                    printf("%d. Name: %s\nLevel: %d\nHealth: %d\n\n",
                           i,
                           current_monsters[i-1].Name,
                           current_monsters[i-1].Level,
                           current_monsters[i-1].Health);
                }
                printf("\n\033[1;33mEnter:\n\n");
                scanf("%d",&sub_choice);
                fighting=true;
                int potions=2;
                int Healthinfight = currentuser->Health;
                int Healthinfight4mons=current_monsters[sub_choice-1].Health;
                while(fighting){
                    system("cls");
                    printf("\033[1;0m\t\t\t\t-->\033[1;32mFighting Monster Portal\033[1;0m<--\033[1;36m\n\n\n");
                    printf("Monster Name: %s\nHealth: \033[0;31m",current_monsters[sub_choice-1].Name);
                    //printf("%d\n\n%d\n\n",Monsters[current_monsters[sub_choice-1].Level-1].Health,current_monsters[sub_choice-1].Health);

                    float health_sofar = (float)Monsters[current_monsters[sub_choice-1].Level-1].Health/10.00;
                    int Health=health_sofar;
                    if(Healthinfight4mons==0)
                     Healthinfight4mons=1;
                    for (int i = Health; i <= Healthinfight4mons; i+=Health) {
                        printf("*");
                    }
                    printf("\n\n\033[0;36m",(Health));
                    printf("Player Name: %s\nHealth: \033[0;31m", currentuser->username);
                    float health_sofar_p = (float)currentuser->Health/10.00;
                    int Health_p=health_sofar_p;
                    if(Healthinfight==0)
                        Healthinfight=1;
                    for (int i = Health_p; i <= Healthinfight; i+=Health_p) {
                        printf("*");
                    }
                    printf("\n\n\033[0;36m",(Health));

                    int wttodo;
                    printf("1. Attack\n2. Heal\n\nEnter:");
                    scanf("%d",&wttodo);
                    switch (wttodo) {
                        case 1:

                            printf("Attack?\n\n1. Yes\t2. No\n\nEnter:");
                            int attackchoice;
                            scanf("%d",&attackchoice);
                            if(attackchoice==1)
                            {
                                Healthinfight4mons-=rf2(currentuser->Min_dmg,currentuser->Max_dmg);
                                if(Healthinfight4mons<=0)
                                {
                                    int temp2;
                                    float expgain= (float)((float)current_monsters[sub_choice-1].Level*70.00*(float)current_monsters[sub_choice-1].Diff)
                                                   /(float)currentuser->Level;
                                    int expFinal = expgain;
                                    printf("You Won!\n\nCombat Gains:\nMoney: +%d\nExp: +%d\n\n(AnyNumber) Continue\n",
                                           current_monsters[sub_choice-1].Money_Drop,
                                           expFinal);
                                    Edit_Money(currentuser->userId,(current_monsters[sub_choice-1].Money_Drop));
                                    UpdateUserInfo(currentuser,0,0,currentuser->Level,expFinal);
                                    SetCharacterInfo(currentuser);
                                    scanf("%d",&temp2);
                                    field=false;
                                    fighting=false;
                                }
                                else
                                {
                                    int temp2;
                                    Healthinfight-=rf2(current_monsters[sub_choice-1].Min_DMG,current_monsters[sub_choice-1].Max_DMG);
                                    float expgain= (float)(currentuser->Level)
                                                   /(float)((float )current_monsters[sub_choice-1].Diff*2.00);
                                    int expFinal = expgain;
                                    printf("Monster is Attacking You!\n(AnyNumber) NextRound\n\nEnter:");
                                    if(expFinal<0)
                                        expFinal=0;
                                    if(Healthinfight<=0) {
                                        printf("You Lost!\n\nCombat Penalty:\nMoney: +%d\nExp: -%d\n",
                                               (1*currentuser->Luck),
                                               expFinal);
                                        Edit_Money(currentuser->userId,
                                                   (1*currentuser->Luck));
                                        UpdateUserInfo(currentuser, 0, 0, currentuser->Level, -expFinal);
                                        SetCharacterInfo(currentuser);
                                        scanf("%d", &temp2);
                                        field = false;
                                        fighting=false;
                                    }
                                }
                            }
                            else
                                continue;
                            break;
                        case 2:
                            printf("You Have %d Left Potions\nAre u Sure?\n\n1. Yes\t2. No\n\nEnter:",potions);
                            int healchoice;
                            scanf("%d",&healchoice);
                            if(healchoice==1&&potions)
                            {
                                potions--;
                                Healthinfight+=Health_p;
                                if(Healthinfight>currentuser->Health)
                                    Healthinfight=currentuser->Health;
                            }
                            else
                            {
                                printf("Attack?\n\n1. Yes\t2. No\n\nEnter:");
                                scanf("%d",&healchoice);
                                if(healchoice==1)
                                {
                                    Healthinfight4mons-=rf2(currentuser->Min_dmg,currentuser->Max_dmg);
                                    if(Healthinfight4mons<=0)
                                    {
                                        int temp2;
                                        float expgain= (float)((float)current_monsters[sub_choice-1].Level*70.00*(float)current_monsters[sub_choice-1].Diff)
                                                /(float)currentuser->Level;
                                        int expFinal = expgain;
                                        printf("You Won!\n\nCombat Gains:\nMoney: +%d\nExp: +%d\n\n(AnyNumber) Continue\n",
                                               current_monsters[sub_choice-1].Money_Drop,
                                               expFinal);
                                        Edit_Money(currentuser->userId,(current_monsters[sub_choice-1].Money_Drop));
                                        UpdateUserInfo(currentuser,0,0,currentuser->Level,expFinal);
                                        SetCharacterInfo(currentuser);
                                        scanf("%d",&temp2);
                                        field=false;
                                        fighting=false;
                                    }
                                    else
                                    {
                                        int temp2;
                                        Healthinfight-=rf2(current_monsters[sub_choice-1].Min_DMG,current_monsters[sub_choice-1].Max_DMG);
                                        float expgain= (float)(currentuser->Level)
                                                       /(float)((float )current_monsters[sub_choice-1].Diff*2.00);
                                        int expFinal = expgain;
                                        if(expFinal<0)
                                            expFinal=0;
                                        printf("Monster is Attacking You!\n(AnyNumber) NextRound\n\nEnter:");
                                        if(Healthinfight<=0) {
                                            printf("You Lost!\n\nCombat Penalty:\nMoney: +%d\nExp: -%d",
                                                   (1*currentuser->Luck),
                                                   expFinal);
                                            Edit_Money(currentuser->userId,
                                                       (1*currentuser->Luck));
                                            UpdateUserInfo(currentuser, 0, 0, currentuser->Level, -expFinal);
                                            SetCharacterInfo(currentuser);
                                            scanf("%d", &temp2);
                                            field = false;
                                            fighting=false;
                                        }
                                    }
                                }
                                else
                                    continue;
                            }
                            break;
                        default:
                            break;
                    }
                }
                break;
            case 3:
                continue;
                break;
            default:
                system("cls");
                field=false;
                break;
        }

    }while(field);
    system("cls");
}


int main() {
    int choice;
    Current_User_Information currentUser;

    printf("\t\t\t\033[1;39m--->Welcome to the MonsterHunter<---\n");

    do {
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Quit\n\n");
        printf("Enter your choice:");
        scanf("%d", &choice);

        switch (choice) {
            char username[MAX_USERNAME_LENGTH];
            char password[MAX_PASSWORD_LENGTH];
            Account account;
            case 1:
                account.userId = getLastUserId() + 1;
                printf("Enter username:");
                scanf("%s", account.username);
                printf("Enter password:");
                scanf("%s", account.password);
                if (check_exist(account.username)) {
                    system("cls"),printf("\033[1;31mUsername is already taken. Please choose a different username.\033[1;0m\n");
                    break;
                }
                if(account.userId>MAX_ACCOUNTS)
                {
                    printf("Sorry Adventurer. Our Server Is Currently Full Please w8 for us to open the registration again!\n");
                    break;
                }
                strcpy(account.role, "Player");
                account.isActive = 1;
                registerAccount(account);
                SetNewCharacterDefaultValues(account.userId);
                break;
            case 2:
                printf("Enter username:");
                scanf("%s", username);
                printf("Enter password:");
                scanf("%s", password);
                Loginstatus Logged=loginAccount(username, password, &currentUser);
                if (Logged ==LoggedIn){
                    printf("\033[1;32mLogin successful.\n\n\n\033[0m\n"),Loggedin=true;
                    SetCharacterInfo(&currentUser);
                    NewSession = false;
                }
                else if(Logged==InvalidAccount)
                    system("cls"),printf("\033[1;31mInvalid Account\033[0m\n");
                else if(Logged==BannedAccount)
                    system("cls"),printf("\033[1;31mYour Account is Banned!\nPlease Try again later\033[0m\n");
                break;
            case 3:

                printf("THANKS FOR YOUR TIME\n");
                exit(0);
                break;
            default:

                printf("Invalid choice. Try again.\n");

                break;
        }
    } while (!Loggedin);
    system("cls");
    do{
        if(upLevel){
            int points=10;
            while(points) {
                system("cls");
                printf("\t\t\t\t\033[0m-->\033[1;32mWELCOME TO LevelUp Portal!\033[0m<--\033[1;36m\n\n\n");
                printf("Hello %s\nYou Have Finally Made it to level %d\n\nAs A Reward you have %d Free Points to add to your status Have Fun!\n\n\n",
                       currentUser.username,
                       currentUser.Level,
                       points);
                ShopItems Shop[MAX_ITEMS];
                int Itemscount = StoreShopItems(Shop);
                for (int i = 1; i <= Itemscount; i++) {
                    printf("%d. ItemName: %s\n\n", i, Shop[i - 1].ItemName);
                }
                int itemid, amount, sub_choice;
                printf("Enter ItemId:");
                scanf("%d", &itemid);
                if (itemid > Itemscount) {
                    printf("\nItem NotFound!\n\n(AnyNumber) Back\nEnter:");
                    scanf("%d", &sub_choice);
                    continue;
                }
                printf("Enter Item Amount:");
                scanf("%d", &amount);
                if(amount>points)
                {
                    printf("\nNot Enough Points!\n\n(AnyNumber) Back\nEnter:");
                    scanf("%d", &sub_choice);
                    continue;
                }
                else{
                    points-=amount;
                    UpdateUserInfo(&currentUser,itemid,amount,currentUser.Level,currentUser.CurExp);
                    SetCharacterInfo(&currentUser);
                }


            }

            upLevel=false;
            system("cls");
        }
        if(strcmp(currentUser.role,"Owner")==0) {
            char Enabled[MAX_USERNAME_LENGTH];
            if(currentUser.GMode==1)
                strcpy(Enabled,"Enabled");
            else
                strcpy(Enabled,"Disabled");

            printf("\t\t\t\t\033[0m-->\033[1;32mWELCOME TO MONSTERHUNER\033[0m<--\033[1;36m\t\t\t\t\t GodMode: %s\n\n\n",Enabled);
        }
        else
            printf("\t\t\t\t\033[0m-->\033[1;32mWELCOME TO MONSTERHUNER\033[0m<--\n\n\n");

        printf("\033[1;33m");

        if(strcmp(currentUser.role,"Owner")==0)
        {
            printf("\033[1;34mHello Admin!\nHow Can I help you today ?\n\n\n");
            int choice,sub_choice,Id;
            //UpdateUserInfo(&currentUser,0,0,currentUser.Level,200);
            //SetCharacterInfo(&currentUser);
            char username[MAX_USERNAME_LENGTH];
            printf("\033[1;33m1. List All Accounts\n");
            printf("2. Register New Monster\n");
            printf("3. Update Account's Role\n");
            printf("4. Add Money To User\n");
            printf("5. Enable/Disable Godmode\n");
            printf("6. Ban Account\n");
            printf("7. UnBan Account\n");
            printf("8. List All Monsters\n");
            printf("9. Status\n");
            printf("10. Enter Shop\n");
            printf("11. Go To Field\n\n\n");
            printf("\033[1;34mEnter your choice:");
            printf("\033[1;33m");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    system("cls");
                    printf("\033[1;36m\t\t\t\t-->List Of All Users<--\n\n");
                    ListAllUsers();
                    do {
                        int back;
                        printf("\n\n1. Back\nEnter:");
                        scanf("%d",&back);
                        if(back==1) {
                            system("cls");
                            break;
                        }
                    }
                    while(1);
                    break;
                case 2:
                    system("cls");
                    MonsterInfo new_monster;
                    printf("\033[1;36m\t\t\t\t-->Register Monster Portal<--\n\n\n");
                    new_monster.EntryId=getLastMonsterId()+1;
                    printf("Enter Monster Name:");
                    scanf("%s",new_monster.Name);
                    printf("Enter Monster Level:");
                    scanf("%d",&new_monster.Level);
                    printf("Enter Monster Difficulty:");
                    scanf("%d",&new_monster.Diff);
                    printf("Enter Monster Health:");
                    scanf("%d",&new_monster.Health);
                    printf("Enter Monster MinDamage:");
                    scanf("%d",&new_monster.Min_DMG);
                    printf("Enter Monster MaxDamage:");
                    scanf("%d",&new_monster.Max_DMG);
                    printf("Enter Monster Luck:");
                    scanf("%d",&new_monster.Lucky);
                    printf("Enter Monster Armor:");
                    scanf("%d",&new_monster.Armor);
                    printf("Enter Monster MoneyDrop:");
                    scanf("%d",&new_monster.Money_Drop);
                    Register_New_Monster(&new_monster);
                    printf("\nMonster Has Been Registered Successfuly!\n");
                    do {
                        int back;
                        printf("\n\n1. Back\nEnter:");
                        scanf("%d",&back);
                        if(back==1) {
                            system("cls");
                            break;
                        }
                    }while(1);
                    break;
                case 3:
                    system("cls");
                    int ids;
                    char Role[MAX_USERNAME_LENGTH];
                    printf("\033[1;36m\t\t\t\t-->Update Account Portal<--\n\n\n");
                    printf("Enter UserId:");
                    scanf("%d",&ids);
                    printf("Enter Role Name:");
                    scanf("%s",Role);
                    SwitchRole(ids,Role);
                    printf("\nUser Role Has Been Updated Successfuly!\n");
                    do {
                        int back;
                        printf("\n\n1. Back\nEnter:");
                        scanf("%d",&back);
                        if(back==1) {
                            system("cls");
                            break;
                        }
                    }while(1);
                    break;
                case 4:
                    system("cls");
                    printf("\033[1;36m\t\t\t\t-->Money Add Portal<--\n\n\n");
                    int USERID,MoneyAmount;
                    printf("Enter UserId:");
                    scanf("%d",&USERID);
                    printf("Enter Amount Of Money:");
                    scanf("%d",&MoneyAmount);
                    Edit_Money(USERID,MoneyAmount);
                    printf("\n%d%$ Amount Of Money Has been added to the user Successfuly!\n",MoneyAmount);
                    do {
                        int back;
                        printf("\n\n1. Back\nEnter:");
                        scanf("%d",&back);
                        if(back==1) {
                            system("cls");
                            break;
                        }
                    }while(1);
                    break;
                case 5:
                    EnableDisableGodMod(&currentUser);
                    system("cls");
                    break;
                case 6:
                    system("cls");
                    printf("\033[1;36m\t\t\t\t-->Ban Accounts System Portal<--\n\n");
                    printf("1. Ban Account Using UserId\n");
                    printf("2. Ban Account Using Username\n");
                    printf("3. Back\n");
                    printf("Enter your choice:");
                    scanf("%d",&sub_choice);
                        switch (sub_choice) {
                            case 1:
                                system("cls");
                                printf("\033[1;36m\t\t\t\t-->Ban Account By Id<--\n\n");
                                printf("Enter Account Id:");
                                scanf("%d", &Id);
                                BanAccountById(Id,1,currentUser.userId);
                                break;
                            case 2:
                                system("cls");
                                printf("\033[1;36m\t\t\t\t-->Ban Account By Name<--\n\n");
                                printf("Enter Username:");
                                char username[MAX_USERNAME_LENGTH];
                                scanf("%s", username);
                                BanAccountByName(username,1);
                                break;
                            case 3:
                                system("cls");
                                break;
                            default:
                                printf("Invalid choice. Try again.\n");
                                break;
                        }

                    break;
                case 7:
                    system("cls");
                    printf("\033[1;36m\t\t\t\t-->UnBan Accounts System Portal<--\n\n");
                    printf("1. UnBan Account Using UserId\n");
                    printf("2. UnBan Account Using Username\n");
                    printf("3. Back\n");
                    printf("Enter your choice:");
                    scanf("%d",&sub_choice);
                    switch (sub_choice) {
                        case 1:
                            system("cls");
                            printf("\033[1;36m\t\t\t\t-->UnBan Account By Id<--\n\n");
                            printf("Enter Account Id:");
                            scanf("%d", &Id);
                            BanAccountById(Id,2,currentUser.userId);
                            break;
                        case 2:
                            system("cls");
                            printf("\033[1;36m\t\t\t\t-->UnBan Account By Name<--\n\n");
                            printf("Enter Username:");
                            char username[MAX_USERNAME_LENGTH];
                            scanf("%s", username);
                            BanAccountByName(username,2);
                            break;
                        case 3:
                            system("cls");
                            break;
                        default:
                            printf("Invalid choice. Try again.\n");
                            break;
                    }
                    break;
                case 8:
                    system("cls");
                    printf("\033[1;36m\t\t\t\t-->List Of All Monsters<--\n\n");
                    ListAllMonsters();
                    do {
                        int back;
                        printf("\n\n1. Back\nEnter:");
                        scanf("%d",&back);
                        if(back==1) {
                            system("cls");
                            break;
                        }
                    }
                    while(1);
                    break;
                case 9:
                    system("cls");
                    printf("\033[1;36m\t\t\t\t-->PlayerStatus<--\n\nName: %s\nLevel: %d\nEXP: %d\nMoney: %d\n"
                           "MinDamage: %d\n"
                           "MaxDamage: %d\n"
                           "Health: %d\n"
                           "Luck: %d\n"
                           "Armor: %d\n",currentUser.username,currentUser.Level,currentUser.CurExp,currentUser.Money,currentUser.Min_dmg,currentUser.Max_dmg, currentUser.Health,currentUser.Luck,currentUser.Armor);
                    do {
                        int back;
                        printf("\n\n1. Back\nEnter:");
                        scanf("%d",&back);
                        if(back==1) {
                            system("cls");
                            break;
                        }
                    }
                    while(1);
                    break;
                case 10:
                    DoShopping(&currentUser);
                    break;
                case 11:
                    system("cls");
                    field=true;
                    EnterField(&currentUser);
                    break;
                default:
                    printf("Invalid choice. Try again.\n");
                    break;
            }
        }
        else
        {
            printf("Hello Adventurer!\nAre u rdy to start your journy today!?\nWhat Do you want to do for now ?\n\n");
            int choice;
            printf("1. Enter Shop\n");
            printf("2. Status\n");
            printf("3. Go To Field\n\n");
            scanf("%d",&choice);
            switch (choice) {
                case 1:
                    DoShopping(&currentUser);
                    break;
                case 2:
                    system("cls");
                    printf("\033[1;36m\t\t\t\t-->PlayerStatus<--\n\nName: %s\nLevel: %d\nEXP: %d\nMoney: %d\n"
                           "MinDamage: %d\n"
                           "MaxDamage: %d\n"
                           "Health: %d\n"
                           "Luck: %d\n"
                           "Armor: %d\n",currentUser.username,currentUser.Level,currentUser.CurExp,currentUser.Money,currentUser.Min_dmg,currentUser.Max_dmg, currentUser.Health,currentUser.Luck,currentUser.Armor);
                    do {
                        int back;
                        printf("\n\n1. Back\nEnter:");
                        scanf("%d",&back);
                        if(back==1) {
                            system("cls");
                            break;
                        }
                    }
                    while(1);
                    break;
                case 3:
                    system("cls");
                    field=true;
                    EnterField(&currentUser);
                    break;
                default:
                    break;
            }
        }
    }while(Loggedin);



}
