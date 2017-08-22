#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

int modelSize=0;
int dpSize=0;
int cardNum;

struct PokerCard
{
    char color;
    int point;
    bool operator==(const PokerCard &pc)const
    {
        return (pc.color==this->color&&pc.point==this->point);
    }
    bool operator!=(const PokerCard &pc)const
    {
        return !(*this==pc);
    }
    bool operator<(const PokerCard &pc)const
    {
        if(this->color!=pc.color) return this->color<pc.color;
        if(this->point!=pc.point) return this->point<pc.point;
        return 0;
    }
    void print()
    {
        if(color=='s'){
            printf("Spade ");
        }
        else if(color=='h'){
            printf("Heart ");
        }
        else if(color=='d'){
            printf("Dimond ");
        }
        else if(color=='c'){
            printf("Club ");
        }
        else {
            printf("no card\n");
            return;
        }
        if(point==14){
            printf("A\n");
        }
        else if(point==13){
            printf("K\n");
        }
        else if(point==12){
            printf("Q\n");
        }
        else if(point==11){
            printf("J\n");
        }
        else{
            printf("%d\n", point);
        }
    }
};

char kingColor;

int battle(PokerCard p1, PokerCard p2)
{
    int ret=1;
    PokerCard bestCard=p1;
    if(p2.color==kingColor&&bestCard.color!=kingColor){
        ret=2;
        bestCard=p2;
    }
    else if(p2.color==bestCard.color&&p2.point>bestCard.point){
        ret=2;
        bestCard=p2;
    }
    return ret;
}

int battle(PokerCard p1, PokerCard p2, PokerCard p3)
{
    int ret=1;
    PokerCard bestCard=p1;
    if(p2.color==kingColor&&bestCard.color!=kingColor){
        ret=2;
        bestCard=p2;
    }
    else if(p2.color==bestCard.color&&p2.point>bestCard.point){
        ret=2;
        bestCard=p2;
    }
    if(p3.color==kingColor&&bestCard.color!=kingColor){
        ret=3;
        bestCard=p3;
    }
    else if(p3.color==bestCard.color&&p3.point>bestCard.point){
        ret=3;
        bestCard=p3;
    }
    return ret;
}

int battle(PokerCard p1, PokerCard p2, PokerCard p3, PokerCard p4)
{
    int ret=1;
    PokerCard bestCard=p1;
    if(p2.color==kingColor&&bestCard.color!=kingColor){
        ret=2;
        bestCard=p2;
    }
    else if(p2.color==bestCard.color&&p2.point>bestCard.point){
        ret=2;
        bestCard=p2;
    }
    if(p3.color==kingColor&&bestCard.color!=kingColor){
        ret=3;
        bestCard=p3;
    }
    else if(p3.color==bestCard.color&&p3.point>bestCard.point){
        ret=3;
        bestCard=p3;
    }
    if(p4.color==kingColor&&bestCard.color!=kingColor){
        ret=4;
        bestCard=p4;
    }
    else if(p4.color==bestCard.color&&p4.point>bestCard.point){
        ret=4;
        bestCard=p4;
    }
    return ret;
}

struct GameScore
{
    int scoreWe;
    int scoreThey;
};

bool changeBridge=0;

struct Situation
{
    vector<PokerCard> nextPlayer;
    vector<PokerCard> matePlayer;
    vector<PokerCard> previousPlayer;
    vector<PokerCard> selfPlayer;
    int existCard;
    PokerCard nextCard;
    PokerCard mateCard;
    PokerCard previousCard;
    bool operator<(const Situation& st)const
    {
        if(this->existCard!=st.existCard) return this->existCard<st.existCard;
        if(this->nextCard.color!=st.nextCard.color) return this->nextCard.color<st.nextCard.color;
        if(this->nextCard.point!=st.nextCard.point) return this->nextCard.point<st.nextCard.point;
        if(this->mateCard.color!=st.mateCard.color) return this->mateCard.color<st.mateCard.color;
        if(this->mateCard.point!=st.mateCard.point) return this->mateCard.point<st.mateCard.point;
        if(this->previousCard.color!=st.previousCard.color) return this->previousCard.color<st.previousCard.color;
        if(this->previousCard.point!=st.previousCard.point) return this->previousCard.point<st.previousCard.point;
        if(this->nextPlayer.size()!=st.nextPlayer.size()) return this->nextPlayer.size()<st.nextPlayer.size();
        for(int i=0;i<this->nextPlayer.size();i++){
            if(this->nextPlayer[i].color!=st.nextPlayer[i].color) return this->nextPlayer[i].color<st.nextPlayer[i].color;
            if(this->nextPlayer[i].point!=st.nextPlayer[i].point) return this->nextPlayer[i].point<st.nextPlayer[i].point;
        }
        if(this->matePlayer.size()!=st.matePlayer.size()) return this->matePlayer.size()<st.matePlayer.size();
        for(int i=0;i<this->matePlayer.size();i++){
            if(this->matePlayer[i].color!=st.matePlayer[i].color) return this->matePlayer[i].color<st.matePlayer[i].color;
            if(this->matePlayer[i].point!=st.matePlayer[i].point) return this->matePlayer[i].point<st.matePlayer[i].point;
        }
        if(this->previousPlayer.size()!=st.previousPlayer.size()) return this->previousPlayer.size()<st.previousPlayer.size();
        for(int i=0;i<this->previousPlayer.size();i++){
            if(this->previousPlayer[i].color!=st.previousPlayer[i].color) return this->previousPlayer[i].color<st.previousPlayer[i].color;
            if(this->previousPlayer[i].point!=st.previousPlayer[i].point) return this->previousPlayer[i].point<st.previousPlayer[i].point;
        }
        if(this->selfPlayer.size()!=st.selfPlayer.size()) return this->selfPlayer.size()<st.selfPlayer.size();
        for(int i=0;i<this->selfPlayer.size();i++){
            if(this->selfPlayer[i].color!=st.selfPlayer[i].color) return this->selfPlayer[i].color<st.selfPlayer[i].color;
            if(this->selfPlayer[i].point!=st.selfPlayer[i].point) return this->selfPlayer[i].point<st.selfPlayer[i].point;
        }
        return 0;
    }

    void NextStep(PokerCard pc)
    {
        for(int i=0;i<selfPlayer.size();i++){
            if(selfPlayer[i].color==pc.color&&selfPlayer[i].point==pc.point){
                selfPlayer.erase(selfPlayer.begin()+i);
                break;
            }
        }
        if(existCard==3){
            changeBridge=0;
            int battleResult=battle(nextCard, mateCard, previousCard, pc);
            PokerCard noneCard;
            noneCard.color=0;
            noneCard.point=0;
            nextCard=noneCard;
            mateCard=noneCard;
            previousCard=noneCard;
            existCard=0;
            if(battleResult==1){//next win
                vector<PokerCard> tmp=selfPlayer;
                selfPlayer=nextPlayer;
                nextPlayer=matePlayer;
                matePlayer=previousPlayer;
                previousPlayer=tmp;
                changeBridge=1;
            }
            else if(battleResult==2){//mate win
                vector<PokerCard> tmp=selfPlayer;
                selfPlayer=matePlayer;
                matePlayer=tmp;
                tmp=nextPlayer;
                nextPlayer=previousPlayer;
                previousPlayer=tmp;
            }
            else if(battleResult==3){//previous win
                vector<PokerCard> tmp=selfPlayer;
                selfPlayer=previousPlayer;
                previousPlayer=matePlayer;
                matePlayer=nextPlayer;
                nextPlayer=tmp;
                changeBridge=1;
            }
        }
        else{
            if(existCard==0){
                previousCard=pc;
                existCard=1;
            }
            else if(existCard==1){
                mateCard=previousCard;
                previousCard=pc;
                existCard=2;
            }
            else{
                nextCard=mateCard;
                mateCard=previousCard;
                previousCard=pc;
                existCard=3;
            }
            vector<PokerCard> tmp=selfPlayer;
            selfPlayer=nextPlayer;
            nextPlayer=matePlayer;
            matePlayer=previousPlayer;
            previousPlayer=tmp;
        }
    }

    void print(bool lite)
    {
        //printf("****print situation****\n");
        printf("exist %d card\n", existCard);
        if(!lite){
            printf("next card: ");
            nextCard.print();
            printf("mate card: ");
            mateCard.print();
            printf("previous card: ");
            previousCard.print();
        }
        if(lite){
            printf("%d %d %d %d\n", selfPlayer.size(), nextPlayer.size(), matePlayer.size(), previousPlayer.size());
            return;
        }
        printf("slef player:\n");
        for(int i=0;i<selfPlayer.size();i++){
            selfPlayer[i].print();
        }
        printf("next player:\n");
        for(int i=0;i<nextPlayer.size();i++){
            nextPlayer[i].print();
        }
        printf("mate player:\n");
        for(int i=0;i<matePlayer.size();i++){
            matePlayer[i].print();
        }
        printf("previous player:\n");
        for(int i=0;i<previousPlayer.size();i++){
            previousPlayer[i].print();
        }
    }
};

map<Situation, GameScore> dp;

bool canWin(vector<PokerCard> pcVec, PokerCard pc)
{
    for(int i=0;i<pcVec.size();i++){
        if(pcVec[i].color!=pc.color) continue;
        if(battle(pc, pcVec[i])==2){
            return 1;
        }
    }
    return 0;
}

bool canLose(vector<PokerCard> pcVec, PokerCard pc)
{
    for(int i=0;i<pcVec.size();i++){
        if(pcVec[i].color!=pc.color) continue;
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

PokerCard FirstSameColorCard(vector<PokerCard> pcVec, PokerCard pc)
{
    for(int i=0;i<pcVec.size();i++){
        if(pcVec[i].color==pc.color) return pcVec[i];
    }
}

GameScore solve(Situation now, int deep)
{
    if(dp.find(now)!=dp.end()){
        return dp[now];
    }
    GameScore ret;
    PokerCard choice;
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
    Situation sit;
    PokerCard noneCard;
    noneCard.color=0;
    noneCard.point=0;
    if(now.existCard==3){
        int follow=canFollow(now.selfPlayer, now.nextCard);
        PokerCard bestCard;
        bool allLose=0;
        bool allWin=0;
        if(follow>1){
            int battleResult=battle(now.nextCard, now.mateCard, now.previousCard);
            if(battleResult==1) bestCard=now.nextCard;
            else if(battleResult==2) bestCard=now.mateCard;
            else bestCard=now.previousCard;
            allLose=!canWin(now.selfPlayer, bestCard);
            if(allLose) bestCard=FirstSameColorCard(now.selfPlayer, now.nextCard);
            else{
                allWin=!canLose(now.selfPlayer, bestCard);
                if(allWin) bestCard=FirstSameColorCard(now.selfPlayer, now.nextCard);
            }
        }
        if(deep<8) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow&&now.selfPlayer[i].color!=now.nextCard.color) continue;
            if(allLose&&now.selfPlayer[i]!=bestCard) continue;
            if(allWin&&now.selfPlayer[i]!=bestCard) continue;
            if(deep<8) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            bool theirTerm=changeBridge;
            GameScore gs=solve(nextSit, deep+1);
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
        }
        if(deep<8) printf("\b\b\b\b");
        return ret;
    }
    if(now.existCard==0){
        if(deep<8) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(deep<8) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
            }
        }
        if(deep<8) printf("\b\b\b\b");
        dp[now]=ret;
    }
    else if(now.existCard==1){
        int follow=canFollow(now.selfPlayer, now.previousCard);
        PokerCard bestCard=now.previousCard;
        bool allLose=0;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard);
            if(allLose) bestCard=FirstSameColorCard(now.selfPlayer, now.previousCard);
        }
        if(deep<8) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow&&now.selfPlayer[i].color!=now.previousCard.color) continue;
            if(allLose&&now.selfPlayer[i]!=bestCard) continue;
            if(deep<8) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
            }
        }
        if(deep<8) printf("\b\b\b\b");
    }
    else if(now.existCard==2){
        sit.existCard=3;
        sit.nextCard=now.mateCard;
        sit.mateCard=now.previousCard;
        int follow=canFollow(now.selfPlayer, now.mateCard);
        PokerCard bestCard;
        bool allLose=0;
        if(follow>1){
            int battleResult=battle(now.mateCard, now.previousCard);
            if(battleResult==1) bestCard=now.mateCard;
            else if(battleResult==2) bestCard=now.previousCard;
            allLose=!canWin(now.selfPlayer, bestCard);
            if(allLose) bestCard=FirstSameColorCard(now.selfPlayer, now.mateCard);
        }
        if(deep<8) printf("%3d ", 0);
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow&&now.selfPlayer[i].color!=now.mateCard.color) continue;
            if(allLose&&now.selfPlayer[i]!=bestCard) continue;
            if(deep<8) printf("\b\b\b\b%3d ", i);
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, deep+1);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
            }
        }
        if(deep<8) printf("\b\b\b\b");
    }
    return ret;
}

void print(Situation now)
{
    GameScore ret;
    PokerCard choice;
    //the last round, directly return the battle result
    if(now.nextPlayer.size()==1&&now.matePlayer.size()==1&&now.previousPlayer.size()==1&&now.selfPlayer.size()==1){
        int battleResult=battle(now.selfPlayer[0], now.nextPlayer[0], now.matePlayer[0], now.previousPlayer[0]);
        ret.scoreWe=0;
        ret.scoreThey=0;
        if(battleResult==1||battleResult==3) ret.scoreWe++;
        else ret.scoreThey++;
        now.selfPlayer[0].print();
        return;
    }
    ret.scoreWe=-1;
    ret.scoreThey=-1;
    Situation sit;
    PokerCard noneCard;
    noneCard.color=0;
    noneCard.point=0;
    if(now.existCard==3){
        int follow=canFollow(now.selfPlayer, now.nextCard);
        PokerCard bestCard;
        bool allLose=0;
        bool allWin=0;
        if(follow>1){
            int battleResult=battle(now.nextCard, now.mateCard, now.previousCard);
            if(battleResult==1) bestCard=now.nextCard;
            else if(battleResult==2) bestCard=now.mateCard;
            else bestCard=now.previousCard;
            allLose=!canWin(now.selfPlayer, bestCard);
            if(allLose) bestCard=FirstSameColorCard(now.selfPlayer, now.nextCard);
            else{
                allWin=!canLose(now.selfPlayer, bestCard);
                if(allWin) bestCard=FirstSameColorCard(now.selfPlayer, now.nextCard);
            }
        }
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow&&now.selfPlayer[i].color!=now.nextCard.color) continue;
            if(allLose&&now.selfPlayer[i]!=bestCard) continue;
            if(allWin&&now.selfPlayer[i]!=bestCard) continue;
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            bool theirTerm=changeBridge;
            GameScore gs=solve(nextSit, 8);
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
        }
    }
    if(now.existCard==0){
        for(int i=0;i<now.selfPlayer.size();i++){
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, 8);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
                choice=now.selfPlayer[i];
            }
        }
    }
    else if(now.existCard==1){
        int follow=canFollow(now.selfPlayer, now.previousCard);
        PokerCard bestCard=now.previousCard;
        bool allLose=0;
        if(follow>1){
            allLose=!canWin(now.selfPlayer, bestCard);
            if(allLose) bestCard=FirstSameColorCard(now.selfPlayer, now.previousCard);
        }
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow&&now.selfPlayer[i].color!=now.previousCard.color) continue;
            if(allLose&&now.selfPlayer[i]!=bestCard) continue;
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, 8);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
                choice=now.selfPlayer[i];
            }
        }
    }
    else if(now.existCard==2){
        sit.existCard=3;
        sit.nextCard=now.mateCard;
        sit.mateCard=now.previousCard;
        int follow=canFollow(now.selfPlayer, now.mateCard);
        PokerCard bestCard;
        bool allLose=0;
        if(follow>1){
            int battleResult=battle(now.mateCard, now.previousCard);
            if(battleResult==1) bestCard=now.mateCard;
            else if(battleResult==2) bestCard=now.previousCard;
            allLose=!canWin(now.selfPlayer, bestCard);
            if(allLose) bestCard=FirstSameColorCard(now.selfPlayer, now.mateCard);
        }
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow&&now.selfPlayer[i].color!=now.mateCard.color) continue;
            if(allLose&&now.selfPlayer[i]!=bestCard) continue;
            Situation nextSit=now;
            nextSit.NextStep(nextSit.selfPlayer[i]);
            GameScore gs=solve(nextSit, 8);
            if(gs.scoreThey>ret.scoreWe){
                ret.scoreWe=gs.scoreThey;
                ret.scoreThey=gs.scoreWe;
                choice=now.selfPlayer[i];
            }
        }
    }
    choice.print();
    now.NextStep(choice);
    print(now);
}

int main()
{
    Situation newSit;
    newSit.existCard=0;
    PokerCard pc;
    pc.color=0;
    pc.point=0;
    newSit.previousCard=pc;
    newSit.nextCard=pc;
    newSit.mateCard=pc;
    char colorStr[10];
    char kingStr[10];
    int pointInt;
    printf("cardNum: ");
    scanf("%d", &cardNum);
    printf("kingColor: ");
    scanf("%s", kingStr);
    kingColor=kingStr[0];
    printf("self Player:\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.selfPlayer.push_back(pc);
    }
    sort(newSit.selfPlayer.begin(), newSit.selfPlayer.end());
    printf("next Player:\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.nextPlayer.push_back(pc);
    }
    sort(newSit.nextPlayer.begin(), newSit.nextPlayer.end());
    printf("mate Player:\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.matePlayer.push_back(pc);
    }
    sort(newSit.matePlayer.begin(), newSit.matePlayer.end());
    printf("previous Player:\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.previousPlayer.push_back(pc);
    }
    sort(newSit.previousPlayer.begin(), newSit.previousPlayer.end());
    newSit.print(0);
    GameScore gs=solve(newSit, 0);
    printf("%d %d\n", gs.scoreWe, gs.scoreThey);
    print(newSit);
    return 0;
}