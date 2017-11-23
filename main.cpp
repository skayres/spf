#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct link{
    string linkID;
    string cost;

    link(string _linkID, string _cost): linkID(_linkID), cost(_cost){

    }

    link(): linkID(""), cost(""){

    }
};

class router {
    private:
        string name;
        string * linksID = new string;
        int linkCounter=0;
        //link * links = new link;

    public:
        link * links = new link;
        void setName(string name){
            this->name = name;
        }
        string getName(){
            return this->name;
        }

        void addLinkBulk(string linkID, string linkCost, char separator){

            int n = linkID.length();
            int n1 = linkCost.length();
            int k = 0;
            // DELETE CURRENT linksID (ON INITIAZITION ONE ELEMENT ONLY)
            delete this->linksID;
            linksID = new string[this->linkCounter];
            delete this->links;
            links = new link[this->linkCounter];

            //LINK ID
             for (int i=0; i<(n-1); i++){
                if (linkID[i] == separator){
                    (links+(++k))->linkID = "";
                }
                else{
                    (links+k)->linkID = (links+k)->linkID + linkID[i];
                }
            }

            //COST
            k=0;
            for (int i=0; i<(n1-1); i++){
                if (linkCost[i] == separator){
                    (links+(++k))->cost = "";
                }
                else{
                    (links+k)->cost = (links+k)->cost + linkCost[i];
                }
            }

        }

        int getLinkCount(){
            return this->linkCounter;
        }

        void increaseLinkCounter(){
            this->linkCounter++;
        }

        void printRouterLinks(){
            for(int i=0; i<(this->linkCounter); i++){
                cout << "Router link #" << i << " : " << "id: " << (links+i)->linkID << " cost: " << (links+i)->cost << "\n";
            }
        }

        router(){
            linkCounter = 0;
        }

};

bool isDirectlyConnected(router r1, router r2){
    for(int i=0; i<r1.getLinkCount(); i++){
        for(int j=0; j<r2.getLinkCount(); j++){
            if(r1.links[i].linkID == r2.links[j].linkID){
                return true;
            }
        }
    }
    return false;
}

router routerDirectlyConnected(router r, string link){
    for(int i=0; i<r.getLinkCount(); i++){
        for(int j=0; j<r2.getLinkCount(); j++){
            if(r1.links[i].linkID == r2.links[j].linkID){
                return true;
            }
        }
    }
    return false;
}

void printAllRoutes(router _router, int _diff, string firstRouter){

    int n = sizeof(_router);
    int ln = _router.getLinkCount();
    int diff = _diff;

    for (int i=0; i<ln; i++){
        if(_router[diff+1].getName() != firstRouter){
            printAllRoutes((_router+diff+1), diff+1, firstRouter);
        }
        else{
            cout << " -> " << _router[diff].getName();
        }
    }
}

void walkGraph(router * _router){

    int n = sizeof(_router);

    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if(i!=j){
                if(isDirectlyConnected(*(_router+i), *(_router+j))){
                    //cout << _router[i].getName() << " is directly connected to " << _router[j].getName() << "\n";
                }
            }
        }
    }
}

void spf(router * _router){

    int n = sizeof(_router);
    walkGraph(_router);
    printAllRoutes(_router, 0, "R1");

}


int main()
{
    const char * topology = "topology.conf";
    ifstream FILE_TOPOLOGY;
    int routerCounter=0;
    router R[4];
    string * _routers;

    try{
        FILE_TOPOLOGY.open(topology, ios::out);
        if (FILE_TOPOLOGY.is_open()){

            string line;
            string findRouterKeyword = "router";

            while (getline(FILE_TOPOLOGY, line)){

                string name = "";
                string linkBlockSeparatorStart = "links";
                string endSeparator = "}";
                int startRouterName = line.find(findRouterKeyword);


                //GET ROUTERS NAMES
                if (startRouterName != line.npos){
                    name = line.substr(startRouterName+findRouterKeyword.length()+1, line.length()-findRouterKeyword.length()-2);
                    R[routerCounter++].setName(name);
                }
                else{
                    //GET LINKS
                    int startLinks = line.find(linkBlockSeparatorStart);
                    int linkIDCounter = 0;

                    if(startLinks != line.npos){
                        //get pairs "id = " and \d{1..5}
                        //get pairs "cost = " and \d{1..5}

                        string linkID = "";
                        string linkCost = "";
                        string startLinkIDSeparator = "id = ";
                        string startLinkCostSeparator = " cost = ";

                        while (getline(FILE_TOPOLOGY, line)){

                            int startLinkID = line.find(startLinkIDSeparator);
                            int startLinkCost = line.find(startLinkCostSeparator);

                            if(startLinkID != line.npos){
                                string costAll = line.substr(startLinkCost);   //FROM START TO END COST PART

                                linkID = linkID + line.substr(startLinkID+startLinkIDSeparator.length(), startLinkCost - startLinkID-startLinkIDSeparator.length())+ '%';
                                linkCost = linkCost + line.substr(startLinkCost+startLinkCostSeparator.length(), line.length()-startLinkCost - startLinkCostSeparator.length()) + '%';

                                R[routerCounter-1].increaseLinkCounter();
                            }
                            else{
                                  break;
                            }
                        }//END WHILE LINKS{...} SECTOR
                        R[routerCounter-1].addLinkBulk(linkID, linkCost, '%');
                    }
                }
            }
        }
        else {
            throw 404;
        }
        for (int i=0; i<routerCounter; i++){
            cout << R[i].getName() << " Count of links: " << R[i].getLinkCount() << "\n";
            cout << "Links: " << "\n";
            R[i].printRouterLinks();
        }
        spf(R);
    }
    catch (int e){
        switch(e){
            case 404:{
                cout << "Error: File not found!\n";
                break;
            }
        }
    }

    return 0;
}
