#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <chrono>
#include <math.h>
#define ll long long
#define ld long double
#define ve vector
#define mp make_pair
using namespace std;

struct
{
    bool operator()(pair<pair<ll, ll>, ld> a, pair<pair<ll, ll>, ld> b) const { return a.second < b.second; }
}
        customLess;

struct DSU{
    vector<ll> parent;
    vector<ll> weight;
    ll getRoot(ll x){
        if(parent[x] == -1){
            return x;
        }
        ll p = getRoot(parent[x]);
        parent[x] = p;
        return p;
    }
    bool isConnected(ll x, ll y){
        x = getRoot(x);
        y = getRoot(y);
        return x==y;
    }
    void addEdge(ll x, ll y){
        x = getRoot(x);
        y = getRoot(y);
        if(weight[x] < weight[y]){
            parent[x] = y;
        }
        if(weight[x] > weight[y]){
            parent[y] = x;
        }
        if(weight[x] == weight[y]){
            parent[x] = y;
            weight[x]++;
        }
    }
};
int main() {
    fstream r;
    r.open("worldcities.csv");
    ve<pair<ld, ld>> cities;
    ve<pair<pair<ll, ll>, ld>> edges;
    ve<string> names;
    vector<string> row;
    string word, line;
    ld radius = 6371;
    ll citiesNum = 47869;
    getline(r, line);
    for(int i = 0; i<200; i++){
        getline(r, line);
        basic_stringstream s(line);

        while(getline(s, word, ',')){
            row.push_back(word.substr(1,word.length()-2));
        }
        names.push_back(row[1]);
        cities.push_back(mp(stold(row[2]), stold(row[3])));
        for(int j = 0; j<i; j++){
            ld phi2 = cities[i].first * M_PI/180;
            ld phi1 = cities[j].first* M_PI/180;
            ld rho2 = cities[i].second* M_PI/180;
            ld rho1 = cities[j].second* M_PI/180;
            ld dist = 2*radius* asin(sqrt(
                    pow(sin((phi2 - phi1)/2), 2)
                    + cos(phi2) * cos(phi1)
                    * pow(sin((rho2 - rho1)/2), 2)));
            edges.push_back(mp(mp(i, j), dist));
        }
        row.clear();
    }
    auto start = std::chrono::high_resolution_clock::now();
    sort(edges.begin(), edges.end(), customLess);
    DSU dsu;
    dsu.parent = ve<ll>(cities.size(), -1);
    dsu.weight = ve<ll>(cities.size(), 0);
    ld minSize = 0;
    ve<pair<pair<ll, ll>, ld>> skelEdge;
    for(int i = 0; i<edges.size(); i++){
        ll x = edges[i].first.first;
        ll y = edges[i].first.second;
        ld dist = edges[i].second;
        if(!dsu.isConnected(x, y)){
            dsu.addEdge(x, y);
            skelEdge.push_back(mp(mp(x, y), dist));
            minSize+=dist;
        }
    }

    for(int i = 0; i<skelEdge.size(); i++){
        cout << names[skelEdge[i].first.first] << "-------" << names[skelEdge[i].first.second] << "     "<< "(Dist:" << skelEdge[i].second<< ")"<<'\n';
    }
    cout << minSize << "km"<< '\n';
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    cout << duration.count() << endl;


    return 0;
}
