#include<bits/stdc++.h>
#include <iomanip>
#define ll long long int
#define ld long double
#define f first
#define inf 100000000000000000
#define ii pair<ll,ll>
#define vi vector<ll>
#define vii vector<ii>
#define MS0(x) memset(x,0,sizeof(x))
#define MS1(x) memset(x,-1,sizeof(x))
#define s second
#define pb push_back
#define forn(i,n) for(int i=0;i<(int)n;i++)
#define Sort(a) sort(a.begin(),a.end());
#define debug(x) cerr << #x << " is " << x << endl;
using namespace std;
ll const mod=998244353;

int error;	// Gloabal variable for error 

// mnemonic table
map<string,int> mnemonic={{"ldc",0},{"adc",1},{"ldl",2},{"stl",3},{"ldnl",4},{"stnl",5},
{"add",6},{"sub",7},{"shl",8},{"shr",9},{"adj",10},{"a2sp",11},{"sp2a",12},{"call",13},
{"return",14},{"brz",15},{"brlz",16},{"br",17},{"halt",18},{"data",0},{"set",0}};

// non operand instructions
set<string> type={"add","sub","shl","shr","a2sp","sp2a","return","halt"};

// branch instructions
set<string> branch_mnemonics={"brz","br","brlz"};

// label tables
map<string,int> used_labels;
map<string,ll> labels;
vector<pair<string,int> > literal;
map<string,int> constants;
map<string,int> defined_labels;

// file variables
ofstream logfile;
ofstream listfile;
ifstream asmfile;
ofstream outfile;

// input format processing

stringstream break_line(stringstream &temp){
	stringstream temp1;
	char ch;
	string word;
	vector<string> vec;
	while(temp>>word)vec.pb(word);
	string ans="";
	for(auto i:vec){
		ans="";
		for(auto j:i){
			j=tolower(j);
			if(j==':'||j==';'){
				temp1<<ans<<" "<<j<<" ";
				ans="";
				continue;
			}
			ans+=j;
		}
		temp1<<ans<<" ";
	}
	return temp1;
}

// function to check if a word is mnemonic
bool check_mnemonic(string word){
	if(mnemonic.find(word)!=mnemonic.end())return 1;
	return 0;
}

// function to check mneomonic type
int check_mn_type(string word){
	if(type.find(word)!=type.end())return 1;
	return 2;
}


// calculating 2s compliment of number ( seperate for 24 bit and 32 bit data)
ll twos_compliment(ll n,int type){
	if(!n)return n;
	ll num=0;
	if(type){
		num=1ll<<32;
	}else{
		num=1ll<<24;
	}
	n=n%num;
	num--;
	n=num-n;
	n++;
	return n;
}


// function to check if a given number is valid
pair<ll,ll> valid_number(string word){
	int flag_compliment=0;
	if(word[0]=='-'){
		flag_compliment=1;
	}
	if(word[0]=='+'||word[0]=='-'){
		word=word.substr(1);
	}
	pair<ll,ll> ans;
	ans.f=1;
	ans.s=0;
	if(word[0]<'0'||word[0]>'9'){
		ans.f=0;
		return ans;
	}
	if(word[0]!='0'){
		for(auto i:word){
			int k=i-'0';
			if(k<0||k>9){
				ans.f=0;
				return ans;
			}
			ans.s*=10;
			ans.s+=k;
		}
	}else{
		if(word.length()==1){
			return ans;
		}
		if(word[1]=='x'){
			word=word.substr(2);
			for(auto i:word){
				int k=i-'0';
				ans.s*=16;
				if(k>=0&&k<=9){
					ans.s+=k;
				}else{
					k=i-'a';
					if(k>=0&&k<=6){
						ans.s+=10+k;
					}else{
						ans.f=0;
						return ans;
					}
				}
				
			}
		}else{
			for(auto i:word){
				ans.s*=8;
				int k=i-'0';
				if(k>=0&&k<=7){
					ans.s+=k;
				}else{
					ans.f=0;
					return ans;
				}
				
			}
		}
	}
	if(flag_compliment){
		ans.s*=-1;
	}
	return ans;
}


// check if a label is repeated
bool check_rep_label(string word){
	if(labels.find(word)!=labels.end())return 1;
	return 0;
}


// to check if a label is valid;
bool check_valid_label(string word){
	int j=0;
	for(auto i:word){
		if(!j&&(i<'a'||i>'z')){
			return 0;
		}
		if(!(i=='_'||(i>='a'&&i<='z')||(i>='0'&&i<='9')))return 0;
		j++;
	}
	return 1;
}


// error table
void print_error(int flag_bogus,int line){
	if(flag_bogus==1){
		error=1;
		logfile<<"Error at line "<<line<<": Bogus mnemonic\n";
	}else if(flag_bogus==2){
		error=1;
		logfile<<"Error at line "<<line<<": Invalid Label name\n";
	}else if(flag_bogus==3){
		error=1;
		logfile<<"Error at line "<<line<<": Repeated Label name\n";
	}else if(flag_bogus==4){
		error=1;
		logfile<<"Error at line "<<line<<": Reference to an invalid Label name\n";
	}else if(flag_bogus==5){
		error=1;
		logfile<<"Error at line "<<line<<": Label name can not be a keyword\n";
	}else if(flag_bogus==6){
		error=1;
		logfile<<"Error at line "<<line<<": missing operand\n";
	}else if(flag_bogus==7){
		error=1;
		logfile<<"Error at line "<<line<<": unexpected operand\n";
	}else if(flag_bogus==8){
		error=1;
		logfile<<"Error at line "<<line<<": Wrong format of numbers\n";
	}else if(flag_bogus==9){
		error=1;
		logfile<<"Error at line "<<line<<": invalid input format\n";
	}else if(flag_bogus==10){
		error=1;
		logfile<<"Error at line "<<line<<": There can not be two labels in same line\n";
	}else if(flag_bogus==11){
		error=1;
		logfile<<"Error at line "<<line<<": No label for set instruction\n";
	}else if(flag_bogus==12){
		error=1;
		logfile<<"Error at line "<<line<<": label not defined\n";
	}
}


// warning table
void print_waring(int flag_warning,int line,string word){
	if(flag_warning==1){

		logfile<<"Waring :: unused label: \""<<word<<"\" at line "<<line<<endl;

	}else if(flag_warning==2){

		logfile<<"Waring :: Infintite loop at line: "<<line<<endl;

	}
}

// Converting a decimal number to hex
string convert_hex(ll n,int type){
	if(n<0)n=twos_compliment(abs(n),type);
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



// function to generate listing file
void print_listing(ll pc,ll offset,ll opcode,string s){
	listfile<<convert_hex(pc,0)<<" ";
	if(opcode==-1)listfile<<"        ";
	else {

		if(opcode==-2){
			listfile<<convert_hex(offset,1)<<" ";	
		}else listfile<<convert_hex(offset,0).substr(2)<<convert_hex(opcode,0).substr(6)<<" ";

	}
	listfile<<s<<endl;
}


// Parser 1
void parser1(string name,char* file_name){

	asmfile.open(file_name);	//read file

	logfile.open(name+".log");	//log file

	string s;
	int pc=0,line=0;	//counter for pc and line address
	error=0;

	while(getline(asmfile,s)){

		line++;
		if(s.empty())continue;
		stringstream temp(s);
		stringstream temp1=break_line(temp);	//final string stream to use
		string word,pre;
		int read_mn=0,flag_bogus=0;
		while(temp1>>word){
			
			if(word[0]==';')break;

			if(check_mnemonic(word)){
				if(word=="set"){
					if(!read_mn){
						flag_bogus=11;
						break;
					}else{
						temp1>>word;
						if(temp1.eof()){
							flag_bogus=6;
							break;
						}
						ii pp=valid_number(word);
						if(!pp.f){
							flag_bogus=8;
							break;
						}else{
							labels[pre]=pp.s;
							labels[word]=pp.s;
						}
						temp1>>word;

						if(temp1.eof()){
							pc++;
							break;
						}
						if(word[0]!=';'){
							flag_bogus=7;
							break;
						}
						pc++;
						
					}
				}
				int mn_type=check_mn_type(word);


				if(mn_type==1){

					temp1>>word;
					if(temp1.eof()){
						pc++;
						break;
					}
					if(word[0]!=';'){
						flag_bogus=7;
						break;
					}
					pc++;

				}else{
					temp1>>word;
					if(temp1.eof()){
						flag_bogus=6;
						break;
					}
					if(word.empty()||word[0]==';'){
						flag_bogus=6;
						break;
					}
					if(word[0]>='0'&&word[0]<='9'||word[0]=='+'||word[0]=='-'){
						ii pp=valid_number(word);
						if(!pp.f){
							flag_bogus=8;
							break;

						}else{
							constants[word]=pc;
							labels[word]=pp.s;

						}
					}else{

						if(word[0]==':'){
							flag_bogus=5;
							break;
						}
						if(word[0]<'a'||word[0]>'z'){

							flag_bogus=4;
							break;
						}else{
							used_labels[word]=line;
						}

					}
					temp1>>word;

					if(temp1.eof()){
						pc++;
						break;
					}
					if(word[0]!=';'){
						flag_bogus=9;
						break;
					}
					pc++;
				}
				break;
			}else{
				pre=word;
				if(read_mn){
					temp1>>word;
					if(word[0]==':'){
						flag_bogus=10;
					}else{
						flag_bogus=1;
					}
					break;
					
				}
				if(check_rep_label(word)){
					error=1;
					flag_bogus=3;
					break;
				}
				if(!check_valid_label(word)){
					flag_bogus=2;
				}
				
				temp1>>word;
				if(word[0]!=':'){
					flag_bogus=1;
					break;
				}else{
					if(flag_bogus)break;
				}
				labels[pre]=pc;
				defined_labels[pre]=line;
				read_mn=1;
				continue;
			}
		}

		print_error(flag_bogus,line);


	}
	for(auto i:used_labels){
		if(labels.find(i.f)==labels.end()){
			print_error(12,i.s);
		}
	}
	for(auto i:defined_labels){
		if(used_labels.find(i.f)==used_labels.end()){
			print_waring(1,i.s,i.f);
		}else{
			if(i.s==used_labels[i.f]){
				print_waring(2,i.s,"");
			}
		}
	}
	
	asmfile.close();


}

// parser 2
void parser2(string name,char* file_name){
	listfile.open(name+".l");	//listing file
	asmfile.open(file_name);		//input file
	outfile.open(name+".o",ios::binary|ios::out);

	string s;
	int pc=0,line=0;
	error=0;
	while(getline(asmfile,s)){
		line++;
		if(s.empty())continue;
		stringstream temp(s);
		stringstream temp1=break_line(temp);
		string word,pre;
		int read_mn=0,flag_bogus=0;
		ll opcode=-1,offset=0;
		while(temp1>>word){
			
			if(word[0]==';')break;
			string mn=word;
			if(check_mnemonic(word)){
				if(word=="set"){
					temp1>>word;
					opcode=-2;
					offset+=labels[word];
					break;
				}
				int mn_type=check_mn_type(word);
				opcode = mnemonic[word];
				if(mn_type==1){
					temp1>>word;
				}else{
					temp1>>word;
					if(labels.find(word)==labels.end()){
						error=1;
						logfile<<"Error at line "<<line<<": undefined label\n";
						break;
					}
					if(mn=="data"){

						opcode=-2;
						offset+=labels[word];

					}
					else {
						if(branch_mnemonics.find(mn)!=branch_mnemonics.end()){
							if(labels[word]==pc){
								print_waring(2,line,"");
							}
							offset += (labels[word]-pc-1);
						}else offset +=  labels[word];
					}

				}
				break;
			}else{
				temp1>>word;
				continue;
			}

		}
		ll instruction_code=opcode;

		instruction_code+=256*(offset<0?twos_compliment(abs(offset),0):offset);

		if(opcode==-2){

			instruction_code=(offset<0?twos_compliment(abs(offset),1):offset);
			
		}
		print_listing(pc,offset,opcode,s);
		if(opcode!=-1){
			outfile.write((char*)&instruction_code,sizeof(int));
			pc++;
		}


	}
	if(error){
		listfile.close();
		listfile.open(name+".l");
		listfile.close();
		outfile.close();
		outfile.open(name+".o",ios::binary|ios::out);
		outfile.close();
	}

}
int main(int argc, char* argv[])
{	
	string s=argv[1];
	string name="";
	for(auto i:s){
		if(i=='.')break;
		name+=i;
	}
	parser1(name,argv[1]);

	if(error){
		return 0;
	}	
	parser2(name,argv[1]);

}