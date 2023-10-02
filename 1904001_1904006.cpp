#include<bits/stdc++.h>
using namespace std;

//Variable Declaration 
const int N = 1005;
int n, P[N], B[N], A[N], W[N], T[N], R[N], S[N], F[N];
int B_[N]; // for retriveing burst time in round robin
double avgT = 0, avgW = 0, avgR = 0;

class CPU_scheduling {
    public:
       //Function Definition 
        void show() {

            //avg
            cout <<"Average Waiting Time = " << avgW << "\n";
            cout <<"Average Turn around Time = " << avgT << "\n";
            cout <<"Average Response Time = " << avgR << "\n";
        }
        void calc() {

            for(int i = 1; i < n + 1; i++) {
                T[i] = F[i] - A[i];
                W[i] = T[i] - B_[i];
                R[i] = S[i] - A[i];
                avgT += T[i];
                avgW += W[i];
                avgR += R[i];
                //cerr << R[i] << T[i] << ' ' << W[i] << '\n';
            }
            avgR = avgR / n;
            avgW = avgW / n;
            avgT = avgT / n;
            show();
        }
        int Time_Quantum(queue<int>Q, int complete) {
            int TQ = 0, x=Q.size();
            while(!Q.empty()) {
                int id = Q.front();
                Q.pop();
                TQ+=B[id];
            }
            return TQ /= x;
        }

        //Algorthim start
        void FCFS() {
            cin >> n ;
            for(int i = 1; i < n + 1; i++) {
                cin >> A[i] >> B[i];
            }
            for(int i = 1; i < n + 1; i++) B_[i] = B[i];

            for(int i = 1; i < n + 1; i++) {
                S[i] = max(F[i-1], A[i]);
                F[i] = S[i] + B[i];
            }
            //Gannt Chart
            cout << "Gannt Chart: ";
            for(int i = 1; i < n + 1; i++) {
                cout << " (" << S[i] << "[P" << i <<"]" << F[i] << ") ";
            } 
            cout<<"\n\n"; 
            calc();
        }
        void SJF_nonPreemptive() {
            multiset<pair<int, int>> s, ready;
            cin >> n ;
            for(int i = 1; i < n + 1; i++) {
                cin >> A[i] >> B[i];
                s.insert({A[i], i});
            }
             for(int i = 1; i < n + 1; i++) {
                B_[i] = B[i];
                S[i] = -1;
                F[i] = 0;
            }
            cout << "Gannt Chart: ";
            int currentTime = 0;
            while(!s.empty()) {
                pair<int, int> x = *s.begin();
                while(x.first <= currentTime) {
                    s.erase(x);
                    ready.insert({B[x.second], x.second});

                    if(s.size())x = *s.begin();
                    else break;
                }
                if(ready.empty()) {
                    currentTime++;
                    continue;
                }
                x = *ready.begin();
                ready.erase(x);
                if(S[x.second] == -1) {
                    S[x.second] = currentTime;
                }
                cout << currentTime << "[P" << x.second << "]";
                currentTime+=x.first;
                cout << currentTime << ' ';
                B[x.second] = 0;
                F[x.second] = currentTime;
            }
            while(ready.size()) {
                pair<int, int> x = *ready.begin();
                ready.erase(x);
                if(S[x.second] == -1) {
                    S[x.second] = currentTime;
                }
                cout << currentTime << "[P" << x.second << "]";
                currentTime+=x.first;
                cout << currentTime << ' ';
                B[x.second] = 0;
                F[x.second] = currentTime;
            }
            cout << endl << endl; //gannt chart endline
            calc();

        }
        void SJF_preemptive() {
            multiset<pair<int, int>>s, ready;
            cin >> n ;
            for(int i = 1; i < n + 1; i++) {
                cin >> A[i] >> B[i];
                s.insert({A[i], i});
            }
            for(int i = 1; i < n + 1; i++) {
                B_[i] = B[i];
                S[i] = -1;
                F[i] = 0;
            }
            cout << "Gannt Chart: ";
            int currentTime = 0;
            while(!s.empty()) {
                pair<int, int> x = *s.begin();
                while(x.first <= currentTime) {
                    s.erase(x);
                    ready.insert({B[x.second], x.second});

                    if(s.size())x = *s.begin();
                    else break;
                }
                if(ready.empty()) {
                    currentTime++;
                    continue;
                }
                x = *ready.begin();
                ready.erase(x);
                if(S[x.second] == -1) {
                    S[x.second] = currentTime;
                }
                cout << currentTime << "[P" << x.second << "]";
                currentTime++;
                cout << currentTime << ' ';
                B[x.second] = B[x.second] - 1;
                if(B[x.second] == 0) {
                    F[x.second] = currentTime;
                }
                else ready.insert({B[x.second], x.second});
            }
            while(ready.size()) {
                pair<int, int> x = *ready.begin();
                ready.erase(x);
                if(S[x.second] == -1) {
                    S[x.second] = currentTime;
                }
                cout << currentTime << "[P" << x.second << "]";
                currentTime+=x.first;
                cout << currentTime << ' ';
                B[x.second] = 0;
                F[x.second] = currentTime;
            }
            cout << endl << endl; //gannt chart endline
            calc();
        } 
        void priority_nonPreemptive() {

            // Higher the value higher the priority
            cin >> n ; multiset<pair<int, int>> s; //sort on the arrival time in s
            for(int i = 1; i < n + 1; i++) {
                cin >> A[i] >> B[i] >> P[i];
                s.insert({A[i], i});
            }
            for(int i = 1; i < n + 1; i++) B_[i] = B[i];
            cout << "Gannt Chart: ";

            multiset<pair<int, int>>ready; // busy the CPU according to priority
            int currentTime = 0;

            pair<int, int> x = *s.begin();
            s.erase(x);
            S[x.second] = max(currentTime, A[x.second]);
            F[x.second] = max(currentTime, A[x.second]) + B[x.second];
            currentTime = F[x.second];

            cout << S[x.second] << "[P" << x.second << "]" << F[x.second] << ' ';
            
            while(s.size()) {
                x = *s.begin();
                while(x.first <= currentTime) {
                    s.erase(x);
                    ready.insert({P[x.second], x.second});
                    if(s.empty()) break;
                    x = *s.begin();
                }
                bool ok = true;
                while(ready.size()) {
                    ok = false;
                    x = *ready.rbegin();
                    ready.erase(x);
                    S[x.second] = max(currentTime, A[x.second]);
                    F[x.second] = max(currentTime, A[x.second]) + B[x.second];
                    currentTime = F[x.second];

                    cout << S[x.second] << "[P" << x.second << "]" << F[x.second] << ' ';

                    break;
                }
                if(ok) currentTime++;
            }
            while(ready.size()) {

                x = *ready.rbegin();
                ready.erase(x);
                S[x.second] = max(currentTime, A[x.second]);
                F[x.second] = max(currentTime, A[x.second]) + B[x.second];
                currentTime = F[x.second];

                cout << S[x.second] << "[P" << x.second << "]" << F[x.second] << ' ';
            }
            cout << "\n\n";
            calc();

        }
        void priority_preemptive() {

            // Higher the value higher the priority

            cin >> n ; multiset<pair<int, int>> s; //sort on the arrival time in s
            for(int i = 1; i < n + 1; i++) {
                cin >> A[i] >> B[i] >> P[i];
                s.insert({A[i], i});
            }
            for(int i = 1; i < n + 1; i++) B_[i] = B[i];

            int currentTime = 0; multiset<pair<int, int>>ready;
            cout << "Gannt Chart: ";
            int define[n+2] = {0};
            while(s.size()) {
                while(s.size()) { //sort based on the burst time in the ready queue
                    pair<int, int>x = *s.begin();
                    if(x.first > currentTime) break;
                    ready.insert({P[x.second], x.second});
                    //cout << x.first << ' ' << x.second << endl;
                    s.erase(x);
                }
                bool ok = true;
                while(ready.size()) {
                    ok = false;
                    cout << currentTime << "[P";


                    pair<int, int> x = *ready.rbegin();
                    ready.erase(x);
                    if(define[x.second] == 0) {
                        define[x.second] = 1;
                        S[x.second] = currentTime;
                    }
                    B[x.second] -= 1;
                    currentTime++;
                    if(B[x.second] == 0) {
                        F[x.second] = currentTime;
                        cout << x.second << "]" << currentTime << ' ';
                        break;
                    }
                    s.insert({A[x.second], x.second});

                    cout << x.second << "]" << currentTime << ' ';
                    break;
                } 
                if(ok)currentTime++;
            }
            int S_[N] = {0};
            while(ready.size()) {

                pair<int, int> x = *ready.rbegin();
                ready.erase(x);
                F[x.second] = max(currentTime, A[x.second]) + B[x.second];
                currentTime = F[x.second];

                cout << S_[x.second] << "[P" << x.second << "]" << F[x.second] << ' ';
            }
            cout << endl << endl; //gannt chart endline
            calc();

        }
        void init() {
            for(int i = 0; i < N; i++) {
                S[i] = -1;
                F[i] = -1;
            }
        }
        void roundRobin() {
            init();
            cin >> n; vector<pair<int, int>>a, b;
            int mean, median; int m[n+1];
            for(int i = 1; i <= n; i++) {
                cin >> A[i] >> B[i];
                a.push_back({A[i], i});
                b.push_back({B[i], i});
                B_[i] = B[i];
            }
           
            int TQ = 25;
            sort(a.begin(), a.end());
            int complete = 0;
            int t = 0;
            queue<int>Q;
            Q.push(a[0].second);
            vector<int>define(n+1, 0);
            define[a[0].second] = 1;
            t = a[0].first;

            cout << "Gannt Chart: ";

            while(complete != n) {
                bool ok = false;
                int id = Q.front();
                Q.pop();
                //cerr << id << ' ';
                cout << t << "[P" << id << "]" ;

                if(B[id] > TQ) {
                    if(S[id] == -1) {
                        S[id] = t;
                    }
                    B[id] -= TQ;
                    t+=TQ;
                    // Q.push(id);
                    ok = true;
                }
                else if(B[id] <= TQ) {
                    if(S[id] == -1) {
                        S[id] = t;
                    }
                    complete++;
                    t+=B[id];
                    B[id] = 0;
                    F[id] = t;
                }
                cout << t << ' ';
                vector<pair<int, int>>C;
                for(int i = 1; i <= n; i++) {
                    if(define[i] == 0) {
                        if(t >= A[i]) {
                            define[i] = 1;
                            C.push_back({A[i], i});
                        }
                    }
                }
                sort(C.begin(), C.end());
                for(int i = 0; i < C.size(); i++) {
                    Q.push(C[i].second);
                }
                if(ok)Q.push(id);
                if(Q.size() == 0) t++;
            }
            cout<<endl<<endl;
            for(int i = 1; i < n + 1; i++) B[i] = B_[i];
            calc();
        }
        int AVG(int complete) {
            int mx = 0;
            for(int i = 1; i < n + 1; i++) {
                mx += B[i] * A[i];
            }
            return sqrt(mx) + 1;
           // return 1;
        }
        void priority_based_hybrid() {
            init();
            cin >> n; vector<pair<int, int>>a, b, c;
            for(int i = 1; i <= n; i++) {
                cin >> A[i] >> B[i] >> P[i];
                c.push_back({B[i], A[i]});
                a.push_back({A[i], i});
                b.push_back({B[i], i});
                B_[i] = B[i];
            }
            int TQ;
            sort(a.begin(), a.end());
            int complete = 0;
            int t = 0;
            queue<int>Q;
            Q.push(a[0].second);
            vector<int>define(n+1, 0);
            define[a[0].second] = 1;
            t = a[0].first;

            cout << 
            "Gannt Chart: ";

            while(complete != n) {
                bool ok = false;
                int id = Q.front();
                Q.pop();
                //cerr << id << ' '; 
                TQ =  n - P[id] + AVG(complete);
                cout << t << "[P" << id << "]" ;

                if(B[id] > TQ) {
                    if(S[id] == -1) {
                        S[id] = t;
                    }
                    B[id] -= TQ;
                    t+=TQ;
                    // Q.push(id);
                    ok = true;
                }
                else if(B[id] <= TQ) {
                    if(S[id] == -1) {
                        S[id] = t;
                    }
                    complete++;
                    t+=B[id];
                    B[id] = 0;
                    F[id] = t;
                }
                cout << t << ' ';
                vector<pair<int, int>>C;
                for(int i = 1; i <= n; i++) {
                    if(define[i] == 0) {
                        if(t >= A[i]) {
                            define[i] = 1;
                            C.push_back({A[i], i});
                        }
                    }
                }
                sort(C.begin(), C.end());
                for(int i = 0; i < C.size(); i++) {
                    Q.push(C[i].second);
                }
                if(ok)Q.push(id);
                if(Q.size() == 0) t++;
            }
            cout<<endl<<endl;
            for(int i = 1; i < n + 1; i++) B[i] = B_[i];
            calc();
        }
        void DABRR() {
            init();
            cin >> n; vector<pair<int, int>>a, b;
            for(int i = 1; i <= n; i++) {
                cin >> A[i] >> B[i];
                a.push_back({A[i], i});
                b.push_back({B[i], i});
                B_[i] = B[i];
            }

            sort(a.begin(), a.end());

            int complete = 0, TQ = 0, t = 0;

            cout << "Gannt Chart: ";
            queue<int>Q;
            for(; complete<n; ) {

                for(int i = 0; i < n; i++) {
                    if(a[i].first <= t && B[a[i].second] > 0) {
                        Q.push(a[i].second);
                    }
                }
                if(!Q.empty())TQ = Time_Quantum(Q, complete);
                else t++;

                while(!Q.empty()) {
                    int id = Q.front();
                    Q.pop();
                    //cout << id << ' ';
                    cout << t << "[P" << id << "]" ;

                    if(B[id] > TQ) {
                        if(S[id] == -1) {
                            S[id] = t;
                        }
                        B[id] -= TQ;
                        t+=TQ;
                    }

                    else if(B[id] <= TQ) {
                        if(S[id] == -1) S[id] = t;

                        t+=B[id];
                        B[id] = 0;
                        F[id] = t;
                        complete++;
                    }

                }
            }
            cout << t << '\n';
            calc();
            // cout<<endl<<endl;
            for(int i = 1; i < n + 1; i++) B[i] = B_[i];
            // calc();
        }
} cpu;
//Driver Code
int32_t main() {
     ios_base::sync_with_stdio(0);cin.tie(0);
     
     int key; cin >> key;
     if(key == 1) cpu.FCFS();
     if(key == 2) cpu.SJF_nonPreemptive();
     if(key == 3) cpu.SJF_preemptive();
     if(key == 4) cpu.priority_nonPreemptive();
     if(key == 5) cpu.priority_preemptive();
     if(key == 6) cpu.roundRobin();
     if(key == 7) cpu.DABRR();
     if(key == 8) cpu.priority_based_hybrid();
     return 0;
}

/*

n = number of process 
P[] = priority array
B[] = burst time 
A[] = arrival time
W[] = waiting time
T[] = turn around time
R[] = response time 
S[] = start time
F[] = ending time

*/
/*
test case-I:
    8 --> key
    5 --> number of process
    A[i] B[i] P[i]
     0    45   0
     5    90   0
     8    70   0
     15   38   0
     20   55   0

*/



