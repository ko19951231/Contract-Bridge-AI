#include "pokerCard.h"

using namespace std;

bool changeBridge=0;

char kingColor;
char colorSet[4]={'c', 'd', 'h', 's'};
char playerSet[4][10]={"North", "East", "South", "West"};
PokerCard noneCard;

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

struct Situation
{
    int selfPlayerDir;
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
                selfPlayerDir=(selfPlayerDir+1)%4;
            }
            else if(battleResult==2){//mate win
                vector<PokerCard> tmp=selfPlayer;
                selfPlayer=matePlayer;
                matePlayer=tmp;
                tmp=nextPlayer;
                nextPlayer=previousPlayer;
                previousPlayer=tmp;
                selfPlayerDir=(selfPlayerDir+2)%4;
            }
            else if(battleResult==3){//previous win
                vector<PokerCard> tmp=selfPlayer;
                selfPlayer=previousPlayer;
                previousPlayer=matePlayer;
                matePlayer=nextPlayer;
                nextPlayer=tmp;
                changeBridge=1;
                selfPlayerDir=(selfPlayerDir+3)%4;
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
            selfPlayerDir=(selfPlayerDir+1)%4;
        }
    }

    void print(bool lite)
    {
        //printf("****print situation****\n");
        /*printf("exist %d card\n", existCard);
        if(!lite){
            printf("next card: ");
            nextCard.print();
            printf("mate card: ");
            mateCard.print();
            printf("previous card: ");
            previousCard.print();
        }*/
        if(lite){
            printf("%d %d %d %d\n", selfPlayer.size(), nextPlayer.size(), matePlayer.size(), previousPlayer.size());
            return;
        }
        printf("\n*****************slef player (%s) :*****************", playerSet[selfPlayerDir]);
        for(int i=0;i<selfPlayer.size();i++){
            if(i==0||selfPlayer[i].color!=selfPlayer[i-1].color){
                puts("");
                if(selfPlayer[i].color==kingColor) printf("*");
                selfPlayer[i].printColor();
                printf(": ");
            }
            selfPlayer[i].printPoint();
        }
        printf("\n*****************next player (%s) :*****************", playerSet[(selfPlayerDir+1)%4]);
        for(int i=0;i<nextPlayer.size();i++){
            if(i==0||nextPlayer[i].color!=nextPlayer[i-1].color){
                puts("");
                if(nextPlayer[i].color==kingColor) printf("*");
                nextPlayer[i].printColor();
                printf(": ");
            }
            nextPlayer[i].printPoint();
        }
        printf("\n*****************mate player (%s) :*****************", playerSet[(selfPlayerDir+2)%4]);
        for(int i=0;i<matePlayer.size();i++){
            if(i==0||matePlayer[i].color!=matePlayer[i-1].color){
                puts("");
                if(matePlayer[i].color==kingColor) printf("*");
                matePlayer[i].printColor();
                printf(": ");
            }
            matePlayer[i].printPoint();
        }
        printf("\n*****************previous player (%s) :*****************", playerSet[(selfPlayerDir+3)%4]);
        for(int i=0;i<previousPlayer.size();i++){
            if(i==0||previousPlayer[i].color!=previousPlayer[i-1].color){
                puts("");
                if(previousPlayer[i].color==kingColor) printf("*");
                previousPlayer[i].printColor();
                printf(": ");
            }
            previousPlayer[i].printPoint();
        }
        puts("");
    }
};