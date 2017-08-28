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
    void printPoint()
    {
        if(point==14){
            printf("A ");
        }
        else if(point==13){
            printf("K ");
        }
        else if(point==12){
            printf("Q ");
        }
        else if(point==11){
            printf("J ");
        }
        else{
            printf("%d ", point);
        }
    }
    void printColor()
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
            printf("no card ");
        }
    }
};