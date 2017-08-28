#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <ctime> 

#include "situation.h"

using namespace std;

int cardNum=13;
int stopPoint;

struct GameScore
{
    int scoreWe;
    int scoreThey;
    GameScore()
    {
        scoreWe=scoreThey=0;
    }
};

Situation globalSit;

map<Situation, GameScore> dp;

bool canWin(vector<PokerCard> pcVec, PokerCard pc, char followColor)
{
    for(int i=0;i<pcVec.size();i++){
        if(pcVec[i].color!=followColor) continue;
        if(battle(pc, pcVec[i])==2){
            return 1;
        }
    }
    return 0;
}

bool canLose(vector<PokerCard> pcVec, PokerCard pc, char followColor)
{
    for(int i=0;i<pcVec.size();i++){
        if(pcVec[i].color!=followColor) continue;
        if(battle(pc, pcVec[i])==1){
            return 1;
        }
    }
    return 0;
}

int canFollow(vector<PokerCard> pcVec, PokerCard pc)
{
    int ret=0;
    for(int i=0;i<pcVec.size();i++){
        if(pcVec[i].color==pc.color) ret++;
    }
    return ret;
}

PokerCard FirstSameColorCard(vector<PokerCard> pcVec, char followColor)
{
    for(int i=0;i<pcVec.size();i++){
        if(pcVec[i].color==followColor) return pcVec[i];
    }
    return noneCard;
}

GameScore solve(Situation now, int deep, bool printProg, int maxThey)
{
    if(dp.find(now)!=dp.end()){
        return dp[now];
    }
    if(deep>=8) printProg=0;
    GameScore ret;
    //the last round, directly return the battle result
    if(now.nextPlayer.size()==1&&now.matePlayer.size()==1&&now.previousPlayer.size()==1&&now.selfPlayer.size()==1){
        int battleResult=battle(now.selfPlayer[0], now.nextPlayer[0], now.matePlayer[0], now.previousPlayer[0]);
        ret.scoreWe=0;
        ret.scoreThey=0;
        if(battleResult==1||battleResult==3) ret.scoreWe++;
        else ret.scoreThey++;
        dp[now]=ret;
        return ret;
    }
    ret.scoreWe=-1;
    ret.scoreThey=-1;
    if(now.existCard==3){
        int follow=canFollow(now.selfPlayer, now.nextCard);
        set<PokerCard> trySet;
        PokerCard bestCard;
        bool allLose=0;
        bool allWin=0;
        int battleResult=battle(now.nextCard, now.mateCard, now.previousCard);
        if(battleResult==1) bestCard=now.nextCard;
        else if(battleResult==2) bestCard=now.mateCard;
        else bestCard=now.previousCard;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard, now.nextCard.color);
            if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, now.nextCard.color));
            else{
                allWin=!canLose(now.selfPlayer, bestCard, now.nextCard.color);
                if(allWin) trySet.insert(FirstSameColorCard(now.selfPlayer, now.nextCard.color));
            }
        }
        else if(follow==0){
            for(int i=0;i<4;i++){
                allLose=!canWin(now.selfPlayer, bestCard, colorSet[i]);
                if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, colorSet[i]));
            }
        }
        if(printProg) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==0){
                if(now.selfPlayer[i].color!=kingColor&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            else{
                if(now.selfPlayer[i].color!=now.nextCard.color) continue;
                if(allLose&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
                if(allWin&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            if(printProg) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            bool theirTerm=changeBridge;
            GameScore gs;
            if(deep+1<stopPoint) gs=solve(nextSit, deep+1, printProg, ret.scoreWe);
            else gs.scoreThey=gs.scoreWe=0;
            if(theirTerm){
                if(gs.scoreThey>ret.scoreWe){
                    ret.scoreWe=gs.scoreThey;
                    ret.scoreThey=gs.scoreWe+1;
                }
            }
            else{//out term
                if(gs.scoreWe+1>ret.scoreWe){
                    ret.scoreWe=gs.scoreWe+1;
                    ret.scoreThey=gs.scoreThey;
                }
            }
            if(ret.scoreThey<=maxThey) break;
            if(ret.scoreThey==0) break;
        }
        if(printProg) printf("\b\b\b\b");
        return ret;
    }
    if(now.existCard==0){
        if(printProg) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(printProg) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1, printProg, ret.scoreWe);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
            }
            if(ret.scoreThey==0) break;
        }
        if(printProg) printf("\b\b\b\b");
        dp[now]=ret;
    }
    else if(now.existCard==1){
        int follow=canFollow(now.selfPlayer, now.previousCard);
        PokerCard bestCard=now.previousCard;
        set<PokerCard> trySet;
        bool allLose=0;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard, now.previousCard.color);
            if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, now.previousCard.color));
        }
        else if(follow==0){
            for(int i=0;i<4;i++){
                allLose=!canWin(now.selfPlayer, bestCard, colorSet[i]);
                if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, colorSet[i]));
            }
        }
        if(printProg) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==0){
                if(now.selfPlayer[i].color!=kingColor&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            else{
                if(now.selfPlayer[i].color!=now.previousCard.color) continue;
                if(allLose&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            if(printProg) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1, printProg, ret.scoreWe);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
            }
            if(ret.scoreThey<=maxThey) break;
            if(ret.scoreThey==0) break;
        }
        if(printProg) printf("\b\b\b\b");
    }
    else if(now.existCard==2){
        int follow=canFollow(now.selfPlayer, now.mateCard);
        PokerCard bestCard;
        set<PokerCard> trySet;
        int battleResult=battle(now.mateCard, now.previousCard);
        if(battleResult==1) bestCard=now.mateCard;
        else if(battleResult==2) bestCard=now.previousCard;
        bool allLose=0;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard, now.mateCard.color);
            if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, now.mateCard.color));
        }
        else if(follow==0){
            for(int i=0;i<4;i++){
                allLose=!canWin(now.selfPlayer, bestCard, colorSet[i]);
                if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, colorSet[i]));
            }
        }
        if(printProg) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==0){
                if(now.selfPlayer[i].color!=kingColor&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            else{
                if(now.selfPlayer[i].color!=now.mateCard.color) continue;
                if(allLose&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            if(printProg) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1, printProg, ret.scoreWe);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
            }
            if(ret.scoreThey<=maxThey) break;
            if(ret.scoreThey==0) break;
        }
        if(printProg) printf("\b\b\b\b");
    }
    if(ret.scoreThey==-1||ret.scoreWe==-1){
        printf("=========================================================\n");
        now.print(0);
        printf("======score %d %d =====================================\n", ret.scoreThey, ret.scoreWe);
    }
    return ret;
}

void print(Situation now, int deep, bool recurPrint)
{
    if(deep==stopPoint) return;
    GameScore ret;
    PokerCard choice;
    //the last round, directly return the battle result
    /*if(now.nextPlayer.size()==1&&now.matePlayer.size()==1&&now.previousPlayer.size()==1&&now.selfPlayer.size()==1){
        int battleResult=battle(now.selfPlayer[0], now.nextPlayer[0], now.matePlayer[0], now.previousPlayer[0]);
        ret.scoreWe=0;
        ret.scoreThey=0;
        if(battleResult==1||battleResult==3) ret.scoreWe++;
        else ret.scoreThey++;
        now.selfPlayer[0].print();
        return;
    }*/
    if(now.selfPlayer.size()==1){
        now.selfPlayer[0].print();
        globalSit.NextStep(now.selfPlayer[0]);
        return;
    }
    ret.scoreWe=-1;
    ret.scoreThey=-1;
    if(now.existCard==3){
        int follow=canFollow(now.selfPlayer, now.nextCard);
        set<PokerCard> trySet;
        PokerCard bestCard;
        bool allLose=0;
        bool allWin=0;
        int battleResult=battle(now.nextCard, now.mateCard, now.previousCard);
        if(battleResult==1) bestCard=now.nextCard;
        else if(battleResult==2) bestCard=now.mateCard;
        else bestCard=now.previousCard;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard, now.nextCard.color);
            if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, now.nextCard.color));
            else{
                allWin=!canLose(now.selfPlayer, bestCard, now.nextCard.color);
                if(allWin) trySet.insert(FirstSameColorCard(now.selfPlayer, now.nextCard.color));
            }
        }
        else if(follow==0){
            for(int i=0;i<4;i++){
                allLose=!canWin(now.selfPlayer, bestCard, colorSet[i]);
                if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, colorSet[i]));
            }
        }
        printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==0){
                if(now.selfPlayer[i].color!=kingColor&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            else{
                if(now.selfPlayer[i].color!=now.nextCard.color) continue;
                if(allLose&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
                if(allWin&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            bool theirTerm=changeBridge;
            GameScore gs=solve(nextSit, deep+1, 0, ret.scoreWe);
            if(theirTerm){
                if(gs.scoreThey>ret.scoreWe){
                    ret.scoreWe=gs.scoreThey;
                    ret.scoreThey=gs.scoreWe+1;
                    choice=now.selfPlayer[i];
                }
            }
            else{//out term
                if(gs.scoreWe+1>ret.scoreWe){
                    ret.scoreWe=gs.scoreWe+1;
                    ret.scoreThey=gs.scoreThey;
                    choice=now.selfPlayer[i];
                }
            }
            if(ret.scoreThey==0) break;
        }
        printf("\b\b\b\b");
    }
    if(now.existCard==0){
        printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1, 0, ret.scoreWe);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
                choice=now.selfPlayer[i];
            }
            if(ret.scoreThey==0) break;
        }
        printf("\b\b\b\b");
    }
    else if(now.existCard==1){
        int follow=canFollow(now.selfPlayer, now.previousCard);
        PokerCard bestCard=now.previousCard;
        set<PokerCard> trySet;
        bool allLose=0;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard, now.previousCard.color);
            if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, now.previousCard.color));
        }
        else if(follow==0){
            for(int i=0;i<4;i++){
                allLose=!canWin(now.selfPlayer, bestCard, colorSet[i]);
                if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, colorSet[i]));
            }
        }
        printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==0){
                if(now.selfPlayer[i].color!=kingColor&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            else{
                if(now.selfPlayer[i].color!=now.previousCard.color) continue;
                if(allLose&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1, 0, ret.scoreWe);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
                choice=now.selfPlayer[i];
            }
            if(ret.scoreThey==0) break;
        }
        printf("\b\b\b\b");
    }
    else if(now.existCard==2){
        int follow=canFollow(now.selfPlayer, now.mateCard);
        PokerCard bestCard;
        set<PokerCard> trySet;
        int battleResult=battle(now.mateCard, now.previousCard);
        if(battleResult==1) bestCard=now.mateCard;
        else if(battleResult==2) bestCard=now.previousCard;
        bool allLose=0;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard, now.mateCard.color);
            if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, now.mateCard.color));
        }
        else if(follow==0){
            for(int i=0;i<4;i++){
                allLose=!canWin(now.selfPlayer, bestCard, colorSet[i]);
                if(allLose) trySet.insert(FirstSameColorCard(now.selfPlayer, colorSet[i]));
            }
        }
        printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==0){
                if(now.selfPlayer[i].color!=kingColor&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            else{
                if(now.selfPlayer[i].color!=now.mateCard.color) continue;
                if(allLose&&trySet.find(now.selfPlayer[i])==trySet.end()) continue;
            }
            printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1, 0, ret.scoreWe);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
                choice=now.selfPlayer[i];
            }
            if(ret.scoreThey==0) break;
        }
        printf("\b\b\b\b");
    }
    choice.print();
    //printf("%d %d\n", ret.scoreWe, ret.scoreThey);
    globalSit.NextStep(choice);
    now.NextStep(choice);
    if(recurPrint) print(now, deep+1, 1);
}

Situation read()
{
    noneCard.color=noneCard.point=0;
    Situation newSit;
    newSit.existCard=0;
    newSit.previousCard=noneCard;
    newSit.nextCard=noneCard;
    newSit.mateCard=noneCard;
    newSit.selfPlayerDir=0;
    char colorStr[10];
    char kingStr[10];
    int pointInt;
    PokerCard pc;
    printf("kingColor: ");
    scanf("%s", kingStr);
    kingColor=kingStr[0];
    printf("self Player (South) :\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.selfPlayer.push_back(pc);
    }
    sort(newSit.selfPlayer.begin(), newSit.selfPlayer.end());
    printf("next Player (West) :\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.nextPlayer.push_back(pc);
    }
    sort(newSit.nextPlayer.begin(), newSit.nextPlayer.end());
    printf("mate Player (North) :\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.matePlayer.push_back(pc);
    }
    sort(newSit.matePlayer.begin(), newSit.matePlayer.end());
    printf("previous Player (East) :\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.previousPlayer.push_back(pc);
    }
    sort(newSit.previousPlayer.begin(), newSit.previousPlayer.end());
    return newSit;
}

Situation rnd()
{
    srand(time(0));
    int rndKing=rand()%5;
    if(rndKing<4) kingColor=colorSet[rndKing];
    else kingColor='n';
    vector<int> v;
    for(int i=0;i<52;i++) v.push_back(i);
    random_shuffle(v.begin(), v.end());
    Situation newSit;
    newSit.existCard=0;
    newSit.previousCard=noneCard;
    newSit.nextCard=noneCard;
    newSit.mateCard=noneCard;
    newSit.selfPlayerDir=0;
    PokerCard pc;
    for(int i=0;i<13;i++){
        pc.color=colorSet[v[i]/13];
        pc.point=v[i]%13+2;
        newSit.selfPlayer.push_back(pc);
    }
    sort(newSit.selfPlayer.begin(), newSit.selfPlayer.end());
    for(int i=13;i<26;i++){
        pc.color=colorSet[v[i]/13];
        pc.point=v[i]%13+2;
        newSit.nextPlayer.push_back(pc);
    }
    sort(newSit.nextPlayer.begin(), newSit.nextPlayer.end());
    for(int i=26;i<39;i++){
        pc.color=colorSet[v[i]/13];
        pc.point=v[i]%13+2;
        newSit.matePlayer.push_back(pc);
    }
    sort(newSit.matePlayer.begin(), newSit.matePlayer.end());
    for(int i=39;i<52;i++){
        pc.color=colorSet[v[i]/13];
        pc.point=v[i]%13+2;
        newSit.previousPlayer.push_back(pc);
    }
    sort(newSit.previousPlayer.begin(), newSit.previousPlayer.end());
    return newSit;
}

int main()
{
    puts("0 for input 4 people's cards and king color manually, other chars for randomly generate 4 people's cards and king color");
    char choice[100];
    scanf("%s", choice);
    char colorStr[10];
    int pointInt;
    PokerCard pc;
    if(strcmp(choice, "0")==0){
        globalSit=read();
    }
    else{
        globalSit=rnd();
    }
    GameScore realScore;
    Situation localSit=globalSit;
    for(int tt=0;tt<2;tt++){
        printf("===========================Please wait 1========================\n");
        stopPoint=20;
        GameScore gs=solve(globalSit, 0, 1, -1);
        printf("########### expect score %d %d ###################\n", gs.scoreWe, gs.scoreThey);
        for(int i=0;i<stopPoint;i++){
            if(i%4==0){
                printf("*********************bridge %d ********************\n", i/4+1);
                globalSit.print(0);
                if(i>0){
                    if(globalSit.selfPlayerDir==tt||globalSit.selfPlayerDir==tt+2) realScore.scoreWe++;
                    else realScore.scoreThey++;
                    printf("Computer %d, You %d\n", realScore.scoreWe, realScore.scoreThey);
                }
            }
            if(globalSit.selfPlayerDir==tt||globalSit.selfPlayerDir==tt+2){
                printf("%s (Computer) :\n", playerSet[globalSit.selfPlayerDir]);
                print(globalSit, 0, 0);
            }
            else{
                printf("%s (You) :", playerSet[globalSit.selfPlayerDir]);
                scanf("%s", colorStr);
                scanf("%d", &pointInt);
                pc.color=colorStr[0];
                pc.point=pointInt;
                while(find(globalSit.selfPlayer.begin(), globalSit.selfPlayer.end(), pc)==globalSit.selfPlayer.end()){
                    printf("Try again, you don't have ");
                    pc.print();
                    scanf("%s", colorStr);
                    scanf("%d", &pointInt);
                    pc.color=colorStr[0];
                    pc.point=pointInt;
                }
                pc.print();
                globalSit.NextStep(pc);
            }
        }
        //print(newSit, 0, 1);
        printf("===========================Please wait 2========================\n");
        //newSit.print(0);
        stopPoint=20;
        dp.clear();
        gs=solve(globalSit, 0, 1, -1);
        printf("########### expect score %d %d ###################\n", gs.scoreWe, gs.scoreThey);
        for(int i=0;i<stopPoint;i++){
            if(i%4==0){
                printf("*********************bridge %d ********************\n", i/4+5);
                globalSit.print(0);
                if(globalSit.selfPlayerDir==tt||globalSit.selfPlayerDir==tt+2) realScore.scoreWe++;
                else realScore.scoreThey++;
                printf("Computer %d, You %d\n", realScore.scoreWe, realScore.scoreThey);
            }
            if(globalSit.selfPlayerDir==tt||globalSit.selfPlayerDir==tt+2){
                printf("%s (Computer) :\n", playerSet[globalSit.selfPlayerDir]);
                print(globalSit, 0, 0);
            }
            else{
                printf("%s (You) :", playerSet[globalSit.selfPlayerDir]);
                scanf("%s", colorStr);
                scanf("%d", &pointInt);
                pc.color=colorStr[0];
                pc.point=pointInt;
                while(find(globalSit.selfPlayer.begin(), globalSit.selfPlayer.end(), pc)==globalSit.selfPlayer.end()){
                    printf("Try again, you don't have ");
                    pc.print();
                    scanf("%s", colorStr);
                    scanf("%d", &pointInt);
                    pc.color=colorStr[0];
                    pc.point=pointInt;
                }
                pc.print();
                globalSit.NextStep(pc);
            }
        }
        //print(newSit, 0, 1);
        printf("===========================Please wait 3========================\n");
        //newSit.print(0);
        stopPoint=12;
        dp.clear();
        gs=solve(globalSit, 0, 1, -1);
        printf("########### expect score %d %d ###################\n", gs.scoreWe, gs.scoreThey);
        for(int i=0;i<stopPoint;i++){
            if(i%4==0){
                printf("*********************bridge %d ********************\n", i/4+11);
                globalSit.print(0);
                if(globalSit.selfPlayerDir==tt||globalSit.selfPlayerDir==tt+2) realScore.scoreWe++;
                else realScore.scoreThey++;
                printf("Computer %d, You %d\n", realScore.scoreWe, realScore.scoreThey);
            }
            if(globalSit.selfPlayerDir==tt||globalSit.selfPlayerDir==tt+2){
                printf("%s (Computer) :\n", playerSet[globalSit.selfPlayerDir]);
                print(globalSit, 0, 0);
            }
            else{
                printf("%s (You) :", playerSet[globalSit.selfPlayerDir]);
                scanf("%s", colorStr);
                scanf("%d", &pointInt);
                pc.color=colorStr[0];
                pc.point=pointInt;
                while(find(globalSit.selfPlayer.begin(), globalSit.selfPlayer.end(), pc)==globalSit.selfPlayer.end()){
                    printf("Try again, you don't have ");
                    pc.print();
                    scanf("%s", colorStr);
                    scanf("%d", &pointInt);
                    pc.color=colorStr[0];
                    pc.point=pointInt;
                }
                pc.print();
                globalSit.NextStep(pc);
            }
        }
        if(globalSit.selfPlayerDir==tt||globalSit.selfPlayerDir==tt+2) realScore.scoreWe++;
        else realScore.scoreThey++;
        printf("Computer %d, You %d\n", realScore.scoreWe, realScore.scoreThey);
        globalSit=localSit;
        //print(newSit, 0, 1);
    }
    return 0;
}