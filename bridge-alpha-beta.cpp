#include <cstdio>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

struct PokerCard
{
    char color;
    int point;
    bool operator==(const PokerCard &pc)const
    {
        return (pc.color==this->color&&pc.point==this->point);
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
            mateCard.print();
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
map<Situation, PokerCard> model;

GameScore solve(Situation now)
{
    if(dp.find(now)!=dp.end()){
        return dp[now];
    }
    GameScore ret;
    PokerCard choice;
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
        sit.existCard=0;
        sit.nextCard=noneCard;
        sit.mateCard=noneCard;
        sit.previousCard=noneCard;
        bool follow=0;
        for(int i=0;i<now.selfPlayer.size();i++){
            if(now.selfPlayer[i].color==now.nextCard.color){
                follow=1;
                break;
            }
        }
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==1&&now.selfPlayer[i].color!=now.nextCard.color) continue;
            Situation nextSit=sit;
            int battleResult=battle(now.nextCard, now.mateCard, now.previousCard, now.selfPlayer[i]);
            if(battleResult==1){//next win
                nextSit.selfPlayer=now.nextPlayer;
                nextSit.nextPlayer=now.matePlayer;
                nextSit.matePlayer=now.previousPlayer;
                nextSit.previousPlayer=now.selfPlayer;
                nextSit.previousPlayer.erase(nextSit.previousPlayer.begin()+i);
                GameScore gs=solve(nextSit);
                gs.scoreWe++;
                if(gs.scoreThey>ret.scoreThey){
                    ret=gs;
                    choice=now.selfPlayer[i];
                }
            }
            else if(battleResult==2){//mate win
                nextSit.selfPlayer=now.matePlayer;
                nextSit.nextPlayer=now.previousPlayer;
                nextSit.matePlayer=now.selfPlayer;
                nextSit.previousPlayer=now.nextPlayer;
                nextSit.matePlayer.erase(nextSit.matePlayer.begin()+i);
                GameScore gs=solve(nextSit);
                int tmp=gs.scoreWe;
                gs.scoreWe=gs.scoreThey;
                gs.scoreThey=tmp;
                gs.scoreThey++;
                if(gs.scoreThey>ret.scoreThey){
                    ret=gs;
                    choice=now.selfPlayer[i];
                }
            }
            else if(battleResult==3){//previous win
                nextSit.selfPlayer=now.previousPlayer;
                nextSit.nextPlayer=now.selfPlayer;
                nextSit.matePlayer=now.nextPlayer;
                nextSit.previousPlayer=now.matePlayer;
                nextSit.nextPlayer.erase(nextSit.nextPlayer.begin()+i);
                GameScore gs=solve(nextSit);
                gs.scoreWe++;
                if(gs.scoreThey>ret.scoreThey){
                    ret=gs;
                    choice=now.selfPlayer[i];
                }
            }
            else{//self win
                nextSit.selfPlayer=now.selfPlayer;
                nextSit.nextPlayer=now.nextPlayer;
                nextSit.matePlayer=now.matePlayer;
                nextSit.previousPlayer=now.previousPlayer;
                nextSit.selfPlayer.erase(nextSit.selfPlayer.begin()+i);
                GameScore gs=solve(nextSit);
                int tmp=gs.scoreWe;
                gs.scoreWe=gs.scoreThey;
                gs.scoreThey=tmp;
                gs.scoreThey++;
                if(gs.scoreThey>ret.scoreThey){
                    ret=gs;
                    choice=now.selfPlayer[i];
                }
            }
        }
        int tmp=ret.scoreWe;
        ret.scoreWe=ret.scoreThey;
        ret.scoreThey=tmp;
        model[now]=choice;
        if(now.existCard==0) dp[now]=ret;
        for(int i=0;i<now.selfPlayer.size();i++){
            if(now.selfPlayer[i]==choice) continue;
            Situation badSit=now;
            badSit.NextStep(now.selfPlayer[i]);
            if(model.find(badSit)!=model.end()) model.erase(badSit);
        }
        return ret;
    }
    sit.selfPlayer=now.nextPlayer;
    sit.nextPlayer=now.matePlayer;
    sit.matePlayer=now.previousPlayer;
    sit.previousPlayer=now.selfPlayer;
    if(now.existCard==0){
        sit.existCard=1;
        sit.nextCard=noneCard;
        sit.mateCard=noneCard;
        for(int i=0;i<now.selfPlayer.size();i++){
            Situation nextSit=sit;
            nextSit.previousCard=nextSit.previousPlayer[i];
            nextSit.previousPlayer.erase(nextSit.previousPlayer.begin()+i);
            GameScore gs=solve(nextSit);
            if(gs.scoreThey>ret.scoreThey){
                ret=gs;
                choice=now.selfPlayer[i];
            }
        }
    }
    else if(now.existCard==1){
        sit.existCard=2;
        sit.nextCard=noneCard;
        sit.mateCard=now.previousCard;
        bool follow=0;
        for(int i=0;i<now.selfPlayer.size();i++){
            if(now.selfPlayer[i].color==now.previousCard.color){
                follow=1;
                break;
            }
        }
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==1&&now.selfPlayer[i].color!=now.previousCard.color) continue;
            Situation nextSit=sit;
            nextSit.previousCard=nextSit.previousPlayer[i];
            nextSit.previousPlayer.erase(nextSit.previousPlayer.begin()+i);
            GameScore gs=solve(nextSit);
            if(gs.scoreThey>ret.scoreThey){
                ret=gs;
                choice=now.selfPlayer[i];
            }
        }
    }
    else if(now.existCard==2){
        sit.existCard=3;
        sit.nextCard=now.mateCard;
        sit.mateCard=now.previousCard;
        bool follow=0;
        for(int i=0;i<now.selfPlayer.size();i++){
            if(now.selfPlayer[i].color==now.mateCard.color){
                follow=1;
                break;
            }
        }
        for(int i=0;i<now.selfPlayer.size();i++){
            if(follow==1&&now.selfPlayer[i].color!=now.mateCard.color) continue;
            Situation nextSit=sit;
            nextSit.previousCard=nextSit.previousPlayer[i];
            nextSit.previousPlayer.erase(nextSit.previousPlayer.begin()+i);
            GameScore gs=solve(nextSit);
            if(gs.scoreThey>ret.scoreThey){
                ret=gs;
                choice=now.selfPlayer[i];
            }
        }
    }
    int tmp=ret.scoreWe;
    ret.scoreWe=ret.scoreThey;
    ret.scoreThey=tmp;
    if(now.existCard==0) dp[now]=ret;
    model[now]=choice;
    for(int i=0;i<now.selfPlayer.size();i++){
        if(now.selfPlayer[i]==choice) continue;
        Situation badSit=now;
        badSit.NextStep(now.selfPlayer[i]);
        if(model.find(badSit)!=model.end()) model.erase(badSit);
    }
    return ret;
}

void print(Situation now)
{
    if(model.find(now)==model.end()){
        printf("end\n");
        return;
    }
    PokerCard currentCard=model[now];
    currentCard.print();
    now.NextStep(currentCard);
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
    int cardNum;
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
    printf("next Player:\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.nextPlayer.push_back(pc);
    }
    printf("mate Player:\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.matePlayer.push_back(pc);
    }
    printf("previous Player:\n");
    for(int i=0;i<cardNum;i++){
        scanf("%s %d", colorStr, &pointInt);
        pc.color=colorStr[0];
        pc.point=pointInt;
        newSit.previousPlayer.push_back(pc);
    }
    GameScore gs=solve(newSit);
    printf("%d %d\n", gs.scoreWe, gs.scoreThey);
    print(newSit);
    return 0;
}