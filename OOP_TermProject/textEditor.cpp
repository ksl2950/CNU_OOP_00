#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

static int first = 0;
static int last = 19;

class CmdStrategy {
private:
    int cnt = 0;
    vector <string>::iterator it;
    char s[100]; 
public:
    virtual void execute(string cmd,vector <vector<string> >& content) = 0;
    void printData(vector <vector<string> >& content){
        cout << "--------------------------------------------------------------------------------" << endl;
        for (int i = first; i <= last; i++) {
            if (i+1 < 10)
                cout << " " << i + 1 << "| ";
            else
                cout << i + 1 << "| ";
            for (int j = 0; j < content[i].size(); j++) {
                cout << content[i][j];
            }
            cout << endl;
            if (i + 1 == content.size()) {
                break;
            }
        }
    }
};

class Cmd_n :public CmdStrategy {

public: 
    void execute(string cmd,vector <vector<string> >& content) {    
        if (last == content.size()-1) {
            throw string("This is the last page!");
        }
        else if (last + 20 >= content.size()) { // 20줄이 전부 채워지지 않는 경우 
            last = content.size() - 1;
            first = last - 19;
        }
        else {
            first += 20;
            last += 20;
        }
        printData(content);
    }
};

class Cmd_p :public CmdStrategy {

public:
    void execute(string cmd, vector <vector<string> >& content) {
        first -= 20;
        last = first + 19;
        if (first < 0 && last >=0) {
            first = 0;
            last = 19;
            printData(content);
        }
        else if (last < 0) {
            first = 0;
            last = 19;
            throw string("This is the first page!");
        }
        else {
            printData(content);
        }
    }
};

class Cmd_i :public CmdStrategy{
private: 
    string temp;
    vector <string> x;
    vector <string>::iterator it;

public:
    void execute(string cmd, vector <vector<string> >& content) {

        if (cmd[1] != '(' || cmd[cmd.size() - 1] != ')' || cmd.size() <= 1) { // 괄호가 알맞지 않은 경우,또는 괄호 없는 경우 
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }

        temp = cmd.substr(2, cmd.size()- 3);//괄호 제거 
        istringstream ss(temp);
        string buffer;
        x.clear();
        int index = 0;

        while (getline(ss, buffer, ',')){  // 콤마 단위로 잘라서 x벡터에 넣는다. 
            int len1 = buffer.size();
            buffer.erase(remove(buffer.begin(), buffer.end(), ' '), buffer.end());
            int len2 = buffer.size();
            if (len1 != len2){
                throw string("괄호 안에 공백이 존재합니다. 다시 입력해주세요!");
            }
            x.push_back(buffer);
        }
        
        if (x.size() != 3) { // 인자로 3개가 들어오지 않은 경우
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }

        if (!atoi(x[0].c_str()) || !atoi(x[1].c_str())) { //행 또는 열이 숫자가 아닌 경우 
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }
        if (x[2].size() > 75) { //입력받은 문자열이 75바이트가 넘는 경우 
            throw string("문자열 길이가 너무 깁니다.다시 입력해주세요.");          
        }
       
        int row = stoi(x[0])-1; //행
        int col = stoi(x[1]); //열 
        string target = x[2];

        if (first > row || last < row) {
            throw string("현재 창에 존재하지 않는 위치입니다. 다시 입력해주세요.");
        }
        it = content[row].begin();//초기화
       
        target = target + " ";
        if (col == content[row].size()) { //맨 뒤에 넣을 경우 
            content[row].push_back(target);
        }
        else if(col < content[row].size()) {
            content[row].insert(it + col, target);
        }
        else { // column인덱스가 맞지 않는 경우 
            throw string("입력받은 위치는 존재하지 않습니다. 다시 입력해주세요.");
        }
        
        int i = 0; // 행
        int cnt = 0;
        
        vector <string> elem;
        while (i != content.size()) {
            cnt += 4;
            for(int j = 0; j<content[i].size(); j++) { //0~size-1
                cnt += content[i][j].size();
            } //한줄의 바이트 수 전부센다.
             while (cnt > 76) { //75바이트가 넘는다면 안넘을때까지 팝해서 아랫줄로 넘김 
                string popValue = content[i].back(); //팝할 원소 
                content[i].pop_back();
                cnt -= popValue.size();    
                if (i + 1 != content.size()) {
                    content[i + 1].insert(content[i + 1].begin(), popValue); //다음 행 맨앞에 삽입 
                }
                else {
                    content.push_back(elem);
                    content[i + 1].insert(content[i + 1].begin(), popValue);
                }
             }
             i++;
             cnt = 0;
            
        }
        printData(content);
    }
};

class Cmd_d :public CmdStrategy {
private:
    string temp;
    vector <string> x;
public:
    void execute(string cmd, vector <vector<string> >& content) {
        
        if (cmd[1] != '(' || cmd[cmd.size() - 1] != ')' || cmd.size() <= 1) { // 괄호가 알맞지 않은 경우,또는 괄호 없는 경우 
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }
        temp = cmd.substr(2, cmd.size() - 3);//괄호 제거 
        istringstream ss(temp);
        string buffer;
        x.clear();
        while (getline(ss, buffer, ',')) {  // 콤마 단위로 잘라서 x벡터에 넣는다. 
            int len1 = buffer.size();
            buffer.erase(remove(buffer.begin(), buffer.end(), ' '), buffer.end());
            int len2 = buffer.size();
            if (len1 != len2) {
                throw string("괄호 안에 공백이 존재합니다. 다시 입력해주세요!");
            }
            else if (!atoi(buffer.c_str())) {
                throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
            }
            x.push_back(buffer);
        }

        if (x.size() != 2) { // 인자로 2개가 들어오지 않은 경우
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }
        int row = stoi(x[0]) - 1; //행
        int col = stoi(x[1]) -1; //열 

        if (!(first <= row && row <= last)) { //현재 창에 존재하는 위치가 아닌 경우 
            throw string("현재 창에 존재하지 않는 위치입니다. 다시 입력해주세요.");
        }
        if (col >= content[row].size()) {
            throw string("입력받은 위치는 존재하지 않습니다. 다시 입력해주세요.");
            return;
        }

        int i = row;
        int cnt = 4;
        content[row].erase(content[row].begin() + col);
        if (content[row].empty()) {
            content.erase(content.begin() + row);
            first -= 1;
            last -= 1;
            i -= 1;
        }
       
        while (i+1!= content.size()) {
            for (int j = 0; j < content[i].size(); j++) {
                cnt += content[i][j].size();
            }
            while (content[i + 1][0].size() <= 76 - cnt ) {
                cnt += content[i + 1][0].size();
                content[i].push_back(content[i + 1][0]);//다음행에서 당겨옴 
                content[i + 1].erase(content[i + 1].begin());//다음행 맨 앞원소 삭제 
                if (content[i + 1].empty()) {
                    content.erase(content.begin() + i + 1);
                    if (i + 1 == content.size()) {
                        break;
                    }
                }
            }
            if (i + 1 == content.size()) {
                break;
            }
            cnt = 4;
            i++;
            
        }
      
        printData(content);

    }
};

class Cmd_c :public CmdStrategy {
private:
    string temp;
    int findRow = 0;
    vector <string> x;
public:
    void execute(string cmd, vector <vector<string> >& content) {
        if (cmd[1] != '(' || cmd[cmd.size() - 1] != ')' || cmd.size() <= 1) { // 괄호가 알맞지 않은 경우,또는 괄호 없는 경우 
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }
        temp = cmd.substr(2, cmd.size() - 3);//괄호안의 내용 추출 
        istringstream ss(temp);
        string buffer;
        x.clear();
        while (getline(ss, buffer, ',')) {  // 콤마 단위로 잘라서 x벡터에 넣는다. 
            int len1 = buffer.size();
            buffer.erase(remove(buffer.begin(), buffer.end(), ' '), buffer.end());
            int len2 = buffer.size();
            if (len1 != len2) {
                throw string("괄호 안에 공백이 존재합니다. 다시 입력해주세요!");
            }
            x.push_back(buffer);
        }
        
        if (x.size() != 2) { // 인자로 2개가 들어오지 않은 경우
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }
        if (x[1].size() > 75) { //바꿀 문자열이 75바이트를 넘는 경우 
            throw string("문자열 길이가 너무 깁니다.다시 입력해주세요.");
        }
        string target = x[0]; // 바뀌는 대상
        string change = x[1]; // 바꿀 문자열 
      
        bool flag = false;
        string compare;

        if (target.size() >= change.size()) {
            compare = "smaller";
        }
        else {
            compare = "bigger";
        }

        for (int i = 0; i < content.size(); i++) {
            for (int j = 0; j < content[i].size(); j++) {
                if (content[i][j] == target+" ") {
                    content[i][j] = change + " ";
                    flag = true;
                }
            }
        }
        if (flag == false) {
            throw string("변경하고자 하는 단어가 존재하지 않습니다.");
        }
        else { //변경하고자 하는 단어가 있는 경우
            if (compare == "smaller") { //변경한 단어가 원래단어보다 길이가 짧은 경우 
                int i = 0;
                int cnt = 4;

                while (i != content.size()) {
                    for (int j = 0; j < content[i].size(); j++) {
                        cnt += content[i][j].size();
                    }
                    while (content[i + 1][0].size() <= 76 - cnt) {
                        cnt += content[i + 1][0].size();
                        content[i].push_back(content[i + 1][0]);//다음행에서 당겨옴 
                        content[i + 1].erase(content[i + 1].begin());//다음행 맨 앞원소 삭제 
                    }
                    cnt = 4;
                    i++;

                }
            }
            else if (compare == "bigger") {//변경한 단어가 원래단어보다 길이가 긴 경우 
                int i = 0; // 행
                int cnt = 0;

                vector <string> elem;
                while (i != content.size()) {
                    cnt += 4;
                    for (int j = 0; j < content[i].size(); j++) { //0~size-1
                        cnt += content[i][j].size();
                    } //한줄의 바이트 수 전부센다.
                    while (cnt > 76) { //75바이트가 넘는다면 안넘을때까지 팝해서 아랫줄로 넘김 
                        string popValue = content[i].back(); //팝할 원소 
                        content[i].pop_back();
                        cnt -= popValue.size();
                        if (i + 1 != content.size()) {
                            content[i + 1].insert(content[i + 1].begin(), popValue); //다음 행 맨앞에 삽입 
                        }
                        else {
                            content.push_back(elem);
                            content[i + 1].insert(content[i + 1].begin(), popValue);
                        }
                    }
                    i++;
                    cnt = 0;

                }
            }
            printData(content);
        }
    }
};

class Cmd_s :public CmdStrategy {
private:
    string target;
    int findRow = 0;
    vector <string> x;
public:
    void execute(string cmd, vector <vector<string> >& content) {
        if (cmd[1] != '(' || cmd[cmd.size() - 1] != ')') {
            throw string("입력형식이 잘못되었습니다. 다시 입력해주세요.");
        }
        target = cmd.substr(2, cmd.size() - 3);//괄호안의 내용 추출 
        int len1 = target.size();
        target.erase(remove(target.begin(), target.end(), ' '), target.end());
        int len2 = target.size();
        if (len1 != len2) {
            throw string("괄호 안에 공백이 존재합니다. 다시 입력해주세요!");
        }
        bool flag = false;
        for (int i = 0; i < content.size(); i++) {
            for (int j = 0; j < content[i].size(); j++) {
                if (content[i][j] == target+" ") {
                    findRow = i;
                    flag = true;
                    break;
                }
            }
            if (flag == true) {
                break;
            }
        }
        if (flag == true) {
            first = findRow;
            last = first + 19;
            printData(content);
        }
        else {
            throw string("찾고자 하는 단어가 존재하지 않습니다.");
        }
    }
};
class Cmd_t : public CmdStrategy {
private:
    ofstream out;
public:
    void execute(string cmd, vector<vector<string>> &content) {
        out.open("D:\\2-2\\ObjectOrientedProgramming\\TermProject\\test.txt");
        if (out.is_open()) {
            for (int i = 0; i < content.size(); i++) {
                for (int j = 0; j < content[i].size(); j++) {
                    out.write(content[i][j].c_str(), content[i][j].size());
                }
            }

        }
        out.close();
    }
};
class TextFile {
private:
    CmdStrategy *cmdStrategy;
    string cmd;
public:
	TextFile() {}
    TextFile(CmdStrategy* cmd) { cmdStrategy = cmd; }
    void executeStrategy(string cmd, vector <vector<string> >& content) {
        cmdStrategy->execute(cmd,content);
    }
    void printFirst(vector <vector<string> >& content) {
        for (int i = 0; i < 20; i++) {
            if (i+1 < 10)
                cout << " " << i + 1 << "| ";
            else
                cout << i + 1 << "| ";
            for (int j = 0; j < content[i].size(); j++) {
                string s = content[i][j];
                cout << s;
            }
            cout << endl;
        }
    }
};

int main() {
    ifstream in;
    in.open("D:\\2-2\\ObjectOrientedProgramming\\TermProject\\test.txt");
    string word;
    int ch_cnt = 0;
    int line_num = 1; // 줄단위로 세는 변수
    vector <vector<string>> content; // 텍스트 파일 내용을 저장할 벡터 
    vector <string> elem;
    ch_cnt += 4;//앞의 번호 바이트만큼

    while (!in.eof()) {
        in >> word;

        word = word + " ";
        ch_cnt += word.size();
        if (ch_cnt > 76) { //해당 줄이 끝난 경우 
            ch_cnt = word.size(); //다음 줄
            ch_cnt += 4; //번호 바이트 
            content.push_back(elem);
            elem.clear();
            elem.push_back(word);
        }
        else {
            elem.push_back(word);
        }
    }
    content.push_back(elem);
    in.close();

    // ============벡터에 데이터 저장 완료==============
    string cmd;

    TextFile* textFile1 = new TextFile();
    TextFile* textFile2 = new TextFile(new Cmd_n);
    TextFile* textFile3 = new TextFile(new Cmd_p);
    TextFile* textFile4 = new TextFile(new Cmd_i);
    TextFile* textFile5 = new TextFile(new Cmd_d);
    TextFile* textFile6 = new TextFile(new Cmd_c);
    TextFile* textFile7 = new TextFile(new Cmd_s);
    TextFile* textFile8 = new TextFile(new Cmd_t);

    textFile1->printFirst(content); //프로그램에서 메모장 처음 출력
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "(콘솔메시지) " << endl;
    while (true) {
        
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << "입력: ";
       getline(cin, cmd);

        if (cmd.at(0) == 'n') {
            try{ textFile2->executeStrategy(cmd, content);  }
            catch (const string s){
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지) " << s << endl;
            }
        }
        else if (cmd.at(0) == 'p') {
            try { textFile3->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지) " << s << endl;
            }          
        }
        else if (cmd.at(0) == 'i') {
            try { textFile4->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지) " << s << endl;
            }
        }
        else if (cmd.at(0) == 'd') {
            try { textFile5->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지) " << s << endl;
            }
        }
        else if (cmd.at(0) == 'c') {
            try { textFile6->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지) " << s << endl;
            }
        }
        else if (cmd.at(0) == 's') {
            try { textFile7->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지) " << s << endl;
            }
        }
        else if (cmd.at(0) == 't') {
            try {
                textFile8->executeStrategy(cmd, content);
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지) 파일이 업데이트되었습니다. 프로그램을 종료합니다." << endl;
                break;
            }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(콘솔메시지)" << s << endl;
            }
        }
        else {
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "(콘솔메시지) 프로그램에 존재하지 않는 명령입니다."<<endl;
        }
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << endl;

    }
}