

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <graphics.h>
#include <windows.h> // Include Windows header for Sleep function

using namespace std;

class File {
    public:
        ifstream graph_file, myfile, myfile1;
        File() {
            graph_file.open("graph_plot.txt", ios::in);
            if (!graph_file.is_open()) {
                cerr << "Error opening graph_plot.txt" << endl;
                exit(1);
            }
            myfile.open("places.txt", ios::in);
            if (!myfile.is_open()) {
                cerr << "Error opening places.txt" << endl;
                exit(1);
            }
            myfile1.open("directions.txt", ios::in);
            if (!myfile1.is_open()) {
                cerr << "Error opening directions.txt" << endl;
                exit(1);
            }
        }
        ~File() {
            myfile.close();
            myfile1.close();
            graph_file.close();    
        }
};

vector<int> getpath(vector<pair<int,int>> adj[], int n, int m, int source, int destination) {
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    vector<int> dist(n + 1, INT_MAX);
    vector<int> parent(n + 1, -1);

    dist[source] = 0;
    pq.push({0, source});
    vector<int> path;

    while (!pq.empty()) {
        int next = pq.top().first;
        int prev = pq.top().second;
        pq.pop();
        for (auto it : adj[prev]) {
            int nextnode = it.first;
            int nextdistance = it.second;
            if (dist[nextnode] > dist[prev] + nextdistance) {
                dist[nextnode] = dist[prev] + nextdistance;
                pq.push({dist[nextnode], nextnode});
                parent[nextnode] = prev;
            }
        }
    }

    for (int x = destination; x != -1; x = parent[x])
        path.push_back(x);

    reverse(path.begin(), path.end());   
    
    return path;
}

void menu(map<int, string>& m) {
    cout << "\t\t\tCAMPUS NAVIGATION SYSTEM\n\n";
    cout << " List of Places : \n\n";
    for (auto it : m) {
        cout << "\t" << it.first << " " << it.second << endl;
    }
    cout << endl;
}

void printpath(vector<int>& pts, map<int, string>& loc) {
    map<int, vector<int>> co;
    ifstream fin;
    string nm;
    setcolor(YELLOW);
    outtextxy(270, 30, (char*)"CEG NAVIGATION SYSTEM");
    fin.open("coordinates.txt", ios::in);
    if (!fin.is_open()) {
        cerr << "Error opening coordinates.txt" << endl;
        return;
    }
    while (getline(fin, nm)) {
        int u, v, w, b1, b2, k = 0;
        for (int j = 0; j < nm.length(); j++) {
            if (nm[j] == ' ' && k == 0) {
                b1 = j;
                k++;
            } else if (nm[j] == ' ' && k == 1) {
                b2 = j;
                break;
            }
        }
        u = stoi(nm.substr(0, b1));
        v = stoi(nm.substr(b1 + 1, b2 - b1 + 1));
        w = stoi(nm.substr(b2 + 1));
        vector<int> t(2);
        t[0] = v;
        t[1] = w;
        co[u] = t;
    }
    fin.close();

    vector<int> t(4);
    for (size_t i = 0; i < pts.size() - 1; i++) {
        int c = 0;
        for (auto it : co[pts[i]])
            t[c++] = it;
        for (auto it : co[pts[i + 1]])
            t[c++] = it;
        char* s = &loc[pts[i]][0];
        outtextxy(t[0] - 30, t[1], s);
        Sleep(1000); // Use Sleep instead of sleep
        line(t[0], t[1], t[2], t[3]);
    }
    char* s = &loc[pts.back()][0];
    outtextxy(t[2] - 30, t[3], s);
}

int main() {
    File f;
    
    // Graph input
    int n, m;
    string nm;
    getline(f.graph_file, nm);
    size_t i = 0;
    for (i = 0; i < nm.length(); i++)
        if (nm[i] == ' ')
            break;
    n = stoi(nm.substr(0, i));
    m = stoi(nm.substr(i + 1));
    vector<pair<int, int>> adj[n + 1];

    for (i = 0; i < m; i++) {
        int u, v, w, b1, b2, k = 0;
        getline(f.graph_file, nm);
        for (size_t j = 0; j < nm.length(); j++) {
            if (nm[j] == ' ' && k == 0) {
                b1 = j;
                k++;
            } else if (nm[j] == ' ' && k == 1) {
                b2 = j;
                break;
            }
        }
        u = stoi(nm.substr(0, b1));
        v = stoi(nm.substr(b1 + 1, b2 - b1 + 1));
        w = stoi(nm.substr(b2 + 1));

        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    
    // Declarations
    string s;
    vector<pair<int, string>> places;
    map<int, string> loc;
    map<string, string> direction;
    
    while (getline(f.myfile, s)) {
        int i = 0;
        for (i = 0; i < s.length(); i++)
            if (s[i] == ' ')
                break;

        string no = s.substr(0, i);
        string name = s.substr(i + 1);

        int x = stoi(no);
        places.push_back({x, name});
        loc[x] = name;
    }
    
    while (getline(f.myfile1, s)) {
        int i = 0;
        for (i = 0; i < s.length(); i++)
            if (s[i] == ' ')
                break;

        string edge = s.substr(0, i);
        string dir = s.substr(i + 1);
        direction[edge] = dir;
    }
    
    while (true) {
        menu(loc);
        // Accepting source and destination
        int source, destination;
        while (true) {
            cout << "\n  Enter Source and Destination: ";
            cin >> source >> destination;
            
            try {
                if (source <= 0 || source >= 17 || destination <= 0 || destination >= 17)
                    throw -1;
                else
                    break;
            } catch (int) {
                cout << "\nEnter proper Source and Destination \n";
            }
        }
        if (source != destination) {
            vector<int> path = getpath(adj, n, m, source, destination);
            vector<string> final_path;
            for (i = 0; i < path.size() - 1; i++) {
                string temp;
                temp = to_string(path[i]) + "-" + to_string(path[i + 1]);
                final_path.push_back(temp);
            }    

            vector<string> route;
            map<char, string> cmd;
            for (auto i : final_path) {
                route.push_back(direction[i]);
            }

            cmd['L'] = "Take Left ";
            cmd['R'] = "Take Right ";
            cmd['O'] = "Turn Around ";
            cmd['S'] = "Head Straight ";

            // Printing final path
            cout << "\n  Directions : ( Always face the building and then follow the directions )\n\n";
            int c = 1;
            for (i = 0; i < path.size() - 1; i++) {
                cout << "\t" << c++ << ". From " << loc[path[i]] << " ";
                if (route[i].length() == 1) {
                    cout << cmd[route[i][0]] << "towards " << loc[path[i + 1]] << endl;
                } else {
                    for (int j = 0; j < route[i].length(); j++) {
                        if (j != route[i].length() - 1)
                            cout << cmd[route[i][j]] << endl;
                        else
                            cout << "\t" << c++ << ". " << cmd[route[i][j]] << "towards " << loc[path[i + 1]] << endl;
                    }
                }
            }
            cout << "\t" << c++ << ". You have arrived at " << loc[path.back()] << endl;
            initwindow(750, 800, "Graph");
            printpath(path, loc);
        } else {
            cout << "\n\tYou are already at the Destination\n";
        }
        
        // While condition
        string condition = "yes";
        int flag = 0;
        
        while (true) {
            cout << "\n  Do you want to continue (YES/NO): ";
            cin >> condition;
            transform(condition.begin(), condition.end(), condition.begin(), ::tolower);
            if (condition == "no") {
                cout << "\n\n\t\tThank you!!\n\n";
                flag++;
                break;    
            } else if (condition == "yes")
                break;
        }
        if (flag == 1)
            break;
        else
            system("cls");
    }
}
