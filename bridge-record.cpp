#include <cstdio>
#include <cstring>

bool club[20];
bool spade[20];
bool heart[20];
bool dimond[20];

void init()
{
    memset(club, 1, sizeof(club));
    memset(spade, 1, sizeof(spade));
    memset(heart, 1, sizeof(heart));
    memset(dimond, 1, sizeof(dimond));
}

void print()
{
    printf("club:\n");
    for(int i=1;i<=13;i++){
        if(club[i]){
            if(i==1) printf("A ");
            else if(i==11) printf("J ");
            else if(i==12) printf("Q ");
            else if(i==13) printf("K ");
            else printf("%d ", i);
        }
        else printf("  ");
    }
    printf("\nspade:\n");
    for(int i=1;i<=13;i++){
        if(spade[i]){
            if(i==1) printf("A ");
            else if(i==11) printf("J ");
            else if(i==12) printf("Q ");
            else if(i==13) printf("K ");
            else printf("%d ", i);
        }
        else printf("  ");
    }
    printf("\nheart:\n");
    for(int i=1;i<=13;i++){
        if(heart[i]){
            if(i==1) printf("A ");
            else if(i==11) printf("J ");
            else if(i==12) printf("Q ");
            else if(i==13) printf("K ");
            else printf("%d ", i);
        }
        else printf("  ");
    }
    printf("\ndimond:\n");
    for(int i=1;i<=13;i++){
        if(dimond[i]){
            if(i==1) printf("A ");
            else if(i==11) printf("J ");
            else if(i==12) printf("Q ");
            else if(i==13) printf("K ");
            else printf("%d ", i);
        }
        else printf("  ");
    }
    printf("\n");
}

int main()
{
    init();
    for(int i=0;i<13;i++){
        for(int j=0;j<4;j++){
            int a, b;
            scanf("%d %d", &a, &b);
            if(a==0) club[b]=0;
            else if(a==1) dimond[b]=0;
            else if(a==2) heart[b]=0;
            else spade[b]=0;
        }
        print();
    }
}