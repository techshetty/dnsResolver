#include<Poco/Net/DNS.h>
#include<Poco/Net/HostEntry.h>
#include<vector>
#include<string>
#include<unordered_map>
#include<chrono>
#include<iostream>
using namespace std;
using namespace std::chrono;
using namespace Poco::Net;
#define TTL 30
class Record{
    public: 
    Record() = default;
    vector<string>addr;steady_clock::time_point ts;
    Record(vector<string>addr,steady_clock::time_point ts):addr(addr),ts(ts){}
};
unordered_map<string,Record> cache;
vector<string> resolve(string dname){
    auto ts=chrono::steady_clock::now();
    if(cache.find(dname)!=cache.end()){
        if(duration_cast<seconds>(ts-cache[dname].ts).count()<TTL){cout<<"cache hit"<<endl;return cache[dname].addr;}
        cache.erase(dname);
    }
    vector<string>ips;
    try{
        HostEntry en=DNS::hostByName(dname);
        for(auto &a: en.addresses()){
            ips.push_back(a.toString());
        }
    }
    catch(Poco::Exception &e){cout<<"Error occured: "<<e.displayText()<<endl;}
    cache[dname]=Record(ips,ts);
    cout<<"cached"<<endl;
    return ips;
}
int main(int argc,char **argv){
    if(argc>1){
    vector<string>ips=resolve(argv[1]);
    for(auto &a:ips)cout<<a<<endl;
    return 0;
    }
    string s;
    while(true){
    cout<<"Enter the domain you want to resolve (ctrl+c to quit): ";
    cin>>s;
    vector<string>ips=resolve(s);
    for(auto &a:ips)cout<<a<<endl;
    }
return 0;
}
