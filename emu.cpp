#include<bits/stdc++.h>
#define ll long long int 
#define F first
#define S second
#define pb push_back
using namespace std;

map<int,string> mnemonic={{0,"ldc"},{1,"adc"},{2,"ldl"},{3,"stl"},{4,"ldnl"},{5,"stnl"},
{6,"add"},{7,"sub"},{8,"shl"},{9,"shr"},{10,"adj"},{11,"a2sp"},{12,"sp2a"},{13,"call"},
{14,"return"},{15,"brz"},{16,"brlz"},{17,"br"},{18,"halt"},{1000,"data"},{100,"set"}};

set<int> type={6,7,8,9,11,12,14,18};
int stop_print;
int A,B,SP,PC,cnt_PC;
vector<int> instructions;
int cnt=0;
int const N=1<<24;
int memory[N];

void ldc(int n){
    B=A;A=n;
}
void adc(int n){
    A=A+n;
}
void ldl(int n){
    B=A;
    if(SP+n>=N||SP+n<0){
        cout<<"Segmentation fault: Out of range memory access Memory \n";
        exit(0);
    }
    
    A=memory[SP+n];
}
void stl(int n){
    if(SP+n>=N||SP+n<0){
        cout<<"Segmentation fault: Out of range memory access Memory \n";
        exit(0);
    }
    
    memory[SP+n]=A;
    A=B;
}
void ldnl(int n){
    if(A+n>=N||A+n<0){
        cout<<"Segmentation fault: Out of range memory access Memory \n";
        exit(0);
    }
    
    A=memory[A+n];
}
void stnl(int n){
    if(A+n>=N||A+n<0){
        cout<<"Segmentation fault: Out of range memory access Memory \n";
         exit(0);
    }
    
    memory[A+n]=B;
}
void add(int n){
    A=B+A;
}
void sub(int n){
    A=B-A;
}
void shl(int n){
    A=B*2;
}
void shr(int n){
    A=B/2;
}
void adj(int n){
    SP=SP+n;
}
void a2sp(int n){
    SP=A;
    A=B;
}
void sp2a(int n){
    B=A;
    A=SP;
}
void call(int n){
    B=A;
    A=PC;
    PC=n;
}
void return1(int n){
    PC=A;
    A=B;
}
void brz(int n){
    if(A==0)PC=PC+n;
}
void brlz(int n){
    if(A<0)PC=PC+n;
}
void br(int n){
    PC=PC+n;
}

void ( *fun_arr[])(int)={ldc,adc,ldl,stl,ldnl,stnl,add,sub,shl,shr,adj,a2sp,sp2a,call,return1,brz,brlz,br};

string convert_hex(ll n){
    // cout<<"n: "<<n<<endl;
    // if(n<0)n=twos_compliment(abs(n));
    string ans ="00000000";
    int ind=7;
    while(n){
        int k=n%16;
        if(k<=9)ans[ind]='0'+k;
        else ans[ind]='a'+k-10;
        ind--;
        n/=16;
    }
    return ans;
}
void print_instruction(unsigned int n){
        cout<<mnemonic[n%256];
    unsigned int offset=n/256;
    if(((offset>>23)&1)){
        offset|=(255<<24);
    }
    if(type.find(n%256)==type.end()){
            printf(" %08x",offset);
    }
        printf("\n");

}
void print_state(){
    printf("A=%08X \t B=%08X \t SP=%08X \t PC=%08X\n",A,B,SP,PC);
}
int perform_instruction(){
    cnt++;
    unsigned int n=instructions[PC];
    
    unsigned int opcode=n%256;
    unsigned int offset=n/256;
    if(mnemonic.find(opcode)==mnemonic.end()){
        cout<<"Invalid instruction:: no vaild mnemonic found\n";
        exit(0);
    }
    if(!stop_print){
        printf("PC=%08X    %08X\t",(unsigned int)PC,(unsigned int)instructions[PC]);
        print_instruction(instructions[PC]);
    }

    PC++;

    if(((offset>>23)&1)){
        offset|=(255<<24);
    }
    
    (*fun_arr[opcode])(offset);
    if(opcode==18)PC=cnt_PC;

    
}
int main(int argc,char* argv[])
{   
    A=B=PC=SP=0;
    int read;
    ifstream infile;
    infile.open (argv[1], ios::in | ios::binary);
    string input;
    string word;
    int num=0,flag=0;
    cout<<"You have the following instructions available for this emulator:\n";
    cout<<"Trace: write -t to follow up a single command\n";
    cout<<"Trace number: write -t num(a number) to follow up num commands\n";
    cout<<"Run at once: write -f to run all commands at once\n";
    cout<<"Future instructions: write -u to see the upcoming instructions\n";
    cout<<"Show memory dump before execution: write -bd to see current memory \n";
    cout<<"Show a memory location: write -data value to see current memory location \n";
    cout<<"Show memory dump after execution: write -ad to see memory after execution\n";
    cnt_PC=0;
    while(!infile.eof()){
        infile.read((char*)&read,sizeof(int));
        memory[cnt_PC]=read;
        instructions.pb(read);
        cnt_PC++;
    }
    
    while(PC<cnt_PC){
        getline(cin,input);
        stringstream input_stream(input);
        input_stream>>word;
        if(word=="-t"){

            if(input_stream.eof()){
                perform_instruction();
                print_state();
            }else{
                input_stream>>num;

                for(int i=0;i<num;i++)perform_instruction(),print_state();
            }
        }else if(word=="-f"){

            while(cnt_PC!=PC)perform_instruction(),print_state();

        }else if(word=="-u"){
            int PC_copy=PC;
            for(int i=0;i<10&&PC<cnt_PC;i++){
                printf("PC=%08X  %08X ",PC,instructions[PC]);
                print_instruction(instructions[PC]);
                PC++;
            }
            PC=PC_copy;
        }else if(word=="-bd"){
            for(int i=0;i<cnt_PC;i++){
                printf("%08x\t",i);
                for(int j=0;j<4;j++){
                    printf("%08X ",memory[i]);
                    i++;
                    if(i>=cnt_PC)break;
                }
                printf("\n");
                i--;
            }

        }else if(word=="-data"){
            input_stream>>hex>>num;
            if(num<0||num>=N){
                cout<<"Warning: Out of bound memory fetch\n";
            }else{
                printf("[%08X] = %08X\n",num,memory[num]);  
            }
        }else if(word=="-ad"){
            stop_print=1;
            cout<<"DATA SEGMENT:\n";
            while(cnt_PC!=PC)perform_instruction();
            for(int i=0;i<cnt_PC;i++){
                printf("%08x\t",i);
                for(int j=0;j<4;j++){
                    printf("%08X ",memory[i]);
                    i++;
                    if(i>=cnt_PC)break;
                }
                printf("\n");
                i--;
            }

        }

    }
    cout<<"end of file is reached\n";
    
}