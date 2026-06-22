#include "water_seven.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////
//Task 0
string validNames[7] = {"LUFFY", "ZORO", "SANJI", "NAMI", "CHOPPER", "USOPP", "ROBIN"};
bool readInput(
    const string &filename,
    char character[FIXED_CHARACTER][MAX_NAME],
    int hp[FIXED_CHARACTER],
    int skill[FIXED_CHARACTER],
    int &shipHP,
    int &repairCost)
{
    ifstream filein(filename);
    if(!filein) return false;

    int i=0;
    for(int j=0; j<FIXED_CHARACTER; j++){
        character[j][0]='\0';
    }
    string s ; int a,b;
    while(filein >> s >> a >> b){
        if(s.find("GOING") != string::npos && s.find("MERRY") != string::npos){
            if(s == "GOING_MERRY"){
                if(a<0) shipHP=0;
                else if(a>1000) shipHP=1000;
                else shipHP=a;
                
                if(b<0) repairCost=0;
                else if(b>3000) repairCost=3000;
                else repairCost=b;
            }
            else continue;
        }

        else{
            string tmp="";
            for(char c:s) tmp += c;
            bool valid = false;
            for(int k=0; k<7; k++){
                if(tmp == validNames[k]){
                    valid = true;
                    break;
                }
            }
            if(!valid) continue;
            bool used = false;
            int j=0;
            for(; j < FIXED_CHARACTER; j++){
                if(tmp == string(character[j])){
                    used = true;
                    break;
                }
            }
            if(!used){
                if(i >= FIXED_CHARACTER) continue;
                
                strcpy(character[i], tmp.c_str());

                if(a<0) hp[i]=0;
                else if(a>1000) hp[i]=1000;
                else hp[i]=a;
                
                if(b<0) skill[i]=0;
                else if(b>100) skill[i]=100;
                else skill[i]=b;
                
                i++;
            }else{
                if(a<0) hp[j]=0;
                else if(a>1000) hp[j]=1000;
                else hp[j]=a;
                
                if(b<0) skill[j]=0;
                else if(b>100) skill[j]=100;
                else skill[j]=b;
            }
        }
    }
    filein.close();
    return true;
}

// Task 1
int digitSum(int n){
    int sum=0;
    while(n!=0){
        sum+=n%10;
        n/=10;
    }
    return sum;
}
bool perfectNum(int n){
    if(n<6) return false;
    int sum=1, tmp=n;
    for(int i=2; i*i<=n; i++){
        if(n%i==0){
            sum+=i;
            if(n/i!=i) sum+=n/i;
        }
    }
    return sum==tmp;
}
int damageEvaluation(int shipHP, int repairCost) {
    if(shipHP < 455 && perfectNum(digitSum(shipHP))){
        double tmp = (1.5)*repairCost ;
        repairCost = int(ceil(tmp));
    }
    if(repairCost > 3000) repairCost = 3000;
    return repairCost;
}

// Task 2
int conflictSimulation(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int shipHP, int repairCost)
{
    int skill_Luffy=0, skill_Usopp=0;
    int conflictIndex=0;
    int event[6]={255,20,50,70,90,100};
    for(int i=0; i<7; i++){
        if(string(character[i]) =="LUFFY"){
            skill_Luffy=skill[i];
        }
        if(string(character[i]) == "USOPP"){
            skill_Usopp=skill[i];
        }
    }   
    conflictIndex = ceil(skill_Luffy - skill_Usopp + (1.0)*repairCost/100 + (1.0)*(500-shipHP)/50);
    for(int i=0; i<10; i++){
        if(conflictIndex >= 255){
            break;
        }
        else{
            int id = conflictIndex % 6;
            conflictIndex += event[id];
        }
    }
    return conflictIndex;
}
// Task 3
int best_subset = -1;
int best_size = INT_MAX;
int best_cost = INT_MAX;
void resolveDuel(
    char character[FIXED_CHARACTER][MAX_NAME],
    int hp[FIXED_CHARACTER],
    int skill[FIXED_CHARACTER],
    int conflictIndex,
    int repairCost,
    char duel[FIXED_CHARACTER][MAX_NAME])
{
    int support_power[5], cost[5], index=0;
    string support_character[5];
    int skill_luffy, skill_usopp;
    for(int i=0; i<7; i++){
        if(string(character[i]) =="LUFFY"){
            skill_luffy=skill[i];
        }
        else if(string(character[i]) == "USOPP"){
            skill_usopp=skill[i];
        }
        else{
            support_character[index] = string(character[i]);
            support_power[index] = skill[i];
            cost[index] = (hp[i] % 10) + 1;
            ++index;
        }
    } 
    int tank_usopp = ceil(skill_usopp + (1.0)*conflictIndex/20 + (1.0)*repairCost/500);
    if(skill_luffy >= tank_usopp){
        return ;
    }   
    for(int i=0; i<(1<<5); i++){
        int subset_size=0, support_sum=0, cost_sum=0;
        for(int j=0; j<5; j++){
            if(i&(1<<j)){
                support_sum +=support_power[j];
                cost_sum += cost[j];
                subset_size++;
            }
        }
        if(skill_luffy + support_sum >= tank_usopp){
            if(cost_sum < best_cost || (cost_sum == best_cost && subset_size < best_size)){
                best_cost = cost_sum;
                best_size = subset_size;
                best_subset = i;
            }
        }
    }
    int duel_index=0;
    if(best_subset!=-1){
        for(int j=0; j<=4; j++){
            if(best_subset&(1<<j)){
                strcpy(duel[duel_index], support_character[j].c_str());
            }
            duel_index++;
        }
    }
    return;
}

// Task 4
string rev(string &s){
    string reverse="";
    for(int i=s.size()-1; i>=0; i--) reverse += s[i];
    return reverse;
}
void decodeCP9Message(char character[FIXED_CHARACTER][MAX_NAME], 
    int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex, 
    int repairCost, char cipherText[], char resultText[])
{
    resultText[0]='\0';
    int key = (conflictIndex + repairCost) % 26;
    int B = (key % 5) + 4;
    //checksum
    string message_input = cipherText;
    int k=message_input.rfind("#");
    int char_sum=0;
    for(int i=0; i<=k-1; i++){
        char_sum += int(message_input[i]);
    }
    int check_sum=stoi(message_input.substr(k+1));
    if((char_sum%100) != check_sum){
        string res="";
        strcpy(resultText, res.c_str());
        return;
    }
    //tách message và đảo
    string message = message_input.substr(0,k);
    string a[100];
    int index=0;
    for (int i = 0; i < int(message.size()); i += B) {
        if (index < 100) {
            a[index++] = message.substr(i, B);
        }
    }
    for(int i=0; i<index; i++){
        a[i]=rev(a[i]);
        for(char &c:a[i]){
            int keyAlpha = key%26;
            int keyDigit = key%10;
            if(c>='A' && c<='Z'){
                c = char((c-'A'-keyAlpha+26)%26 + 'A');
            }
            else if(c>='a' && c<='z'){
                c = char((c-'a'-keyAlpha+26)%26 + 'a');
            }
            else if(c>='0' && c<='9'){
                c = char((c-'0'-keyDigit+10)%10 + '0');
            }
        }
        strcat(resultText, a[i].c_str());
    }
    string result = resultText;
    if(result.find("CP9")!=string::npos || result.find("ENIESLOBBY")!=string::npos){
        result += "_TRUE";
    }
    else{
        result += "_FALSE";
    }
    strcpy(resultText, result.c_str());
    return ;
}

// Task 5
//5.1
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols){
    int maxRowSum = INT_MIN;
    int maxCell = INT_MIN;
    for(int i=0; i<rows; i++){
        int rowSum=0;
        for(int j=0; j<cols; j++){
            if(grid[i][j] >= 0){
                rowSum+=grid[i][j];
            }
            maxCell=max(maxCell,grid[i][j]);
        }
        maxRowSum=max(rowSum,maxRowSum);
    }
    return maxRowSum + maxCell ;
}
//5.2
int dp[55][55];

bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit){
    if(grid[0][0]==-1 || grid[rows-1][cols-1]==-1){
        return false;
    }
    dp[0][0] = grid[0][0];
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(i==0 && j==0) continue;
            if(grid[i][j]==-1){
                dp[i][j] = INT_MAX;
                continue;
            }
            else{
                int minDanger = INT_MAX;
                if(i > 0 && dp[i-1][j] != INT_MAX){
                    minDanger = dp[i-1][j];
                }
                if(j > 0 && dp[i][j-1] != INT_MAX){
                    if(dp[i][j-1] < minDanger){
                        minDanger = dp[i][j-1];
                    }
                }
                // Cập nhật
                if(minDanger != INT_MAX) {
                    dp[i][j] = grid[i][j] + minDanger;
                }else{
                    dp[i][j] = INT_MAX;
                }
            }
        }
    }
    if(dp[rows-1][cols-1]==INT_MAX) return false;
    else if(dp[rows-1][cols-1] <=dangerLimit ) return true;
    else return false;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
