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
        else if (last + 20 >= content.size()) { // 20���� ���� ä������ �ʴ� ��� 
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

        if (cmd[1] != '(' || cmd[cmd.size() - 1] != ')' || cmd.size() <= 1) { // ��ȣ�� �˸��� ���� ���,�Ǵ� ��ȣ ���� ��� 
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }

        temp = cmd.substr(2, cmd.size()- 3);//��ȣ ���� 
        istringstream ss(temp);
        string buffer;
        x.clear();
        int index = 0;

        while (getline(ss, buffer, ',')){  // �޸� ������ �߶� x���Ϳ� �ִ´�. 
            int len1 = buffer.size();
            buffer.erase(remove(buffer.begin(), buffer.end(), ' '), buffer.end());
            int len2 = buffer.size();
            if (len1 != len2){
                throw string("��ȣ �ȿ� ������ �����մϴ�. �ٽ� �Է����ּ���!");
            }
            x.push_back(buffer);
        }
        
        if (x.size() != 3) { // ���ڷ� 3���� ������ ���� ���
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }

        if (!atoi(x[0].c_str()) || !atoi(x[1].c_str())) { //�� �Ǵ� ���� ���ڰ� �ƴ� ��� 
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }
        if (x[2].size() > 75) { //�Է¹��� ���ڿ��� 75����Ʈ�� �Ѵ� ��� 
            throw string("���ڿ� ���̰� �ʹ� ��ϴ�.�ٽ� �Է����ּ���.");          
        }
       
        int row = stoi(x[0])-1; //��
        int col = stoi(x[1]); //�� 
        string target = x[2];

        if (first > row || last < row) {
            throw string("���� â�� �������� �ʴ� ��ġ�Դϴ�. �ٽ� �Է����ּ���.");
        }
        it = content[row].begin();//�ʱ�ȭ
       
        target = target + " ";
        if (col == content[row].size()) { //�� �ڿ� ���� ��� 
            content[row].push_back(target);
        }
        else if(col < content[row].size()) {
            content[row].insert(it + col, target);
        }
        else { // column�ε����� ���� �ʴ� ��� 
            throw string("�Է¹��� ��ġ�� �������� �ʽ��ϴ�. �ٽ� �Է����ּ���.");
        }
        
        int i = 0; // ��
        int cnt = 0;
        
        vector <string> elem;
        while (i != content.size()) {
            cnt += 4;
            for(int j = 0; j<content[i].size(); j++) { //0~size-1
                cnt += content[i][j].size();
            } //������ ����Ʈ �� ���μ���.
             while (cnt > 76) { //75����Ʈ�� �Ѵ´ٸ� �ȳ��������� ���ؼ� �Ʒ��ٷ� �ѱ� 
                string popValue = content[i].back(); //���� ���� 
                content[i].pop_back();
                cnt -= popValue.size();    
                if (i + 1 != content.size()) {
                    content[i + 1].insert(content[i + 1].begin(), popValue); //���� �� �Ǿտ� ���� 
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
        
        if (cmd[1] != '(' || cmd[cmd.size() - 1] != ')' || cmd.size() <= 1) { // ��ȣ�� �˸��� ���� ���,�Ǵ� ��ȣ ���� ��� 
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }
        temp = cmd.substr(2, cmd.size() - 3);//��ȣ ���� 
        istringstream ss(temp);
        string buffer;
        x.clear();
        while (getline(ss, buffer, ',')) {  // �޸� ������ �߶� x���Ϳ� �ִ´�. 
            int len1 = buffer.size();
            buffer.erase(remove(buffer.begin(), buffer.end(), ' '), buffer.end());
            int len2 = buffer.size();
            if (len1 != len2) {
                throw string("��ȣ �ȿ� ������ �����մϴ�. �ٽ� �Է����ּ���!");
            }
            else if (!atoi(buffer.c_str())) {
                throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
            }
            x.push_back(buffer);
        }

        if (x.size() != 2) { // ���ڷ� 2���� ������ ���� ���
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }
        int row = stoi(x[0]) - 1; //��
        int col = stoi(x[1]) -1; //�� 

        if (!(first <= row && row <= last)) { //���� â�� �����ϴ� ��ġ�� �ƴ� ��� 
            throw string("���� â�� �������� �ʴ� ��ġ�Դϴ�. �ٽ� �Է����ּ���.");
        }
        if (col >= content[row].size()) {
            throw string("�Է¹��� ��ġ�� �������� �ʽ��ϴ�. �ٽ� �Է����ּ���.");
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
                content[i].push_back(content[i + 1][0]);//�����࿡�� ��ܿ� 
                content[i + 1].erase(content[i + 1].begin());//������ �� �տ��� ���� 
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
        if (cmd[1] != '(' || cmd[cmd.size() - 1] != ')' || cmd.size() <= 1) { // ��ȣ�� �˸��� ���� ���,�Ǵ� ��ȣ ���� ��� 
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }
        temp = cmd.substr(2, cmd.size() - 3);//��ȣ���� ���� ���� 
        istringstream ss(temp);
        string buffer;
        x.clear();
        while (getline(ss, buffer, ',')) {  // �޸� ������ �߶� x���Ϳ� �ִ´�. 
            int len1 = buffer.size();
            buffer.erase(remove(buffer.begin(), buffer.end(), ' '), buffer.end());
            int len2 = buffer.size();
            if (len1 != len2) {
                throw string("��ȣ �ȿ� ������ �����մϴ�. �ٽ� �Է����ּ���!");
            }
            x.push_back(buffer);
        }
        
        if (x.size() != 2) { // ���ڷ� 2���� ������ ���� ���
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }
        if (x[1].size() > 75) { //�ٲ� ���ڿ��� 75����Ʈ�� �Ѵ� ��� 
            throw string("���ڿ� ���̰� �ʹ� ��ϴ�.�ٽ� �Է����ּ���.");
        }
        string target = x[0]; // �ٲ�� ���
        string change = x[1]; // �ٲ� ���ڿ� 
      
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
            throw string("�����ϰ��� �ϴ� �ܾ �������� �ʽ��ϴ�.");
        }
        else { //�����ϰ��� �ϴ� �ܾ �ִ� ���
            if (compare == "smaller") { //������ �ܾ �����ܾ�� ���̰� ª�� ��� 
                int i = 0;
                int cnt = 4;

                while (i != content.size()) {
                    for (int j = 0; j < content[i].size(); j++) {
                        cnt += content[i][j].size();
                    }
                    while (content[i + 1][0].size() <= 76 - cnt) {
                        cnt += content[i + 1][0].size();
                        content[i].push_back(content[i + 1][0]);//�����࿡�� ��ܿ� 
                        content[i + 1].erase(content[i + 1].begin());//������ �� �տ��� ���� 
                    }
                    cnt = 4;
                    i++;

                }
            }
            else if (compare == "bigger") {//������ �ܾ �����ܾ�� ���̰� �� ��� 
                int i = 0; // ��
                int cnt = 0;

                vector <string> elem;
                while (i != content.size()) {
                    cnt += 4;
                    for (int j = 0; j < content[i].size(); j++) { //0~size-1
                        cnt += content[i][j].size();
                    } //������ ����Ʈ �� ���μ���.
                    while (cnt > 76) { //75����Ʈ�� �Ѵ´ٸ� �ȳ��������� ���ؼ� �Ʒ��ٷ� �ѱ� 
                        string popValue = content[i].back(); //���� ���� 
                        content[i].pop_back();
                        cnt -= popValue.size();
                        if (i + 1 != content.size()) {
                            content[i + 1].insert(content[i + 1].begin(), popValue); //���� �� �Ǿտ� ���� 
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
            throw string("�Է������� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.");
        }
        target = cmd.substr(2, cmd.size() - 3);//��ȣ���� ���� ���� 
        int len1 = target.size();
        target.erase(remove(target.begin(), target.end(), ' '), target.end());
        int len2 = target.size();
        if (len1 != len2) {
            throw string("��ȣ �ȿ� ������ �����մϴ�. �ٽ� �Է����ּ���!");
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
            throw string("ã���� �ϴ� �ܾ �������� �ʽ��ϴ�.");
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
    int line_num = 1; // �ٴ����� ���� ����
    vector <vector<string>> content; // �ؽ�Ʈ ���� ������ ������ ���� 
    vector <string> elem;
    ch_cnt += 4;//���� ��ȣ ����Ʈ��ŭ

    while (!in.eof()) {
        in >> word;

        word = word + " ";
        ch_cnt += word.size();
        if (ch_cnt > 76) { //�ش� ���� ���� ��� 
            ch_cnt = word.size(); //���� ��
            ch_cnt += 4; //��ȣ ����Ʈ 
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

    // ============���Ϳ� ������ ���� �Ϸ�==============
    string cmd;

    TextFile* textFile1 = new TextFile();
    TextFile* textFile2 = new TextFile(new Cmd_n);
    TextFile* textFile3 = new TextFile(new Cmd_p);
    TextFile* textFile4 = new TextFile(new Cmd_i);
    TextFile* textFile5 = new TextFile(new Cmd_d);
    TextFile* textFile6 = new TextFile(new Cmd_c);
    TextFile* textFile7 = new TextFile(new Cmd_s);
    TextFile* textFile8 = new TextFile(new Cmd_t);

    textFile1->printFirst(content); //���α׷����� �޸��� ó�� ���
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "(�ָܼ޽���) " << endl;
    while (true) {
        
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << "�Է�: ";
       getline(cin, cmd);

        if (cmd.at(0) == 'n') {
            try{ textFile2->executeStrategy(cmd, content);  }
            catch (const string s){
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���) " << s << endl;
            }
        }
        else if (cmd.at(0) == 'p') {
            try { textFile3->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���) " << s << endl;
            }          
        }
        else if (cmd.at(0) == 'i') {
            try { textFile4->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���) " << s << endl;
            }
        }
        else if (cmd.at(0) == 'd') {
            try { textFile5->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���) " << s << endl;
            }
        }
        else if (cmd.at(0) == 'c') {
            try { textFile6->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���) " << s << endl;
            }
        }
        else if (cmd.at(0) == 's') {
            try { textFile7->executeStrategy(cmd, content); }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���) " << s << endl;
            }
        }
        else if (cmd.at(0) == 't') {
            try {
                textFile8->executeStrategy(cmd, content);
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���) ������ ������Ʈ�Ǿ����ϴ�. ���α׷��� �����մϴ�." << endl;
                break;
            }
            catch (const string s) {
                cout << "--------------------------------------------------------------------------------" << endl;
                cout << "(�ָܼ޽���)" << s << endl;
            }
        }
        else {
            cout << "--------------------------------------------------------------------------------" << endl;
            cout << "(�ָܼ޽���) ���α׷��� �������� �ʴ� ����Դϴ�."<<endl;
        }
        cout << "--------------------------------------------------------------------------------" << endl;
        cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;

    }
}